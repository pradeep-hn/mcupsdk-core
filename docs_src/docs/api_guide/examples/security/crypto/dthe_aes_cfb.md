# DTHE AES CFB {#EXAMPLES_DRIVERS_DTHE_AES_CFB}

[TOC]

# Introduction
This example demonstrates Cipher Feedback mode(CFB) of AES(Advanced Encryption Standard) using AES accelerator. The AES supports all the key sizes like 128-bit, 192-bit and 256-bit implementations. This example explains the steps to build and run for DTHE AES CFB with 128-bit, 192-bit and 256-bit keys.

\cond SOC_AM263X || SOC_AM263PX || SOC_AM273X

 Parameter             | Value
 ----------------------|-----------
 CPU + OS              | r5fss0-0 nortos
 Toolchain             | ti-arm-clang
 Boards                | @VAR_BOARD_NAME_LOWER
 Example folder        | examples/security/crypto/dthe_aes/crypto_aes_cfb/
 Supported Device Type | HS-FS, HS-SE

\endcond

## Build the aes cfb 128 example
\code
$make -s -C examples/security/crypto/dthe_aes/crypto_aes_cfb/<soc>-<board>/r5fss0-0_nortos/ti-arm-clang all DEVICE=<soc>
\endcode


# Steps to Run through ROM Boot flow

\note If the device type is HS-SE, an additional flag "DEVICE_TYPE=HS" is passed for building SBL. The flag results in generation of SBL image with suffix "hs.tiimage" which can only be used when device type is HS-SE.

## Via SBL_uart bootloader

### Build sbl_uart
\code
make -s -C examples/drivers/boot/sbl_uart/<soc>-<board>/r5fss0-0_nortos/ti-arm-clang/ all DEVICE=<soc>
\endcode

### Set board for UART boot

See [EVM setup](https://software-dl.ti.com/mcu-plus-sdk/esd/@VAR_SOC_NAME/latest/exports/docs/api_guide_@VAR_SOC_NAME_LOWER/EVM_SETUP_PAGE.html#autotoc_md29)

### Run UART_bootloader
\code
python uart_bootloader.py -p <COMxx> --bootloader=sbl_prebuilt/<soc>-<board>/sbl_uart.release.hs.tiimage --file=../../examples/security/crypto/dthe_aes/crypto_aes_cfb/<soc>-<board>/r5fss0-0_nortos/ti-arm-clang/crypto_dthe_aes_cfb.release.appimage
\endcode

### Sample output
On successful boot, R5 log at uart terminal, will have the following output.
UART Console:
\code
[CRYPTO] DTHE AES CFB example started ...
[CRYPTO] DTHE AES CFB example completed!!
All tests have passed!!

\endcode

## Via SBL_qspi bootloader

### Build sbl_qspi
\code
make -C examples/drivers/boot/sbl_qspi/<soc>-<board>/r5fss0-0_nortos/ti-arm-clang/ all DEVICE=<soc>
\endcode

### Build sbl_uart_uniflash
\code
make -s -C examples/drivers/boot/sbl_uart_uniflash/<soc>-<board>/r5fss0-0_nortos/ti-arm-clang/ all DEVICE=<soc>
\endcode


### Set UART boot mode
See [EVM setup](https://software-dl.ti.com/mcu-plus-sdk/esd/@VAR_SOC_NAME/latest/exports/docs/api_guide_@VAR_SOC_NAME_LOWER/EVM_SETUP_PAGE.html#autotoc_md29)

### Edit default_sbl_qspi.cfg to include the correct images

### Run UART_uniflash
\code
python uart_uniflash.py -p <COMxx> --cfg=sbl_prebuilt/<soc>-<board>/default_sbl_qspi.cfg
\endcode

### Set QSPI boot mode
See [EVM setup](https://software-dl.ti.com/mcu-plus-sdk/esd/@VAR_SOC_NAME/latest/exports/docs/api_guide_@VAR_SOC_NAME_LOWER/EVM_SETUP_PAGE.html#autotoc_md29)

### Sample output
On successful boot, R5 log at uart terminal, will have the following output.

UART Console:
\code
[CRYPTO] DTHE AES CFB example started ...
[CRYPTO] DTHE AES CFB example completed!!
All tests have passed!!

\endcode

# See Also

\ref DRIVERS_DTHE_PAGE