# linux uart monitor

The monitor `linux/libs/monitor/monitor.cpp` and `linux/libs/uart/uart.cpp` are responsible for asyncronous sending and receiving of data over the serial port. The `linux/libs/epollevent/epollevent.cpp` is responsible for calling the correct callback when data has been received in the readerThread.

1. register command object with eventprocess
2. write serialized output to the serial port
3. response is read in separate reader thread
4. reader thread calls callback in command object

