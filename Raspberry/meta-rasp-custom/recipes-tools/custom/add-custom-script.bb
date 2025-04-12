SUMMARY = "Add a service to start the Jig script related."
VERSION = "1.0"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COREBASE}/meta/COPYING.MIT;md5=3da9cfbcb788c80a0384361b4de20420"

# Specify source URIs for configuration files
SRC_URI = " file://custom-script.service \
            file://script/ \
        "

# Inherit features_check to check required features
inherit features_check
# Specify that the image requires systemd service manager
REQUIRED_DISTRO_FEATURES = "systemd"
# Enable automatic starting of the systemd service
SYSTEMD_AUTO_ENABLE = "enable"
# Append the systemd service for custom-script to start automatically
SYSTEMD_SERVICE:${PN}:append = " custom-script.service"
# Script dependencies
RDEPENDS:${PN} = "\
                    python3 \
                    python3-modules \
                    python3-threading \
                    rpi-gpio \
                    python3-pyserial \
                    python3-requests \
                "

# Installation task
do_install:append () {    
    # Install script files 
    install -d -m 755 ${D}${datadir}/custom-script        
    install -p -m 644 ${WORKDIR}/script/* ${D}${datadir}/custom-script/
    chown -R root:root ${D}${datadir}/custom-script

    # Install the service file to the systemd folders
    install -d ${D}${systemd_system_unitdir}
    install -p -m 0644 ${WORKDIR}/custom-script.service ${D}${systemd_system_unitdir}/custom-script.service

    # Create a symbolic link for systemd service
    install -d ${D}${sysconfdir}/systemd/system/multi-user.target.wants/
    ln -s ${systemd_system_unitdir}/custom-script.service ${D}${sysconfdir}/systemd/system/multi-user.target.wants/custom-script.service
}

FILES:${PN} =  "\
                ${datadir}/custom-script/* \
                ${systemd_system_unitdir}/custom-script.service \
                ${sysconfdir}/systemd/system/multi-user.target.wants/custom-script.service \                
                \
               "

# Prevents do_package failures with:
# debugsources.list: No such file or directory:
INHIBIT_PACKAGE_DEBUG_SPLIT = "1"