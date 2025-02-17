# Ethernet TSN gPTP on CPSW along with LwIP stack {#EXAMPLES_ENET_CPSW_TSN_LWIP_GPTP}

[TOC]

# Introduction
This ethernet TSN example illustrates the usage of gPTP IEEE 802.1AS stack with CPSW peripheral with LwIP network stack running in parallel. Here we demonstrate a simple TCP echo server using netconn APIs with the LwIP stack.
On the gPTP front, the application used here supports all the below modes:
    - gPTP End-Point time_transmitter mode (i.e. master mode)
    - gPTP End-Point time_receiver mode (i.e. slave mode)
    - gPTP Bridge mode

In this example, We use two Rx and two Tx DMA channel, one Rx and one Tx channel specific to gPTP traffic and one Rx and one Tx channel specific to non-gPTP traffic, this is handled by LwIP stack.

  \imageStyle{gPTPwithLwIP.png,width:20%}
  \image html gPTPwithLwIP.png 

Please refer to the \ref EXAMPLES_ENET_LWIP_CPSW_TCPSERVER page for LwIP TCP echo server documentation.

Please refer to the \ref EXAMPLES_ENET_CPSW_TSN_GPTP page for the above gPTP mode specific documentation.

See also :\ref ENET_CPSW_TSN_GPTP for gPTP stack documentation.



# Supported Combinations

\cond SOC_AM64X

 Parameter      | Value
 ---------------|-----------
 CPU + OS       | r5fss0-0_freertos
 Toolchain      | ti-arm-clang
 Board          | @VAR_BOARD_NAME_LOWER
 Example folder | examples/networking/tsn/gptp_lwip_cpsw

\endcond

\cond SOC_AM243X

 Parameter      | Value
 ---------------|-----------
 CPU + OS       | r5fss0-0_freertos
 Toolchain      | ti-arm-clang
 Boards         | @VAR_BOARD_NAME_LOWER, @VAR_LP_BOARD_NAME_LOWER
 Example folder | examples/networking/tsn/gptp_lwip_cpsw

\endcond

\cond SOC_AM263X

 Parameter      | Value
 ---------------|-----------
 CPU + OS       | r5fss0-0_freertos
 Toolchain      | ti-arm-clang
 Boards         | @VAR_BOARD_NAME_LOWER, @VAR_LP_BOARD_NAME_LOWER
 Example folder | examples/networking/tsn/gptp_lwip_cpsw

\endcond

\cond SOC_AM263PX

 Parameter      | Value
 ---------------|-----------
 CPU + OS       | r5fss0-0_freertos
 Toolchain      | ti-arm-clang
 Boards         | @VAR_BOARD_NAME_LOWER, @VAR_LP_BOARD_NAME_LOWER
 Example folder | examples/networking/tsn/gptp_lwip_cpsw

\endcond

# Steps to Run the Example

## Prerequisites
-  EVM Board
- Cat6 ethernet cable
- PC with Linux Ubuntu OS (or any PC running bash shell) and PTP capable network card
- Install `linuxptp`
\code
$ sudo apt-get install linuxptp
$ ptp4l -v
\endcode
- Configure linuxptp
\code
$ wget https://raw.githubusercontent.com/richardcochran/linuxptp/master/configs/gPTP.cfg -O ~/gptp_config.cfg
\endcode
\attention Change the value of `priority1` in ~/gptp_config.cfg file to `100`, to enforce it to gPTP master

\attention For some network cards, there is a bug with internal propagation delay. So, in those cases you might need to increase the `neighborPropDelayThresh` in ptp_config.cfg as below-

\code
$ cat ~/gptp_config.cfg
\#
\# 802.1AS example configuration containing those attributes which
\# differ from the defaults.  See the file, default.cfg, for the
/# complete list of available options.
/#
[global]
gmCapable               1
priority1               100
priority2               248
logAnnounceInterval     0
logSyncInterval         -3
syncReceiptTimeout      3
neighborPropDelayThresh 10000
min_neighbor_prop_delay -20000000
assume_two_step         1
path_trace_enabled      1
follow_up_info          1
transportSpecific       0x1
ptp_dst_mac             01:80:C2:00:00:0E
network_transport       L2
delay_mechanism         P2P

