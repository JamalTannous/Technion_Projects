/*
 * RPLNode.h
 *
 *  Created on: Sep 4, 2022
 *      Author: jamal
 */

#ifndef RPLNODE_H_
#define RPLNODE_H_
#include <vector>
#include "Queue.h"
using namespace std;

class RPLNode {
public:
	RPLNode(RPLNode* parent, string nodeIP, string nodeMAC, bool isRoot);
	virtual ~RPLNode();
	int getRank();
	void calculateRank();
	void setParent (RPLNode* parent);
	string getIP();
	void addChild(RPLNode* node);

	/// Return index of node in children where messageDestinationIP exists in its subnet or -1
	int determineIPExistsInSubnet(string messageDestinationIP);
	void handleMessage(RPLMessage* dioMessage);
	virtual void sendDIOMessage(RPLMessage* dioMessage);
	void sendTCPMessage(RPLMessage* tcpMessage);
	void sendTCP_ACKMessage(RPLMessage* tcpAckMessage);
	void receiveMessage(RPLMessage* message);
	void resolveMessage(int messageID);
	void addToMessagesOnTheWay (RPLMessage* message, bool toParent = false);
	void moveFromTransmissionQueueToWaitQueue(RPLMessage * message);
	void moveFromTimeoutQueueToWaitQueue(RPLMessage* message);
	void determineIfMoveFromTransmissionQueueToWaitQueue();
	void moveTimeStamp();
	bool determineIfNothingToDo();
	void nodeDebugMessage(string message);
	void determineTimeout();
	void calculateStatistics();
	void printStatistics(string toPrint);
	void printNumberOfMessagesSent();

public:
	RPLNode* parent;
	string nodeIP;
	string nodeMAC;
	int rank;
	bool isRoot;
	static int weight;
	int messagesSent;
	Queue messageQueue;// messages in queue waiting to be sent
	Queue timeoutQueue; // tcp messages that have been sent and awaiting ack
	Queue resolvedMessagesQueue; // tcp messages that received an ack
	Queue messagesOnTheWay; // messages that are on their way to current node
	vector <RPLNode*> children;
	bool isHackerNode;
};

#endif /* RPLNODE_H_ */
