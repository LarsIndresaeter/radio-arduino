import os

def snakecaseToCamelCase(variableName):
    result = variableName.capitalize()
    
    idx = result.find("_")
    while idx > 0:
        result = result[:idx] + result[idx + 1:].capitalize()
        idx = result.find("_")

    return result

def arraySizeFromVariableName(variableName):
    result = 1

    try:
        first = variableName.index("[")
        last = variableName.index("]")

        if(first > 0 and last > 0):
            result = int(variableName[first+1:last])
            return result
    except:
        pass

    return result

def arrayBasenameFromVariableName(variableName):
    result = variableName

    try:
        first = variableName.index("[")

        if(first > 0):
            result = variableName[0:first]
            return result
    except:
        pass

    return result

def calculateStructSizeFromNames(commandPayloadByteNames):
    length = 0

    for item in commandPayloadByteNames:
        length = length + arraySizeFromVariableName(item)

    return length 

def generateCommandIdOpen():
    os.makedirs("include/common", exist_ok=True)
    with open("include/common/command_id.hpp", 'w') as outfile:
        outfile.write("#pragma once\n") 
        outfile.write("// This file is generated with the script: `interface/libs/commands/generate.py`\n")
        outfile.write("\n") 
        outfile.write("namespace COMMANDS {\n") 
        outfile.write("\n") 
        outfile.write("constexpr uint8_t MAX_PAYLOAD_LENGTH = 136;\n")
        outfile.write("constexpr uint8_t MAX_PACKAGE_LENGTH = (MAX_PAYLOAD_LENGTH + 4 + 2 + 20); // + header, crypto, checksum\n")
        outfile.write("\n")
        outfile.write("enum class OI\n")
        outfile.write("{\n")
        outfile.write("    UNDEFINED = 0,\n")
        outfile.write("    RESERVED = 1,\n")

def generateCommandIdClose():
    os.makedirs("include/common", exist_ok=True)
    with open("include/common/command_id.hpp", 'a') as outfile:
        outfile.write("};\n")
        outfile.write("\n")
        outfile.write("} // namespace COMMANDS\n")

