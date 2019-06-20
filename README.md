# axiom-control-daemon
AXIOM internal camera settings/commands/status control daemon

Build and usage instructions: https://wiki.apertus.org/index.php/Control_Daemon

# Build status
| master | dev |
|:------:|:------:|
|[![CircleCI](https://circleci.com/gh/apertus-open-source-cinema/axiom-control-daemon/tree/master.svg?style=svg)](https://circleci.com/gh/apertus-open-source-cinema/axiom-control-daemon/tree/master)| [![CircleCI](https://circleci.com/gh/apertus-open-source-cinema/axiom-control-daemon/tree/dev.svg?style=svg)](https://circleci.com/gh/apertus-open-source-cinema/axiom-control-daemon/tree/dev) |

# Commands Added
- Environment Module(Makes Use of SysfsAdapter)
  - `./DaemonCLI temperatures get zynq_temp`(Returns the zynqboard temperature)
  - `./DaemonCLI temperatures set zynq_temp x` (Returns with message that set is not allowed for the given parameter)
  - `./DaemonCLI voltages get voltage_all` (Gives list of all voltages listed below seperated by newline )
  - `./DaemonCLI voltages get vccint`
  - `./DaemonCLI voltages get vccaux`
  - `./DaemonCLI voltages get vccbram`
  - `./DaemonCLI voltages get vccpint`
  - `./DaemonCLI voltages get vccpaux`
  - `./DaemonCLI voltages get vccoddr`
  - `./DaemonCLI voltages get vrefp`
  - `./DaemonCLI voltages get vrefn`

- I2CTestModule (Makes use of I2CAdapter , Should be renamed to Pac1720InfoModule)
  -`./DaemonCLI i2c_test get pac1720info` (Returns all the info listed below in combined form)
  -`./DaemonCLI i2c_test get ZED_5V `
  -`./DaemonCLI i2c_test get HDN`
  -`./DaemonCLI i2c_test get RFW_V`
  -`./DaemonCLI i2c_test get VCCO_35`
  -`./DaemonCLI i2c_test get PCIE_IO`
  -`./DaemonCLI i2c_test get W_VW`
  -`./DaemonCLI i2c_test get N_VN`
  -`./DaemonCLI i2c_test get E_VE`
  -`./DaemonCLI i2c_test get S_VS`
  -`./DaemonCLI i2c_test get BETA_5V`
  -`./DaemonCLI i2c_test get PCIE_S_V`
  -`./DaemonCLI i2c_test get IOW_V`
  -`./DaemonCLI i2c_test get IOE_V`
  -`./DaemonCLI i2c_test get VCCO_13`
  -`./DaemonCLI i2c_test get VCCO_34`
  -`./DaemonCLI i2c_test get N_VW`
  -`./DaemonCLI i2c_test get N_VE`
  -`./DaemonCLI i2c_test get S_VE`
  -`./DaemonCLI i2c_test get S_VW`
  
