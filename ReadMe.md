# ModBus corss platform C library 
![GitHub License](https://img.shields.io/github/license/niwciu/MODBUS)  
![GitHub top language](https://img.shields.io/github/languages/top/niwciu/MODBUS)
![GitHub Release](https://img.shields.io/github/v/release/niwciu/MODBUS)  
![GitHub branch check runs](https://img.shields.io/github/check-runs/niwciu/LCD_HD44780/main)  
![example workflow](https://github.com/niwciu/MODBUS/actions/workflows/run_modbus_tests.yml/badge.svg)
![example workflow](https://github.com/niwciu/MODBUS/actions/workflows/run_cppcheck.yml/badge.svg)
![example workflow](https://github.com/niwciu/MODBUS/actions/workflows/clang-format_check.yml/badge.svg)
![example workflow](https://github.com/niwciu/MODBUS/actions/workflows/run_lizard_lib_check.yml/badge.svg) 

## Features
- Contain ModBus RTU Master and Slave implementation
- Support the following functions:
  - Read Coils (0x01)
  - Read Discrete Inputs (0x02)
  - Read Holding Registers (0x03)
  - Read Input Registers (0x04)
  - Write Single Coil (0x05)
  - Write Single Register (0x06)
  - Write Multiple Coils (0x0F)
  - Write Multiple registers (0x10)
- Possible to run master and slave functionality on one hardware platform
- Contain examples of:
  - ModBus Master RTU implementation
  - ModBus Slave RTU implementation
  - ModBus Master & Slave implementation on one harwdware platform
- Library contain unit test and integration tests that can be reused in project if needed
## Library contents description
TBD
## Examples
Library contain an examples of three different configurations that can be implemented in project. The fallowin configurations allow to run device as: 
- ModBus RTU Slave (ModBus RTU Sever)
- ModBus RTU Master (ModBus RTU Client)
- ModBus RTU Slave&Master (ModBus RTU Sever&Client)

### How to run STM32G071RB ModBus RTU Slave example
#### Requirements for compiling and running the example:
  1. CMake installed
  2. Make or Ninja installed
  3. ARM GNU Toolchain (gcc-arm-none-eabi) installed
  4. STM32_Programmer_CLI installed
  5. ST-link (placed on Nucleo Board) installed  
   
  In order to test the slave node, any software that can simulate Modbus Master RTU is required. In my case I was using [qModMaster](https://sourceforge.net/projects/qmodmaster/)


#### Hardware requirements, configuration, and connections
  1. STM32G071 Nucleo-64<br>
    ![drawing](https://raw.githubusercontent.com/niwciu/MODBUS/main/examples/doc/images/STM32G071RB_Nucleo.png){width=500;}

#### How to build and run the example
  1. Open the location you want to clone the repository to in your termina
  3. Clone the repository to your preferred localization
      ```bash
      git clone https://github.com/niwciu/MODBUS.git
      ``` 
  4. Enter to  MODBUS/examples/STM32G071RB_MODBUS_SLAVE
      ```bash
      cd ./MODBUS/examples/STM32G071RB_MODBUS_SLAVE
      ``` 
  5. For Make type:
      ```bash
      cmake -S ./ -B Debug -G"Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug
      ```
      ```bash
      cd Debug
      ```
      ```bash 
      make all
      ```
      ```bash 
      make flash
      ``` 
  6. For Ninja type:
      ```bash 
      cmake -S ./ -B Debug -G"Ninja" -DCMAKE_BUILD_TYPE=Debug
      ```
      ```bash
      cd Debug
      ```
      ```bash
      ninja
      ```
      ```bash 
      ninja flash
      ```
   7. In some cases Nucleo board requires plugging out and in a USB port to run the program.
### How to run ModBus STM32G071RB RTU Master example
#### Requirements for compiling and running the example:
  1. CMake installed
  2. Make or Ninja installed
  3. ARM GNU Toolchain (gcc-arm-none-eabi) installed
  4. STM32_Programmer_CLI installed
  5. ST-link (placed on Nucleo Board) installed  
   
  In order to test the Master node, any software that can simulate Modbus Slave RTU is required. In my case I was using [xxx]()


#### Hardware requirements, configuration, and connections
  1. STM32G071 Nucleo-64<br>
    ![drawing](https://raw.githubusercontent.com/niwciu/MODBUS/main/examples/doc/images/STM32G071RB_Nucleo.png){width=500;}
### How to run ModBus STM32G071RB RTU Master&Slave example  
#### Requirements for compiling and running the example:  
  1. CMake installed
  2. Make or Ninja installed
  3. ARM GNU Toolchain (gcc-arm-none-eabi) installed
  4. STM32_Programmer_CLI installed
  5. ST-link (placed on Nucleo Board) installed  
   
  In order to test the Master node, any software that can simulate Modbus Slave RTU is required. In my case I was using [xxx]()  

#### Hardware requirements, configuration, and connections
  1. STM32G071 Nucleo-64<br>
    ![drawing](https://raw.githubusercontent.com/niwciu/MODBUS/main/examples/doc/images/STM32G071RB_Nucleo.png){width=500;}  

## How to configure, use in project, colaborate and more
For information about the configuration and usage of the library, as well as collaboration in the project please refer to 
[MODBUS wiki](https://github.com/niwciu/MODBUS/wiki)

