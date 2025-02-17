/*
 *  Copyright (C) 2018-2022 Texas Instruments Incorporated
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdlib.h>
#include "ti_drivers_config.h"
#include "ti_drivers_open_close.h"
#include "ti_board_open_close.h"
#include <drivers/sciclient.h>
#include <drivers/bootloader/soc/bootloader_soc.h>
#include <drivers/bootloader.h>
#include <kernel/dpl/ClockP.h>

#define DELAY_SEC (1000U)

void flashFixUpQspiBoot(void);

/* call this API to stop the booting process and spin, do that you can connect
 * debugger, load symbols and then make the 'loop' variable as 0 to continue execution
 * with debugger connected.
 */
void loop_forever(void)
{
    volatile uint32_t loop = 1;
    while(loop)
        ;
}

int main(void)
{
    int32_t status;

    Bootloader_profileReset();

    Bootloader_socWaitForFWBoot();

#ifndef DISABLE_WARM_REST_WA
    /* Warm Reset Workaround to prevent CPSW register lockup */
    if (!Bootloader_socIsMCUResetIsoEnabled())
    {
        Bootloader_socResetWorkaround();
    }
#endif

    Bootloader_profileAddProfilePoint("SYSFW init");

    if (!Bootloader_socIsMCUResetIsoEnabled())
    {
        /* Update devGrp to ALL to initialize MCU domain when reset isolation is
        not enabled */
        Sciclient_BoardCfgPrms_t boardCfgPrms_pm =
            {
                .boardConfigLow = (uint32_t)0,
                .boardConfigHigh = 0,
                .boardConfigSize = 0,
                .devGrp = DEVGRP_ALL,
            };

        status = Sciclient_boardCfgPm(&boardCfgPrms_pm);

        Sciclient_BoardCfgPrms_t boardCfgPrms_rm =
        {
            .boardConfigLow = (uint32_t)0,
            .boardConfigHigh = 0,
            .boardConfigSize = 0,
            .devGrp = DEVGRP_ALL,
        };

        status = Sciclient_boardCfgRm(&boardCfgPrms_rm);

        /* Enable MCU PLL. MCU PLL will not be enabled by DMSC when devGrp is set
        to Main in boardCfg */
        Bootloader_enableMCUPLL();
    }

    Bootloader_socOpenFirewalls();

    Bootloader_socNotifyFirewallOpen();

    System_init();
    Bootloader_profileAddProfilePoint("System_init");

    Drivers_open();
    Bootloader_profileAddProfilePoint("Drivers_open");

    #if 0
    DebugP_log("\r\n");
    DebugP_log("Starting OSPI Bootloader ... \r\n");
    #endif

	/* ROM doesn't reset the QSPI flash. So do a flash reset */
    flashFixUpQspiBoot();

    status = Board_driversOpen();
    DebugP_assert(status == SystemP_SUCCESS);
    Bootloader_profileAddProfilePoint("Board_driversOpen");

    status = Sciclient_getVersionCheck(1);
    Bootloader_profileAddProfilePoint("Sciclient Get Version");
    if(SystemP_SUCCESS == status)
    {
        Bootloader_BootImageInfo bootImageInfo;
        Bootloader_Params bootParams;
        Bootloader_Handle bootHandle;

        Bootloader_Params_init(&bootParams);
        Bootloader_BootImageInfo_init(&bootImageInfo);

        bootHandle = Bootloader_open(CONFIG_BOOTLOADER_FLASH0, &bootParams);
        if(bootHandle != NULL)
        {
            /* Initialize PRU Cores if applicable */
            Bootloader_Config *cfg = (Bootloader_Config *)bootHandle;
            if(TRUE == cfg->initICSSCores)
            {
                status = Bootloader_socEnableICSSCores(BOOTLOADER_ICSS_CORE_DEFAULT_FREQUENCY);
                DebugP_assert(status == SystemP_SUCCESS);
            }

            status = Bootloader_parseMultiCoreAppImage(bootHandle, &bootImageInfo);
            /* Load CPUs */
            /* Do not load M4 when MCU domain is reset isolated */

            uint32_t coreVariant = Bootloader_socGetCoreVariant();
            /*Checks the core variant(Dual/Quad) */


            if (!Bootloader_socIsMCUResetIsoEnabled())
            {
                if(status == SystemP_SUCCESS && (TRUE == Bootloader_isCorePresent(bootHandle, CSL_CORE_ID_M4FSS0_0)))
                {
                    bootImageInfo.cpuInfo[CSL_CORE_ID_M4FSS0_0].clkHz = Bootloader_socCpuGetClkDefault(CSL_CORE_ID_M4FSS0_0);
                    Bootloader_profileAddCore(CSL_CORE_ID_M4FSS0_0);
                    status = Bootloader_loadCpu(bootHandle, &bootImageInfo.cpuInfo[CSL_CORE_ID_M4FSS0_0]);
                }
            }
            if(status == SystemP_SUCCESS && (TRUE == Bootloader_isCorePresent(bootHandle, CSL_CORE_ID_R5FSS1_0)))
            {
                bootImageInfo.cpuInfo[CSL_CORE_ID_R5FSS1_0].clkHz = Bootloader_socCpuGetClkDefault(CSL_CORE_ID_R5FSS1_0);
                Bootloader_profileAddCore(CSL_CORE_ID_R5FSS1_0);
                status = Bootloader_loadCpu(bootHandle, &bootImageInfo.cpuInfo[CSL_CORE_ID_R5FSS1_0]);
            }
            /*Checks the core variant(Dual/Quad) */
            if((coreVariant == BOOTLOADER_DEVICE_VARIANT_QUAD_CORE) && status == SystemP_SUCCESS && (TRUE == Bootloader_isCorePresent(bootHandle, CSL_CORE_ID_R5FSS1_1)))
            {
                bootImageInfo.cpuInfo[CSL_CORE_ID_R5FSS1_1].clkHz = Bootloader_socCpuGetClkDefault(CSL_CORE_ID_R5FSS1_1);
                Bootloader_profileAddCore(CSL_CORE_ID_R5FSS1_1);
                status = Bootloader_loadCpu(bootHandle, &bootImageInfo.cpuInfo[CSL_CORE_ID_R5FSS1_1]);
            }

            /* Assume self boot for either of the cores of R50 cluster */
            uint32_t isSelfBoot = FALSE;
            if(TRUE == Bootloader_isCorePresent(bootHandle, CSL_CORE_ID_R5FSS0_0))
            {
                isSelfBoot = TRUE;
                Bootloader_profileAddCore(CSL_CORE_ID_R5FSS0_0);
            }
            /*Checks the core variant(Dual/Quad) */
            if((coreVariant == BOOTLOADER_DEVICE_VARIANT_QUAD_CORE) && TRUE == Bootloader_isCorePresent(bootHandle, CSL_CORE_ID_R5FSS0_1))
            {
                isSelfBoot = TRUE;
                Bootloader_profileAddCore(CSL_CORE_ID_R5FSS0_1);
            }

            /* Self cores has to be reset together, so check for both */
            if(status == SystemP_SUCCESS && (TRUE == isSelfBoot))
            {
                /* Set clocks for self cluster */
                bootImageInfo.cpuInfo[CSL_CORE_ID_R5FSS0_0].clkHz = Bootloader_socCpuGetClkDefault(CSL_CORE_ID_R5FSS0_0);
                if((Bootloader_socIsR5FSSDual(BOOTLOADER_R5FSS0)))
                {
                    bootImageInfo.cpuInfo[CSL_CORE_ID_R5FSS0_1].clkHz = Bootloader_socCpuGetClkDefault(CSL_CORE_ID_R5FSS0_1);
                }

                /* Reset self cluster, both Core0 and Core 1. Init RAMs and load the app  */
                status = Bootloader_loadSelfCpu(bootHandle, &bootImageInfo.cpuInfo[CSL_CORE_ID_R5FSS0_0], FALSE);
                if((status == SystemP_SUCCESS) && (TRUE == Bootloader_socIsR5FSSDual(BOOTLOADER_R5FSS0)))
                {
                    status = Bootloader_loadSelfCpu(bootHandle, &bootImageInfo.cpuInfo[CSL_CORE_ID_R5FSS0_1], FALSE);
                }
            }
            Bootloader_profileAddProfilePoint("CPU load");
            Bootloader_profileUpdateAppimageSize(Bootloader_getMulticoreImageSize(bootHandle));
            Bootloader_profileUpdateMediaAndClk(BOOTLOADER_MEDIA_FLASH, OSPI_getInputClk(gOspiHandle[CONFIG_OSPI0]));

            #if 1
            if( status == SystemP_SUCCESS)
            {
                /* enable Phy and Phy pipeline for XIP execution */
                if( OSPI_isPhyEnable(gOspiHandle[CONFIG_OSPI0]) )
                {
                    status = OSPI_enablePhy(gOspiHandle[CONFIG_OSPI0]);
                    DebugP_assert(status == SystemP_SUCCESS);

                    status = OSPI_enablePhyPipeline(gOspiHandle[CONFIG_OSPI0]);
                    DebugP_assert(status == SystemP_SUCCESS);
                }
            }
            #endif

            if(status == SystemP_SUCCESS)
            {
                /* Print SBL Profiling logs to UART as other cores may use the UART for logging */
                Bootloader_profileAddProfilePoint("SBL End");
                Bootloader_profilePrintProfileLog();
                DebugP_log("Image loading done, switching to application ...\r\n");
                UART_flushTxFifo(gUartHandle[CONFIG_UART0]);
            }

            /* Run CPUs */
            /* Do not run M4 when MCU domain is reset isolated */
            if (!Bootloader_socIsMCUResetIsoEnabled())
            {
                if(status == SystemP_SUCCESS && (TRUE == Bootloader_isCorePresent(bootHandle, CSL_CORE_ID_M4FSS0_0)))
                {
                    status = Bootloader_runCpu(bootHandle, &bootImageInfo.cpuInfo[CSL_CORE_ID_M4FSS0_0]);
                }
            }
            if(status == SystemP_SUCCESS && (TRUE == Bootloader_isCorePresent(bootHandle, CSL_CORE_ID_R5FSS1_0)))
            {
                status = Bootloader_runCpu(bootHandle, &bootImageInfo.cpuInfo[CSL_CORE_ID_R5FSS1_0]);
            }
            /*Checks the core variant(Dual/Quad) */
            if((Bootloader_socIsR5FSSDual(BOOTLOADER_R5FSS1)) && status == SystemP_SUCCESS && (TRUE == Bootloader_isCorePresent(bootHandle, CSL_CORE_ID_R5FSS1_1)))
            {
                status = Bootloader_runCpu(bootHandle, &bootImageInfo.cpuInfo[CSL_CORE_ID_R5FSS1_1]);
            }
            if(status == SystemP_SUCCESS)
            {
                /* Reset self cluster, both Core0 and Core 1. Init RAMs and run the app  */
                status = Bootloader_runSelfCpu(bootHandle, &bootImageInfo);
            }
            /* it should not return here, if it does, then there was some error */
            Bootloader_close(bootHandle);
        }
    }
    if(status != SystemP_SUCCESS )
    {
        DebugP_log("Some tests have failed!!\r\n");
    }
    Drivers_close();
    System_deinit();

    return 0;
}

void flashFixUpQspiBoot(void)
{
    uint32_t gpiobaseAddr, pinnum;

	/* Get address after translation translate */
    gpiobaseAddr = (uint32_t) AddrTranslateP_getLocalAddr(CONFIG_GPIO0_BASE_ADDR);

    pinnum = CONFIG_GPIO0_PIN;

	/* Drive the GPIO Pin low to assert the reset signal */
    GPIO_pinWriteLow(gpiobaseAddr, pinnum);
    ClockP_usleep(DELAY_SEC);

	/* Drive the GPIO Pin high to deassert the reset signal */
    GPIO_pinWriteHigh(gpiobaseAddr, pinnum);
    ClockP_usleep(DELAY_SEC);
}