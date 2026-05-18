#include "commandCallback.hpp"
#include "include/commandCallback.hpp"
#include <json_schema.h>
#include <nlohmann/json-schema.hpp>

using nlohmann::json;
using nlohmann::json_schema::json_validator;

CommandCallback::CommandCallback() {}

void CommandCallback::addDeviceController(std::shared_ptr<DeviceController> dc)
{
    m_deviceControllerList.push_back(dc);
}

void CommandCallback::message_arrived(mqtt::const_message_ptr message)
{
    try {
        std::string payload = message->get_payload_str();
        auto jsonData = json::parse(payload);

        json schema = json::parse(JSON_SCHEMA::adapter_json_schema);
        json_validator validator;
        validator.set_root_schema(schema);
        validator.validate(jsonData);
    }
    catch (std::exception const& e) {
        std::cout << "ERROR: Validation failed: " << e.what() << std::endl;
        // TODO: publish this to raduino-log/
    }

    if (message->get_topic() == "raduino-adapter/RCMD") {
        try {
            std::string payload = message->get_payload_str();
            auto jsonData = json::parse(payload);
            std::string command = jsonData["command"];

            if (command == "resendBirthCertificate") {
                for (int i = 0; i < m_deviceControllerList.size(); i++) {
                    m_deviceControllerList.at(i)->setPublishBirth(true);
                }
            }
        }
        catch (std::exception const& e) {
            std::cout << "ERROR: malformed RCMD" << std::endl;
        }
    }

    // command for node
    for (int i = 0; i < m_deviceControllerList.size(); i++) {
        if (m_deviceControllerList.at(i)->getTopicString() == message->get_topic()) {
            m_deviceControllerList.at(i)->parseMessage(message->get_topic(), message->get_payload_str());
        }
    }
}

void CommandCallback::connection_lost(const std::string& cause)
{
    std::cout << "Connection lost: " << cause << std::endl;
}

void CommandCallback::delivery_complete(mqtt::delivery_token_ptr token)
{
    std::cout << "Message delivered" << std::endl;
}
