# Release Notes 08.05.00 {#RELEASE_NOTES_08_05_00_PAGE}

[TOC]

\attention Also refer to individual module pages for more details on each feature, unsupported features, important usage guidelines.

\note The examples will show usage of SW modules and APIs on a specific CPU instance and OS combination. \n
      Unless explicitly noted otherwise, the SW modules would work in both FreeRTOS and no-RTOS environment. \n
      Unless explicitly noted otherwise, the SW modules would work on any of the R5F's present on the SOC. \n

\attention Klockwork Static Analysis report is not updated for this release

## New in this Release

Feature                                                                                         | Module
------------------------------------------------------------------------------------------------|-----------------------------------
Crypto driver support AES, SHA, HMAC, RSA, EDSA                                                 | Security
HSM support for HS-FS device                                                                    | Security
Added get version example for HSM Client                                                        | Security
ADC examples - Burst mode, differencial mode, synchronous SW trigger, interleaved averaging      | ADC
ADC examples - continous self trigger, EPWM triggered conversion, Oversampling                  | ADC
MMCSD driver with raw IO support                                                                | MMCSD
Ethernet (CPSW) dual mac/multiple netif support                                                 | Ethernet
Ethernet Bare metal support                                                                     | Ethernet
Enet (CPSW) SysConfig support for MDIO, MAC PORT, ALE, Phy configurations etc                   | Ethernet
PTP Timesync demo support in \ref EXAMPLES_ENET_LAYER2_MULTI_CHANNEL_PTP example                    | Ethernet
Software Diagnostics Library                                                                    | SDL


## Device and Validation Information

\cond SOC_AM263X
SOC   | Supported CPUs  | EVM                                                                          | Host PC
------|-----------------|------------------------------------------------------------------------------|-----------------------------------------
AM263x| R5F             | AM263x ControlCard Revision E1  (referred to as am263x-cc in code). \n       | Windows 10 64b or Ubuntu 18.04 64b
AM263x| R5F             | AM263x LaunchPad Revision E1  (referred to as am263x-lp in code)             | Windows 10 64b or Ubuntu 18.04 64b
\endcond

## Dependent Tools and Compiler Information

Tools                   | Supported CPUs | Version
------------------------|----------------|-----------------------
Code Composer Studio    | R5F            | 12.1.0
SysConfig               | R5F            | 1.14.0 build, build 2667
TI ARM CLANG            | R5F            | 2.1.2.LTS
FreeRTOS Kernel         | R5F            | 10.4.3
LwIP                    | R5F            | STABLE-2_1_2_RELEASE

\attention TI ARM CLANG 2.1.2.LTS is not part of CCS by default, Follow steps at \ref INSTALL_TIARMCLANG to install the compiler

## Key Features

### Experimental Features

\attention Features listed below are early versions and should be considered as "experimental".
\attention Users can evaluate the feature, however the feature is not fully tested at TI side.
\attention TI would not support these feature on public e2e.
\attention Experimental features will be enabled with limited examples and SW modules.

Feature                                                             | Module
--------------------------------------------------------------------|--------------------------
GUI for UART Uniflash Tool                                          | Bootloader

### OS Kernel

OS              | Supported CPUs  | SysConfig Support | Key features tested                                             | Key features not tested / NOT supported
----------------|-----------------|-------------------|-----------------------------------------------------------------|----------------------------------------
FreeRTOS Kernel | R5F             | NA                | Task, Task notification, interrupts, semaphores, mutexs, timers | Task load measurement using FreeRTOS run time statistics APIs. Limited support for ROV features.
FreeRTOS POSIX  | R5F             | NA                | pthread, mqueue, semaphore, clock                               | -
NO RTOS         | R5F             | NA                | See **Driver Porting Layer (DPL)** below                        | -

### Driver Porting Layer (DPL)