\endcode
- `priority1` in ~/gptp_config.cfg file can be changed to make the Linux PC master or slave. Lower the number higher the priority to become master.

Please refer to the \ref EXAMPLES_ENET_LWIP_CPSW_TCPSERVER page to start a TCP client on PC.

## To Configure Static IP
Please refer to \ref NETWORKING_LWIP_STATIC_IP.

## Build the example

- When using CCS projects to build, import the CCS project for the required combination
  and build it using the CCS project menu (see \ref CCS_PROJECTS_PAGE).
- When using makefiles to build, note the required combination and build using
  make command (see \ref MAKEFILE_BUILD_PAGE)

## HW Setup

\note Make sure you have setup the EVM with cable connections as shown here, \ref EVM_SETUP_PAGE.
      In addition do below steps.

\cond SOC_AM64X

### AM64X-EVM

#### For CPSW based example

- Connect a ethernet cable to the EVM from host PC as shown below

  \imageStyle{am64x_evm_lwip_example_00.png,width:30%}
  \image html am64x_evm_lwip_example_00.png Ethernet cable for CPSW based ethernet

\endcond

\cond SOC_AM243X

### AM243X-EVM

#### For CPSW based example

- Connect a ethernet cable to the EVM from host PC as shown below.

  \imageStyle{am64x_evm_lwip_example_00.png,width:30%}
  \image html am64x_evm_lwip_example_00.png Ethernet cable for CPSW based ethernet

### AM243X-LP

\note AM243X-LP has two ethernet Ports which can be configured as both CPSW/ICSS ports.

#### For CPSW based examples

- Connect a ethernet cable to the EVM from host PC as shown below.

  \imageStyle{am243x_lp_lwip_example_00.png,width:30%}
  \image html am243x_lp_lwip_example_00.png Ethernet cable for CPSW based ethernet

\endcond

### PPS Output
PPS output is a square wave signal generated by the device. It is used for comparison between the TT (master) and TR( slave) nodes when they are time synchronized.
On the device, PPS is generated on the SYNC_OUT signal pin from CPTS (Common Platform Time Synchronization) of CPSW.

Out of box configuration for PPS signal output of this example is as follow:

| EVM | Mapped Signal Name | SOC Pin Name | PPS frequency | Output |
|---------|---------------|------------|-------------------------|-------------|
| am64x-EVM | PRG0_PRU0_GPO17 | U1 | 3.814 KHz | Pin B8 on J2 (i.e PIN8 on J2B) connector |
| am243x-EVM | PRG0_PRU0_GPO17 | U1 | 3.814 KHz | Pin B8 on J2 (i.e PIN8 on J2B) connector |
| am243x-LP | MMC1_DAT2 | K20 | 3.814 KHz | Pin3 on J6 connector |
| am263x-CC | SFDM0_CLK1 | A16 | 3.814 KHz | Pin4 on J6 connector | 
| am263x-LP | SFDM0_CLK1 | A16 | 3.814 KHz | Pin4 on J6 connector |

To set/modify configuration of PPS signal , you may follow the below steps:

  1. Configure the bitSelect in EnetApp_enableTsSync() function in tsnapp_cpsw_main.c file. If bit n is selected, 2^(n+1) nano seconds is the time period of the square wave. Please note bitSelect starts from bit 17 which corresponds to 3.814 KHz.
  2. Configure pinmux for PPS Output signal under ENET(CPSW)->'pinmux Config'->'CPTS0_TS_SYNC(CPTS0_TS_SYNC)' and select the appropriate pin as per your EVM.
    \imageStyle{gptp_pps_out_syscfg_gui1.png,width:50%}
    \image html gptp_pps_out_syscfg_gui1.png  **Figure**: Syscfg tool CPSW pinmux changes to select PPS signal pin
    
  3. Signal is generated on the above configured PIN. You may connect oscilloscope on the pin to visualize and compare.
One sample signal captured using oscilloscope. Blue from gPTP TT (master) and purple is from gPTP TR (slave)
    \imageStyle{gptp_pps_out_scope_capture.jpg,width:50%}
    \image html gptp_pps_out_scope_capture.jpg **Figure**: Signal captured on oscilloscope