def generateCommandFile(commandName,
                 commandPayloadByteNames, 
                 responsePayloadByteNames):
    os.makedirs("include/" + commandName, exist_ok=True)

    with open("include/" + commandName + "/command.hpp", 'w') as outfile:
        outfile.write("#pragma once\n")
        outfile.write("// This file is generated with the script: `interface/libs/commands/generate.py`\n")
        outfile.write("\n")
        outfile.write("#include <common/uartCommandBase.hpp>\n")
        outfile.write("\n")
        outfile.write("class UartCommand" + snakecaseToCamelCase(commandName) + " : public UartCommandBase {\n")
        outfile.write("public:\n")
        outfile.write("    UartCommand" + snakecaseToCamelCase(commandName) + "(")
        # add parameters
        items = len(commandPayloadByteNames)
        for item in commandPayloadByteNames:
            arraySize = arraySizeFromVariableName(item)
            if(arraySize == 1):
                outfile.write("uint8_t " + item)
            if(arraySize == 2):
                outfile.write("uint16_t " + arrayBasenameFromVariableName(item))
            if(arraySize == 4):
                outfile.write("uint32_t " + arrayBasenameFromVariableName(item))
            if(arraySize > 4):
                outfile.write("std::vector<uint8_t> " + arrayBasenameFromVariableName(item))
            items = items - 1
            if items > 0:
                outfile.write(", ")
 
        outfile.write(")\n")
        outfile.write("        : UartCommandBase(\n")
        outfile.write("            static_cast<uint8_t>(COMMANDS::OI::" + commandName.upper() + "),\n")
        outfile.write("            COMMANDS::" + commandName.upper() + "::COMMAND_LENGTH)\n")
        outfile.write("    {\n")
        # initialize command
        outfile.write("        COMMANDS::" + commandName.upper() + "::command_t command;\n")
        outfile.write("\n")
        for item in commandPayloadByteNames:
            arraySize = arraySizeFromVariableName(item)
            if(arraySize == 1):
                outfile.write("        m_payload.at(offsetof(COMMANDS::" + commandName.upper() + "::command_t, " + item + ")) = " + item + ";\n")
                outfile.write("\n")
            if(arraySize == 2):
                outfile.write("        m_payload.at(offsetof(COMMANDS::" + commandName.upper() + "::command_t, " + arrayBasenameFromVariableName(item) + ") + 1) = " + arrayBasenameFromVariableName(item) + ">>8;\n")
                outfile.write("        m_payload.at(offsetof(COMMANDS::" + commandName.upper() + "::command_t, " + arrayBasenameFromVariableName(item) + ")) = " + arrayBasenameFromVariableName(item) + ";\n")
                outfile.write("\n")
            if(arraySize == 4):
                outfile.write("        m_payload.at(offsetof(COMMANDS::" + commandName.upper() + "::command_t, " + arrayBasenameFromVariableName(item) + ") + 3) = " + arrayBasenameFromVariableName(item) + ">>24;\n")
                outfile.write("        m_payload.at(offsetof(COMMANDS::" + commandName.upper() + "::command_t, " + arrayBasenameFromVariableName(item) + ") + 2) = " + arrayBasenameFromVariableName(item) + ">>16;\n")
                outfile.write("        m_payload.at(offsetof(COMMANDS::" + commandName.upper() + "::command_t, " + arrayBasenameFromVariableName(item) + ") + 1) = " + arrayBasenameFromVariableName(item) + ">>8;\n")
                outfile.write("        m_payload.at(offsetof(COMMANDS::" + commandName.upper() + "::command_t, " + arrayBasenameFromVariableName(item) + ")) = " + arrayBasenameFromVariableName(item) + ";\n")
                outfile.write("\n")
            if(arraySize > 4):
                outfile.write("        for (int i = 0; i < " + arrayBasenameFromVariableName(item) + ".size(); i++) {\n")
                outfile.write("            if (i >= sizeof(command." + arrayBasenameFromVariableName(item) + ")) {\n")
                outfile.write("                break;\n")
                outfile.write("            }\n")
                outfile.write("            m_payload.at(\n")
                outfile.write("                offsetof(COMMANDS::" + commandName.upper() + "::command_t, " + arrayBasenameFromVariableName(item) + "[0]) + i)\n")
                outfile.write("                = " + arrayBasenameFromVariableName(item) + ".at(i);\n")
                outfile.write("        }\n")
                outfile.write("\n")
        outfile.write("    };\n")

        outfile.write("\n")
        outfile.write("    void printResponse(std::ostream& out, COMMANDS::" + commandName.upper() + "::response_t response) const\n") 
        outfile.write("    {\n")
        outfile.write("        out << \"" + commandName.upper().ljust(20) + "   : \";\n")

        for item in responsePayloadByteNames:
            arraySize = arraySizeFromVariableName(item)
            if(arraySize <= 4):
                outfile.write("        out << \" " + arrayBasenameFromVariableName(item) + "=\" << " + "static_cast<int>(response.get" + arrayBasenameFromVariableName(item).capitalize() + "());\n");
            else:
                if(arrayBasenameFromVariableName(item).lower().find("string") > 0):
                    outfile.write("        out << \" " + arrayBasenameFromVariableName(item) + "=\\\"\";\n")
                    outfile.write("        for(uint8_t i=0; i<" + str(arraySize) + "; i++)\n")
                    outfile.write("        {\n")
                    outfile.write("            if(response." + arrayBasenameFromVariableName(item) + "[i])\n")
                    outfile.write("            {\n")
                    outfile.write("                out << static_cast<char>(response." + arrayBasenameFromVariableName(item) + "[i]);\n")
                    outfile.write("            }\n")
                    outfile.write("        }\n")
                    outfile.write("        out << \"\\\"\";\n")
                else:
                    outfile.write("        out << \" " + arrayBasenameFromVariableName(item) + "=[ \";\n")
                    outfile.write("        out << std::setfill('0') << std::hex << std::uppercase;\n")
                    outfile.write("        for(uint8_t i=0; i<" + str(arraySize) + "; i++)\n")
                    outfile.write("        {\n")
                    outfile.write("            out << std::setw(2) << static_cast<int>(response." + arrayBasenameFromVariableName(item) + "[i]) << \" \";\n")
                    outfile.write("        }\n")
                    outfile.write("        out << \"]\";\n")
                    outfile.write("        out << std::dec;\n")

        outfile.write("    }\n")
        outfile.write("\n")
        outfile.write("    void print(std::ostream& out, std::vector<uint8_t> responsePayload) const override\n")
        outfile.write("    {\n")
        outfile.write("        if (m_response.size() >= (COMMANDS::" + commandName.upper() + "::RESPONSE_LENGTH + 4)) {\n")
        outfile.write("            COMMANDS::" + commandName.upper() + "::response_t response(\n")
        outfile.write("                (uint8_t*)&responsePayload.data()[0]);\n")
        outfile.write("            printResponse(out, response);\n")
        outfile.write("        } else\n")
        outfile.write("        {\n")
        outfile.write("            std::cout << \"" + commandName.upper() + ": insufficient data\" << std::endl;\n")
        outfile.write("        }\n")
        outfile.write("    };\n")
        outfile.write("\n")
        outfile.write("    COMMANDS::" + commandName.upper() + "::response_t responseStruct()\n")
        outfile.write("    {\n")
        outfile.write("        return { (uint8_t*)&m_response.data()[PROTOCOL::HEADER::LENGTH] };\n")
        outfile.write("    };\n")
        outfile.write("};\n")
        outfile.write("\n")

