DESCRIPTION = "Set system-wide locale to en_US.UTF-8"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COREBASE}/meta/COPYING.MIT;md5=3da9cfbcb788c80a0384361b4de20420"

SRC_URI = "file://lang.sh"

S = "${WORKDIR}"

inherit features_check
REQUIRED_DISTRO_FEATURES = "en-us-locale"

# Specify runtime dependencies for this package
RDEPENDS:${PN} += "bash glibc-utils localedef"

do_install() {
    install -d ${D}${sysconfdir}/profile.d/
    install -m 0755 ${S}/lang.sh ${D}${sysconfdir}/profile.d/lang.sh
}

FILES:${PN} += "${sysconfdir}/profile.d/lang.sh"