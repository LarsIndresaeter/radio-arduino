# interface

the interface folder contains the definition of the binary protocol and the binary commands.

The protocol has base classes for parsing byte streams and the specialization if done in a child class in the arduino or linux folders. The common part is in the header files that is packaged as a conan package with a cmake header library.

The commands are constructed from a base class specialized for each command type. The specializaions are generated from a python script.
