#include <iostream>
#include <string>
#include <cstring>
#include <cstdio>

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "mqtt.h"

using namespace std;

#define CLIENT_ID "mosquitto"
#define BROKER_ADDRESS "localhost"
#define MQTT_PORT 1883
#define MQTT_TOPIC "report"

int main() {

	class mqtt_client *iot_client;
	//csatlakozashoz az adatok
	char client_id[] = CLIENT_ID;
	int sadf = 13245;
	int * clint_id = &sadf;
	char host[] = BROKER_ADDRESS;
	int port = MQTT_PORT;
	string topicString = MQTT_TOPIC;

	string f;

	mosqpp::lib_init();
	
	iot_client = new mqtt_client(client_id, host, port);

	cout << "clientid: " << client_id << endl;
	cout << "host: " << host << endl << "port: " << port << endl;

	cout << "Subsribing to " << MQTT_TOPIC << ": ";
	if(iot_client->subscribe(clint_id, MQTT_TOPIC, 2) == 0) cout << "OK" << endl;

		while (iot_client->O)
		{
			if (!iot_client->O) {
				iot_client->disconnect();
			}
			iot_client->loop_misc();
			iot_client->loop_read();
		};
	
	mosqpp::lib_cleanup();

	return 0;
}