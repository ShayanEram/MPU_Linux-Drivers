#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/of_device.h>
#include <linux/gpio/consumer.h>
#include <linux/interrupt.h>
#include <linux/miscdevice.h>
#include <linux/uaccess.h>
#include <linux/wait.h>
#include <linux/poll.h>
#include <linux/spinlock.h>
#include <linux/ktime.h>

#define BUF_SIZE 1024
#define RCIOC_FLUSH _IO('r', 0x01)

struct rc_event {
    u64 timestamp_ns;
    u8 level;
};

struct rc_priv {
    struct gpio_desc *gpiod;
    int irq;
    struct rc_event buf[BUF_SIZE];
    unsigned int head, tail;
    spinlock_t lock;
    wait_queue_head_t wq;
    struct miscdevice misc;
};

static irqreturn_t rc_irq(int irq, void *data)
{
    struct rc_priv *rc = data;
    unsigned long flags;
    struct rc_event evt = {
        .timestamp_ns = ktime_to_ns(ktime_get()),
        .level = gpiod_get_value(rc->gpiod) ? 1 : 0
    };

    spin_lock_irqsave(&rc->lock, flags);
    rc->buf[rc->head] = evt;
    rc->head = (rc->head + 1) % BUF_SIZE;
    if (rc->head == rc->tail)
        rc->tail = (rc->tail + 1) % BUF_SIZE;
    spin_unlock_irqrestore(&rc->lock, flags);

    wake_up_interruptible(&rc->wq);
    return IRQ_HANDLED;
}

static ssize_t rc_read(struct file *f, char __user *ubuf, size_t len, loff_t *ppos)
{
    struct rc_priv *rc = container_of(f->private_data, struct rc_priv, misc);
    struct rc_event evt;
    unsigned long flags;

    if (len < sizeof(evt))
        return -EINVAL;

    // Blocking read: wait until data available
    if (wait_event_interruptible(rc->wq, rc->head != rc->tail))
        return -ERESTARTSYS;

    spin_lock_irqsave(&rc->lock, flags);
    evt = rc->buf[rc->tail];
    rc->tail = (rc->tail + 1) % BUF_SIZE;
    spin_unlock_irqrestore(&rc->lock, flags);

    if (copy_to_user(ubuf, &evt, sizeof(evt)))
        return -EFAULT;

    return sizeof(evt);
}

static __poll_t rc_poll(struct file *f, poll_table *wait)
{
    struct rc_priv *rc = container_of(f->private_data, struct rc_priv, misc);
    __poll_t mask = 0;

    poll_wait(f, &rc->wq, wait);
    if (rc->head != rc->tail)
        mask |= POLLIN | POLLRDNORM;

    return mask;
}

static long rc_ioctl(struct file *f, unsigned int cmd, unsigned long arg)
{
    struct rc_priv *rc = container_of(f->private_data, struct rc_priv, misc);
    unsigned long flags;

    switch (cmd) {
    case RCIOC_FLUSH:
        spin_lock_irqsave(&rc->lock, flags);
        rc->head = rc->tail = 0;
        spin_unlock_irqrestore(&rc->lock, flags);
        return 0;
    default:
        return -ENOTTY;
    }
}

static const struct file_operations rc_fops = {
    .owner          = THIS_MODULE,
    .read           = rc_read,
    .unlocked_ioctl = rc_ioctl,
    .poll           = rc_poll,
    .llseek         = no_llseek,
};

static int rc_probe(struct platform_device *pdev)
{
    struct rc_priv *rc;
    int ret;

    rc = devm_kzalloc(&pdev->dev, sizeof(*rc), GFP_KERNEL);
    if (!rc) return -ENOMEM;

    rc->gpiod = devm_gpiod_get(&pdev->dev, "rcin", GPIOD_IN);
    if (IS_ERR(rc->gpiod))
        return PTR_ERR(rc->gpiod);

    rc->irq = gpiod_to_irq(rc->gpiod);
    if (rc->irq < 0)
        return rc->irq;

    spin_lock_init(&rc->lock);
    init_waitqueue_head(&rc->wq);
    rc->head = rc->tail = 0;

    ret = devm_request_irq(&pdev->dev, rc->irq, rc_irq,
                           IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING,
                           dev_name(&pdev->dev), rc);
    if (ret)
        return ret;

    rc->misc.minor = MISC_DYNAMIC_MINOR;
    rc->misc.name  = devm_kasprintf(&pdev->dev, GFP_KERNEL, "rc_capture%d", pdev->id);
    rc->misc.fops  = &rc_fops;
    rc->misc.parent = &pdev->dev;

    ret = misc_register(&rc->misc);
    if (ret)
        return ret;

    platform_set_drvdata(pdev, rc);
    dev_info(&pdev->dev, "rc-capture registered as /dev/%s (irq=%d)\n", rc->misc.name, rc->irq);
    return 0;
}

static int rc_remove(struct platform_device *pdev)
{
    struct rc_priv *rc = platform_get_drvdata(pdev);
    misc_deregister(&rc->misc);
    return 0;
}

static const struct of_device_id rc_of_match[] = {
    { .compatible = "shayan,rc-capture" },
    { }
};
MODULE_DEVICE_TABLE(of, rc_of_match);

static struct platform_driver rc_driver = {
    .driver = {
        .name           = "rc-capture",
        .of_match_table = rc_of_match,
    },
    .probe  = rc_probe,
    .remove = rc_remove,
};

module_platform_driver(rc_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Shayan");
MODULE_DESCRIPTION("RC input edge timestamp capture (IRQ, miscdevice)");
