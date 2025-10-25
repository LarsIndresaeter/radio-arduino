#pragma once

#include "mqtt-common.hpp"
#include "mqtt/async_client.h"
#include <chrono>
#include <cmath>
#include <monitor.hpp>
#include <nlohmann/json.hpp>
#include <numeric>
#include <vector>

constexpr float EVENT_THRESHOLD = 0.01; // 1% change before sending new message
constexpr uint EVENT_TIMEOUT = 10 * 60; // seconds before sending new message

int timeMs();
float calculateStddev(std::vector<float> data);
void readCurrentAndVoltage(monitor& mon, mqtt::async_client& mqtt_client, int samples);
