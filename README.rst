radio-arduino
=============

radio-arduino is a suite of code and tools for programming arduino nano with nrf24l01+ 2.4GHz radio chip. For example the `rf-nano`.

The linux command line tools let you send commands to communicate with a device configured as a gateway or to a node through the gateway. The role as `gateway` or `node` is configured in eeprom of the atmega328p chip using the provided program `raduino-personalize`.

* multiple radio devices supported
* AES-128 CBC encryption of serial and radio commands supported

communication from PC to arduino
--------------------------------

.. code-block:: sh

    ┌───────────┐                 ┌─────────────────┐                ┌─────────────────┐
    │           │                 │                 │                │                 │
    │           │     RS 232      │ ARDUINO NANO    │     RADIO      │ ARDUINO NANO    │
    │    PC     │◄───────────────►│                 │◄──────────────►│                 │
    │           │                 │ WITH NRF24L01+  │                │ WITH NRF24L01+  │
    │           │                 │                 │                │                 │
    └───────────┘                 └─────────────────┘                └─────────────────┘
                                       gateway                              node

communication from PC to MQTT broker
------------------------------------

.. code-block:: sh

    ┌───────────┐                 ┌────────────┐
    │           │                 │            │
    │    PC     │                 │   MQTT     │
    │           │       MQTT      │            │
    │           │◄───────────────►│   BROKER   │
    │           │                 │            │
    │           │                 │            │
    └───────────┘                 └────────────┘

documentation
-------------

Full documentation is located in the `docs/` folder

What can this be used for
-------------------------

* extend your serial port using rf-nano
* blink the led on a rf-nano connected to serial port or radio
* read a sensor and publish data to a mqtt broker
* update text on a lcd display from a predefined mqtt topic
* control a RC servo
* set pattern on RGB-leds
* extend to fit your purpose
