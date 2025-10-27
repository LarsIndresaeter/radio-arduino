arduino
=======

* how a typical command handler on the arduino is implemented

.. code-block:: c++

    void commandBlink(uint8_t* commandPayload, uint8_t* responsePayload)
    {
        COMMANDS::BLINK::command_t command(commandPayload);
        COMMANDS::BLINK::response_t response;

        GPIO::blink();

        response.serialize(responsePayload);
    }

A typical command handler starts by defining the command struct and initializing it from they command payload read by the protocol handler and an empty resonse struct.

The middle of the command handler reads from the command struct and writes to the response struct.

The final part of the command handler serializes the response struct and write to the response payload pointer.

both the start and the end of the command handler is boilerplate code while the middle part is where the magic happends.

