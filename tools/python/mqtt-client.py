import sys

import paho.mqtt.client as mqttClient
import json

print("raduino mqtt python client")

client = mqttClient.Client(client_id="")

if client.connect("localhost", 1883, 60) != 0:
    print("Couldn't connect to the mqtt broker")
    sys.exit(1)

def sendCommand(client, gatewayName, nodeAddress, command):
    data = {
            "command": "passThrough",
            "name": command,
            "device": "node",
            "gatewayAddress": 1234,
            "nodeAddress": nodeAddress,
            "expirationTime": 1234,
    }

    cmd = json.dumps(data)
    client.publish("radio-arduino/RCMD/" + gatewayName + "/" + str(nodeAddress), cmd, 1)

def sendPublishBirth(client):
    client.publish("radio-arduino/RCMD", "{\"command\":\"resendBirthCertificate\"}", 1)

def controlLoop(client):
    # todo: watch for new nodes discovered
    pass

# if (param_command != ""):
if (len(sys.argv) > 3):
    param_gw = sys.argv[1]
    param_node = int(sys.argv[2])
    param_command = sys.argv[3]
    sendCommand(client, param_gw, param_node, param_command)
else:
    sendPublishBirth(client)
    # sendCommand(client, "gw4", 1765313195, "vcc")

client.disconnect()
