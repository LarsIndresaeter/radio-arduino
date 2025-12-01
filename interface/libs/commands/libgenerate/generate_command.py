#!/usre/bin/env python3

import os
from libgenerate.common import *

def generateCommandIdOpen(codePath):
    os.makedirs(codePath, exist_ok=True)
    with open(codePath + "/command_id.hxx", 'w') as outfile:
        outfile.write("#pragma once\n") 
        outfile.write("// This file is generated with the script: `interface/libs/commands/generate.py`\n")
        outfile.write("\n") 
        outfile.write("namespace COMMANDS {\n") 
        outfile.write("\n") 
        outfile.write("constexpr uint8_t MAX_PAYLOAD_LENGTH = 64;\n")
        outfile.write("constexpr uint8_t MAX_PACKAGE_LENGTH = (MAX_PAYLOAD_LENGTH + 4 + 2 + 20); // + header, crypto, checksum\n")
        outfile.write("\n")
        outfile.write("enum class OI\n")
        outfile.write("{\n")
        outfile.write("    UNDEFINED = 0,\n")
        outfile.write("    RESERVED = 1,\n")

def generatePayloadsOpen(codePath):
    os.makedirs(codePath, exist_ok=True)
    with open(codePath + "/payloads.hxx", 'w') as outfile:
        outfile.write("#pragma once\n")
        outfile.write("\n")
        outfile.write("// This file is generated with the script: `interface/libs/commands/generate.py`\n")
        outfile.write("\n")
        outfile.write("#include <stdint.h>\n")
        outfile.write("\n")
        outfile.write("#include <cmd/command_id.hxx>\n")

def generateCommandsOpen(codePath):
    os.makedirs(codePath, exist_ok=True)
    with open(codePath + "/commands.hxx", 'w') as outfile:
        outfile.write("#pragma once\n")
        outfile.write("\n")
        outfile.write("// This file is generated with the script: `interface/libs/commands/generate.py`\n")
        outfile.write("\n")
        outfile.write("#include <common/raduinoCommandBase.hpp>\n")
        outfile.write("\n")

def generateCommonHeaderFilesOpen(codePath): 
    generateCommandIdOpen(codePath)
    generatePayloadsOpen(codePath)
    generateCommandsOpen(codePath)

def generateCommandIdClose(codePath):
    os.makedirs(codePath, exist_ok=True)
    with open(codePath + "/command_id.hxx", 'a') as outfile:
        outfile.write("};\n")
        outfile.write("\n")
        outfile.write("} // namespace COMMANDS\n")

def generateCommonHeaderFilesClose(codePath): 
    generateCommandIdClose(codePath)

def generateCommandIdAppend(codePath, commandId, commandName):
    os.makedirs(codePath, exist_ok=True)

    # add command to command_id.hxx
    with open(f"{codePath}/command_id.hxx", 'a') as outfile:
        outfile.write("    " + commandName.upper() + " = " + str(commandId) + ",\n")

    # add include to payloads.hxx
    with open(f"{codePath}/payloads.hxx", 'a') as outfile:
        outfile.write(f"#include <cmd/{commandName}/payload.hxx>\n")

    # add include to commands.hxx
    with open(f"{codePath}/commands.hxx", 'a') as outfile:
        outfile.write(f"#include <cmd/{commandName}/command.hxx>\n")

