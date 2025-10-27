use cases
=========

The primary use case for the code in this project is the `gateway and node <./gateway-and-node.rst>`_ use-case but other use-cases are also supported.

* `ina219 power monitor <./arduino-nano-ina219-power-monitor.rst>`_ : read voltage and current and report min, max and average per second
* `nrf24l01 sniffer <./nrf24l01-sniffer.rst>`_ : configure the nrf24l01 chip to be in an undocumented verbose mode and send data to the serial port
* `i2c bridge <./i2c-bridge.rst>`_ : is a debug app that is useful for talking to a i2c device connected to a gateway or node
* `spi bridge <./spi-bridge.rst>`_ : is a debug app that is useful for talking to a spi device connected to a gateway or node
* `device ina219 <./device-ina219.rst>`_ : read current and voltage from an ina219 power monitor chip connected to a gateway or node
* `quadrature encoder <./quadrature-encoder.rst>`_ : read quadrature encoder using the RaduinoCommandQuadratureEncoder command
* `rs232 radio bridge <./rs232-radio-bridge.rst>`_ : configure two gateways to bridge data from rs232 to to the serial port. One device is the sender but communication is two-way

