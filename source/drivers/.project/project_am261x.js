let path = require('path');

let device = "am261x";

const files_r5f = {
    common: [
		"adc.c",
		"bootloader.c",
		"bootloader_buf_io.c",
		"bootloader_can.c",
		"bootloader_flash.c",
		"bootloader_hsmRt_load.c",
		"bootloader_mem.c",
		"bootloader_profile.c",
		"bootloader_soc.c",
		"bootloader_uniflash.c",
		"bootloader_xmodem.c",
		"cmpss.c",
		"dac.c",
		"ecap.c",
		"edma.c",
		"eqep.c",
		"etpwm.c",
		"flc.c",
		"fsi_rx.c",
		"fsi_tx.c",
		"gpio.c",
		// "gpmc_v0.c",
        // "gpmc_priv_v0.c",
        // "gpmc_nandlike_v0.c",
        // "gpmc_norlike_v0.c",
        // "gpmc_dma.c",
		"i2c_v1.c",
		"i2c_v1_lld.c",
		"ipc_notify_v1.c",
		"ipc_notify_v1_cfg.c",
		"ipc_rpmsg.c",
		"ipc_rpmsg_vring.c",
		"lin.c",
		"mcan.c",
		"mcspi_dma_edma.c",
		"mcspi_v0.c",
		"mcspi_v0_lld.c",
		"mdio_v0.c",
		"mmcsd_v1.c",
        "mpu_firewall.c",
        "mpu_firewall_v0_cfg.c",
		"ospi_dma.c",
		"ospi_dma_edma.c",
		"ospi_nor_flash.c",
		"ospi_phy.c",
		"ospi_v0.c",
		"pinmux.c",
		"pmu.c",
		"pruicss_m_v0.c",
		"pruicss_m_v0_cfg.c",
		"rl2.c",
		"rti.c",
		"sdfm.c",
		"soc.c",
		"soc_rcm.c",
		"spinlock.c",
		"uart_dma_edma.c",
		"uart_v0.c",
		"uart_v0_lld.c",
		"watchdog_rti.c",
		"watchdog_soc.c",
	],
};

const filedirs = {
    common: [
    	`pinmux/am261x`,
		`pruicss/m_v0`,
		`pruicss/soc/am261x`,
		`watchdog/v0/soc/${device}`,
		"adc/v2",
		"bootloader",
		"bootloader/soc/am261x",
		"cmpss/v0",
		"csl_arm_r5_pmu.S",
		"dac/v0",
		"ecap/v1",
		"edma/v0",
		"epwm/v1",
		"eqep/v1",
		"fsi/v1",
		"gpio/v0",
		// "gpmc/v0",
        // "gpmc/v0/dma",
		"i2c/v1",
		"i2c/v1/lld",
		"ipc_notify/v1",
		"ipc_notify/v1/soc/am261x",
		"ipc_rpmsg/",
		"lin/v0",
		"mcan/v0",
		"mcspi/v0",
		"mcspi/v0/lld",
		"mcspi/v0/lld/dma",
		"mcspi/v0/lld/dma/edma",
		"mdio/v0",
		"mmcsd/v1",
        "mpu_firewall/v0",
        "mpu_firewall/v0/soc/am261x",
		"ospi",
		"ospi/v0",
		"ospi/v0/dma",
		"ospi/v0/dma/edma",
		"pmu",
		"pmu/r5f",
		"rti/v0",
		"sdfm/v0",
		"soc/am261x",
		"spinlock/v0",
		"uart/v0",
		"uart/v0/lld",
		"uart/v0/lld/dma",
		"uart/v0/lld/dma/edma",
		"watchdog/v0",
        "optiflash/v0/flc",
        "optiflash/v0/rl2",
	],
};

const filedirs_r5f =  {
    common: [
        "pmu",
        "pmu/r5f",
    ]
};

const asmfiles_r5f = {
    common: [
        "csl_arm_r5_pmu.S",
    ]
};

const cflags_r5f = {
    release: [
        "-Oz",
        "-flto",
    ],
};

const includes = {
    common: [
        "${MCU_PLUS_SDK_PATH}/source/security",
    ],
};

const buildOptionCombos = [
    { device: device, cpu: "r5f", cgt: "ti-arm-clang"},
];

function getComponentProperty() {
    let property = {};

    property.dirPath = path.resolve(__dirname, "..");
    property.type = "library";
    property.name = "drivers";
    property.isInternal = false;
    property.buildOptionCombos = buildOptionCombos;

    return property;
}

function getComponentBuildProperty(buildOption) {
    let build_property = {};

    build_property.filedirs = filedirs;
    if(buildOption.cpu.match(/r5f*/)) {
        build_property.filedirs = {common: [...filedirs.common, ...filedirs_r5f.common]};
        build_property.cflags = cflags_r5f;
        build_property.files = files_r5f;
        build_property.asmfiles = asmfiles_r5f;
    }
	build_property.includes = includes;

    return build_property;
}

module.exports = {
    getComponentProperty,
    getComponentBuildProperty,
};
