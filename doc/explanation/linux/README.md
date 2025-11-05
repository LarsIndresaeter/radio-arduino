mqtt
====

The [mqtt](./mqtt.md) protocol is used for reading desired state on a topic and publishing responses from the gateway and node to a broker

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

