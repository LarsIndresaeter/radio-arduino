#include "commandCallback.hpp"
#include "include/commandCallback.hpp"

using nlohmann::json;

CommandCallback::CommandCallback(

    mqtt::async_client& mqtt_client)
    : m_mqttClient(mqtt_client)
{
}

void CommandCallback::publishMessage(std::string topic, std::string message)
{
    const int QOS = 0;
    mqtt::topic mqttTopic(m_mqttClient, topic, QOS, false);

    try {
        mqttTopic.publish(std::move(message));
    }
    catch (const mqtt::exception& exc) {
        std::cerr << exc.what() << std::endl;
        std::cerr << "topic: " << topic << std::endl;
        std::cerr << "message: " << message << std::endl;
    }
}

nodepath_t CommandCallback::getNodePath(uint32_t nodeAddress)
{
    nodepath_t nodePath {};
    int bestHealthIndicator = 0;


    for (auto n : m_nodePath) {
        if (n.nodeAddress == nodeAddress) {
            if (n.healthIndicator >= bestHealthIndicator) {
                bestHealthIndicator = n.healthIndicator;
                nodePath = n;
            }
        }
    }

    return (nodePath);
}

void CommandCallback::updatePath(std::string gatewayName, uint32_t nodeAddress, uint64_t lastSeen, int healthIndicator)
{
    bool nodePathFound = false;
    for (auto n : m_nodePath) {
        if ((n.nodeAddress == nodeAddress) && (n.gatewayName == gatewayName)) {
            nodePathFound = true;
            n.gatewayName = gatewayName;
            n.nodeAddress = nodeAddress;
            n.lastSeen = lastSeen;
            n.healthIndicator = healthIndicator;
        }
    }

    if (!nodePathFound) {
        nodepath_t n;
        n.gatewayName = gatewayName;
        n.nodeAddress = nodeAddress;
        n.lastSeen = lastSeen;
        n.healthIndicator = healthIndicator;
        m_nodePath.push_back(n);
    }
}

void CommandCallback::message_arrived(mqtt::const_message_ptr message)
{
    std::string topic_orig = message->get_topic();
    std::string payload = message->get_payload_str();

    if (topic_orig.starts_with("radio-arduino/DBIRTH/")) {

        try {
            auto jsonData = json::parse(payload);
            std::string gatewayName = jsonData["gateway"];
            int healthIndicator = jsonData["healthIndicator"];
            uint64_t lastSeen = jsonData["lastSeen"];
            uint32_t nodeAddress = jsonData["nodeAddress"];
            updatePath(gatewayName, nodeAddress, lastSeen, healthIndicator);
        }
        catch (std::exception const& e) {
            std::cout << "DEBUG: malformed birth certificate" << std::endl;
        }
    }

    if (topic_orig.starts_with("radio-arduino/RCMD/proxy/")) {

        try {
            auto jsonData = json::parse(payload);
            uint32_t nodeAddress = jsonData["nodeAddress"];

            nodepath_t n = getNodePath(nodeAddress);
            if (n.nodeAddress == 0) {
                std::cout << "DEBUG: no path found" << std::endl;
            }
            else {
                std::string topic_new = "radio-arduino/RCMD/" + n.gatewayName + "/" + std::to_string(n.nodeAddress);

                publishMessage(topic_new, payload);
            }
        }
        catch (std::exception const& e) {
            std::cout << "DEBUG: malformed birth certificate" << std::endl;
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