Module            | Supported CPUs  | SysConfig Support | OS support       | Key features tested                                           | Key features not tested / NOT supported
------------------|-----------------|-------------------|------------------|---------------------------------------------------------------|----------------------------------------
Cache             | R5F             | YES               | FreeRTOS, NORTOS | Cache write back, invalidate, enable/disable                  | -
Clock             | R5F             | YES               | FreeRTOS, NORTOS | Tick timer at user specified resolution, timeouts and delays  | -
CpuId             | R5F             | NA                | FreeRTOS, NORTOS | Verify Core ID and Cluster ID that application is currently running on    | -
CycleCounter      | R5F             | NA                | FreeRTOS, NORTOS | Measure CPU cycles using CPU specific internal counters       | -
Debug             | R5F             | YES               | FreeRTOS, NORTOS | Logging and assert to any combo of: UART, CCS, shared memory  | -
Heap              | R5F             | NA                | FreeRTOS, NORTOS | Create arbitrary heaps in user defined memory segments        | -
Hwi               | R5F             | YES               | FreeRTOS, NORTOS | Interrupt register, enable/disable/restore, Interrupt prioritization                    | -
MPU               | R5F             | YES               | FreeRTOS, NORTOS | Setup MPU and control access to address space                 | -
Semaphore         | R5F             | NA                | FreeRTOS, NORTOS | Binary, Counting Semaphore, recursive mutexs with timeout     | -
Task              | R5F             | NA                | FreeRTOS         | Create, delete tasks                                          | -
Timer             | R5F             | YES               | FreeRTOS, NORTOS | Configure arbitrary timers                                    | -

### Secondary Bootloader (SBL)

Module     | Supported CPUs  | SysConfig Support | OS support       | Key features tested                                                         | Key features not tested / NOT supported
-----------|-----------------|-------------------|------------------|-----------------------------------------------------------------------------|----------------------------------------------------
Bootloader | R5FSS0-0        | YES               | NORTOS           | Boot modes: QSPI, UART. All R5F's. RPRC, multi-core image format            | Force Dual Core Mode, Disable Dual Core Switch and R5SS1 only not tested

### SOC Device Drivers

