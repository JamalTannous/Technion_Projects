/*
 * RPLMessage.cpp
 *
 *  Created on: Sep 4, 2022
 *      Author: jamal
 */

#include "RPLMessage.h"
int RPLMessage::currentSimulationTime = 0;
RPLMessage::RPLMessage(){
	messageType = DIO;
	messageCreationTime = 0;
	timeInAir = TIME_BETWEEN_HOPS;
	next = nullptr;
	previous = nullptr;
}

RPLMessage::RPLMessage(int messageID,MessageType type, string sourceIP, string destinationIP, string sourceMAC, string destinationMAC) {
		messageType = type;
		this -> sourceIP = sourceIP;
		this -> sourceMAC = sourceMAC;
		this -> destinationIP = destinationIP;
		this -> destinationMAC = destinationMAC;
		messageCreationTime = currentSimulationTime;
		previous = nullptr;
		next = nullptr;
		this ->messageID = messageID;
}

RPLMessage::RPLMessage(RPLMessage* message){
		messageType = message->messageType;
		this -> sourceIP = message->sourceIP;
		this -> sourceMAC = message->sourceMAC;
		this -> destinationIP = message->destinationIP;
		this -> destinationMAC = message->destinationMAC;
		messageCreationTime = currentSimulationTime;
		previous = nullptr;
		next = nullptr;
		messageID = message->messageID;
}

void RPLMessage::setDestinationMAC(string destinationMAC){
	this -> destinationMAC = destinationMAC;
}

void RPLMessage::setSourceMAC(string sourceMAC){
	this -> sourceMAC  = sourceMAC;
}

string RPLMessage::getSourceIP(){
	return sourceIP;
}

string RPLMessage::getDestinationIP(){
	return destinationIP;
}

RPLMessage::~RPLMessage() {
	// TODO Auto-generated destructor stub
}

