/*
 *  Copyright (C) 2021-24 Texas Instruments Incorporated
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

 /**
 *  \file   mcspi_performance.c
 *
 *  \brief This file contains the Controller application which demonstrates
 *         data transfer in controller mode with performance measurment.
 *
 *         McSPI is configured in Tx Only mode with FIFO enabled for Tx.
 *         Word Length tested is 8 bits.
 *         SPI CLK Frequency used is 12 MHZ.
 *         Number of Words is 5.
 *         Data is transmitted on D0 pin.
 *         Data transmission is in polled mode.
 *
 */

#include <kernel/dpl/DebugP.h>
#include "ti_drivers_config.h"
#include "ti_drivers_open_close.h"
#include "ti_board_open_close.h"
#include <drivers/hw_include/tistdtypes.h>

/* Number of Word count */
#define APP_MCSPI_MSGSIZE                   (5U)
#define APP_MCSPI_TRANSFER_LOOPCOUNT        (10U)

/* Global variables */
uint8_t     gMcspiTxBuffer[APP_MCSPI_MSGSIZE];
uint8_t     gMcspiRxBuffer[APP_MCSPI_MSGSIZE];
uint32_t    gChEnableRegVal, gChDisableRegVal;
uint32_t    gCsAssertRegVal, gCsDeAssertRegVal;

static void mcspi_low_latency_transfer_8bit(uint32_t baseAddr,
                                            uint32_t  chNum,
                                            uint8_t   *txBuff,
                                            uint8_t   *rxBuff,
                                            uint32_t  length,
                                            uint32_t  dataWidth);

void *mcspi_performance_main(void *args)
{
    uint32_t            i, j;
    uint32_t            dataLength, dataWidth, bitRate, bufWidthShift;
    uint32_t            baseAddr, chNum;
    uint32_t            chCtrlRegVal, chConfRegVal;
    uint32_t            startTimeInUSec, elapsedTimeInUsecs;

    Drivers_open();
    Board_driversOpen();

    DebugP_log("[MCSPI] Performance Example Started...\r\n\n");

    /* Memfill buffers */
    for(i = 0U; i < APP_MCSPI_MSGSIZE; i++)
    {
        gMcspiTxBuffer[i] = i;
        gMcspiRxBuffer[i] = 0;
    }

    /* Initialize SPI Channel Number */
    chNum  = gConfigMcspi0ChCfg[0U].chNum;

    /* Get SPI Channel Info */
    baseAddr = MCSPI_getBaseAddr(gMcspiHandle[CONFIG_MCSPI0]);
    DebugP_assert(baseAddr != 0U); /* MCSPI baseAddr Invalid!! */

    /* Set dataWidth */
    dataWidth  = 8;
    MCSPI_setDataWidth(baseAddr, chNum, dataWidth);

    /* Enable the transmitter FIFO of McSPI peripheral. */
    MCSPI_enableTxFIFO(baseAddr, chNum, MCSPI_TX_FIFO_ENABLE);

    /* Disable the receiver FIFO of McSPI peripheral for Tx only mode. */
    MCSPI_enableRxFIFO(baseAddr, chNum, MCSPI_RX_FIFO_ENABLE);

    /*
     * Channel Control and config registers are updated after Open/Reconfigure.
     * Channel enable/disable and CS assert/deassert require updation of bits in
     * control and config registers. Also these registers will not be updated
     * during data transfer. So reg read modify write operations can be updated
     * to write only operations.
     * Store ch enable/disable reg val and cs assert/deassert reg vals.
     */
    chCtrlRegVal     = MCSPI_readChCtrlReg(baseAddr, chNum);
    gChEnableRegVal  = chCtrlRegVal | CSL_MCSPI_CH0CTRL_EN_MASK;
    gChDisableRegVal = chCtrlRegVal & (~CSL_MCSPI_CH0CTRL_EN_MASK);

    chConfRegVal      = MCSPI_readChConf(baseAddr, chNum);
    gCsAssertRegVal   = chConfRegVal | CSL_MCSPI_CH0CONF_FORCE_MASK;
    gCsDeAssertRegVal = chConfRegVal & (~CSL_MCSPI_CH0CONF_FORCE_MASK);

    bitRate    = gConfigMcspi0ChCfg[0U].bitRate;
    /* Number of Word Count */
    dataLength = APP_MCSPI_MSGSIZE;

    /*  Calculate buffer width shift value.
     *  When dataWidth <= 8,           bufWidth = uint8_t  (1 byte - 0 shift)
     *  When dataWidth > 8  && <= 16,  bufWidth = uint16_t (2 bytes - 1 shift)
     *  When dataWidth > 16 && <= 32,  bufWidth = uint32_t (4 bytes - 2 shift)
     */
    bufWidthShift = MCSPI_getBufWidthShift(dataWidth);
    /* Initiate transfer */
    startTimeInUSec = ClockP_getTimeUsec();
    for(j = 0U; j < APP_MCSPI_TRANSFER_LOOPCOUNT; j++)
    {
        mcspi_low_latency_transfer_8bit(baseAddr, chNum,
                                   &gMcspiTxBuffer[0], &gMcspiRxBuffer[0], dataLength, bufWidthShift);
    }
    elapsedTimeInUsecs = ClockP_getTimeUsec() - startTimeInUSec;

    for (j = 0U; j < dataLength; j++) {
        if (gMcspiTxBuffer[j] != gMcspiRxBuffer[j])
        {
            DebugP_log("mcspi_low_latency_transfer_8bit: Data Mismatch at offset %d \r\n", j);
            break;
        }
    }

    DebugP_log("----------------------------------------------------------\r\n");
    DebugP_log("McSPI Clock %d Hz\r\n", bitRate);
    DebugP_log("----------------------------------------------------------\r\n");
    DebugP_log("Data Width \tData Length \tTransfer Time (micro sec)\r\n");
    DebugP_log("%u\t\t%u\t\t%5.2f\r\n", dataWidth, dataLength,
                        (Float32)(elapsedTimeInUsecs / (uint32_t) APP_MCSPI_TRANSFER_LOOPCOUNT));
    DebugP_log("----------------------------------------------------------\r\n\n");
    DebugP_log("All tests have passed!!\r\n");

    Board_driversClose();
    Drivers_close();

    return NULL;
}

