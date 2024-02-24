# Provides the i.MX common settings

include conf/machine/include/fsl-default-settings.inc
include conf/machine/include/fsl-default-versions.inc

require conf/machine/include/utilities.inc

MACHINEOVERRIDES =. "use-${IMX_DEFAULT_BSP}-bsp:"

# UBOOT_BINARY is used inside the wks files to dynamically find the required
# U-Boot file.
UBOOT_BINARY ?= "u-boot.${UBOOT_SUFFIX}"

UBOOT_SUFFIX ?= "${UBOOT_SUFFIX:pn-${IMX_DEFAULT_BOOTLOADER}}"

# We need to export the original variable to allow it to be used when generating
# wic based images.
SPL_BINARY ??= "${@get_spl_binary(d)}"

IMX_DEFAULT_MFGTOOL = "${@bb.utils.contains('IMX_DEFAULT_BOOTLOADER', 'u-boot-imx','u-boot-imx-mfgtool', 'u-boot-fslc', d)}"

PREFERRED_PROVIDER_u-boot ??= "${IMX_DEFAULT_BOOTLOADER}"
PREFERRED_PROVIDER_u-boot-mfgtool ??= "${IMX_DEFAULT_MFGTOOL}"
PREFERRED_PROVIDER_virtual/bootloader ??= "${IMX_DEFAULT_BOOTLOADER}"

PREFERRED_PROVIDER_u-boot-mxsboot-native ??= "u-boot-fslc-mxsboot-native"

UBOOT_ENTRYPOINT:mx8m-generic-bsp   ?= "0x40480000"

# Some derivates can utilize the boot container provided by U-Boot,
# below variable sets that those machines which have a imx-boot-container
# in their MACHINEOVERRIDES can inherit a imx-boot-container class
UBOOT_PROVIDES_BOOT_CONTAINER = "0"
UBOOT_PROVIDES_BOOT_CONTAINER:imx-boot-container = "1"

# Trusted Firmware for Cortex-A (TF-A) can have different providers, either
# from upstream or from NXP downstream fork. Below variable defines which TF-A
# shall be taken into the build, and will be integrated into runtime image.
#
# Upstream TF-A recipe resides in the meta-arm layer and in maintained by OE
# community. Therefore, in order to add upstream TF-A - additional layer has
# to be included in the bblayers.con file. Compatible machines are added to
# this layer via dynamic-layers mechanism.
#
# NOTE: Current upstream TF-A version (v2.7) does not support HAB feature of
# i.MX8M family. If the upstream TF-A version is chosen, then HAB will not be
# available for all SoCs that are opting-in. This might change with future TF-A
# release, so this statement shall be kept here until support is added.
#
# Default TF-A provider to NXP downstream fork
IMX_DEFAULT_ATF_PROVIDER ??= "imx-atf"

# Allow setting the UART used during the boot by ATF.

# FIXME: We should return INVALID here but currently only i.MX8M has support to override the UART
# base address in source code.
SOC_ATF_BOOT_UART_BASE                    = ""

SOC_ATF_BOOT_UART_BASE:mx8m-generic-bsp   = "0x30890000"
ATF_BOOT_UART_BASE                       ?= "${SOC_ATF_BOOT_UART_BASE}"

PREFERRED_PROVIDER_virtual/xserver      = "xserver-xorg"
XSERVER_DRIVER                          = "xf86-video-fbdev"
XSERVER_DRIVER:append:mx8-generic-bsp   = " xf86-video-modesetting"
XSERVER_DRIVER:imx-mainline-bsp = " \
    xf86-video-fbdev \
    xf86-video-modesetting \
    xserver-xorg-extension-glx \
"
XSERVER = "xserver-xorg \
           xf86-input-evdev \
           ${XSERVER_DRIVER}"

# Ship kernel modules
MACHINE_EXTRA_RRECOMMENDS = "kernel-modules"

# Tunes for hard/soft float-point selection. Note that we allow building for
# thumb support giving distros the chance to enable thumb by setting
# ARM_INSTRUCTION_SET = "thumb"
#
DEFAULTTUNE:mx8m-generic-bsp   ?= "cortexa53-crypto"

INHERIT += "machine-overrides-extender"

#######
### NXP BSP specific overrides
#######