Peripheral   | Supported CPUs | SysConfig Support | DMA Supported                         | Key features tested                                                                                                                                             | Key features not tested / NOT supported
-------------|----------------|-------------------|---------------------------------------|-----------------------------------------------------------------------------------------------------------------------------------------------------------------|---------------------------------------------------
ADC          | R5F            | YES               | Yes. Example:  adc_soc_continuous_dma | Single software triggered conversion, Multiple ADC trigger using PWM, Result read using DMA, EPWM trip through PPB limit, PPB limits, PPB offsets, burst mode oversampling, differential mode, Offset, EPWM triggered conversion                | -
Bootloader   | R5F            | YES               | Yes. DMA enabled for SBL QSPI         | Boot modes: QSPI, UART. All R5F's                                                                                                                               | -
CMPSS        | R5F            | YES               | NA                                    | Asynchronous PWM trip                                                                                                                                           | -
CPSW         | R5F            | YES               | No                                    | MAC loopback, PHY loopback, LWIP: Getting IP, Ping, Iperf, Layer 2 MAC, Layer 2 PTP Timestamping and CPSW Switch support                                        | -
DAC          | R5F            | YES               | Yes. Example: dac_sine_dma            | Constant voltage, Square wave generation, Sine wave generation with and without DMA, Ramp wave generation, Random Voltage generation                            | -
ECAP         | R5F            | YES               | No                                    | ECAP APWM mode, PWM capture                                                                                                                                     | -
EDMA         | R5F            | YES               | NA                                    | DMA transfer using interrupt and polling mode, QDMA Transfer, Channel Chaining, PaRAM Linking                                                                   | -
EPWM         | R5F            | YES               | Yes. Example: epwm_dma                | PWM outputs A and B in up-down count mode, Trip zone, Update PWM using EDMA, Valley switching, High resolution time period adjustment, type5 feature            | -
EQEP         | R5F            | YES               | NA                                    | Speed and Position measurement.                                                                                                                                 | Frequency Measurement not tested
FSI          | R5F            | YES               | Yes. Example: fsi_loopback_dma        | RX, TX, polling, interrupt mode, Dma, single lane loopback.                                                                                                     | - FSI Spi Mode
GPIO         | R5F            | YES               | NA                                    | Output, Input and Interrupt functionality                                                                                                                       | -
I2C          | R5F            | YES               | No                                    | Controller mode, basic read/write                                                                                                                                   | -
IPC Notify   | R5F            | YES               | NA                                    | Mailbox functionality, IPC between RTOS/NORTOS CPUs                                                                                                             | M4F core
IPC Rpmsg    | R5F            | YES               | NA                                    | RPMessage protocol based IPC                                                                                                                                    | M4F core
MCAN         | R5F            | YES               | No                                    | RX, TX, interrupt and polling mode, Corrupt Message Transmission Prevention, Error Passive state, Bus Off State, Bus Monitoring Mode                            | -
MCSPI        | R5F            | YES               | Yes. Example: mcspi_loopback_dma      | Controller/Peripheral mode, basic read/write, polling, interrupt and DMA mode                                                                                            | -
MDIO         | R5F            | YES               | NA                                    | Register read/write, link status and link interrupt enable API                                                                                                  | -
MPU Firewall | R5F            | YES               | NA                                    | Only compiled (Works only on HS-SE  device)                                                                                                                     | -
MMCSD        | R5F            | YES               | NA                                    | MMCSD 4bit, Raw read/write                                                                                                  | - file IO, eMMC
PINMUX       | R5F            | YES               | NA                                    | Tested with multiple peripheral pinmuxes                                                                                                                        | -
PRUICSS      | R5F            | YES               | NA                                    | Tested with Ethercat FW HAL                                                                                                                                     | -
QSPI         | R5F            | YES               | Yes. Example: qspi_flash_dma_transfer | Read direct, Write indirect, Read/Write commands, DMA for read                                                                                                  | -
RTI          | R5F            | YES               | No                                    | Counter read, timebase selction, comparator setup for Interrupt, DMA requests                                                                                   | Capture feature, fast enabling/disabling of events not tested
SDFM         | R5F            | YES               | No                                    | Filter data read from CPU, Filter data read with PWM sync                                                                                                       | -
SOC          | R5F            | YES               | NA                                    | Lock/unlock MMRs, clock enable, set Hz, Xbar configuration, SW Warm Reset, Address Translation                                                                  | -
SPINLOCK     | R5F            | NA                | NA                                    | Lock, unlock HW spinlocks                                                                                                                                       | -
UART         | R5F            | YES               | Yes. Example: uart_echo_dma           | Basic read/write at baud rate 115200, polling, interrupt mode                                                                                                   | HW flow control not tested, DMA mode not supported
WATCHDOG     | R5F            | YES               | NA                                    | Reset mode                                                                                                                                                      | Interrupt mode

### Board Device Drivers

Peripheral | Supported CPUs | SysConfig Support | Key features tested                                         | Key features not tested
-----------|----------------|-------------------|-------------------------------------------------------------|------------------------
EEPROM     | R5F            | YES               | Only compiled                                               | -
ETHPHY     | R5F            | YES               | Tested with ethercat_slave_beckhoff_ssc_demo example        | -
FLASH      | R5F            | YES               | QSPI Flash                                                  | -
LED        | R5F            | YES               | GPIO                                                        | -


### CMSIS

Module                      | Supported CPUs | SysConfig Support | OS Support        | Key features tested                                                                         | Key features not tested
----------------------------|----------------|-------------------|-------------------|---------------------------------------------------------------------------------------------|------------------------
-                           | -              | -                 | -                 | -                                                                                           |  -

### Industrial Communications Toolkit

Module                                | Supported CPUs | SysConfig Support | OS Support  | Key features tested                                                                                      | Key features not tested
--------------------------------------|----------------|-------------------|-------------|----------------------------------------------------------------------------------------------------------|------------------------
EtherCAT SubDevice FWHAL                  | R5F            | NO                | FreeRTOS    | Tested with ethercat_slave_beckhoff_ssc_demo example                                                     | Reset isolation

### Motor Control

Module                      | Supported CPUs | SysConfig Support | OS Support        | Key features tested                             | Key features not tested
----------------------------|----------------|-------------------|-------------------|-------------------------------------------------|------------------------
-                           | -              | -                 | -                 | -                                               |  -

