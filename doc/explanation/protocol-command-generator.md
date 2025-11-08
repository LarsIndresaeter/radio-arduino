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