## Create a network between EVM and host PC
EVM and PC has to connected directly as shown below using CAT6 or CAT5 cable. If there is ethernet switch placed in between, make sure the switch is gPTP capable. Connect both EVM and PC to a DHCP server to get the IP addressess. 
  \imageStyle{Lwip-gPTP-setup.png,width:30%}
  \image html Lwip-gPTP-setup.png Local network between PC and EVM
 
PORT1 instead of PORT0 on EVM can be used as well.

## Run the example
  
\attention If you need to reload and run again, a CPU power-cycle is MUST

- Execute the below command in PC terminal to start the gPTP on linux: 
\code
$ sudo ptp4l -i eno1 -m -l 6 -q -f ~/gptp_config.cfg
\endcode
Replace eno1 with the network interface connected to your PC.

- After the evm gets the IP address, you can ping the IP address to verify the connection.
- Execute the below command in PC terminal to start a TCP communication with the EVM.
\code
ncat <ip-address-of-evm> 8888
\endcode
- You will see logs in the UART terminal as shown in the next section. PC side logs are with Intel i210 card.
- Launch a CCS debug session and run the example executable, see \ref CCS_LAUNCH_PAGE
- Connect board and PC as mentioned in "HW Setup" above.

## Sample Log Output
### PC Output
\code
$ sudo ptp4l -i eno1 -m -l 6 -q -f ~/gptp_config.cfg
ptp4l[7727.045]: selected /dev/ptp0 as PTP clock
ptp4l[7727.088]: port 1: INITIALIZING to LISTENING on INIT_COMPLETE
ptp4l[7727.088]: port 0: INITIALIZING to LISTENING on INIT_COMPLETE
ptp4l[7727.088]: port 1: link down
ptp4l[7727.088]: port 1: LISTENING to FAULTY on FAULT_DETECTED (FT_UNSPECIFIED)
ptp4l[7727.127]: selected local clock 6805ca.fffe.c87ac2 as best master
ptp4l[7727.127]: assuming the grand master role
ptp4l[7729.260]: port 1: link up
ptp4l[7729.300]: port 1: FAULTY to LISTENING on INIT_COMPLETE
ptp4l[7732.330]: port 1: new foreign master f4844c.fffe.fbc042-1
ptp4l[7732.354]: port 1: LISTENING to MASTER on ANNOUNCE_RECEIPT_TIMEOUT_EXPIRES
ptp4l[7732.354]: selected local clock 6805ca.fffe.c87ac2 as best master
ptp4l[7732.354]: assuming the grand master role
\endcode
### DUT output
\code
==================================
  CPSW GPTP LWIP TCP ECHO SERVER 