### Networking

Module                      | Supported CPUs | SysConfig Support | OS Support  | Key features tested                                                                    | Key features not tested
----------------------------|----------------|-------------------|-------------|----------------------------------------------------------------------------------------|------------------------
LwIP                        | R5F            | YES               | FreeRTOS    | TCP/UDP IP networking stack with and without checksum offload enabled,, TCP/UDP IP networking stack, DHCP, ping, TCP iperf, TCP/UDP IP, scatter-gather                         | Other LwIP features, checksum offload with VLAN_Tag, more robustness tests pending
Ethernet driver (ENET)      | R5F            | YES               | FreeRTOS    | Ethernet as port using CPSW and ICSS,Layer 2 MAC, Layer 2 PTP Timestamping, CPSW Switch, CPSW EST, interrupt pacing, Policer, MDIO Manual Mode | -
ICSS-EMAC                   | R5F            | YES               | FreeRTOS    | Only compiled                                                                          | Not tested


### Demos

Module                      | Supported CPUs | SysConfig Support | OS Support        | Key features tested                             | Key features not tested
----------------------------|----------------|-------------------|-------------------|-------------------------------------------------|------------------------
-                           | -              | -                 | -                 | -                                               |  -

### Safety Diagnostic Library

Module            | Supported CPUs  | SysConfig Support | OS support       | Key features tested                                                                            | Key features not tested / NOT supported
------------------|-----------------|-------------------|------------------|------------------------------------------------------------------------------------------------|----------------------------------------
MCRC              | R5F             | NA                |  NORTOS | Full CPU, Auto CPU Mode                                                          | Semi CPU Auto Mode.
DCC               | R5F             | NA                |  NORTOS | Single Shot and Continuous modes                                    | -
PBIST             | R5F             | NA                |  NORTOS | Memories supported by MSS PBIST controller.          | -
ESM               | R5F             | NA                |  NORTOS | Tested in combination with RTI, DCC                                        | -
RTI               | R5F             | NA                |  NORTOS | WINDOWSIZE_100_PERCENT, WINDOWSIZE_50_PERCENT ,Latency/Propagation timing error(early)(50% window),Latency/Propagation timing error(late)(50% window)                                     | -
ECC               | R5F             | NA                |  NORTOS | ECC of MSS_L2, R5F TCM, MCAN      | R5F Cache, VIM, HSM, ICSSM, CPSW
Bus Safety        | R5F             | NA                |  NORTOS | AHB, AXI, TPTC                           | -
CCM               | R5F             | NA                |  NORTOS | CCM Self Test Mode.                                                 | Error Forcing Mode and Self Test Error Forcing Mode.
R5F STC(LBIST)    | R5F             | NA                |  NORTOS | STC of R5F.                                                 |-
## Fixed Issues

<table>
<tr>
    <th> ID
    <th> Head Line
    <th> Module
    <th> Applicable Releases
    <th> Applicable Devices
    <th> Resolution/Comments
</tr>
<tr>
    <td> MCUSDK-1016
    <td> Semaphore does not function as expected when "post" call is present in multiple ISRs at different priorities
    <td> DPL, FreeRTOS
    <td> 07.03.02 onwards
    <td> AM263x
    <td> Fixed
</tr>
<tr>
    <td> MCUSDK-3797
    <td> Fix for enabling --rom_model linker optiom
    <td> DPL
    <td> 08.01.00 onwards
    <td> AM263x
    <td> Fixed
</tr>
<tr>
    <td> MCUSDK-4059
    <td> FSI first frame transmitted is incorrect in DMA mode
    <td> FSI
    <td> 08.02.00 onwards
    <td> AM263x
    <td> Fixed
</tr>
<tr>
    <td> MCUSDK-6908
    <td> ECAP triggers missing from ADC drivers
    <td> ADC
    <td> 08.03.00 onwards
    <td> AM263x
    <td> Fixed
</tr>
<tr>
    <td> MCUSDK-8079
    <td> EDMA: Aggregated interrupt used instead of Region interrupt
    <td> EDMA
    <td> 08.04.00 onwards
    <td> AM263x
    <td> Fixed
