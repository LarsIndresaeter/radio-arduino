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
    int bestHealthIndicator = -10000;

    for (int i = 0; i < m_nodePath.size(); i++) {
        if (m_nodePath.at(i).nodeAddress == nodeAddress) {
            if (m_nodePath.at(i).healthIndicator >= bestHealthIndicator) {
                bestHealthIndicator = m_nodePath.at(i).healthIndicator;
                nodePath.gatewayAddress = m_nodePath.at(i).gatewayAddress;
                nodePath.nodeAddress = m_nodePath.at(i).nodeAddress;
                nodePath.lastAdvertisement = m_nodePath.at(i).lastAdvertisement;
                nodePath.healthIndicator = m_nodePath.at(i).healthIndicator;
            }
        }
    }

    return (nodePath);
}

void CommandCallback::updatePath(
    uint32_t gatewayAddress, uint32_t nodeAddress, uint64_t lastAdvertisement, int healthIndicator)
{
    bool nodePathFound = false;
    for (int i = 0; i < m_nodePath.size(); i++) {
        if ((m_nodePath.at(i).nodeAddress == nodeAddress) && (m_nodePath.at(i).gatewayAddress == gatewayAddress)) {
            nodePathFound = true;
            m_nodePath.at(i).lastAdvertisement = lastAdvertisement;
            m_nodePath.at(i).healthIndicator = healthIndicator;
        }
    }

    if (!nodePathFound) {
        nodepath_t n;
        n.gatewayAddress = gatewayAddress;
        n.nodeAddress = nodeAddress;
        n.lastAdvertisement = lastAdvertisement;
        n.healthIndicator = healthIndicator;
        m_nodePath.push_back(n);
    }
}

void CommandCallback::message_arrived(mqtt::const_message_ptr message)
{
    std::string topic_orig = message->get_topic();
    std::string payload = message->get_payload_str();

    bool resentBirthCertificates = false;

    if (topic_orig.starts_with("radio-arduino/DBIRTH/")) {
        try {
            auto jsonData = json::parse(payload);
            uint32_t gatewayAddress = jsonData["gateway"];
            int healthIndicator = jsonData["healthIndicator"].get<int>();
            uint64_t lastAdvertisement = jsonData["lastAdvertisement"];
            uint32_t nodeAddress = jsonData["nodeAddress"];
            updatePath(gatewayAddress, nodeAddress, lastAdvertisement, healthIndicator);
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
                std::cout << "DEBUG: no path found for " << nodeAddress << std::endl;
                resentBirthCertificates = true;
            }
            else {
                std::string topic_new = "radio-arduino/RCMD/" + std::to_string(n.gatewayAddress) + "/" + std::to_string(n.nodeAddress);

                publishMessage(topic_new, payload);
            }
        }
        catch (std::exception const& e) {
            std::cout << "DEBUG: malformed RCMD" << std::endl;
        }
    }

    if (resentBirthCertificates) {
        std::string topic = "radio-arduino/RCMD";
        json command = {"command", "resendBirthCertificate"};

        publishMessage(topic, command.dump());
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
