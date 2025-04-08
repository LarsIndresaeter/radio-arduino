#pragma once

#include <string>
#include <chrono>
#include <nlohmann/json.hpp>
#include "mqtt/async_client.h"
#include "desiredStateConfiguration.hpp"
#include <monitor.hpp>

uint64_t secondsSinceEpoch();
std::string createMqttTopic(std::string type, std::string eon, std::string device);
std::string getDateTimeString();
std::string getMasterNameAndPublishBirth(monitor& mon, mqtt::async_client& mqtt_client);
std::string getSlaveNameAndPublishBirth(monitor& mon, mqtt::async_client& mqtt_client);

void publishMonitorProtocolStatistics(monitor& mon, mqtt::async_client& mqtt_client, std::string& masterName);
void publishDesiredStatePollInterval(mqtt::async_client& mqtt_client, std::shared_ptr<DesiredStateConfiguration> ds);
void updateDisplayText(monitor& mon, mqtt::async_client& mqtt_client, std::shared_ptr<DesiredStateConfiguration> ds);
void readVccAndPublish(monitor& mon, mqtt::async_client& mqtt_client, std::string slaveName);