static void mcspi_low_latency_transfer_8bit(uint32_t baseAddr,
                                            uint32_t chNum,
                                            uint8_t  *txBuff,
                                            uint8_t  *rxBuff,
                                            uint32_t length,
                                            uint32_t bufWidthShift)
{
    /* Effective FIFO depth in bytes(64/32/16) depending on datawidth */
    uint32_t effTxFifoDepth = (uint32_t)((uint32_t) MCSPI_FIFO_LENGTH / 2U) >> bufWidthShift;
    uint32_t i, numWordsWritten = 0U, numWordsRead = 0U, transferLengthTx = length, transferLengthRx = length;
    uint8_t  *rxBuffer = rxBuff;
    uint8_t  *txBuffer = txBuff;

    /* Enable the McSPI channel for communication.*/
    /* Updated for write only operation. */
    MCSPI_writeChCtrlReg(baseAddr, chNum, gChEnableRegVal);

    /* SPIEN line is forced to low state.*/
    /* Updated for write only operation. */
    MCSPI_writeChConfReg(baseAddr, chNum, gCsAssertRegVal);

    while (transferLengthTx != 0U)
    {
        if (0U != (MCSPI_readChStatusReg(baseAddr, chNum) &
                        CSL_MCSPI_CH0STAT_TXFFE_MASK))
        {
            /* Write Effective TX FIFO depth */
            if (transferLengthTx >= effTxFifoDepth)
            {
                transferLengthTx = effTxFifoDepth;
            }
            /* Write the data in Tx FIFO. */
            for (i = 0U; i < transferLengthTx; i++)
            {
                MCSPI_writeTxDataReg(baseAddr, (uint8_t) *txBuffer, chNum);
                txBuffer++;
            }
            numWordsWritten  += transferLengthTx;
            transferLengthTx    = length - numWordsWritten;
        }
        if (0U != (MCSPI_readChStatusReg(baseAddr, chNum) &
                        CSL_MCSPI_CH0STAT_RXFFF_MASK))
        {
            /* Write Effective TX FIFO depth */
            if (transferLengthRx >= effTxFifoDepth)
            {
                transferLengthRx = effTxFifoDepth;
            }
            /* Write the data in Tx FIFO. */
            for (i = 0; i < transferLengthRx; i++)
            {
                *rxBuffer = (uint8_t) MCSPI_readRxDataReg(baseAddr, chNum);
                rxBuffer++;
            }
            numWordsRead  += transferLengthRx;
            transferLengthRx    = length - numWordsRead;
        }
    }

    while (0U == (MCSPI_readChStatusReg(baseAddr, chNum) &
                    CSL_MCSPI_CH0STAT_TXFFE_MASK))
    {
        /* Wait fot Tx FIFO to be empty for the last set of data. */
    }
    /* read out all the remaining bytes in RXS */
    if (0U != (MCSPI_readChStatusReg(baseAddr, chNum) &
                    CSL_MCSPI_CH0STAT_RXS_MASK))
    {
            if (transferLengthRx >= effTxFifoDepth)
            {
                transferLengthRx = effTxFifoDepth;
            }
            /* Write the data in Tx FIFO. */
            for (i = 0U; i < transferLengthRx; i++)
            {
                *rxBuffer = (uint8_t) MCSPI_readRxDataReg(baseAddr, chNum);
                rxBuffer++;
            }
    }

    while (0U == (MCSPI_readChStatusReg(baseAddr, chNum) &
                    CSL_MCSPI_CH0STAT_EOT_MASK))
    {
        /* Tx FIFO Empty is triggered when last word from FIFO is written to
           internal shift register. SO wait for the end of transfer of last word.
           The EOT gets set after every word when the transfer from shift
           register is complete and is reset when the transmission starts.
           So FIFO empty check is required to make sure the data in FIFO is
           sent out then wait for EOT for the last word. */
    }

    /* Force SPIEN line to the inactive state.*/
    /* Updated for write only operation. */
    MCSPI_writeChConfReg(baseAddr, chNum, gCsDeAssertRegVal);

    /* Disable the McSPI channel.*/
    /* Updated for write only operation. */
    MCSPI_writeChCtrlReg(baseAddr, chNum, gChDisableRegVal);
}
