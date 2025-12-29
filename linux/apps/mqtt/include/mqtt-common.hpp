#pragma once

#include "mqtt/async_client.h"
#include <chrono>
#include <desiredState.hpp>
#include <monitor.hpp>
#include <nlohmann/json.hpp>
#include <string>

uint64_t milliSecondsSinceEpoch();
uint64_t secondsSinceEpoch();
std::string createMqttTopic(std::string type, std::string eon, std::string device);
std::string getDateTimeString();
std::string getGatewayNameAndPublishBirth(monitor& mon, mqtt::async_client& mqtt_client);

void publishNbirth(mqtt::async_client& mqtt_client, std::string deviceName);

void publishMonitorProtocolStatistics(monitor& mon, mqtt::async_client& mqtt_client, std::string& gatewayName);
void publishGatewayInfo(mqtt::async_client& mqtt_client);

