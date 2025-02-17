/*
 *  Copyright (C) 2023 Texas Instruments Incorporated
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

#include <stdio.h>
#include <kernel/dpl/DebugP.h>
#include <kernel/dpl/SemaphoreP.h>
#include "ti_drivers_config.h"
#include "ti_drivers_open_close.h"
#include "ti_board_open_close.h"

#include <drivers/pruicss.h>
#include <pru_ipc.h>


#define WRITE_CLOCK_REG_COMMAND     (0x00618400)  
#define CLOCK_REG_VALUE             (0x00FF0200)
/*
 *  This is an example project to show ADC samples transfered to R5F TCM
 *  by PRU and read by R5F on interrupt.
 */

/** \brief Global Structure pointer holding PRUSS1 memory Map. */
PRUICSS_Handle gPruIcss0Handle;

PRU_IPC_Handle gPruIpc0Handle;

/* semaphore used to indicate that the ISR has finished reading samples */
SemaphoreP_Object gAdcDataRecSem;

int32_t samples[CONFIG_PRU_IPC0_BUFFERS][CONFIG_PRU_IPC0_BLOCKSIZE];

#ifdef _DEBUG_
void print_samples(int32_t array[CONFIG_PRU_IPC0_BUFFERS][CONFIG_PRU_IPC0_BLOCKSIZE])
{
    for (uint32_t channel = 0; channel < CONFIG_PRU_IPC0_BUFFERS; channel++) {
        DebugP_log("----- Channel: %d ----- \r\n", channel+1);
        for (uint32_t sample = 0; sample < CONFIG_PRU_IPC0_BLOCKSIZE; sample++) {
            DebugP_log("Sample %d:  %d  \r\n", sample+1, array[channel][sample]);
        }
    }
    DebugP_log("-----------------------\r\n");
}
#endif

/* Functions defined in adc_functions.c */
void ADC_init();
void ADC_powerUp();
void ADC_reset();
void ADC_configure(uint32_t noOfCommands, uint32_t adcConfigCommands[]);
void ADC_startConversion();
void ADC_stopConversion();

void PRU_IPC_Isr(void *args)
{
    PRU_IPC_getData(gPruIpc0Handle, samples);
    SemaphoreP_post(&gAdcDataRecSem);
    PRUICSS_clearEvent(gPruIcss0Handle, gPruIpc0Handle->attrs->sysEventNum);
}

void ads_example_main(void *args)
{

    /*
    *  In example, the interfacing with ADS131M08 operates in following configuration:
    *  8 simultaneously sampling differential inputs
    *  Programmable data rate up to 32 kSPS
    *  Continous sampling of data
    */  

    Drivers_open(); // check return status
    int status;
    status = Board_driversOpen();
    DebugP_assert(SystemP_SUCCESS == status);

    gPruIcss0Handle = PRUICSS_open(CONFIG_PRU_ICSS0);
    status = PRUICSS_intcInit(gPruIcss0Handle, &icss0_intc_initdata);
    DebugP_assert(SystemP_SUCCESS == status);

    /* R5F Function:
    *  Configure the IO Expander to connect the PRU IOs to HSE
    *  Send ADC register configuration information to the PRU core
    *  Send instructions to PRU core
    *  Wait for interrupt from PRU indicating 1 block of adc samples has been received
    *  On interrupt fetch the block id to read and read the samples
    *  Process them as needed
    */

    /* Construct semaphore for ADC samples received */
    SemaphoreP_constructBinary(&gAdcDataRecSem, 0);
    /*  Total number of Commands to send to ADC */
    uint32_t noOfCommands = 2;

    /*  Commands array
     *  Add your commands to this (maximum = noOfCommands)
     */

    /*
    * command length or word length default value set to 24bits
    * Refer Section 8.5.1.8 - 8.5.1.10 to understand on programming to configure adc
    * So Each command sent to adc is of size 24bit, LSb 24bits[23:0] are sent by PRU
    * Write Reg: 0000 0000 011a aaaa annn nnnn, Writes nnn nnnn plus 1 registers beginning at address aa aaaa
    * With  0000 0000 dddd dddd dddd dddd (nnn nnnn plus 1 registers values) 
    * To configure ADC sampling rate to 32KSPS write to CLOCK_REG With value 0x00FF02 (WRITE_CLOCK_REG_COMMAND & CLOCK_REG_VALUE)
    * Refer Section 8.6 of ADS131M08_Datasheet,to find Register offset address & Register contents
    */

    uint32_t adcConfigCommands[2] = {WRITE_CLOCK_REG_COMMAND,CLOCK_REG_VALUE};

    DebugP_log("--------------------------------------------------\r\n");
    DebugP_log("Initializing App \r\n");
    ADC_init();
    DebugP_log("Initialized \r\n");

    DebugP_log("--------------------------------------------------\r\n");
    DebugP_log("Powering up ADC... \r\n");
    ADC_powerUp();
    DebugP_log("ADC Powered Up \r\n");

    DebugP_log("--------------------------------------------------\r\n");
    DebugP_log("Resetting ADC \r\n");
    ADC_reset();

    DebugP_log("--------------------------------------------------\r\n");
    DebugP_log("Configuring ADC... \r\n");
    ADC_configure(noOfCommands, adcConfigCommands);
    DebugP_log("ADC Configuration completed \r\n");

    DebugP_log("--------------------------------------------------\r\n");
    DebugP_log("Starting ADC Conversion \r\n");
    ADC_startConversion();
    DebugP_log("Started ADC Conversion \r\n");
    DebugP_log("--------------------------------------------------\r\n");

    while (1)
    {
        SemaphoreP_pend(&gAdcDataRecSem, SystemP_WAIT_FOREVER);
        #ifdef _DEBUG_
        print_samples(samples);
        #endif
    }

    Board_driversClose();
    Drivers_close();
}
