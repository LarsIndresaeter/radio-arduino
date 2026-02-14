#include "mqtt/async_client.h"
#include <cmath>
#include <commandCallback.hpp>
#include <getopt.h>
#include <string>
#include <thread>

using namespace std::chrono_literals;

void readMultipleRadioNodes(mqtt::async_client& mqtt_client)
{
    const int QOS = 0;

    CommandCallback commandCallback(mqtt_client);

    mqtt_client.set_callback(commandCallback);
    mqtt_client.subscribe("radio-arduino/RCMD/proxy/#", QOS)->wait();
    mqtt_client.subscribe("radio-arduino/DBIRTH/#", QOS)->wait();

    while (true) {
        // loop forever
    }
}

void print_usage()
{
    std::cout << "raduino-mqtt-proxy" << std::endl;
    std::cout << "                 -h : p rint this text" << std::endl;
}

void parseOpt(int argc, char* argv[])
{
    const std::string DFLT_ADDRESS { "tcp://localhost:1883" };
    const auto PERIOD = std::chrono::seconds(5);
    const int MAX_BUFFERED_MSGS = 120;

    std::string address = DFLT_ADDRESS;
    mqtt::async_client mqtt_client(address, "", MAX_BUFFERED_MSGS, 0);

    mqtt::connect_options connOpts;
    connOpts.set_keep_alive_interval(MAX_BUFFERED_MSGS * PERIOD);
    connOpts.set_clean_session(true);
    connOpts.set_automatic_reconnect(true);

    mqtt_client.start_consuming();

    mqtt_client.connect(connOpts)->wait();

    char option = 0;

    while ((option = getopt(argc, argv, "K:h")) != -1) {
        switch (option) {
        case 'h':
            print_usage();
            break;
        }
    }

    readMultipleRadioNodes(mqtt_client);

    mqtt_client.disconnect()->wait();

    exit(0);
}

int main(int argc, char* argv[])
{
    parseOpt(argc, argv);

    return 0;
}