</tr>
<tr>
    <td> MCUSDK-8235
    <td> CMPSS: Example fails - PWM not tripped on LP
    <td> CMPSS
    <td> 08.04.00 onwards
    <td> AM263x
    <td> Fixed
</tr>
<tr>
    <td> MCUSDK-8348
    <td> [Enet] EnetDma_initPktInfo does not initialized chkSumInfo member
    <td> Enet
    <td> 08.04.00 onwards
    <td> AM263x
    <td> Fixed
</tr>
<tr>
    <td> MCUSDK-8394
    <td> Trip zone retains previous code when Advanced option is chosen
    <td> EPWM
    <td> 08.03.00 onwards
    <td> AM263x
    <td> Fixed
</tr>
<tr>
    <td> MCUSDK-8395
    <td> Action Qualifier T1 trigger source and T2 trigger source configuration is being overwritten by syscfg
    <td> EPWM
    <td> 08.03.00 onwards
    <td> AM263x
    <td> Fixed
</tr>
<tr>
    <td> MCUSDK-8396
    <td> [EPWM]EALLOW comments needs to be changed for EPWM_lockRegisters API (C2K feature)
    <td> EPWM
    <td> 08.03.00 onwards
    <td> AM263x
    <td> Fixed
</tr>
<tr>
    <td> MCUSDK-8423
    <td> [enet] MDIO manual mode delay implementation is incorrect
    <td> Enet
    <td> 08.04.00 onwards
    <td> AM263x
    <td> Fixed
</tr>
<tr>
    <td> MCUSDK-8430
    <td> Digital Compare Event CBC Event and Latch is not present in Syscfg
    <td> EPWM
    <td> 08.03.00 onwards
    <td> AM263x
    <td> Fixed
</tr>
<tr>
    <td> MCUSDK-8432
    <td> Digital Compare DCxH(L) Trip syscfg configurable changes
    <td> EPWM
    <td> 08.03.00 onwards
    <td> AM263x
    <td> Fixed
</tr>
<tr>
    <td> MCUSDK-8460
    <td> hardcoded sysconfig for HSM Server not allowing to change the Secure Controller for HSM
    <td> Security
    <td> 08.03.00 onwards
    <td> AM263x
    <td> Fixed
</tr>
<tr>
    <td> MCUSDK-8467
    <td> EQEP strobe not OR'd with EPWM ADCSOCA/B
    <td> EQEP
    <td> 08.04.00 onwards
    <td> AM263x
    <td> Fixed
</tr>
<tr>
    <td> MCUSDK-8554
    <td> [Eqep]Compiler error when EQEP is configured for 2 instances
    <td> EQEP
    <td> 08.04.00 onwards
    <td> AM263x
    <td> Fixed
</tr>
<tr>
    <td> MCUSDK-8616
    <td> Changing RGMII mode in CPSW lwip example leads to assert
    <td> Enet
    <td> 08.04.00 onwards
    <td> AM243x, AM64x
    <td> Fixed
</tr>
<tr>
    <td> MCUSDK-8731
    <td> "[ERROR] XMODEM recv failed" during SBL UART boot across multiple examples
    <td> Enet, IPC, SBL
    <td> 08.04.00 onwards
    <td> AM243x, AM64x
    <td> Fixed
</tr>
</table>


## Known Issues
<table>
<tr>
    <th> ID
    <th> Head Line
    <th> Module
    <th> Reported in release
    <th> Workaround
</tr>
<tr>
    <td> MCUSDK-2294
    <td> GPIO Pin Direction
    <td> GPIO. GPIO Pin Direction not getting automatically configured.
    <td> 8.0.0
    <td> Use GPIO_setDirMode to set pin direction for GPIO pin.
</tr>
<tr>
    <td> MCUSDK-2557
    <td> eqep_frequency_measurement example is failing
    <td> SBL
    <td> 8.2.0
    <td> -
</tr>
<tr>
    <td> MCUSDK-4234
    <td> FSI RX Generic Trigger Test is not working
    <td> FSI
    <td> 8.3.0
    <td> -
