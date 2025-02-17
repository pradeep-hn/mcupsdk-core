# EQEP frequency measurement {#EXAMPLES_DRIVERS_EQEP_FREQUENCY_MEASUREMENT}

[TOC]

# Introduction

This example uses the EQEP to measure frequency of a signal generated by ePWM.

\imageStyle{am263_eqep_frequency_measurement_fig1a.png,width:50%}
\image html am263_eqep_frequency_measurement_fig1a.png "Figure 1a. Block diagram"


The example does the below
- Configures EPWM to generate a signal and EQEP to measure frequency of this generated signal (a loopback connection is required).
- The application runs for the specified time and the frequency calculation is done using the EPWM ISR.
- After the specified time, the application checks if the measured frequency was within range of the generated frequency.


\imageStyle{am263_eqep_frequency_measurement_fig1b.png,width:80%}
\image html am263_eqep_frequency_measurement_fig1b.png "Figure 1b. Implementation details"


# External connections

- Connect ePWM0A to eQEP0A (EPWM simulates eQEP signal)

## AM263X-CC or AM263PX-CC

When using AM263x-CC with TMDSHSECDOCK (HSEC180 controlCARD Baseboard Docking Station)
- Connect HSEC Pin 49 (ePWM0A) to HSEC Pin 102 (eQEP0A)


## AM263X-LP or AM263PX-LP
- Connect J2 Pin 11 (ePWM0A) to J25 Pin 1 (eQEP0A)



# Supported Combinations {#EXAMPLES_DRIVERS_EQEP_FREQUENCY_MEASUREMENT_COMBOS}

\cond SOC_AM263X || SOC_AM263PX

 Parameter      | Value
 ---------------|-----------
 CPU + OS       | r5fss0-0 nortos
 Toolchain      | ti-arm-clang
 Board          | @VAR_BOARD_NAME_LOWER, @VAR_LP_BOARD_NAME_LOWER
 Example folder | examples/drivers/eqep/eqep_frequency_measurement/

\endcond

# Steps to Run the Example

- **When using CCS projects to build**, import the CCS project for the required combination
  and build it using the CCS project menu (see \ref CCS_PROJECTS_PAGE).
- **When using makefiles to build**, note the required combination and build using
  make command (see \ref MAKEFILE_BUILD_PAGE)
- Establish connections as mentioned in External Connections section
- Launch a CCS debug session and run the executable, see \ref CCS_LAUNCH_PAGE
- View the UART console logs for results

# See Also

\ref DRIVERS_EQEP_PAGE

# Sample Output

Shown below is a sample output when the application is run,

\code
EQEP Frequency Measurement Test Started ...
Please ensure EPWM to EQEP loopback is connected...
Please wait 10 seconds ...
Expected frequency = 10000 Hz, Measured frequency = 10000 Hz
EQEP Frequency Measurement Test Passed!!
All tests have passed!!
\endcode
