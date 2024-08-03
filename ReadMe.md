# ModBus corss platform C library 
![example workflow](https://github.com/niwciu/MODBUS/actions/workflows/run_modbus_tests.yml/badge.svg)
![example workflow](https://github.com/niwciu/MODBUS/actions/workflows/run_cppcheck.yml/badge.svg)
![example workflow](https://github.com/niwciu/MODBUS/actions/workflows/clang-format_check.yml/badge.svg)
![example workflow](https://github.com/niwciu/MODBUS/actions/workflows/run_lizard_lib_check.yml/badge.svg)  
![GitHub License](https://img.shields.io/github/license/niwciu/MODBUS)

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

### How to run ModBus RTU Slave example
TBD
### How to run ModBus RTU Master example
TBD
### How to run ModBus RTU Master&Slave example
TBD
## How to use in project
TBD