</tr>
<tr>
    <td> SITARAAPPS-2040
    <td> Dual Core configuration issue with CSP 1.1.3 (Sitara MCU Device Support) on AM263x
    <td> CSP Gel Scripts
    <td> 8.2.1
    <td> Edit gel file as mentioned in  \ref PREREQUISITES while running  multi core applications.
</tr>
<tr>
    <td> MCUSDK-6909
    <td> EPWM: Emulation mode doesn't work
    <td> EPWM
    <td> 8.4.0
    <td> -
</tr>
<tr>
    <td> MCUSDK-7030
    <td> Interrupt nesting is not functional as expected when you have 2 or more interrupts with different priorities
    <td> MCAN
    <td> 8.4.0
    <td> Keep the interrupt priority same in system
</tr>
<tr>
    <td> MCUSDK-7319
    <td> CONTROLSS-SDFM: Two Back-to-Back Writes to SDCPARMx Register Bit Fields CEVT1SEL, CEVT2SEL, and HZEN Within Three SD-Modulator Clock Cycles can Corrupt SDFM State Machine, Resulting in Spurious Comparator Events
    <td> SDFM
    <td> 8.4.0
    <td> Avoid back-to-back writes within three SD-modulator clock cycles or have the SDCPARMx register bit fields configured in one register write.
</tr>
<tr>
    <td> MCUSDK-7811
    <td> CPSW: Ethernet Packet corruption occurs if CPDMA fetches a packet which spans across memory banks
    <td> CPSW
    <td> 8.3.0 onwards
    <td> Ensure from application side single ethernet packet does not span across memory banks.
</tr>
<tr>
    <td> MCUSDK-7915
    <td> SDFM: EPWM filter sync example does not configure and check the PWM synchronization
    <td> SDFM
    <td> 8.3.0 onwards
    <td> None
</tr>
<tr>
    <td> MCUSDK-8072
    <td> EnetBoard_setupPorts does not provide config option to enable internal delay for RGMII
    <td> Enet
    <td> 8.4.0 onwards
    <td> None
</tr>
<tr>
    <td> MCUSDK-8073
    <td> UART1 not working as expected while configuring two uarts i.e UART0 and UART1 for two different cores
    <td> UART
    <td> 8.4.0 onwards
    <td> UART1 configuration from other core should be done after UART0 is configured and initialized
</tr>
<tr>
    <td> MCUSDK-8348
    <td> EnetDma_initPktInfo does not initialized chkSumInfo member
    <td> Enet
    <td> 8.4.0 onwards
    <td> All L2 based applications need to explicitly set EnetDma_initPktInfo.chkSumInfo = 0
</tr>
<tr>
    <td> MCUSDK-8391
    <td> PRU Pin Mux configuration missing in syscfg am263x
    <td> PRU
    <td> 8.4.0
    <td> -
</tr>
<tr>
    <td> MCUSDK-8403
    <td> 1000000(1MHz) baud rate not working on UART
    <td> UART
    <td> 8.4.0
    <td> -
</tr>
<tr>
    <td> MCUSDK-8825
    <td> MCAN bit timing parameters not correct in Sysconfig generated code
    <td> MCAN
    <td> 8.4.0
    <td> -
</tr>
<tr>
    <td> MCUSDK-8974
    <td> LIN External Example is only sending the LIN ID and no packet data
    <td> LIN
    <td> 8.4.0
    <td> -
</tr>
<tr>
    <td> MCUSDK-8989
    <td> WDT Reset example takes more than expiration time to reset.
    <td> WDT
    <td> 8.5.0
    <td> None
</tr>
    <td> PROC_SDL-4558
    <td> Binary generated from MSS ECC CCS based example(sdl_ecc_r5_atcm0) does not work.
    <td> SDL
    <td> 8.5.0 onwards
    <td> Add the resetvecs.S manually in to CCS project / Use the binary generated from gmake.
</tr>
<tr>
    <td> PROC_SDL-4749
    <td> AXI DED Bus Safety fail.
    <td> SDL
    <td> 8.5.0 onwards
    <td> None.
