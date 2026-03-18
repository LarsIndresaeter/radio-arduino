#include "commandCallback.hpp"
#include "include/commandCallback.hpp"
#include <ratio>
#include <thread>

using namespace std::chrono_literals;

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
    std::string message = "{\"command\":\"resendBirthCertificate\"}";

    publishMessage(topic, message);
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

std::vector<uint32_t> CommandCallback::getRadioNodeIdList() { return (m_radioNodeIdList); }

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

void CommandCallback::pollNode(std::string commandName, uint32_t nodeAddress)
{
    // std::cout << "DEBUG: pollNode(" << commandName << ", " << std::to_string(nodeAddress) << ")" << std::endl;

    std::string topic = "radio-arduino/RCMD/proxy/" + std::to_string(nodeAddress);
    std::string message = "{\"name\": \"" + commandName + "\", \"nodeAddress\": " + std::to_string(nodeAddress)
        + ", \"expirationTime\": 0}";

    publishMessage(topic, message);
}

void CommandCallback::executeSubscriptions()
{
    std::vector<uint32_t> radioNodes = getRadioNodeIdList();

    // std::cout << "DEBUG: executeSubscriptions" << std::endl;
    for (int i = 0; i < m_nodeTwins.size(); i++) {
        // std::cout << "DEBUG: execute subscriptions for node " << std::to_string(nodeTwin.nodeAddress) << std::endl;

        for (auto subscription : m_subscriptions) {
            int loopCounter = 0;
            while (false == m_nodeTwins.at(i).readyForCommand) {
                if (loopCounter > 1000) {
                    break;
                }
                loopCounter++;
                std::this_thread::sleep_for(10ms);
                // std::cout << "DEBUG: wait loop" << std::endl;
            }

            if (subscription.nodeAddress == m_nodeTwins.at(i).nodeAddress) {
                if (m_nodeTwins.at(i).readyForCommand) {
                    using namespace std::chrono;
                    auto start = std::chrono::high_resolution_clock::now();
                    uint64_t time_since_epoch_ms
                        = std::chrono::duration_cast<std::chrono::milliseconds>(start.time_since_epoch()).count();

                    int diff = time_since_epoch_ms - subscription.timestampLastValidResponse;
                    // std::cout << "diff: " << std::to_string(diff) << std::endl;

                    if ((diff > subscription.intervalInSeconds * 1000) || diff < 0) {
                        pollNode(subscription.commandName, subscription.nodeAddress);
                        m_nodeTwins.at(i).readyForCommand = false;
                        std::this_thread::sleep_for(10000ms); // TODO: do we need this?
                    }
                }
                else {
                    std::cout << "WARNING: node '" << std::to_string(subscription.nodeAddress)
                              << "' not ready for command" << std::endl;
                }
            }
        }
        if (false == m_nodeTwins.at(i).readyForCommand) {
            m_nodeTwins.at(i).readyForCommand = true;
            std::this_thread::sleep_for(1000ms);
        }
    }

    std::this_thread::sleep_for(10000ms);
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
            int healthIndicator = jsonData["healthIndicator"].get<int>();
            uint64_t lastAdvertisement = jsonData["lastAdvertisement"].get<uint64_t>();

            bool nodeExists
                = std::find(m_radioNodeIdList.begin(), m_radioNodeIdList.end(), nodeAddress) != m_radioNodeIdList.end();

            if (!nodeExists) {
                std::cout << "Added radio node: " << std::to_string(nodeAddress) << std::endl;
                m_radioNodeIdList.push_back(nodeAddress);
                nodeTwin_t nodeTwin;
                nodeTwin.nodeAddress = nodeAddress;
                nodeTwin.readyForCommand = true;
                m_nodeTwins.push_back(nodeTwin);
                // add subscriptions
                if (topic_orig.starts_with(
                        "radio-arduino/DBIRTH/" + std::to_string(gatewayAddress) + "/"
                        + std::to_string(gatewayAddress))) {
                    // std::cout << "DEBUG: add subscription for gatway" << std::endl;

                    m_subscriptions.push_back({ "get_device_name", 24 * 60 * 60, nodeAddress });
                    //m_subscriptions.push_back({ "get_attached_devices_csv_string", 24 * 60 * 60, nodeAddress });
                    m_subscriptions.push_back({ "get_statistics", 60 * 120, nodeAddress });
                    m_subscriptions.push_back({ "get_version", 24 * 60 * 60, nodeAddress });
                }
                else {
                    // std::cout << "DEBUG: add subscription for node" << std::endl;

                    m_subscriptions.push_back({ "vcc", 60 * 10, nodeAddress });
                    m_subscriptions.push_back({ "gpio", 60 * 60, nodeAddress });
                    m_subscriptions.push_back({ "get_active_time_counter", 60, nodeAddress });
                    m_subscriptions.push_back({ "get_device_name", 24 * 60 * 60, nodeAddress });
                    m_subscriptions.push_back({ "get_statistics", 60 * 120, nodeAddress });
                    m_subscriptions.push_back({ "get_attached_devices_csv_string", 24 * 60 * 60, nodeAddress });
                    // m_subscriptions.push_back({ "get_version", 24*60*60, nodeAddress });
                }
            }
        }
        catch (std::exception const& e) {
            std::cout << "DEBUG: malformed birth certificate" << std::endl;
            std::cout << "     : " << payload << std::endl;
        }
    }

    if (topic_orig.starts_with("radio-arduino/STATE/")) {
        auto tokens = splitString(topic_orig, "/");
        std::string nodeAddressString = tokens.at(3);

        for (int i = 0; i < m_nodeTwins.size(); i++) {
            if ((std::to_string(m_subscriptions.at(i).nodeAddress) == nodeAddressString)) {
                if (payload.empty()) {
                    m_nodeTwins.at(i).readyForCommand = true;
                    // std::cout << "DEBUG: readyForCommand(" << nodeAddressString << ")=true" << std::endl;
                }
                else {
                    m_nodeTwins.at(i).readyForCommand = false;
                    // std::cout << "DEBUG: readyForCommand(" << nodeAddressString << ")=false" << std::endl;
                }
            }
        }
    }

    if (topic_orig.starts_with("radio-arduino/DDATA/")) {
        try {
            auto tokens = splitString(topic_orig, "/");
            if(tokens.size() < 5)
            {
                std::cout << "DEBUG: ABORT! parsing" << std::endl;
            }
            uint32_t nodeAddress = std::stoul(tokens.at(3));
            std::string command_name = tokens.at(4);

            if (command_name == "get_attached_devices_csv_string") {
                auto jsonData = json::parse(payload);
                std::string csvString = jsonData["payload"]["csvString"];
                if (csvString.find("lsm303d") != std::string::npos) {
                    m_subscriptions.push_back({ "get_lsm303d", 60, nodeAddress });
                }
                else if (csvString.find("quad") != std::string::npos) {
                    m_subscriptions.push_back({ "quadrature_encoder", 60, nodeAddress });
                }
                else if (csvString.find("ina219") != std::string::npos) {
                    m_subscriptions.push_back({ "ina219", 60, nodeAddress });
                }
            }

            // seher
        }
        catch (std::exception const& e) {
            std::cout << "DEBUG: malformed DDATA" << std::endl;
        }

        try {
            auto jsonData = json::parse(payload);

            auto tokens = splitString(topic_orig, "/");
            std::string nodeAddressString = tokens.at(3);
            // std::cout << "DEBUG: nodeAddressString=" << nodeAddressString << std::endl;

            std::string responseCode = jsonData["responseCode"];
            std::string commandName = jsonData["name"];
            uint64_t timestamp = jsonData["timestamp"].get<uint64_t>();

            for (int i = 0; i < m_subscriptions.size(); i++) {
                if ((std::to_string(m_subscriptions.at(i).nodeAddress) == nodeAddressString)
                    && (responseCode == "success") && (commandName == m_subscriptions.at(i).commandName)

                ) {
                    m_subscriptions.at(i).lastValidResponse = payload;
                    m_subscriptions.at(i).timestampLastValidResponse = timestamp;
                    // std::cout << "DEBUG: -----> saveResponse(" << commandName << ", " << nodeAddressString << ")"
                    //<< std::endl;
                }
            }

            for (int i = 0; i < m_nodeTwins.size(); i++) {
                if (nodeAddressString == std::to_string(m_nodeTwins.at(i).nodeAddress)) {
                    // we got respnse so node is ready for new command
                    m_nodeTwins.at(i).readyForCommand = true;
                }
            }
        }
        catch (std::exception const& e) {
            std::cout << "DEBUG: malformed DDATA" << std::endl;
        }
    }
}