def generateFile(commandId, commandName, 
                 commandPayloadByteNames, 
                 responsePayloadByteNames):

    generateCommandFile(commandName, commandPayloadByteNames, responsePayloadByteNames)

    os.makedirs("include", exist_ok=True)

    # add include to payloads.hpp
    with open("include/payloads.hpp", 'a') as outfile:
        outfile.write(f"#include <{commandName}/payload.hpp>\n")

    # add include to commands.hpp
    with open("include/commands.hpp", 'a') as outfile:
        outfile.write(f"#include <{commandName}/command.hpp>\n")

    os.makedirs("include/common", exist_ok=True)
    with open("include/common/command_id.hpp", 'a') as outfile:
        outfile.write("    " + commandName.upper() + " = " + str(commandId) + ",\n")

    os.makedirs("include/" + commandName, exist_ok=True)

    payloadFile = f"include/{commandName}/payload.hpp"

    # header and open both namespaces
    with open(payloadFile, 'w') as outfile:
        outfile.write("#pragma once\n")
        outfile.write("\n")
        outfile.write("// This file is generated with the script: `interface/libs/commands/generate.py`\n")
        outfile.write("\n")

        outfile.write("#include <common/command_id.hpp>\n")
        outfile.write("\n")
        outfile.write("namespace COMMANDS {\n")
        outfile.write("\n")
        outfile.write("namespace " + commandName.upper() + " {\n")

    # add constexpr
    with open(payloadFile, 'a') as outfile:
        outfile.write("    constexpr uint8_t COMMAND_LENGTH = " + str(calculateStructSizeFromNames(commandPayloadByteNames)) + ";\n")
        outfile.write("    constexpr uint8_t RESPONSE_LENGTH = " + str(calculateStructSizeFromNames(responsePayloadByteNames)) + ";\n")
        outfile.write("\n")
        outfile.write("    static_assert(COMMAND_LENGTH < COMMANDS::MAX_PAYLOAD_LENGTH, \"COMMAND_LENGTH larger than max payload\");\n")
        outfile.write("    static_assert(RESPONSE_LENGTH < COMMANDS::MAX_PAYLOAD_LENGTH, \"RESPONSE_LENGTH larger than max payload\");\n")
        outfile.write("\n")

    # add command struct
    with open(payloadFile, 'a') as outfile:
        outfile.write("    typedef struct command {\n")
        outfile.write("        command()\n")
        outfile.write("        {\n")
        outfile.write("            OI = static_cast<uint8_t>(COMMANDS::OI::" + commandName.upper() + ");\n")
        outfile.write("            OL = COMMAND_LENGTH;\n")
        outfile.write("        }\n")
        outfile.write("\n")
        outfile.write("        command(uint8_t* cmd)\n")
        outfile.write("        {\n")
        outfile.write("            OI = cmd[0];\n")
        outfile.write("            OL = cmd[1];\n")

        # loop through commandPayloadByteNames
        index = 2
        for item in commandPayloadByteNames:
            arraySize = arraySizeFromVariableName(item)
            if(arraySize > 1):
                outfile.write("            for(uint8_t i=0; i<" + str(arraySize) + "; i++)\n")
                outfile.write("            {\n")
                outfile.write("                " + arrayBasenameFromVariableName(item) + "[i] = cmd[" + str(index) + " + i];\n")
                outfile.write("            }\n")
            else:
                outfile.write("            " + item + " = cmd[" + str(index) + "];\n")
            index = index + arraySize

        outfile.write("        }\n")
        outfile.write("\n")

        # autogenerated getters and setters
        for item in commandPayloadByteNames:

            arraySize = arraySizeFromVariableName(item)
            if(arraySize == 1):
                outfile.write("        uint8_t get" + item.capitalize() + "()\n")
                outfile.write("        {\n")
                outfile.write("            return(" + item + ");\n")
                outfile.write("        }\n")
                outfile.write("\n")
                outfile.write("        void set" + item.capitalize() + "(uint8_t value)\n")
                outfile.write("        {\n")
                outfile.write("            " + item + " = value;\n")
                outfile.write("        }\n")
                outfile.write("\n")
            if(arraySize == 2):
                outfile.write("        uint16_t get" + arrayBasenameFromVariableName(item).capitalize() + "()\n")
                outfile.write("        {\n")
                outfile.write("            return(((uint16_t)" + arrayBasenameFromVariableName(item) + "[1]) << 8 | " + arrayBasenameFromVariableName(item) + "[0]);\n")
                outfile.write("        }\n")
                outfile.write("\n")
                outfile.write("        void set" + arrayBasenameFromVariableName(item).capitalize() + "(uint16_t value)\n")
                outfile.write("        {\n")
                outfile.write("            " + arrayBasenameFromVariableName(item) + "[1] = (uint8_t)(value >> 8);\n")
                outfile.write("            " + arrayBasenameFromVariableName(item) + "[0] = (uint8_t)value;\n")
                outfile.write("        }\n")
                outfile.write("\n")

        outfile.write("        uint8_t OI;\n")
        outfile.write("        uint8_t OL;\n")

        # loop through commandPayloadByteNames
        index = 2
        for item in commandPayloadByteNames:
            outfile.write("        uint8_t " + item + ";\n")
            index = index + 1

        outfile.write("    } command_t;\n")
        outfile.write("\n")

    # add response struct
    with open(payloadFile, 'a') as outfile:
        outfile.write("    typedef struct response {\n")
        outfile.write("        response()\n")
        outfile.write("        {\n")
        outfile.write("            OI = static_cast<uint8_t>(COMMANDS::OI::" + commandName.upper() + ");\n")
        outfile.write("            OL = RESPONSE_LENGTH;\n")
        outfile.write("        }\n")
        outfile.write("\n")
        outfile.write("        response(uint8_t* res)\n")
        outfile.write("        {\n")
        outfile.write("            OI = res[0];\n")
        outfile.write("            OL = res[1];\n")

        # loop through commandPayloadByteNames
        index = 2
        for item in responsePayloadByteNames:
            arraySize = arraySizeFromVariableName(item)
            if(arraySize > 1):
                outfile.write("            for(uint8_t i=0; i<" + str(arraySize) + "; i++)\n")
                outfile.write("            {\n")
                outfile.write("                " + arrayBasenameFromVariableName(item) + "[i] = res[" + str(index) + " + i];\n")
                outfile.write("            }\n")
            else:
                outfile.write("            " + item + " = res[" + str(index) + "];\n")
            index = index + arraySize

        outfile.write("        }\n")
        outfile.write("\n")
        outfile.write("        void serialize(uint8_t* response)\n")
        outfile.write("        {\n")
        outfile.write("            response[0] = OI;\n")
        outfile.write("            response[1] = OL;\n")

        # loop through commandPayloadByteNames
        index = 2
        for item in responsePayloadByteNames:
            arraySize = arraySizeFromVariableName(item)
            if(arraySize > 1):
                outfile.write("            for(uint8_t i=0; i<" + str(arraySize) + "; i++)\n")
                outfile.write("            {\n")
                outfile.write("                response[" + str(index) + " + i] = " + arrayBasenameFromVariableName(item) + "[i];\n")
                outfile.write("            }\n")
            else:
                outfile.write("            response[" + str(index) + "] = " + item + ";\n")
            index = index + arraySize

        outfile.write("        }\n")
        outfile.write("\n")

        # autogenerated getters and setters
        for item in responsePayloadByteNames:

            arraySize = arraySizeFromVariableName(item)
            if(arraySize == 1):
                outfile.write("        uint8_t get" + item.capitalize() + "()\n")
                outfile.write("        {\n")
                outfile.write("            return(" + item + ");\n")
                outfile.write("        }\n")
                outfile.write("\n")
                outfile.write("        void set" + item.capitalize() + "(uint8_t value)\n")
                outfile.write("        {\n")
                outfile.write("            " + item + " = value;\n")
                outfile.write("        }\n")
                outfile.write("\n")
            if(arraySize == 2):
                outfile.write("        uint16_t get" + arrayBasenameFromVariableName(item).capitalize() + "()\n")
                outfile.write("        {\n")
                outfile.write("            return(((uint16_t)" + arrayBasenameFromVariableName(item) + "[1]) << 8 | " + arrayBasenameFromVariableName(item) + "[0]);\n")
                outfile.write("        }\n")
                outfile.write("\n")
                outfile.write("        void set" + arrayBasenameFromVariableName(item).capitalize() + "(uint16_t value)\n")
                outfile.write("        {\n")
                outfile.write("            " + arrayBasenameFromVariableName(item) + "[1] = (uint8_t)(value >> 8);\n")
                outfile.write("            " + arrayBasenameFromVariableName(item) + "[0] = (uint8_t)value;\n")
                outfile.write("        }\n")
                outfile.write("\n")

        outfile.write("        uint8_t OI;\n")
        outfile.write("        uint8_t OL;\n")

        # loop through responsePayloadByteNames
        index = 2
        for item in responsePayloadByteNames:
            outfile.write(f"        uint8_t {item};\n")
            index = index + 1

        outfile.write("\n")
        outfile.write("    } response_t;\n")

    # close both namespaces
    with open(payloadFile, 'a') as outfile:
        outfile.write("}\n")
        outfile.write("\n")
        outfile.write("} // namespace commands\n")

