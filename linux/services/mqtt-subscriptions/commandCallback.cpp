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
    std::string topic = "raduino-router/RCMD";

    json command;
    command["command"] = "resendBirthCertificate";

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
    // TODO: keep timestamp of last poll. do not send a new command before 10 seconds have passed

    std::string topic = "raduino-router/RCMD/" + std::to_string(nodeAddress);

    json jsonCommandList;
    for (std::string command : commandList) {
        jsonCommandList.push_back(command);
    }

    uint64_t timestamp = (duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count());

    json command = { { "commandList", jsonCommandList }, { "nodeAddress", nodeAddress }, { "timestamp", timestamp } };

    publishMessage(topic, command.dump());
}

void CommandCallback::executeSubscriptionsForNode(uint32_t nodeAddress)
{
    // called after heartbeat (advertisement) or after a successful response
    std::vector<std::string> commandList;

    for (int j = 0; j < m_subscriptions.size(); j++) {
        if (m_subscriptions.at(j).nodeAddress == nodeAddress) {
            using namespace std::chrono;
            auto start = std::chrono::high_resolution_clock::now();
            uint64_t time_since_epoch_ms
                = std::chrono::duration_cast<std::chrono::milliseconds>(start.time_since_epoch()).count();

            int diff = time_since_epoch_ms - m_subscriptions.at(j).timestampLastValidResponse;

            if (((diff > m_subscriptions.at(j).intervalInSeconds * 1000) || diff < 0) && m_subscriptions.at(j).active) {
                commandList.push_back(m_subscriptions.at(j).commandName);
            }
        }
    }
    if (commandList.size() > 0) {
        pollNode(commandList, nodeAddress);
    }
}

void CommandCallback::updateJsonNodeInfoList()
{
    json jsonNodeInfoList;
    for (auto nodeInfo : m_nodeInfoList) {
        jsonNodeInfoList.push_back(nodeInfo.nodeAddress);
    }
    subscriptionStatus["nodes"] = jsonNodeInfoList;

    for (auto subscription : m_subscriptions) {
        subscriptionStatus["subscriptions"][std::to_string(subscription.nodeAddress)][subscription.commandName]
            = { { "intervalInSeconds", subscription.intervalInSeconds },
                { "active", subscription.active },
                { "consequtiveErrorResponses", subscription.consequtiveErrorResponses } };
    }
}

void CommandCallback::registerSubscription(std::string commandName, uint32_t interval, uint32_t nodeAddress)
{
    bool subscriptionExist = false;

    for (auto subscription : m_subscriptions) {
        if ((subscription.commandName == commandName) && (nodeAddress == subscription.nodeAddress)) {
            subscriptionExist = true;
        }
    }

    if (false == subscriptionExist) {
        m_subscriptions.push_back({ commandName, interval, nodeAddress });
    }
}