MACHINEOVERRIDES_EXTENDER:mx8mn:use-nxp-bsp  = "imx-generic-bsp:imx-nxp-bsp:imxdrm:imxgpu:imxgpu3d:imxvulkan:mx8-generic-bsp:mx8-nxp-bsp:mx8m-generic-bsp:mx8m-nxp-bsp:mx8mn-generic-bsp:mx8mn-nxp-bsp"

#######
### Mainline BSP specific overrides
#######

MACHINEOVERRIDES_EXTENDER:mx8mn:use-mainline-bsp  = "imx-generic-bsp:imx-mainline-bsp:mx8-generic-bsp:mx8-mainline-bsp:mx8m-generic-bsp:mx8m-mainline-bsp:mx8mn-generic-bsp:mx8mn-mainline-bsp"

MACHINEOVERRIDES_EXTENDER_FILTER_OUT = " \
    mx8 \
    mx8m \
    mx8mn \
"

MACHINEOVERRIDES_FILTERED_OUT_QA_ERROR = "%s overrides cannot be used and need conversion to use the new BSP-specific overrides. Check 'meta-freescale/scripts/convert-bsp-specific-overrides'."

# Sub-architecture support
MACHINE_SOCARCH_SUFFIX ?= ""

MACHINE_SOCARCH_SUFFIX:mx8mn-nxp-bsp   = "-mx8mn"

# For Mainline we use a single SoC suffix as we don't have different build options
MACHINE_SOCARCH_SUFFIX:imx-mainline-bsp = "-imx"

MACHINE_ARCH_FILTER = "virtual/kernel"
MACHINE_SOCARCH_FILTER:append:imx-nxp-bsp = " \
    alsa-lib \
    gstreamer1.0 \
    weston \
"
MACHINE_SOCARCH_FILTER:append:imxvpu = " \
    imx-codec \
    imx-vpuwrap \
    libimxvpuapi \
    virtual/imxvpu \
"
MACHINE_SOCARCH_FILTER:append:imxvulkan = " \
    vulkan-headers \
    vulkan-loader \
    vulkan-tools \
"
MACHINE_SOCARCH_FILTER:append:imxgpu = " \
    virtual/egl \
    virtual/libopenvg \
    virtual/mesa \
    virtual/opencl-icd \
    libdrm \
    cairo \
    libgal-imx \
    opencl-icd-loader \
    opencl-clhpp \
    opencl-headers \
    opencv \
    pango \
"
MACHINE_SOCARCH_FILTER:append:imxgpu2d = " \
    virtual/libg2d \
"
MACHINE_SOCARCH_FILTER:append:imxgpu3d = " \
    virtual/libgl \
    virtual/libgles1 \
    virtual/libgles2 \
"
MACHINE_SOCARCH_FILTER:append:imx-mainline-bsp = " \
    virtual/egl \
    virtual/libopenvg \
    virtual/libg2d \
    virtual/libgl \
    virtual/libgles1 \
    virtual/libgles2 \
    virtual/mesa \
    cairo \
    pango \
    qtbase \
"
MACHINE_SOCARCH_FILTER:append:mx8qm-nxp-bsp = " \
    virtual/libopenvx \
"

INHERIT += "fsl-dynamic-packagearch"

SIGGEN_EXCLUDE_SAFE_RECIPE_DEPS:append = " \
    imx-gpu-viv->kernel-module-imx-gpu-viv \
    libimxvpuapi->virtual/imxvpu \
    imx-vpuwrap->virtual/imxvpu \
    imx-codec->virtual/imxvpu \
    imx-test->virtual/imxvpu \
"

# Firmware used for boot.
IMX_EXTRA_FIRMWARE                   ?= ""
IMX_EXTRA_FIRMWARE:mx8-generic-bsp    = "firmware-imx-8 imx-sc-firmware imx-seco"
IMX_EXTRA_FIRMWARE:mx8m-generic-bsp   = "firmware-imx-8m"

# Firmware
MACHINE_FIRMWARE ?= ""
MACHINE_FIRMWARE:append:mx8mn-generic-bsp    = " linux-firmware-imx-sdma-imx7d firmware-imx-easrc-imx8mn"
MACHINE_FIRMWARE:append:imx-mainline-bsp     = " linux-firmware-imx-sdma-imx6q linux-firmware-imx-sdma-imx7d firmware-imx-vpu-imx6q firmware-imx-vpu-imx6d"

