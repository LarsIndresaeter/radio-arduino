#include "commandCallback.hpp"
#include "include/commandCallback.hpp"
#include <ratio>
#include <thread>

using namespace std::chrono_literals;
using namespace std::chrono;

using nlohmann::json;

CommandCallback::CommandCallback(

    mqtt::async_client& mqtt_client)
    : m_mqttClient(mqtt_client)
{
}

void CommandCallback::connection_lost(const std::string& cause)
{
    std::cout << "Connection lost: " << cause << std::endl;
}

void CommandCallback::delivery_complete(mqtt::delivery_token_ptr token)
{
    std::cout << "Message delivered" << std::endl;
}

void CommandCallback::resendBirthCertificate()
{
    std::string topic = "radio-arduino/RCMD";

    json command = { "command", "resendBirthCertificate" };

    publishMessage(topic, command.dump());
}

void CommandCallback::publishMessage(std::string topic, std::string message)
{
    const int QOS = 0;
    mqtt::topic mqttTopic(m_mqttClient, topic, QOS, false);

    try {
        mqttTopic.publish(std::move(message));
    }
    catch (std::exception const& e) {
        std::cout << "ERROR: publishMessage" << std::endl;
        std::cerr << e.what() << std::endl;
        std::cerr << "topic: " << topic << std::endl;
        std::cerr << "message: " << message << std::endl;
    }
}

std::vector<std::string> CommandCallback::splitString(std::string s, const std::string& delimiter)
{
    std::vector<std::string> tokens;
    size_t pos = 0;
    std::string token;

    while ((pos = s.find(delimiter)) != std::string::npos) {
        token = s.substr(0, pos);
        tokens.push_back(token);
        s.erase(0, pos + delimiter.length());
    }

    tokens.push_back(s);
    return tokens;
}

void CommandCallback::pollNode(std::vector<std::string> commandList, uint32_t nodeAddress)
{
    //TODO: keep timestamp of last poll. do not send a new command before 10 seconds have passed
   
    std::string topic = "radio-arduino/RCMD/proxy/" + std::to_string(nodeAddress);

    json jsonCommandList;
    for (std::string command : commandList) {
        jsonCommandList.push_back(command);
    }

    uint64_t timestamp = (duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count());

    json command = { { "commandList", jsonCommandList }, { "nodeAddress", nodeAddress }, { "timestamp", timestamp } };

    publishMessage(topic, command.dump());
}

void CommandCallback::executeSubscriptionsForNode(uint32_t nodeId)
{
    std::vector<std::string> commandList;

    for (int j = 0; j < m_subscriptions.size(); j++) {
        if (m_subscriptions.at(j).nodeAddress == nodeId) {
            using namespace std::chrono;
            auto start = std::chrono::high_resolution_clock::now();
            uint64_t time_since_epoch_ms
                = std::chrono::duration_cast<std::chrono::milliseconds>(start.time_since_epoch()).count();

            int diff = time_since_epoch_ms - m_subscriptions.at(j).timestampLastValidResponse;

            if ((diff > m_subscriptions.at(j).intervalInSeconds * 1000) || diff < 0) {
                commandList.push_back(m_subscriptions.at(j).commandName);
            }
        }
    }
    if (commandList.size() > 0) {
        pollNode(commandList, nodeId);
    }
}