</tr>
<tr>
    <td> PROC_SDL-4751
    <td> In CCM mode, only self test mode tested.Error Forcing Mode and Self Test Error Forcing Mode are not yet supported.
    <td> SDL
    <td> 8.5.0 onwards
    <td> None.
</tr>
<tr>
    <td> PROC_SDL-4755
    <td> MCRC Semi CPU mode is not supported.
    <td> SDL
    <td> 8.5.0 onwards
    <td> Use Full CPU mode ot Auto mode.
</tr>
</table>

## Limitations

<table>
<tr>
    <th> ID
    <th> Head Line
    <th> Module
    <th> Reported in release
    <th> Workaround
</tr>
<tr>
    <td> -
    <td> -
    <td> -
    <td> -
    <td> -
</tr>
</table>

## Upgrade and Compatibility Information

### Compiler Options

<table>
<tr>
    <th> Module
    <th> Affected API
    <th> Change
    <th> Additional Remarks
</tr>
</table>

### SOC Device Drivers

<table>
<tr>
    <th> Module
    <th> Affected API
    <th> Change
    <th> Additional Remarks
</tr>
<tr>
    <td> EPWM sysconfig
    <td> Following configurables - \n
    Under 'EPWMXA OutputOutput Configuration' \n
    EPWMXA T1 Trigger Source (epwmActionQualifier_EPWM_AQ_OUTPUT_A_t1Source) \n
    EPWMXA T2 Trigger Source (epwmActionQualifier_EPWM_AQ_OUTPUT_A_t2Source) \n
    Under 'EPWMXB OutputOutput Configuration' \n
    EPWMXB T1 Trigger Source (epwmActionQualifier_EPWM_AQ_OUTPUT_B_t1Source) \n
    EPWMXB T2 Trigger Source (epwmActionQualifier_EPWM_AQ_OUTPUT_B_t2Source) \n
    <td> Renamed from epwmActionQualifier_EPWM_AQ_OUTPUT_A_t1Source to epwmActionQualifier_t1Source
    and epwmActionQualifier_EPWM_AQ_OUTPUT_A_t2Source to epwmActionQualifier_t2Source. \n
    Removed epwmActionQualifier_EPWM_AQ_OUTPUT_B_t1Source and epwmActionQualifier_EPWM_AQ_OUTPUT_B_t2Source
    <td> Action Qualifier has T1 and T2 trigger source. This is independent of EPWMA and EPWMB. Up to 8.4, EPWM sysconfig module supported EPWM_A/EPWM_B dependent configurables (which is redundant) for T1 and T2 trigger source. \n
    Modify the SysCfg file as follows: \n
    Search and replace \n
    epwmActionQualifier_EPWM_AQ_OUTPUT_A_t1Source with epwmActionQualifier_t1Source \n
    epwmActionQualifier_EPWM_AQ_OUTPUT_A_t2Source with epwmActionQualifier_t2Source \n
    Remove \n
    epwmActionQualifier_EPWM_AQ_OUTPUT_B_t1Source and epwmActionQualifier_EPWM_AQ_OUTPUT_B_t2Source
</tr>
</table>

### Networking

<table>
<tr>
    <th> Module
    <th> Affected API
    <th> Change
    <th> Additional Remarks
</tr>
<tr>
    <td> ENET
    <td> SysConfig
    <td> For all LWIP based examples, packet buffer memory management is moved out of driver and expected to handle at application side. This requires user to regenerate the code via SysConfig
    <td> This is applicable only for RX, TX remains same. Custom pBuff enabled LwIP application to manage the buffer memory efficiently
</tr>
<tr>
    <td> ENET, LWIP
    <td> Library Names
    <td> Library 'enet-lwip-cpsw' is split into 'enet-cpsw' and 'lwipif-cpsw-freertos'. 'lwip-contrib' is renamed to 'lwip-contrib-freertos'
    <td> Additionally while using NoRTOS (baremetal) based examples nortos version of 'lwipif-cpsw' and 'lwip-contrib' needs to be used
</tr>
</table>
