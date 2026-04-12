#pragma once

#include "mqtt/async_client.h"
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

using nlohmann::json;

typedef struct nodeInfo {
    uint32_t nodeAddress;
    //bool readyForCommand;
} nodeInfo_t;

typedef struct subscription {
    subscription(std::string cn, uint32_t iis, uint32_t na)
        : commandName(cn)
        , intervalInSeconds(iis)
        , nodeAddress(na)
        , lastValidResponse("")
    {
        timestampLastValidResponse=0;
        consequtiveErrorResponses=0;
        active=true;
    }

    std::string commandName;
    uint32_t intervalInSeconds;
    uint32_t nodeAddress;
    std::string lastValidResponse;
    uint64_t timestampLastValidResponse;
    uint32_t consequtiveErrorResponses;
    bool active;
} subscription_t;

class CommandCallback : public virtual mqtt::callback {
public:
    CommandCallback(mqtt::async_client& mqtt_client);
    void connection_lost(const std::string& cause) override;
    void message_arrived(mqtt::const_message_ptr message) override;
    void delivery_complete(mqtt::delivery_token_ptr token) override;

    void resendBirthCertificate();
    void executeSubscriptionsForNode(uint32_t nodeAddress);

private:
    std::vector<std::string> splitString(std::string s, const std::string& delimiter);
    void pollNode(std::vector<std::string> commandList, uint32_t nodeAddress);
    void publishMessage(std::string topic, std::string message);
    json subscriptionStatus;
    void registerSubscription(std::string commandName, uint32_t interval, uint32_t nodeAddress);
    void updateJsonNodeInfoList();

    mqtt::async_client& m_mqttClient;
    std::vector<uint32_t> m_radioNodeIdList;
    std::vector<nodeInfo_t> m_nodeInfoList;
    std::vector<subscription_t> m_subscriptions;
};
