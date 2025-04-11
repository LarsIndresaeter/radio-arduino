#pragma once

#include <string>
#include <chrono>
#include <nlohmann/json.hpp>
#include "mqtt/async_client.h"
#include <monitor.hpp>

uint64_t secondsSinceEpoch();
std::string createMqttTopic(std::string type, std::string eon, std::string device);
std::string getDateTimeString();
std::string getMasterNameAndPublishBirth(monitor& mon, mqtt::async_client& mqtt_client);

void publishNbirth(mqtt::async_client& mqtt_client, std::string nodeName);
void publishNdeath(mqtt::async_client& mqtt_client, std::string slaveName);

void publishMonitorProtocolStatistics(monitor& mon, mqtt::async_client& mqtt_client, std::string& masterName);
