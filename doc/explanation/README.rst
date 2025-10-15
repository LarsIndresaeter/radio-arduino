explanation
===========

structure
---------

Explanations follow the folder structure of the project with arduino, interface, linux and tools as the main folders.

+-----------+---------------------------------------------------------+
| folder    | description                                             |
+===========+=========================================================+
| arduino   | source code for atmega328p                              |
| interface | API interface between arduino and linux                 |
| linux     | source code for linux host applications                 |
| build     | build area for arduino, interface and linux source code |
| bin       | compiled binary files                                   |
+-----------+---------------------------------------------------------+

tools
-----

* `tools/raduino.rst` explain the tool suite purpose made for this project 

interface
---------

* Why it makes sense to define a binary interface and why a custom code generator is used

arduino
-------

* how a typical command handler is implemented

.. code-block:: c++

    void commandBlink(uint8_t* commandPayload, uint8_t* responsePayload)
    {
        COMMANDS::BLINK::command_t command(commandPayload);
        COMMANDS::BLINK::response_t response;

        GPIO::blink();

        response.serialize(responsePayload);
    }

TODO: explain the different parts

Architecture Decisions (ADR)
----------------------------

* Why this project is organized as a mono-repo and what the tradeoffs are if you decide to split it into several repositories.
* build system (conan and cmake, why and how)
