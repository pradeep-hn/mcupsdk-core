
# UART High Level Driver{#DRIVERS_UART_HLD_PAGE}

## Features Supported

- Write and Read mode of operation
- Interrupt, Polled Mode
\cond !SOC_AM62X
- DMA mode of operation
\endcond
- Blocking and Non-blocking (Callback) transfers
- Write and Read Cancel mode of operation
\cond !(SOC_AM263X || SOC_AM263PX || SOC_AM261X)
- For low latency direct register read/write API, refer \ref EXAMPLES_DRIVERS_UART_ECHO_LOW_LATENCY_INTERRUPT and \ref EXAMPLES_DRIVERS_UART_ECHO_LOW_LATENCY_POLLING example.
\endcond

## Features NOT Supported

\cond !SOC_AM62X
- #UART_READ_RETURN_MODE_PARTIAL is not supported in DMA mode of operation
\endcond
\cond SOC_AM62X
- DMA mode is not supported.
\endcond
- MODEM control functions
- IrDA(Infrared Data Association) and CIR(Consumer Infrared) features

## Usage Overview

### API Sequence

To use the UART driver to send data or receive, the application
calls the following APIs:

- #UART_init() : Initialize the UART driver.
- #UART_Params_init():  Initialize a #UART_Params structure with default
  values.  Then change the parameters from non-default values as needed.
- #UART_open() :  Open an instance of the UART driver, passing the
  initialized parameters, or NULL, and an index to the configuration to
  open (detailed later).
- #UART_write():  Transmit data.  This function takes a
  #UART_Transaction argument that describes the transfer that is requested.
- #UART_read() :  Receive data.  This function takes a
  #UART_Transaction as an argument that describes the receive that is requested.
- #UART_close():  Close the UART instance.
- #UART_deinit(): De-Initialize the UART driver.

### Initializing the UART Driver

#UART_init() must be called before any other UART APIs.  This function
iterates through the elements of the UART_config[] array, calling
the element's device implementation UART initialization function.
Please note that initializing UART driver is taken care by the
SysConfig generated code.

### Opening the UART Driver

After initializing the UART driver by calling #UART_init(), the application
can open a UART instance by calling #UART_open().
Please note that opening UART driver is taken care by the
SysConfig generated code.
This function takes an index into the UART_config[] array, and the UART parameters data
structure. The UART instance is specified by the index of the UART in
UART_config[]. Calling #UART_open() second time with the same index
previously passed to #UART_open() will result in an error.  You can,
though, re-use the index if the instance is closed via #UART_close().

If no #UART_Params_init structure is passed to UART_open(), default values are
used. If the open call is successful, it returns a non-NULL value.

### UART Write Mode

The UART driver supports two transfer modes of operation: interrupt and polling mode.
In polling mode a task's code execution is blocked until a UART
transaction has completed or a timeout has occurred.

In interrupt mode, again there are two modes blocking and callback.
The transfer mode is determined by the #UART_Params.writeMode parameter.
The UART driver defaults to blocking mode, if the application does not set it.
Once a UART driver is opened, the only way to change the operation mode
is to close and re-open the UART instance with the new write mode.

In blocking mode, a task's code execution is blocked until a UART
transaction has completed or a timeout has occurred. This ensures
that only one UART transfer operates at a given time. Other tasks requesting
UART transfers while a transfer is currently taking place will receive
an error as return value. If a timeout occurs the transfer is cancelled, the
task is unblocked & will receive an error as return value. The transaction
count field will have the number of bytes transferred
successfully before the timeout.

In callback mode, a UART transaction functions asynchronously, which
means that it does not block code execution. After a UART transaction
has been completed, the UART driver calls a user-provided callback function.
Callback mode is supported in the execution context of tasks and
hardware interrupt routines.

### UART Read Mode

The UART driver supports two read modes of operation: interrupt and polling mode.
In polling mode a task's code execution is blocked until a UART
transaction has completed or a timeout has occurred.

In interrupt mode, again there are two modes blocking and callback.
The read mode is determined by the #UART_Params.readMode parameter.
The UART driver defaults to blocking mode, if the application does not set it.
Once a UART driver is opened, the only way to change the operation mode
is to close and re-open the UART instance with the new read mode.

In blocking mode, a task's code execution is blocked until a UART
transaction has completed or a timeout has occurred. This ensures
that only one UART read completes at a given time. Other tasks requesting
UART read while a read is currently taking place will receive
an error as return value. If a timeout occurs the read is cancelled, the
task is unblocked & will receive an error as return value. The transaction
count field will have the number of bytes read
successfully before the timeout.

In callback mode, a UART transaction functions asynchronously, which
means that it does not block code execution. After a UART transaction
has been completed, the UART driver calls a user-provided callback function.
Callback mode is supported in the execution context of tasks and
hardware interrupt routines.

There is an additional #UART_Params.readReturnMode parameter.
#UART_READ_RETURN_MODE_FULL unblocks or performs a callback when the read
buffer has been filled with the number of bytes passed to #UART_read().
#UART_READ_RETURN_MODE_PARTIAL unblocks or performs a callback whenever a
read timeout error occurs on the UART peripheral.
The read timeout occurs if the read FIFO is non-empty and no new
data has been received for a specific number of
clock cycles w.r.o device/baudrate dependent. This mode can be used when
the exact number of bytes to be read is not known.

\cond !SOC_AM62X
## Important Usage Guidelines

- In case of DMA mode, as R5F core is not Cache Coherent, Cache Writeback is required if R5F writes to the buffers.
  And before reading the buffers, application needs to invalidate those. Please refer \ref EXAMPLES_DRIVERS_UART_ECHO_DMA.
\endcond
## Example Usage

Include the below file to access the APIs
\snippet Uart_sample.c include

Instance Open Example
\snippet Uart_sample.c open

Instance Close Example
\snippet Uart_sample.c close

Write Transfer Example
\snippet Uart_sample.c write_transfer_blocking

Read Transfer Example
\snippet Uart_sample.c read_transfer_blocking

Write Non-Blocking Transfer Example
\snippet Uart_sample.c write_transfer_nonblocking

Read Non-Blocking Transfer Example
\snippet Uart_sample.c read_transfer_nonblocking

## API

\ref DRV_UART_MODULE
