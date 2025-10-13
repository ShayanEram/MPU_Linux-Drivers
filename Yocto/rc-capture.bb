SUMMARY = "RC input edge timestamp capture driver"
LICENSE = "GPL-2.0-only"
LIC_FILES_CHKSUM = "file://LICENSE;md5=751419260aa954499f7bbb25b02fbbab"
SRC_URI = "file://rc_capture.c file://LICENSE"
S = "${WORKDIR}"
inherit module
KERNEL_MODULE_AUTOLOAD += "rc_capture"
