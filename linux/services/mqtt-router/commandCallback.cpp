#include "commandCallback.hpp"
#include "include/commandCallback.hpp"

using nlohmann::json;

CommandCallback::CommandCallback(

    mqtt::async_client& mqtt_client)
    : m_mqttClient(mqtt_client)
{
    resendBirthCertificate();
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

    if (topic_orig.starts_with("raduino-adapter/DBIRTH/") || topic_orig.starts_with("raduino-adapter/DDATA/")
        || topic_orig.starts_with("raduino-adapter/ADVERTISEMENT/")) {
        // resend response from adapter
        try {
            auto tokens = splitString(topic_orig, "/");
            std::string messageType = tokens.at(1);
            std::string nodeAddressString = tokens.at(3);
            std::string topic_new = "raduino-router/" + messageType + "/" + nodeAddressString;

            publishMessage(topic_new, payload);
        }
        catch (std::exception const& e) {
            std::cout << "ERROR: malformed RCMD" << std::endl;
        }
    }

    if (topic_orig.starts_with("raduino-adapter/DBIRTH/")) {
        try {
            auto jsonData = json::parse(payload);
            uint32_t gatewayAddress = jsonData["gateway"];
            int healthIndicator = jsonData["healthIndicator"].get<int>();
            uint64_t lastAdvertisement = jsonData["lastAdvertisement"];
            uint32_t nodeAddress = jsonData["nodeAddress"];
            updatePath(gatewayAddress, nodeAddress, lastAdvertisement, healthIndicator);
        }
        catch (std::exception const& e) {
            std::cout << "ERROR: malformed birth certificate" << std::endl;
        }
    }

    if (topic_orig == "raduino-router/RCMD") {
        // command to the router
        try {
            auto jsonData = json::parse(payload);
            // std::string command = jsonData["command"];
            // seher

            // if (command == "resendBirthCertificate") {
            resendBirthCertificate();

            //}
        }
        catch (std::exception const& e) {
            std::cout << "ERROR: malformed RCMD: " << payload << std::endl;
        }
    }

    if (topic_orig.starts_with("raduino-router/RCMD/")) {
        // command to a specific node
        try {
            auto jsonData = json::parse(payload);
            uint32_t nodeAddress = jsonData["nodeAddress"];

            nodepath_t n = getNodePath(nodeAddress);
            if (n.nodeAddress == 0) {
                std::cout << "DEBUG: no path found for " << nodeAddress << std::endl;
                resendBirthCertificate();
            }
            else {
                std::string topic_new
                    = "raduino-adapter/RCMD/" + std::to_string(n.gatewayAddress) + "/" + std::to_string(n.nodeAddress);

                publishMessage(topic_new, payload);
            }
        }
        catch (std::exception const& e) {
            std::cout << "ERROR: malformed RCMD" << std::endl;
        }
    }
}

void CommandCallback::resendBirthCertificate()
{
    std::string topic = "raduino-adapter/RCMD";
    json command;
    command["command"] = "resendBirthCertificate";

    publishMessage(topic, command.dump());
}

void CommandCallback::connection_lost(const std::string& cause)
{
    std::cout << "Connection lost: " << cause << std::endl;
}

void CommandCallback::delivery_complete(mqtt::delivery_token_ptr token)
{
    std::cout << "Message delivered" << std::endl;
}
