# ModBus corss platform C library 
![GitHub License](https://img.shields.io/github/license/niwciu/MODBUS)  
![GitHub top language](https://img.shields.io/github/languages/top/niwciu/MODBUS)
![GitHub Release](https://img.shields.io/github/v/release/niwciu/MODBUS)  


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

### STM32G071RB ModBus RTU Slave example
By running this example you can read all types of ModBus data in the address range from 0 to 19.
Below are the default values stored in all object types.
```C
#define EXAMPLE_DATA_QTY 20

modbus_coil_disin_t coil_data[EXAMPLE_DATA_QTY] = {1, 0, 1, 0, 0, 1, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1};
modbus_coil_disin_t din_data[EXAMPLE_DATA_QTY] = {1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1};
modbus_reg_t hreg_data[EXAMPLE_DATA_QTY] = {2001, 2002, 2003, 2004, 2005, 2006, 2007, 2008, 2009, 2010, 3001, 3002, 3003, 3004, 3005, 3006, 3007, 3008, 3009, 3010};
modbus_reg_t inreg_data[EXAMPLE_DATA_QTY] = {4201, 4202, 4203, 4204, 4205, 4206, 4207, 4208, 4209, 4210, 4301, 4302, 4303, 4304, 4305, 4306, 4307, 4308, 4309, 4310};
```

You can also write coils and registers using the same address range.

Additionally, it's possible to control the state of the user LED on the Nucleo board. To turn on the LED set coil under address 21 to 1, to turn off the LED set the same coil to 0.  

#### Requirements for compiling and running the example:
  1. CMake installed
  2. Make or Ninja installed
  3. ARM GNU Toolchain (gcc-arm-none-eabi) installed
  4. STM32_Programmer_CLI installed
  5. ST-link (placed on Nucleo Board) installed  
   
  In order to test the slave node, any software that can simulate Modbus Master RTU is required. In my case I was using [qModMaster](https://sourceforge.net/projects/qmodmaster/)


#### Hardware requirements, configuration, and connections
  1. STM32G071 Nucleo-64<br>
      <img src="https://raw.githubusercontent.com/niwciu/MODBUS/main/examples/doc/images/STM32G071RB_Nucleo.png"   width="400"><br> <br> 

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
  1. STM32G071 Nucleo-64  
  <img src="https://raw.githubusercontent.com/niwciu/MODBUS/main/examples/doc/images/STM32G071RB_Nucleo.png"   width="400"><br> <br> 
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
    <img src="https://raw.githubusercontent.com/niwciu/MODBUS/main/examples/doc/images/STM32G071RB_Nucleo.png"   width="400"><br> <br>  

## How to configure, use in project, colaborate and more
For information about the configuration and usage of the library, as well as collaboration in the project please refer to 
[MODBUS wiki](https://github.com/niwciu/MODBUS/wiki)

***

![myEmbeddedWayBanerWhiteSmaller](https://github.com/user-attachments/assets/f4825882-e285-4e02-a75c-68fc86ff5716)
***