def generateCommonHeaderFiles(): 
    # add include to payloads.hpp

    os.makedirs("include", exist_ok=True)

    with open("include/payloads.hpp", 'w') as outfile:
        outfile.write("#pragma once\n")
        outfile.write("\n")
        outfile.write("// This file is generated with the script: `interface/libs/commands/generate.py`\n")
        outfile.write("\n")
        outfile.write("#include <stdint.h>\n")
        outfile.write("\n")
        outfile.write("#include <common/command_id.hpp>\n")

    # add include to commands.hpp
    with open("include/commands.hpp", 'w') as outfile:
        outfile.write("#pragma once\n")
        outfile.write("\n")
        outfile.write("// This file is generated with the script: `interface/libs/commands/generate.py`\n")
        outfile.write("\n")
        outfile.write("#include <common/uartCommandBase.hpp>\n")
        outfile.write("\n")

generateCommonHeaderFiles()

generateCommandIdOpen()

# command definitions. Parameters:
# generateFile(commandName, commandData, responseData)

generateFile(2, "blink", [], [])
generateFile(3, "sha1", ["data[20]"], ["data[20]"])
generateFile(4, "hotp", [], ["data[16]"])
generateFile(5, "eeprom_write", ["address[2]", "data"], ["address[2]", "data"])
generateFile(6, "eeprom_read", ["address[2]"], ["address[2]", "data"])
generateFile(7, "aes", ["type", "data[16]"], ["type", "data[16]"])
generateFile(8, "pwm", ["port", "pin", "value"], ["port", "pin", "value"])
generateFile(9, "random", [], ["data[16]"])
generateFile(10, "debug", [], ["data[32]"])
generateFile(11, "gpio", [], ["portB", "portC", "portD"])
generateFile(12, "i2c_write", ["device", "registerAddress[2]", "length", "data[16]"], ["status"])
generateFile(13, "i2c_read", ["device", "registerAddress[2]", "length"], ["device", "registerAddress[2]", "status", "length", "data[16]"])
generateFile(14, "ina219", [], ["current[2]", "voltage[2]", "status"])
generateFile(15, "ds18b20", [], ["temperature[2]", "status"])
generateFile(16, "set_key", ["keyId", "keyValue[16]"], ["status"])
generateFile(17, "set_device_info", ["name[16]"], ["status"])
generateFile(18, "get_device_info", [], ["nameString[16]", "versionString[32]", "status"])
generateFile(19, "ws2812b", ["red[45]", "green[45]", "blue[45]"], [])
generateFile(20, "ssd1306", ["line", "data[16]"], [])
generateFile(21, "timer", [], ["pulseWidth[2]"])
generateFile(22, "spi_read", ["reg", "length"], ["reg", "length", "data[32]"])
generateFile(23, "spi_write", ["reg", "length", "data[32]"], ["status"])
generateFile(24, "nrf24l01_init", ["txAddr[5]", "rxAddr[5]", "rfChannel", "gateway"], ["status"])
generateFile(25, "nrf24l01_read", ["length", "data[128]"], ["length", "data[128]"])
generateFile(26, "nrf24l01_write", ["length", "data[128]"], ["status", "length", "data[128]"])
generateFile(27, "radio_uart", ["mode"], ["status"])
generateFile(28, "vcc", [], ["vcc[2]"])
generateFile(29, "sleep", ["delay[4]"], ["status"])
generateFile(30, "wakeup", ["checkAttentionFlag"], ["status", "attention"])
generateFile(31, "set_node_address", ["nodeAddress"], ["status"])
generateFile(32, "keep_alive", ["time"], ["status"])
generateFile(33, "ping", [], [])
generateFile(34, "quadrature_encoder", [], ["countnegative[2]", "countpositive[2]", "switchposition", "switchcount[2]", "status"])

generateCommandIdClose()
