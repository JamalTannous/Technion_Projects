/*
 * RPLMessage.h
 *
 *  Created on: Sep 4, 2022
 *      Author: jamal
 */

#ifndef RPLMESSAGE_H_
#define RPLMESSAGE_H_
#include <string>
using namespace std;
#define TIME_BETWEEN_HOPS 2
enum MessageType{
	DIO,
	TCP,
	TCP_ACK,
	UDP
};

class RPLMessage {
public:
	RPLMessage();
	RPLMessage(int messageID,MessageType type, string sourceIP, string destinationIP, string sourceMAC = "", string destinationMAC ="");
	RPLMessage(RPLMessage* message);
	virtual ~RPLMessage();
	RPLMessage * next;
	RPLMessage * previous;
	void decrementMessageTime();
	void setDestinationMAC(string destinationMAC);
	void setSourceMAC (string sourceMAC);
	string getSourceIP();
	string getDestinationIP();
	bool determineTimeout();

public:
	int messageCreationTime;
	int messageSendTime;
	int messageResolveTime;
	int timeInAir;
	MessageType messageType;
	string sourceIP;
	string destinationIP;
	string sourceMAC;
	string destinationMAC;
	int messageID;
	static int currentSimulationTime;
};

#endif /* RPLMESSAGE_H_ */