==================================
Enabling clocks!
EnetAppUtils_reduceCoreMacAllocation: Reduced Mac Address Allocation for CoreId:1 From 4 To 2 
Mdio_open:294 
EnetPhy_bindDriver:1828 
EnetPhy_bindDriver:1828 
PHY 3 is alive
PHY 15 is alive
unibase-1.1.4
INF:cbase:tilld0: has mac: F4:84:4C:FB:C0:42
INF:cbase:tilld1: has mac: 00:00:00:00:00:00
Start: uniconf_task
EnetApp_uniconfTask: dbname: NULL
INF:uconf:simpledb_open:no data is imported
INF:uconf:uc_hwal_open:
INF:cbase:cb_rawsock_open:combase-1.1.3
INF:cbase:cb_rawsock_open:dmaTxChId=-1 numRxChannels=0 dmaRxChId=-1 nTxPkts=0 nRxPkts=0 pktSize=0
INF:uconf:create_semname_with_dbname:null dbname is specified.
INF:cbase:cb_lld_task_create:alloc stack size=16384
INF:uconf:000000-097424:uniconf_main:uniconf started
EnetApp_gptpYangConfig:domain=0
INF:uconf:get_exmodid_in_db:first xl4gptp:exmodid=0
INF:uconf:create_semname_with_dbname:null dbname is specified.
EnetApp_gptpNonYangConfig:XL4_EXTMOD_XL4GPTP_SINGLE_CLOCK_MODE=1
EnetApp_gptpNonYangConfig:XL4_EXTMOD_XL4GPTP_USE_HW_PHASE_ADJUSTMENT=1
EnetApp_gptpNonYangConfig:XL4_EXTMOD_XL4GPTP_CLOCK_COMPUTE_INTERVAL_MSEC=100
EnetApp_gptpNonYangConfig:XL4_EXTMOD_XL4GPTP_FREQ_OFFSET_IIR_ALPHA_START_VALUE=1
EnetApp_gptpNonYangConfig:XL4_EXTMOD_XL4GPTP_FREQ_OFFSET_IIR_ALPHA_STABLE_VALUE=4
EnetApp_gptpNonYangConfig:XL4_EXTMOD_XL4GPTP_PHASE_OFFSET_IIR_ALPHA_START_VALUE=1
EnetApp_gptpNonYangConfig:XL4_EXTMOD_XL4GPTP_PHASE_OFFSET_IIR_ALPHA_STABLE_VALUE=4
EnetApp_gptpNonYangConfig:XL4_EXTMOD_XL4GPTP_MAX_DOMAIN_NUMBER=1
Start: gptp2d_task
EnetApp_initTsn:TSN app start done!
Starting lwIP, local interface IP is dhcp-enabled
[LWIPIF_LWIP] NETIF INIT SUCCESS
Host MAC address-0 : f4:84:4c:fb:c0:42
[0]Enet IF UP Event. Local interface IP:0.0.0.0
[LWIPIF_LWIP] Enet has been started successfully
INF:gptp:gptpman_run:max_domains=1, max_ports=2
INF:cbase:cb_rawsock_open:combase-1.1.3
INF:cbase:cb_rawsock_open:dmaTxChId=1 numRxChannels=1 dmaRxChId=1 nTxPkts=8 nRxPkts=16 pktSize=1536
INF:cbase:rxChId 1 has owner dmaRxShared 0
[0]Waiting for network UP ...
INF:cbase:cbl_query_response:tilld0 link DOWN !!!!
INF:gptp:dev:tilld0 open success
INF:cbase:tilld1: alloc mac: 70:FF:76:1E:3A:07
INF:gptp:dev:tilld1 open success
INF:cbase:cbl_query_response:tilld1 link DOWN !!!!
INF:gptp:gptpnet_init:Open lldtsync OK!
INF:gptp:IEEE1588-2019 performance monitoring disabled.
INF:uconf:ydbi_get_item_ifk3vk0:no data
INF:uconf:ydbi_get_item_ifk3vk0:no data
INF:gptp:onenet_activate:tilld0 status=0, duplex=1, speed=0Mbps
INF:uconf:ydbi_get_item_ifk3vk0:no data
INF:uconf:ydbi_get_item_ifk3vk0:no data
INF:gptp:onenet_activate:tilld1 status=0, duplex=1, speed=0Mbps
INF:ubase:GPTP_MEDIUM_ALLOC: fragsize=16 fragused/fragnum=830/921 (90%)
INF:ubase:GPTP_SMALL_ALLOC: fragsize=4 fragused/fragnum=19/71 (26%)
INF:ubase:SM_DATA_INST: fragsize=8 fragused/fragnum=2002/2002 (100%)
INF:gptp:gptpman_run:GPTPNET_INTERVAL_TIMEOUT_NSEC=125000000
WRN:gptp:gptpgcfg_link_check:can't read speed

