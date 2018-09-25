#ifndef SIMPLECLIENT_MQTT_H
#define SIMPLECLIENT_MQTT_H

#include <iostream>
#include <mosquittopp.h>
#include <mosquitto.h>
#include <cstring>
#include <cstdio>
#include <string>

#define MAX_PAYLOAD 50
#define DEFAULT_KEEP_ALIVE 60

#define CLIENT_ID "mosquitto"
#define BROKER_ADDRESS "localhost"
#define MQTT_PORT 1883
#define MQTT_TOPIC "report"

class mqtt_client : public mosqpp::mosquittopp
{
public:

	bool O = true;
	bool Connected = false;
	int j = 12345;
	int * i = &j;
	mqtt_client(const char *id, const char *host, int port);
	~mqtt_client();

	//int send(char* message, messCont&);
	int send(const mosquitto_message * msg, std::string newPl);
	void reconnect();

	void on_connect(int rc);
	void on_message(const struct mosquitto_message *message);
	void on_subscribe(int mid, int qos_count, const int *granted_qos);
	void on_disconnect(int rc);

	int szam(std::string);
	int cmd(std::string);

	int getX();
	int getY();
	int getZ();
	void setX(int);
	void setY(int);
	void setZ(int);


private:
	int x, y, z;
};

#endif //SIMPLECLIENT_MQTT_H
