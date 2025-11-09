# add new command

The best way to learn how to add a command is to inspect the code changes for a new command. The example below is from a branch that adds commands for setting and getting unique id.
use actual commits (merge commit) as an example

```console
*   5f5a598 (HEAD -> main) Merge branch 'unique-id'
|\  
| * 31e4e1c (unique-id) feat(unique-id): use unique id commands in apps
| * 89fa21f feat(unique-id): implement unique id command on arduino
| * 910e7ca feat(unique-id): add commands for unique id
|/  
* 7d3442e feat: add parameter to raduino-system-commands
```

## add commands for unique id

This commit adds two lines to the generation script.

```python
generateCommandAndPayloadFile(41, "set_unique_id", ["id[4]"], [])
generateCommandAndPayloadFile(42, "get_unique_id", [], ["id[4]"])
```

Running the script with the command `python generate.py` in the folder `interface/libs/commands/` created payload structs used by the arduino code and command classes used by the linux application code

this can be done using the command

```console
raduino generate commands
```

### files changed in interface directory

```
interface/libs/commands/generate.py
interface/libs/commands/include/cmd/command_id.hxx
interface/libs/commands/include/cmd/commands.hxx
interface/libs/commands/include/cmd/get_unique_id/command.hxx
interface/libs/commands/include/cmd/get_unique_id/payload.hxx
interface/libs/commands/include/cmd/payloads.hxx
interface/libs/commands/include/cmd/set_unique_id/command.hxx
interface/libs/commands/include/cmd/set_unique_id/payload.hxx
```

## implement unique id command on arduino

Command handlers are added following a common pattern where a command struct is constructed from the payload struct extracted by the parser in `arduino/libs/parser/parser.cpp`. 

Getter and setter functions are created by the [generator script](doc/explanation/protocol-command-generator.md) and used for extracting the id value in the command or setting the id value in the response.

changes to `arduino/libs/eeprom/command-handlers-eeprom.cpp`

```cpp
void commandSetUniqueId(uint8_t* commandPayload, uint8_t* responsePayload)
{
    COMMANDS::SET_UNIQUE_ID::command_t command(commandPayload);
    COMMANDS::SET_UNIQUE_ID::response_t response;

    EEPROM_DATA_STORE::setUniqueId(command.getId());

    response.serialize(responsePayload);
}

void commandGetUniqueId(uint8_t* commandPayload, uint8_t* responsePayload)
{
    COMMANDS::GET_UNIQUE_ID::command_t command(commandPayload);
    COMMANDS::GET_UNIQUE_ID::response_t response;

    response.setId(EEPROM_DATA_STORE::getUniqueId());

    response.serialize(responsePayload);
}


```

changes to `arduino/src/main`

```cpp
case COMMANDS::OI::SET_UNIQUE_ID:
    EEPROM::commandSetUniqueId(commandPayload, responsePayload);
    break;
case COMMANDS::OI::GET_UNIQUE_ID:
    EEPROM::commandGetUniqueId(commandPayload, responsePayload);
    break;
```

### files changed in arduino directory

```
arduino/libs/eeprom/command-handlers-eeprom.cpp
arduino/libs/eeprom/eeprom.cpp
arduino/libs/eeprom/include/command-handlers-eeprom.hpp
arduino/libs/eeprom/include/eeprom.hpp
arduino/src/main.cpp
```
## use unique id commands in apps

Sending a command and printing the response is as simple as:

1. initializing an object with `RaduinoCommandGetUniqueId()`
2. calling a template with a [command object](doc/explanation/command-objects.md) `mon.get<>()` will return a response object of the same class. 
3. printing the response object using the `<<` stream operator

```cpp
case 'i':
    std::cout << mon.get<>(RaduinoCommandGetUniqueId()) << std::endl;
    break;
case 'I':
    std::cout << mon.getRadio<>(RaduinoCommandGetUniqueId()) << std::endl;
    break;
```

Sending a command is equally simple, just initialize the command object with the parameter values.

```cpp
std::cout << mon.get<>(RaduinoCommandSetUniqueId(uniqueId)) << std::endl;
```

Learn more about the [command objects](doc/reference-guides/commands/list-of-commands.md).

### files changed linux directory

```
linux/apps/personalize/main.cpp
linux/apps/system-commands/main.cpp
```

