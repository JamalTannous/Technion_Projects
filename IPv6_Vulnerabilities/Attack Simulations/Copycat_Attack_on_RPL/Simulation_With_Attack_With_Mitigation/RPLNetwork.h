/*
 * RPLNetwork.h
 *
 *  Created on: Sep 4, 2022
 *      Author: jamal
 */

#ifndef RPLNETWORK_H_
#define RPLNETWORK_H_
#include "RPLNode.h"
#include "HackerNode.h"
#include <vector>
#include <algorithm>
#include <string>
using namespace std;
class RPL_Network {
public:
	RPL_Network(int parentIndex=0);
	virtual ~RPL_Network();
	void moveTimeStamp();
	bool determineSimulationFinished();
	void generateTCPMessageSequence();
	void generateDIOMessageSequence();
	void printStatistics();
	void analyzeAndCheckForHacker();
	void addHackerNode(RPLNode* parent);

public:
	RPLNode* root;
	vector <RPLNode*> nodes;
	vector <RPLMessage*> messagesInTransmission;
	int messagesIDPool;
	int tcpGenerateCounter;
	int dioGenerateCounter;
	RPLNode* strikeOneNode;
	RPLNode * strikeTwoNode;
	RPLNode * detectedHacker;
	bool hackerDetected;
};

#endif /* RPLNETWORK_H_ */