def generateCommandFile(codePath,
                        commandId,
                        commandName,
                        commandPayloadFields,
                        responsePayloadFields):
    os.makedirs(f"{codePath}/{commandName}", exist_ok=True)
    commandFile = f"{codePath}/{commandName}/command.hxx"

    generateCommandIdAppend(codePath, commandId, commandName)

    with open(commandFile, 'w') as outfile:
        createStringConstructor = False
        outfile.write("#pragma once\n")
        outfile.write("// This file is generated with the script: `interface/libs/commands/generate.py`\n")
        outfile.write("\n")
        outfile.write("#include <common/raduinoCommandBase.hpp>\n")
        outfile.write("\n")
        outfile.write("class RaduinoCommand" + snakecaseToCamelCase(commandName) + " : public RaduinoCommandBase {\n")
        outfile.write("public:\n")
        outfile.write("    RaduinoCommand" + snakecaseToCamelCase(commandName) + "(")
        # add parameters
        items = len(commandPayloadFields)
        for item in commandPayloadFields:
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
        outfile.write("        : RaduinoCommandBase(\n")
        outfile.write("              static_cast<uint8_t>(COMMANDS::OI::" + commandName.upper() + "),\n")
        outfile.write("              COMMANDS::" + commandName.upper() + "::COMMAND_LENGTH)\n")
        outfile.write("    {\n")
        # initialize command
        outfile.write("        COMMANDS::" + commandName.upper() + "::command_t command;\n")
        for item in commandPayloadFields:
            arraySize = arraySizeFromVariableName(item)
            if(arraySize == 1):
                outfile.write("\n")
                outfile.write("        m_payload.at(offsetof(COMMANDS::" + commandName.upper() + "::command_t, " + item + ")) = " + item + ";\n")
            if(arraySize == 2):
                outfile.write("\n")
                outfile.write("        m_payload.at(offsetof(COMMANDS::" + commandName.upper() + "::command_t, " + arrayBasenameFromVariableName(item) + ") + 1) = " + arrayBasenameFromVariableName(item) + ">>8;\n")
                outfile.write("        m_payload.at(offsetof(COMMANDS::" + commandName.upper() + "::command_t, " + arrayBasenameFromVariableName(item) + ")) = " + arrayBasenameFromVariableName(item) + ";\n")
            if(arraySize == 4):
                outfile.write("\n")
                outfile.write("        m_payload.at(offsetof(COMMANDS::" + commandName.upper() + "::command_t, " + arrayBasenameFromVariableName(item) + ") + 3) = " + arrayBasenameFromVariableName(item) + ">>24;\n")
                outfile.write("        m_payload.at(offsetof(COMMANDS::" + commandName.upper() + "::command_t, " + arrayBasenameFromVariableName(item) + ") + 2) = " + arrayBasenameFromVariableName(item) + ">>16;\n")
                outfile.write("        m_payload.at(offsetof(COMMANDS::" + commandName.upper() + "::command_t, " + arrayBasenameFromVariableName(item) + ") + 1) = " + arrayBasenameFromVariableName(item) + ">>8;\n")
                outfile.write("        m_payload.at(offsetof(COMMANDS::" + commandName.upper() + "::command_t, " + arrayBasenameFromVariableName(item) + ")) = " + arrayBasenameFromVariableName(item) + ";\n")
            if(arraySize > 4):
                createStringConstructor = True
                outfile.write("\n")
                outfile.write("        for (int i = 0; i < sizeof(command." + arrayBasenameFromVariableName(item) + "); i++) {\n")
                outfile.write("            if (i >= " + arrayBasenameFromVariableName(item) + ".size()) {\n")
                outfile.write("                break;\n")
                outfile.write("            }\n")
                outfile.write("            m_payload.at(\n")
                outfile.write("                offsetof(COMMANDS::" + commandName.upper() + "::command_t, " + arrayBasenameFromVariableName(item) + "[0]) + i)\n")
                outfile.write("                = " + arrayBasenameFromVariableName(item) + ".at(i);\n")
                outfile.write("        }\n")
        outfile.write("    };\n")

        if createStringConstructor:
            outfile.write("\n")
            outfile.write("    // string constructor\n")
            outfile.write("    RaduinoCommand" + snakecaseToCamelCase(commandName) + "(")
            # add parameters
            items = len(commandPayloadFields)
            for item in commandPayloadFields:
                arraySize = arraySizeFromVariableName(item)
                if(arraySize == 1):
                    outfile.write("uint8_t " + item)
                if(arraySize == 2):
                    outfile.write("uint16_t " + arrayBasenameFromVariableName(item))
                if(arraySize == 4):
                    outfile.write("uint32_t " + arrayBasenameFromVariableName(item))
                if(arraySize > 4):
                    outfile.write("std::string " + arrayBasenameFromVariableName(item))
                items = items - 1
                if items > 0:
                    outfile.write(", ")
            outfile.write(")\n")
     
            outfile.write("        : RaduinoCommandBase(\n")
            outfile.write("              static_cast<uint8_t>(COMMANDS::OI::" + commandName.upper() + "),\n")
            outfile.write("              COMMANDS::" + commandName.upper() + "::COMMAND_LENGTH)\n")
            outfile.write("    {\n")
            # initialize command
            outfile.write("        COMMANDS::" + commandName.upper() + "::command_t command;\n")
            for item in commandPayloadFields:
                arraySize = arraySizeFromVariableName(item)
                if(arraySize == 1):
                    outfile.write("\n")
                    outfile.write("        m_payload.at(offsetof(COMMANDS::" + commandName.upper() + "::command_t, " + item + ")) = " + item + ";\n")
                if(arraySize == 2):
                    outfile.write("\n")
                    outfile.write("        m_payload.at(offsetof(COMMANDS::" + commandName.upper() + "::command_t, " + arrayBasenameFromVariableName(item) + ") + 1) = " + arrayBasenameFromVariableName(item) + ">>8;\n")
                    outfile.write("        m_payload.at(offsetof(COMMANDS::" + commandName.upper() + "::command_t, " + arrayBasenameFromVariableName(item) + ")) = " + arrayBasenameFromVariableName(item) + ";\n")
                if(arraySize == 4):
                    outfile.write("\n")
                    outfile.write("        m_payload.at(offsetof(COMMANDS::" + commandName.upper() + "::command_t, " + arrayBasenameFromVariableName(item) + ") + 3) = " + arrayBasenameFromVariableName(item) + ">>24;\n")
                    outfile.write("        m_payload.at(offsetof(COMMANDS::" + commandName.upper() + "::command_t, " + arrayBasenameFromVariableName(item) + ") + 2) = " + arrayBasenameFromVariableName(item) + ">>16;\n")
                    outfile.write("        m_payload.at(offsetof(COMMANDS::" + commandName.upper() + "::command_t, " + arrayBasenameFromVariableName(item) + ") + 1) = " + arrayBasenameFromVariableName(item) + ">>8;\n")
                    outfile.write("        m_payload.at(offsetof(COMMANDS::" + commandName.upper() + "::command_t, " + arrayBasenameFromVariableName(item) + ")) = " + arrayBasenameFromVariableName(item) + ";\n")
                if(arraySize > 4):
                    outfile.write("\n")
                    outfile.write("        for (int i = 0; i < " + arrayBasenameFromVariableName(item) + ".size() && i < " + str(arraySize) + "; i++) {\n")
                    outfile.write("            m_payload.at(\n")
                    outfile.write("                offsetof(COMMANDS::" + commandName.upper() + "::command_t, " + arrayBasenameFromVariableName(item) + "[0]) + i)\n")
                    outfile.write("                = " + arrayBasenameFromVariableName(item) + ".at(i);\n")
                    outfile.write("        }\n")
            outfile.write("    };\n")

        outfile.write("\n")
        outfile.write("    void printResponse(std::ostream& out, COMMANDS::" + commandName.upper() + "::response_t response) const\n") 
        outfile.write("    {\n")
        outfile.write("        out << \"" + commandName.upper().ljust(20) + "   : \";\n")

        for item in responsePayloadFields:
            arraySize = arraySizeFromVariableName(item)
            if(arraySize <= 4):
                outfile.write("        out << \" " + arrayBasenameFromVariableName(item) + "=\" << " + "static_cast<uint32_t>(response.get" + arrayBasenameFromVariableName(item).capitalize() + "());\n");
            else:
                if(arrayBasenameFromVariableName(item).lower().find("string") > 0):
                    outfile.write("        out << \" " + arrayBasenameFromVariableName(item) + "=\\\"\";\n")
                    outfile.write("        for (uint8_t i = 0; i < " + str(arraySize) + "; i++) {\n")
                    outfile.write("            if (response." + arrayBasenameFromVariableName(item) + "[i])\n")
                    outfile.write("            {\n")
                    outfile.write("                out << static_cast<char>(response." + arrayBasenameFromVariableName(item) + "[i]);\n")
                    outfile.write("            }\n")
                    outfile.write("        }\n")
                    outfile.write("        out << \"\\\"\";\n")
                else:
                    outfile.write("        out << \" " + arrayBasenameFromVariableName(item) + "=[ \";\n")
                    outfile.write("        out << std::setfill('0') << std::hex << std::uppercase;\n")
                    outfile.write("        for (uint8_t i = 0; i < " + str(arraySize) + "; i++) {\n")
                    outfile.write("            out << std::setw(2) << static_cast<uint32_t>(response." + arrayBasenameFromVariableName(item) + "[i]) << \" \";\n")
                    outfile.write("        }\n")
                    outfile.write("        out << \"]\";\n")
                    outfile.write("        out << std::dec;\n")
        outfile.write("    }\n")
        outfile.write("\n")

        # getters for arrays
        for item in responsePayloadFields:
            arraySize = arraySizeFromVariableName(item)
            if(arraySize > 4):
                if(arrayBasenameFromVariableName(item).lower().find("string") > 0):
                    outfile.write("    std::string get" + arrayBasenameFromVariableName(item).capitalize() + "()\n")
                    outfile.write("    {\n")
                    outfile.write("        std::string retval;\n")
                    outfile.write("        COMMANDS::" + arrayBasenameFromVariableName(commandName).upper() + "::response_t response = responseStruct();\n")
                    outfile.write("\n")
                    # outfile.write("        retval.append(\"\\\"\");\n")
                    outfile.write("        for (uint8_t i = 0; i < " + str(arraySizeFromVariableName(item)) + "; i++) {\n")
                    outfile.write("            if (response." + arrayBasenameFromVariableName(item) + "[i])\n")
                    outfile.write("            {\n")
                    outfile.write("                retval.push_back(static_cast<char>(response." + arrayBasenameFromVariableName(item) + "[i]));\n")
                    outfile.write("            }\n")
                    outfile.write("        }\n")
                    # outfile.write("        retval.append(\"\\\"\");\n")
                    outfile.write("\n")
                    outfile.write("        return (retval);\n")
                    outfile.write("    }\n")
                else:
                    outfile.write("    std::string get" + arrayBasenameFromVariableName(item).capitalize() + "()\n")
                    outfile.write("    {\n")
                    outfile.write("        std::string retval;\n")
                    outfile.write("        COMMANDS::" + arrayBasenameFromVariableName(commandName).upper() + "::response_t response = responseStruct();\n")
                    outfile.write("\n")
                    outfile.write("        retval.append(\"[\");\n")
                    outfile.write("        for (uint8_t i = 0; i < " + str(arraySize) + "; i++) {\n")
                    outfile.write("            retval.append(\" \\\"\");\n")
                    outfile.write("            retval.append(std::to_string(static_cast<uint32_t>(response." + arrayBasenameFromVariableName(item) + "[i])));\n")
                    outfile.write("            if (i < (" + str(arraySize) + " - 1)) {\n")
                    outfile.write("                retval.append(\"\\\",\");\n")
                    outfile.write("            }\n")
                    outfile.write("            else {\n")
                    outfile.write("                retval.append(\"\\\"\");\n")
                    outfile.write("            }\n")
                    outfile.write("        }\n")
                    outfile.write("        retval.append(\" ]\");\n")
                    outfile.write("\n")
                    outfile.write("        return (retval);\n")
                    outfile.write("    }\n")

        outfile.write("    std::string getCommandName() { return \"" + commandName + "\"; }\n");
        outfile.write("\n")
        outfile.write("    std::string getJson()\n");
        outfile.write("    {\n");
        outfile.write("        std::string json;\n");
        outfile.write("        json.append(\"{\");\n");
        outfile.write("        json.append(\"\\\"name\\\":\\\"\" + getCommandName() + \"\\\", \");\n");
        outfile.write("        json.append(getJsonCommonFields());\n")
        outfile.write("        json.append(\"\\\"payload\\\":{\");\n")

        index = 0
        for item in responsePayloadFields:
            index = index + 1
            arraySize = arraySizeFromVariableName(item)
            if(arraySize <= 4):
                outfile.write("        json.append(\"\\\"" + arrayBasenameFromVariableName(item) + "\\\":\");\n");
                outfile.write("        json.append(std::to_string(responseStruct().get" + arrayBasenameFromVariableName(item).capitalize() + "()));\n");
            else:
                outfile.write("        json.append(\"\\\"" + arrayBasenameFromVariableName(item) + "\\\": \");\n");
                outfile.write("        json.append(\"\\\"\" + get" + arrayBasenameFromVariableName(item).capitalize() + "() + \"\\\"\");\n");
                outfile.write("        json.append(\"\");\n")

            if index < len(responsePayloadFields):
                outfile.write("        json.append(\", \");\n");

        outfile.write("        json.append(\"}}\");\n");
        outfile.write("        return (json);\n");
        outfile.write("    };\n");
        outfile.write("\n")
        outfile.write("    void print(std::ostream& out, std::vector<uint8_t> responsePayload) const override\n")
        outfile.write("    {\n")
        outfile.write("        if (m_response.size() >= (COMMANDS::" + commandName.upper() + "::RESPONSE_LENGTH + 4)) {\n")
        outfile.write("            COMMANDS::" + commandName.upper() + "::response_t response(\n")
        outfile.write("                (uint8_t*)&responsePayload.data()[0]);\n")
        outfile.write("            printResponse(out, response);\n")
        outfile.write("        }\n")
        outfile.write("        else {\n")
        outfile.write("            std::cout << \"" + commandName.upper() + ": insufficient data\" << std::endl;\n")
        outfile.write("        }\n")
        outfile.write("    };\n")
        outfile.write("\n")
        outfile.write("    COMMANDS::" + commandName.upper() + "::response_t responseStruct()\n")
        outfile.write("    {\n")
        outfile.write("        COMMANDS::" + commandName.upper() + "::response_t response;\n")
        outfile.write("\n")
        outfile.write("        if (m_responsePayload.size() >= sizeof(response)) {\n")
        outfile.write("            return { (uint8_t*)&m_responsePayload[0] };\n")
        outfile.write("        }\n")
        outfile.write("\n")
        outfile.write("        return (response);\n")
        outfile.write("    };\n")
        outfile.write("};\n")
        outfile.write("\n")