MACHINE_EXTRA_RRECOMMENDS += "${MACHINE_FIRMWARE}"

# Extra audio support
IMX_ALSA_EXTRA = ""
IMX_ALSA_EXTRA:imx-nxp-bsp = "imx-alsa-plugins"
MACHINE_EXTRA_RRECOMMENDS:append:mx8-nxp-bsp = " ${@bb.utils.contains('DISTRO_FEATURES', 'alsa', '${IMX_ALSA_EXTRA}', '', d)}"

# Extra Cypress Wi-Fi and BTW firmware (Murata)
MACHINE_FIRMWARE:append = " ${@bb.utils.contains('MACHINE_FEATURES', 'bcm4339', 'linux-firmware-bcm4339', '', d)}"
MACHINE_FIRMWARE:append = " ${@bb.utils.contains('MACHINE_FEATURES', 'bcm43430', 'linux-firmware-bcm43430', '', d)}"
MACHINE_FIRMWARE:append = " ${@bb.utils.contains('MACHINE_FEATURES', 'bcm43455', 'linux-firmware-bcm43455', '', d)}"
MACHINE_FIRMWARE:append = " ${@bb.utils.contains('MACHINE_FEATURES', 'bcm4356', 'linux-firmware-bcm4356-pcie', '', d)}"
MACHINE_FIRMWARE:append = " ${@bb.utils.contains('MACHINE_FEATURES', 'bcm4359', 'firmware-nxp-wifi-bcm4359-pcie', '', d)}"

# Extra NXP Wi-Fi and Bluetooth driver firmware and driver
MACHINE_FIRMWARE:append = " ${@bb.utils.contains('MACHINE_FEATURES', 'nxp8801-sdio', 'firmware-nxp-wifi-nxp8801-sdio', '', d)}"
MACHINE_EXTRA_RRECOMMENDS:append = " ${@bb.utils.contains('MACHINE_FEATURES', 'nxp8801-sdio', 'kernel-module-nxp89xx', '', d)}"

MACHINE_FIRMWARE:append = " ${@bb.utils.contains('MACHINE_FEATURES', 'nxp8987-sdio', 'firmware-nxp-wifi-nxp8987-sdio', '', d)}"
MACHINE_EXTRA_RRECOMMENDS:append = " ${@bb.utils.contains('MACHINE_FEATURES', 'nxp8987-sdio', 'kernel-module-nxp89xx', '', d)}"

MACHINE_FIRMWARE:append = " ${@bb.utils.contains('MACHINE_FEATURES', 'nxp8997-pcie', 'firmware-nxp-wifi-nxp8997-pcie', '', d)}"
MACHINE_EXTRA_RRECOMMENDS:append = " ${@bb.utils.contains('MACHINE_FEATURES', 'nxp8997-pcie', 'kernel-module-nxp89xx', '', d)}"

MACHINE_FIRMWARE:append = " ${@bb.utils.contains('MACHINE_FEATURES', 'nxp8997-sdio', 'firmware-nxp-wifi-nxp8997-sdio', '', d)}"
MACHINE_EXTRA_RRECOMMENDS:append = " ${@bb.utils.contains('MACHINE_FEATURES', 'nxp8997-sdio', 'kernel-module-nxp89xx', '', d)}"

MACHINE_FIRMWARE:append = " ${@bb.utils.contains('MACHINE_FEATURES', 'nxp9098-pcie', 'firmware-nxp-wifi-nxp9098-pcie', '', d)}"
MACHINE_EXTRA_RRECOMMENDS:append = " ${@bb.utils.contains('MACHINE_FEATURES', 'nxp9098-pcie', 'kernel-module-nxp89xx', '', d)}"

MACHINE_FIRMWARE:append = " ${@bb.utils.contains('MACHINE_FEATURES', 'nxp9098-sdio', 'firmware-nxp-wifi-nxp9098-sdio', '', d)}"
MACHINE_EXTRA_RRECOMMENDS:append = " ${@bb.utils.contains('MACHINE_FEATURES', 'nxp9098-sdio', 'kernel-module-nxp89xx', '', d)}"