INF:gptp:000000-379622:domainIndex=0, GM changed old=00:00:00:00:00:00:00:00, new=F4:84:4C:FF:FE:FB:C0:42
INF:gptp:gptpclock_set_gmsync:gptpInstanceIndex=0, domainIndex=0, gmstate=2
INF:gptp:set_phase_offsetGM:domainIndex=0, New adjustment(New GM?)
[0]Waiting for network UP ...
Cpsw_handleLinkUp:1450 
MAC Port 2: link up
INF:cbase:cbl_query_response:tilld1: link UP, speed=1000, duplex=1 !!!!
[0]Network Link UP Event
INF:gptp:index=2 speed=1000, duplex=full, ptpdev=tilld1
WRN:gptp:000003-251701:waiting_for_pdelay_interval_timer_proc:portIndex=2, sourcePortIdentity=68:05:CA:FF:FE:C8:7A:C2, thisClock=F4:84:4C:FF:FE:FB:C0:42, neighborPropDelay=203
INF:gptp:waiting_for_pdelay_interval_timer_proc:portIndex=2, not asCapable
Cpsw_handleLinkUp:1450 
MAC Port 1: link up
INF:cbase:cbl_query_response:tilld0: link UP, speed=1000, duplex=1 !!!!
INF:gptp:index=1 speed=1000, duplex=full, ptpdev=tilld0
INF:gptp:md_pdelay_resp_sm_recv_req:port=2, set receivedNonCMLDSPdelayReq=1
[0]Waiting for network UP ...
INF:gptp:waiting_for_pdelay_interval_timer_proc:set asCapableAcrossDomains, portIndex=2
INF:gptp:set asCapable for domainIndex=0, portIndex=2
INF:gptp:000004-259132:gptpgcfg_set_asCapable:domainInde=0, portIndex=2, ascapable=1
INF:gptp:000004-266523:gm_stable:gm_unstable_proc:domainIndex=0
INF:gptp:gptpclock_set_gmsync:gptpInstanceIndex=0, domainIndex=0, gmstate=1
NF:gptp:000004-278712:gm_stable:gm_unstable_proc:domainIndex=0
INF:gptp:000004-374156:setSyncTwoStep_txSync:domainIndex=0, portIndex=2, sync gap=4375msec
INF:gptp:000004-379390:setFollowUp_txFollowUp:domainIndex=0, portIndex=2, fup gap=4382msec
INF:gptp:000005-374200:gm_stable:gm_stable_proc:domainIndex=0
INF:gptp:gptpclock_set_gmsync:gptpInstanceIndex=0, domainIndex=0, gmstate=2
[0]Waiting for network UP ...
INF:gptp:000006-344047:domainIndex=0, GM changed old=F4:84:4C:FF:FE:FB:C0:42, new=68:05:CA:FF:FE:C8:7A:C2
INF:gptp:gptpclock_set_gmsync:gptpInstanceIndex=0, domainIndex=0, gmstate=0
INF:gptp:000006-357144:gm_stable:gm_unstable_proc:domainIndex=0
INF:gptp:gptpclock_set_gmsync:gptpInstanceIndex=0, domainIndex=0, gmstate=1
INF:gptp:set_phase_offsetGM:domainIndex=0, New adjustment(New GM?)
INF:gptp:set_phase_offsetGM:domainIndex=0, offset adjustment, diff=1754370932
INF:gptp:set_phase_offsetGM:domainIndex=0, stable
IFV:gptp:domainIndex=0, clock_master_sync_receive:the master clock rate to 3156ppb, GMdiff=9781nsec
IFV:gptp:domainIndex=0, clock_master_sync_receive:the master clock rate to 5707ppb, GMdiff=9794nsec
IFV:gptp:domainIndex=0, clock_master_sync_receive:the master clock rate to 7727ppb, GMdiff=9742nsec
................
...............
[0]Enet IF UP Event. Local interface IP:192.168.1.10
IFV:gptp:domainIndex=0, clock_master_sync_receive:the master clock rate to 4291ppb, GMdiff=12nsec
IFV:gptp:domainIndex=0, clock_master_sync_receive:the master clock rate to 4264ppb, GMdiff=-2nsec
IFV:gptp:domainIndex=0, clock_master_sync_receive:the master clock rate to 4222ppb, GMdiff=-21nsec
IFV:gptp:domainIndex=0, clock_master_sync_receive:the master clock rate to 4189ppb, GMdiff=-34nsec


\endcode

# See Also

\ref NETWORKING |
\ref EXAMPLES_ENET_CPSW_TSN_GPTP_BRIDGE |
\ref EXAMPLES_ENET_CPSW_TSN_GPTP_TR |
\ref EXAMPLES_ENET_CPSW_TSN_GPTP_TT |
\ref ENET_CPSW_TSN_GPTP