void CommandCallback::message_arrived(mqtt::const_message_ptr message)
{
    std::string topic_orig = message->get_topic();
    std::string payload = message->get_payload_str();

    if (topic_orig.starts_with("raduino-subscription/RCMD")) {
        try {
            auto jsonData = json::parse(payload);
            std::string command = jsonData["command"];

            if (command == "getSubscriptionStatus") {
                std::string topic = "raduino-subscription/status";
                updateJsonNodeInfoList();
                publishMessage(topic, subscriptionStatus.dump());
            }
            else if (command == "enableSubscription") {
                std::string commandName = jsonData["name"];
                uint32_t nodeAddress = jsonData["nodeAddress"].get<uint32_t>();

                for (int i = 0; i < m_subscriptions.size(); i++) {
                    if ((m_subscriptions.at(i).nodeAddress == nodeAddress)
                        && (commandName == m_subscriptions.at(i).commandName)) {
                        m_subscriptions.at(i).active = true;
                    }
                }
            }
            else if (command == "setInterval") {
                std::string commandName = jsonData["name"];
                uint32_t nodeAddress = jsonData["nodeAddress"].get<uint32_t>();
                uint32_t intervalInSeconds = jsonData["interval"];

                for (int i = 0; i < m_subscriptions.size(); i++) {
                    if ((m_subscriptions.at(i).nodeAddress == nodeAddress)
                        && (commandName == m_subscriptions.at(i).commandName)) {
                        m_subscriptions.at(i).intervalInSeconds = intervalInSeconds;
                    }
                }
            }
        }
        catch (std::exception const& e) {
            std::cout << "DEBUG: malformed command" << std::endl;
            std::cout << "     : " << payload << std::endl;
        }
    }

    auto tokens = splitString(topic_orig, "/");

    if (topic_orig.starts_with("raduino-router/DBIRTH/")) {
        try {
            auto jsonData = json::parse(payload);
            uint32_t nodeAddress = jsonData["nodeAddress"].get<uint32_t>();
            uint32_t gatewayAddress = jsonData["gateway"];

            bool nodeExists
                = std::find(m_radioNodeIdList.begin(), m_radioNodeIdList.end(), nodeAddress) != m_radioNodeIdList.end();

            if (!nodeExists) {
                std::cout << "Added raduino node: " << std::to_string(nodeAddress) << std::endl;
                m_radioNodeIdList.push_back(nodeAddress);
                nodeInfo_t nodeInfo;
                nodeInfo.nodeAddress = nodeAddress;
                m_nodeInfoList.push_back(nodeInfo);
                // add subscriptions
                if (nodeAddress == gatewayAddress) {
                    registerSubscription("get_device_name", 60 * 120, nodeAddress);
                    registerSubscription("get_statistics", 60 * 1, nodeAddress);
                    registerSubscription("get_version", 60 * 120, nodeAddress);
                    registerSubscription("get_attached_devices_csv_string", 60 * 120, nodeAddress);
                }
                else {
                    registerSubscription("vcc", 60 * 10, nodeAddress);
                    registerSubscription("gpio", 60 * 10, nodeAddress);
                    registerSubscription("get_device_name", 60 * 120, nodeAddress);
                    registerSubscription("get_statistics", 60 * 120, nodeAddress);
                    registerSubscription("get_version", 60 * 120, nodeAddress);
                    registerSubscription("get_attached_devices_csv_string", 60 * 120, nodeAddress);
                }
            }
        }
        catch (std::exception const& e) {
            std::cout << "DEBUG: malformed birth certificate" << std::endl;
            std::cout << "     : " << payload << std::endl;
        }

        updateJsonNodeInfoList();
    }

    if (topic_orig.starts_with("raduino-router/ADVERTISEMENT/")) {
        try {
            auto jsonData = json::parse(payload);
            std::string nodeAddressString = tokens.at(2);
            uint32_t nodeAddress = stoul(tokens.at(2));

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

    if (topic_orig.starts_with("raduino-router/STATE/")) {
        // TODO: state is not used, perhaps remove it?
        std::string nodeAddressString = tokens.at(3);
        uint32_t nodeAddress = stoul(tokens.at(3));
    }

    if (topic_orig.starts_with("raduino-router/DDATA/")) {
        try {
            auto jsonData = json::parse(payload);

            uint32_t nodeAddress = std::stoul(tokens.at(2));
            std::string responseCode = jsonData["responseCode"];
            std::string commandName = jsonData["name"];
            uint64_t timestamp = jsonData["timestamp"].get<uint64_t>();

            for (int i = 0; i < m_subscriptions.size(); i++) {
                if ((m_subscriptions.at(i).nodeAddress == nodeAddress)
                    && (commandName == m_subscriptions.at(i).commandName)) {
                    if (responseCode == "success") {
                        m_subscriptions.at(i).consequtiveErrorResponses = 0;
                    }
                    else {
                        m_subscriptions.at(i).consequtiveErrorResponses++;
                        if (m_subscriptions.at(i).consequtiveErrorResponses > 25) {
                            m_subscriptions.at(i).active = false;
                        }
                    }
                }

                if ((m_subscriptions.at(i).nodeAddress == nodeAddress) && (responseCode == "success")
                    && (commandName == m_subscriptions.at(i).commandName)

                ) {
                    if (commandName == "get_attached_devices_csv_string") {
                        auto jsonData = json::parse(payload);
                        std::string csvString = jsonData["payload"]["csvString"];
                        if (csvString.find("lsm303d") != std::string::npos) {
                            registerSubscription("get_lsm303d", 60 * 1, nodeAddress);
                        }
                        else if (csvString.find("quad") != std::string::npos) {
                            registerSubscription("quadrature_encoder", 60 * 1, nodeAddress);
                        }
                        else if (csvString.find("ina219") != std::string::npos) {
                            registerSubscription("ina219", 60 * 1, nodeAddress);
                        }
                    }

                    subscriptionStatus["response"][std::to_string(nodeAddress)][commandName] = jsonData;

                    m_subscriptions.at(i).lastValidResponse = payload;
                    m_subscriptions.at(i).timestampLastValidResponse = timestamp;

                    std::string republish_topic = "raduino-subscription/DDATA/"
                        + std::to_string(m_subscriptions.at(i).nodeAddress) + "/" + commandName;

                    publishMessage(republish_topic, payload);
                }
            }

            if (responseCode == "success") {
                executeSubscriptionsForNode(nodeAddress);
            }
        }
        catch (std::exception const& e) {
            std::cout << "DEBUG: malformed DDATA. " << e.what() << std::endl;
        }
    }
}