# Extra QCA Wi-Fi & BTE driver and firmware
MACHINE_EXTRA_RRECOMMENDS:append = " ${@bb.utils.contains('MACHINE_FEATURES', 'qca6174', 'packagegroup-fsl-qca6174', '', d)}"
MACHINE_EXTRA_RRECOMMENDS:append = " ${@bb.utils.contains('MACHINE_FEATURES', 'qca9377', 'packagegroup-fsl-qca9377', '', d)}"

# Extra udev rules
MACHINE_EXTRA_RRECOMMENDS += "udev-rules-imx"

# Jailhouse
MACHINE_EXTRA_RRECOMMENDS += " \
    ${@bb.utils.filter('COMBINED_FEATURES', 'jailhouse', d)} \
"

# GStreamer 1.0 plugins
MACHINE_GSTREAMER_1_0_PLUGIN                ?= ""
MACHINE_GSTREAMER_1_0_PLUGIN:mx8mn-nxp-bsp  ?= "imx-gst1.0-plugin"

PREFERRED_VERSION_gstreamer1.0:mx8-nxp-bsp              ??= "1.20.3.imx"
PREFERRED_VERSION_gstreamer1.0-plugins-base:mx8-nxp-bsp ??= "1.20.3.imx"
PREFERRED_VERSION_gstreamer1.0-plugins-good:mx8-nxp-bsp ??= "1.20.3.imx"
PREFERRED_VERSION_gstreamer1.0-plugins-bad:mx8-nxp-bsp  ??= "1.20.3.imx"
PREFERRED_VERSION_ffmpeg:mx8-nxp-bsp                    ??= "4.4.1"

# Determines if the SoC has support for Vivante kernel driver
SOC_HAS_VIVANTE_KERNEL_DRIVER_SUPPORT        = "0"
SOC_HAS_VIVANTE_KERNEL_DRIVER_SUPPORT:imxgpu = "1"

# Handle Vivante kernel driver setting:
#   0 - machine does not have Vivante GPU driver support
#   1 - machine has Vivante GPU driver support
MACHINE_HAS_VIVANTE_KERNEL_DRIVER_SUPPORT ?= "${SOC_HAS_VIVANTE_KERNEL_DRIVER_SUPPORT}"

# Graphics libraries
PREFERRED_PROVIDER_virtual/egl      ?= "mesa"
PREFERRED_PROVIDER_virtual/libgl    ?= "mesa"
PREFERRED_PROVIDER_virtual/libgles1 ?= "mesa"
PREFERRED_PROVIDER_virtual/libgles2 ?= "mesa"

PREFERRED_PROVIDER_virtual/egl:imxgpu        ?= "imx-gpu-viv"
PREFERRED_PROVIDER_virtual/libgl:imxgpu3d    ?= "imx-gpu-viv"
PREFERRED_PROVIDER_virtual/libgles1:imxgpu3d ?= "imx-gpu-viv"
PREFERRED_PROVIDER_virtual/libgles2:imxgpu3d ?= "imx-gpu-viv"
PREFERRED_PROVIDER_virtual/libgles3:imxgpu3d ?= "imx-gpu-viv"
PREFERRED_PROVIDER_virtual/libg2d            ?= "imx-gpu-g2d"
PREFERRED_PROVIDER_virtual/libg2d:imxdpu     ?= "imx-dpu-g2d"
PREFERRED_PROVIDER_opencl-clhpp:imxgpu       ?= "imx-gpu-viv"
PREFERRED_PROVIDER_opencl-headers:imxgpu     ?= "imx-gpu-viv"
PREFERRED_PROVIDER_opencl-icd-loader:imxgpu  ?= "imx-gpu-viv"
PREFERRED_PROVIDER_virtual/opencl-icd:imxgpu ?= "imx-gpu-viv"

PREFERRED_VERSION_weston:imx-nxp-bsp      ??= "10.0.1.imx"
PREFERRED_VERSION_weston:imx-mainline-bsp   = ""

PREFERRED_VERSION_wayland-protocols:mx8-nxp-bsp  ??= "1.25.imx"

# Use i.MX libdrm Version
PREFERRED_VERSION_libdrm:mx8-nxp-bsp ??= "2.4.109.imx"

