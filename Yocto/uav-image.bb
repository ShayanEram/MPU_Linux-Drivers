SUMMARY = "UAV image for Raspberry Pi 5"
LICENSE = "MIT"
inherit core-image

IMAGE_INSTALL:append = " \
    rc-capture \
    openssh \
    iproute2 \
    ethtool \
"

DISTRO_FEATURES:append = " systemd"
VIRTUAL-RUNTIME_init_manager = "systemd"
