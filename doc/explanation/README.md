# explanation

Explanation, or discussions, clarify and illuminate a particular topic.

* [about documentation](./about-documentation.md) answers some of the decisions made for this project.
* [project](./project.md) answers more questions about this project.
* [binary protocol interface](./binary-protocol-interface.md) : explain how the binary interface between Linux and the gateway works
* [licences](./licences.md) for third party software used by the code in this project
* [motivation for this work](./motivation-for-this-work.md) give you insight into what I learned from making this and what you can use
* [naming conventions](./naming-conventions.md) used for the code
* [protocol command generator](./protocol-command-generator.md) is a python script for generating the mostly boilerplate code used for payload structs for arduino and C++ classes for Linux

## repo folder structure structure

Explanations follow the folder structure of the project with arduino, interface, linux and tools as the main folders.

| folder                                | description                                             |
| ------------------------------------- | ------------------------------------------------------- |
| [arduino](./arduino/README.md)        | source code for atmega328p                              |
| [interface](./interface/README.md)    | API interface between arduino and linux                 |
| [linux](./linux/README.md)            | source code for linux host applications                 |
| tools                                 | toolkit for working with the code in this repository    |
| doc                                   | documentation folder                                    |
| build                                 | build area for arduino, interface and linux source code |
| bin                                   | compiled binary files                                   |

