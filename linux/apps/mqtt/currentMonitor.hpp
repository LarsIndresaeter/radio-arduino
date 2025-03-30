#pragma once

#include <chrono>
#include <numeric>
#include <cmath>
#include <vector>
#include <monitor.hpp>
#include <nlohmann/json.hpp>
#include "mqtt/async_client.h"
#include "mqtt_common.hpp"

constexpr float EVENT_THRESHOLD = 0.01; // 1% change before sending new message
constexpr uint EVENT_TIMEOUT = 10 * 60; // seconds before sending new message

int timeMs();
float calculateStddev(std::vector<float> data);
void readCurrentAndVoltage(monitor& mon, mqtt::async_client& mqtt_client, int samples);
