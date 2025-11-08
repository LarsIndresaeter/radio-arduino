# digital twin

the raduino-mqtt app has a class named digitalTwin which is responsible for reconsiling the actual and desired state of the node. This is an architectural pattern where the state of the node is tracked on the host computer. The purpose of this is to reduce complexity in the node and to be able to respond to mqtt commands and send updates when the state changes.

