# device controller

the raduino-mqtt app has a class named deviceController which is responsible for
reconsiling the actual and desired state of devices (gateway or node).
This follows an architectural pattern where the state of the node is tracked on
the host computer to reduce complexity on the devices with less processing power.
This concept is sometimes referred to as a digital twin but device controller
is a better name for the class since digital twin is not very precise.

