explanation
===========

structure
---------

Explanations follow the folder structure of the project with arduino, interface, linux and tools as the main folders.

+---------------------------------------+---------------------------------------------------------+
| folder                                | description                                             |
+=======================================+=========================================================+
| `arduino <./arduino/README.rst>`_     | source code for atmega328p                              |
+---------------------------------------+---------------------------------------------------------+
| `interface <./interface/README.rst>`_ | API interface between arduino and linux                 |
+---------------------------------------+---------------------------------------------------------+
| `linux <./linux/README.rst>`_         | source code for linux host applications                 |
+---------------------------------------+---------------------------------------------------------+
| build                                 | build area for arduino, interface and linux source code |
+---------------------------------------+---------------------------------------------------------+
| bin                                   | compiled binary files                                   |
+---------------------------------------+---------------------------------------------------------+

* `binary protocol interface <./binary-protocol-interface.rst>`_ : explain how the binary interface between Linux and the gateway works
* `licences <./licences.rst>`_ for third party software used by the code in this project
* `motivation for this work <./motivation-for-this-work.rst>`_ give you insight into what I learned from making this and what you can use
* `naming conventions <./naming-conventions.rst>`_ used for the code
* `protocol command generator <./protocol-command-generator.rst>`_ is a python script for generating the mostly boilerplate code used for payload structs for arduino and C++ classes for Linux

Architecture Decisions (ADR)
----------------------------

* Why this project is organized as a mono-repo and what the tradeoffs are if you decide to split it into several repositories.
* build system (conan and cmake, why and how)