void CommandCallback::message_arrived(mqtt::const_message_ptr message)
{
    std::string topic_orig = message->get_topic();
    std::string payload = message->get_payload_str();

    auto tokens = splitString(topic_orig, "/");
    uint32_t gatewayAddress = std::stoul(tokens.at(2));

    if (topic_orig.starts_with("radio-arduino/DBIRTH/")) {
        try {
            auto jsonData = json::parse(payload);
            uint32_t nodeAddress = jsonData["nodeAddress"];

            bool nodeExists
                = std::find(m_radioNodeIdList.begin(), m_radioNodeIdList.end(), nodeAddress) != m_radioNodeIdList.end();

            if (!nodeExists) {
                std::cout << "Added raduino node: " << std::to_string(nodeAddress) << std::endl;
                m_radioNodeIdList.push_back(nodeAddress);
                nodeInfo_t nodeInfo;
                nodeInfo.nodeAddress = nodeAddress;
                m_nodeInfoList.push_back(nodeInfo);
                // add subscriptions
                if (topic_orig.starts_with(
                        "radio-arduino/DBIRTH/" + std::to_string(gatewayAddress) + "/"
                        + std::to_string(gatewayAddress))) {
                    m_subscriptions.push_back({ "get_device_name", 60 * 120, nodeAddress });
                    m_subscriptions.push_back({ "get_attached_devices_csv_string", 60 * 120, nodeAddress });
                    m_subscriptions.push_back({ "get_statistics", 60 * 120, nodeAddress });
                    m_subscriptions.push_back({ "get_version", 60 * 120, nodeAddress });
                }
                else {
                    m_subscriptions.push_back({ "vcc", 60 * 10, nodeAddress });
                    m_subscriptions.push_back({ "gpio", 60 * 60, nodeAddress });
                    m_subscriptions.push_back({ "get_active_time_counter", 60, nodeAddress });
                    m_subscriptions.push_back({ "get_device_name", 60 * 120, nodeAddress });
                    m_subscriptions.push_back({ "get_statistics", 60 * 120, nodeAddress });
                    m_subscriptions.push_back({ "get_attached_devices_csv_string", 60 * 120, nodeAddress });
                }
            }
        }
        catch (std::exception const& e) {
            std::cout << "DEBUG: malformed birth certificate" << std::endl;
            std::cout << "     : " << payload << std::endl;
        }
    }

    if (topic_orig.starts_with("radio-arduino/ADVERTISEMENT/")) {
        try {
            auto jsonData = json::parse(payload);
            std::string nodeAddressString = tokens.at(3);
            uint32_t nodeAddress = stoul(tokens.at(3));

            bool nodeExists = false;
            for (int j = 0; j < m_nodeInfoList.size(); j++) {
                if (m_nodeInfoList.at(j).nodeAddress == nodeAddress) {
                    nodeExists = true;
                }
            }

            if (nodeExists) {
                executeSubscriptionsForNode(nodeAddress);
            }
            else {
                resendBirthCertificate();
            }
        }
        catch (std::exception const& e) {
            std::cout << "DEBUG: malformed ADVERTISEMENT" << std::endl;
            std::cout << "     : " << payload << std::endl;
        }
    }

    if (topic_orig.starts_with("radio-arduino/STATE/")) {
        std::string nodeAddressString = tokens.at(3);
        uint32_t nodeAddress = stoul(tokens.at(3));
    }

    if (topic_orig.starts_with("radio-arduino/DDATA/")) {
        try {
            auto jsonData = json::parse(payload);

            std::string nodeAddressString = tokens.at(3);
            std::string responseCode = jsonData["responseCode"];
            std::string commandName = jsonData["name"];
            uint64_t timestamp = jsonData["timestamp"].get<uint64_t>();

            for (int i = 0; i < m_subscriptions.size(); i++) {
                if ((std::to_string(m_subscriptions.at(i).nodeAddress) == nodeAddressString)
                    && (responseCode == "success") && (commandName == m_subscriptions.at(i).commandName)

                ) {
                    if (commandName == "get_attached_devices_csv_string") {
                        uint32_t nodeAddress = std::stoul(nodeAddressString);
                        auto jsonData = json::parse(payload);
                        std::string csvString = jsonData["payload"]["csvString"];
                        if (csvString.find("lsm303d") != std::string::npos) {
                            // TODO: make method that check if exist before adding
                            m_subscriptions.push_back({ "get_lsm303d", 60 * 5, nodeAddress });
                        }
                        else if (csvString.find("quad") != std::string::npos) {
                            m_subscriptions.push_back({ "quadrature_encoder", 60 * 5, nodeAddress });
                        }
                        else if (csvString.find("ina219") != std::string::npos) {
                            m_subscriptions.push_back({ "ina219", 5, nodeAddress });
                        }
                    }

                    m_subscriptions.at(i).lastValidResponse = payload;
                    m_subscriptions.at(i).timestampLastValidResponse = timestamp;

                    std::string republish_topic = "raduino-subscriptions/DDATA/"
                        + std::to_string(m_subscriptions.at(i).nodeAddress) + "/" + commandName;

                    publishMessage(republish_topic, payload);
                }
            }
        }
        catch (std::exception const& e) {
            std::cout << "DEBUG: malformed DDATA. " << e.what() << std::endl;
        }
    }
}