PREFERRED_VERSION_vulkan-headers:imxvulkan ??= "1.2.182.0"
PREFERRED_VERSION_vulkan-loader:imxvulkan  ??= "1.2.182.0"
PREFERRED_VERSION_vulkan-tools:imxvulkan   ??= "1.2.182.0"

# Use i.MX optee Version
PREFERRED_VERSION_optee-os:mx8-nxp-bsp     ??= "3.19.0.imx"
PREFERRED_VERSION_optee-client:mx8-nxp-bsp ??= "3.19.0.imx"
PREFERRED_VERSION_optee-test:mx8-nxp-bsp   ??= "3.19.0.imx"

# Handle default kernel
IMX_DEFAULT_KERNEL:imx-mainline-bsp = "linux-fslc"
IMX_DEFAULT_KERNEL:imx-nxp-bsp      = "linux-imx"

PREFERRED_PROVIDER_virtual/kernel ??= "${IMX_DEFAULT_KERNEL}"

SOC_DEFAULT_IMAGE_FSTYPES = "wic.bmap wic.gz"
SOC_DEFAULT_IMAGE_FSTYPES:mxs-generic-bsp = "uboot-mxsboot-sdcard wic.bmap wic.gz"

# Do not update fstab file when using wic images
WIC_CREATE_EXTRA_ARGS ?= "--no-fstab-update"

OPTEE_BIN_EXT ??= ""
OPTEE_BOOT_IMAGE         = "tee.bin uTee-${OPTEE_BIN_EXT}"
OPTEE_BOOT_IMAGE:aarch64 = "tee.bin"

IMAGE_FSTYPES ?= "${SOC_DEFAULT_IMAGE_FSTYPES}"

IMAGE_BOOT_FILES ?= " \
    ${KERNEL_IMAGETYPE} \
    ${@make_dtb_boot_files(d)} \
    ${@bb.utils.contains('MACHINE_FEATURES', 'optee', '${OPTEE_BOOT_IMAGE}', '', d)} \
"

### wic default support
OPTEE_WKS_FILE_DEPENDS         = "optee-os"
OPTEE_WKS_FILE_DEPENDS:aarch64 = ""

WKS_FILE_DEPENDS ?= " \
    virtual/bootloader \
    \
    e2fsprogs-native \
    bmap-tools-native \
    ${@bb.utils.contains('MACHINE_FEATURES', 'optee', '${OPTEE_WKS_FILE_DEPENDS}', '', d)} \
"

# We need to restrict the append so we don't add this for other i.MX SoC's.
# Derivatives that are not yet adopted the usage of boot container provided
# by U-Boot build are still targeted to use 'imx-boot' package provided by
# NXP.
#
# Moving those derivatives to mainline BSP would require to define an
# 'imx-boot-container' override, and test if the U-Boot built 'flash.bin'
# binary is used a replacement.
#
# NOTE: the results binary name of the boot container is set to 'imx-boot'
# for both NXP and Mainline BSP.
WKS_FILE_DEPENDS:append:imx-generic-bsp:aarch64 = " \
    ${@oe.utils.ifelse(d.getVar('UBOOT_PROVIDES_BOOT_CONTAINER') == '0', 'imx-boot', '')} \
"

SOC_DEFAULT_WKS_FILE ?= "imx-uboot-bootpart.wks.in"
SOC_DEFAULT_WKS_FILE:mx8-generic-bsp ?= "imx-imx-boot-bootpart.wks.in"
SOC_DEFAULT_WKS_FILE:mxs-generic-bsp ?= "imx-uboot-mxs-bootpart.wks.in"
SOC_DEFAULT_WKS_FILE:mx9-generic-bsp ?= "imx-imx-boot-bootpart.wks.in"

WKS_FILE ?= "${SOC_DEFAULT_WKS_FILE}"

SERIAL_CONSOLES = "115200;ttymxc0"
SERIAL_CONSOLES:mxs-generic-bsp = "115200;ttyAMA0"

KERNEL_IMAGETYPE = "zImage"
KERNEL_IMAGETYPE:aarch64 = "Image"

MACHINE_FEATURES = "usbgadget usbhost vfat alsa touchscreen screen"

HOSTTOOLS_NONFATAL:append:mx8-nxp-bsp = " sha384sum"

# Allow meta-imx to add NIP devices information until upstreamed.
include conf/machine/include/imx-base-extend.inc