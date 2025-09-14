import os

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

def generateFile(commandName, 
                 commandPayloadByteNames, 
                 responsePayloadByteNames,
                 includeFiles):

    # add include to payloads.hpp
    with open("include/payloads.hpp", 'a') as outfile:
        outfile.write("#include <" + commandName + "/payload.hpp>\n")

    # add include to commands.hpp
    with open("include/commands.hpp", 'a') as outfile:
        outfile.write("#include <" + commandName + "/command.hpp>\n")

    payloadFile = f"include/{commandName}/payload.hpp"

    # header and open both namespaces
    with open(payloadFile, 'w') as outfile:
        outfile.write("#pragma once\n")
        outfile.write("\n")

        if len(includeFiles) > 0:
            for item in includeFiles:
                outfile.write("#include <" + item + ">\n")
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
        outfile.write("        uint8_t OI;\n")
        outfile.write("        uint8_t OL;\n")

        # loop through responsePayloadByteNames
        index = 2
        for item in responsePayloadByteNames:
            outfile.write("        uint8_t " + item + ";\n")
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
    with open("include/payloads.hpp", 'w') as outfile:
        outfile.write("#pragma once\n")
        outfile.write("\n")
        outfile.write("#include <stdint.h>\n")
        outfile.write("\n")
        outfile.write("#include <common/command_id.hpp>\n")

    # add include to commands.hpp
    with open("include/commands.hpp", 'w') as outfile:
        outfile.write("#pragma once\n")
        outfile.write("\n")
        outfile.write("#include <common/uartCommandBase.hpp>\n")
        outfile.write("\n")

generateCommonHeaderFiles()

generateFile("aes", ["type", "data[16]"], ["type", "data[16]"], [])
generateFile("blink", [], [], [])
generateFile("debug", [], ["data[32]"], [])
generateFile("ds18b20", [], ["temperature[2]", "status"], [])
generateFile("eeprom_read", ["addressHigh", "addressLow"], ["addressHigh", "addressLow", "data"], [])
generateFile("eeprom_write", ["addressHigh", "addressLow", "data"], ["addressHigh", "addressLow", "data"], [])
generateFile("get_device_info", [], ["name[16]", "version[32]", "status"], [])
generateFile("gpio", [], ["portB", "portC", "portD"], [])
generateFile("hotp", [], ["data[16]"], [])
generateFile("i2c_read", ["device", "registerHigh", "registerLow", "length"], ["device", "registerHigh", "registerLow", "status", "length", "data[16]"], [])
generateFile("i2c_write", ["device", "registerHigh", "registerLow", "length", "data[16]"], ["status"], [])
generateFile("ina219", [], ["current[2]", "voltage[2]", "status"], [])
generateFile("keep_alive", ["time"], ["status"], [])
generateFile("nrf24l01_init", ["tx_addr[5]", "rx_addr[5]", "rf_channel", "gateway"], ["status"], [])
generateFile("nrf24l01_read", ["length", "data[128]"], ["length", "data[128]"], [])
generateFile("nrf24l01_write", ["length", "data[128]"], ["status", "length", "data[128]"], [])
generateFile("ping", [], [], [])
generateFile("pwm", ["port", "pin", "value"], ["port", "pin", "value"], [])
generateFile("quadrature_encoder", [], ["cnt_neg_high", "cnt_neg_low", "cnt_pos_high", "cnt_pos_low", "sw_pos", "sw_cnt_high", "sw_cnt_low", "status"], [])
generateFile("radio_uart", ["mode"], ["status"], [])
generateFile("random", [], ["data[16]"], [])
generateFile("set_device_info", ["name[16]"], ["status"], [])
generateFile("set_key", ["key_id", "key_value[16]"], ["status"], [])
generateFile("set_node_address", ["node_address"], ["status"], [])
generateFile("sha1", ["data[20]"], ["data[20]"], [])
generateFile("sleep", ["delay_0", "delay_1", "delay_2", "delay_3"], ["status"], [])
generateFile("spi_read", ["reg", "length"], ["reg", "length", "data[32]"], [])
generateFile("spi_write", ["reg", "length", "data[32]"], ["status"], [])
generateFile("ssd1306", ["line", "data[16]"], [], [])
generateFile("timer", [], ["pulse_width_high", "pulse_width_low"], [])
generateFile("vcc", [], ["vcc_h", "vcc_l"], [])
generateFile("wakeup", ["check_attention_flag"], ["status", "attention"], [])
generateFile("ws2812b", ["red[45]", "green[45]", "blue[45]"], [], [])

