FILESEXTRAPATHS:prepend := "${THISDIR}/files:"
SRC_URI:append = " file://rc-capture.dtsi"

do_compile:append() {
    ${STAGING_BINDIR_NATIVE}/dtc -@ -I dts -O dtb \
      ${WORKDIR}/rc-capture.dtsi -o ${B}/rc-capture.dtbo
}

do_install:append() {
    install -d ${D}/boot/overlays
    install -m 0644 ${B}/rc-capture.dtbo ${D}/boot/overlays/rc-capture.dtbo
}
