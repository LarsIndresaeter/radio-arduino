# protocol generator

## why create a custom generator

The C++ files for payload structs and commands are mostly boilerplate code and suitable for code generation. The code has to be very compact to fit into the avr and no usable protocol was found.

## the generator

The generator is located in one python file: `interface/libs/commands/generate.py` and is executed with the command:

```console
cd interace/libs/commands/
python generator.py
```

## adding a new command

add a new line:

```python
generateCommandAndPayloadFile(33, "new_command", ["c1", "c2[2]"], ["r1"])
```

in this example the command is named `new_command` which will generate a `RaduinoCommandNewCommand`

The command payload will have two members: c1 which is of type `uint8_t` adn c2 which is of type `uint16_t`. The array syntax indicate how many bytes. A value of 4 indicate a `uint32_t` while a value larger than 4 indicate a list. If the member has the substring `string` then string constructors will be created for that value.

## about the generator

### tradeoffs

- The generator is a single file and modularised into libraries to keep it simple for non-pythonistas.
- Read the program from the bottom up
- the generator script is not called by the build tools to give control to the developer
- generated files are added to the repository to avoid confusion for developers

### interface of the generateCommandAndPayloadFile method

`main` writes common header files and commands and payloads for each command

The function that create command and payload files has the following interface.

```python
def generateCommandAndPayloadFile(commandId, commandName, 
                 commandPayloadByteNames, 
                 responsePayloadByteNames):
```

the commandPayloadByteNames is a list of names for the payload variables. Payload variables are 1 byte or more.

for example

```python
generateCommandAndPayloadFile(28, "vcc", [], ["vcc[2]"])
```

will create a command named `vcc` which takes no parameters and returns two bytes named vcc. The response struct will have a getter command, `uint16_t getVcc()` automatically generated.

```python
generateCommandAndPayloadFile(29, "sleep", ["delay[4]"], [])
```

will create a command named sleep. The command payload will have a setter method `setDelay(uint32_t value)` automatically generated.

### behavior of the generateCommandFile method

This method is responsible for creating command classes, for example `interface/libs/commands/include/cmd/vcc/command.hxx`

### behavior of the generatePayloadFile method

This method is responsible for creating payload structs, for example `interface/libs/commands/include/cmd/vcc/payload.hxx`



