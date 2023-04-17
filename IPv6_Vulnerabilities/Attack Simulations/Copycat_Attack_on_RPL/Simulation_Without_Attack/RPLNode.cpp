/*
 * RPLNode.cpp
 *
 *  Created on: Sep 4, 2022
 *      Author: jamal
 */

#include "RPLNode.h"
#include <iostream>
int RPLNode::weight = 2;
#define IP_MATCH 500
#define MESSAGES_HANDLED_PER_TIMESTAMP 1
#define DEBUG 0
#define TCP_MESSAGE_TIMEOUT 600000
RPLNode::RPLNode(RPLNode* parent, string nodeIP, string nodeMAC, bool isRoot) {
	// TODO Auto-generated constructor stub
	this ->parent = parent;
	this -> nodeIP = nodeIP;
	this -> nodeMAC = nodeMAC;
	this -> isRoot = isRoot;
	rank = 0;
	isHackerNode = false;
	messagesSent = 0;

}

void RPLNode::calculateRank(){
	if (parent == nullptr or isRoot){
		rank = 0;
	}
	rank = 1 + weight*parent->getRank();
}

int RPLNode::getRank(){
	return rank;
}


string RPLNode::getIP(){
	return nodeIP;
}

RPLNode::~RPLNode() {
	// TODO Auto-generated destructor stub
}

void RPLNode::addChild(RPLNode * node){
	children.push_back(node);
}

int RPLNode::determineIPExistsInSubnet(string messageDestinationIP){
	if (messageDestinationIP == nodeIP){
		return IP_MATCH;
	}
	if (children.size() == 0){
		return -1;
	}
	for (int i = 0; i<children.size();i++){
		if (children[i]->determineIPExistsInSubnet(messageDestinationIP) > -1){
			return i;
		}
	}
	return -1;
}

void RPLNode::sendDIOMessage(RPLMessage * dioMessage){
    if (parent!=nullptr){
	if (dioMessage -> sourceMAC != parent-> nodeMAC){
		// MessageType type, string sourceIP, string destinationIP, string sourceMAC = "", string destinationMAC =""
		RPLMessage* messageToSend = new RPLMessage (dioMessage->messageID,dioMessage->messageType, dioMessage->sourceIP, dioMessage->destinationIP, nodeMAC, dioMessage->destinationMAC);
		//parent->receiveMessage(messageToSend);
		messageToSend->timeInAir = TIME_BETWEEN_HOPS;
		parent -> addToMessagesOnTheWay(messageToSend, true);
		messagesSent += 1;
	}
    }
	for (int i = 0; i < children.size(); i++){
		if (dioMessage -> sourceMAC != children[i]-> nodeMAC){
			RPLMessage* messageToSend = new RPLMessage (dioMessage->messageID,dioMessage->messageType, dioMessage->sourceIP, dioMessage->destinationIP, nodeMAC, dioMessage->destinationMAC);
			//children[i]->receiveMessage(messageToSend);
			messageToSend->timeInAir = TIME_BETWEEN_HOPS;
			children[i]->addToMessagesOnTheWay(messageToSend);
            messagesSent += 1;
		}
	}
	//delete [] (dioMessage);
}

void RPLNode::sendTCPMessage(RPLMessage* tcpMessage){

	if (tcpMessage->destinationIP == nodeIP){
	    if (DEBUG){
	        string toPrint= "received tcp message sending ack" ;
	        nodeDebugMessage(toPrint);
	    }
		/// MessageType type, string sourceIP, string destinationIP, string sourceMAC = "", string destinationMAC ="");
		RPLMessage* tcpAckMessage = new RPLMessage(tcpMessage->messageID,MessageType::TCP_ACK, nodeIP, tcpMessage->sourceIP, nodeMAC, tcpMessage->sourceMAC);
		//delete [] (tcpMessage);
		messageQueue.pushback(tcpAckMessage);
		return;
	}

	int subnetChildIndex = determineIPExistsInSubnet(tcpMessage->destinationIP);
	if (subnetChildIndex != -1){
		RPLMessage* tcpMessageToAdd = new RPLMessage(tcpMessage);
		tcpMessageToAdd -> sourceMAC = this -> nodeMAC;
		tcpMessageToAdd -> destinationMAC =  children[subnetChildIndex]->nodeMAC;
		//children[subnetChildIndex]->receiveMessage(tcpMessageToAdd);
		tcpMessageToAdd->timeInAir = TIME_BETWEEN_HOPS;
		tcpMessageToAdd -> messageSendTime = RPLMessage::currentSimulationTime;
		children[subnetChildIndex]->addToMessagesOnTheWay(tcpMessageToAdd);
        messagesSent += 1;
	}
	else {
		RPLMessage* tcpMessageToAdd = new RPLMessage(tcpMessage);
		tcpMessageToAdd -> sourceMAC = this -> nodeMAC;
		tcpMessageToAdd -> destinationMAC = parent->nodeMAC;
		//parent->receiveMessage(tcpMessageToAdd);
		tcpMessageToAdd->timeInAir = TIME_BETWEEN_HOPS;
		tcpMessageToAdd -> messageSendTime = RPLMessage::currentSimulationTime;
		parent->addToMessagesOnTheWay(tcpMessageToAdd, true);
        messagesSent += 1;
	}

	if (tcpMessage->sourceIP == nodeIP){
	    tcpMessage -> messageSendTime = RPLMessage::currentSimulationTime;
		timeoutQueue.pushback(tcpMessage);
	}
}

void RPLNode::resolveMessage(int messageID){
    if (resolvedMessagesQueue.find(messageID) != nullptr){
        return;
    }
	RPLMessage* resolvedMessage = timeoutQueue.popChosen(messageID);
	if (resolvedMessage != nullptr){
	    resolvedMessage->messageResolveTime = RPLMessage::currentSimulationTime;
	    if (DEBUG){
	        cout << RPLMessage::currentSimulationTime << endl;
	        nodeDebugMessage("message resolved in " + to_string(resolvedMessage->messageResolveTime - resolvedMessage->messageSendTime) + " timeStamps");
	    }
    	resolvedMessagesQueue.pushback(resolvedMessage);
	}
	else {
	    resolvedMessage = messageQueue.popChosen(messageID);
	    if (resolvedMessage != nullptr){
	        resolvedMessage->messageResolveTime = RPLMessage::currentSimulationTime;
	        if (DEBUG){
	         cout << RPLMessage::currentSimulationTime << endl;
	         nodeDebugMessage("message resolved in " + to_string(resolvedMessage->messageResolveTime - resolvedMessage->messageSendTime) + " timeStamps");
	        }
    	    resolvedMessagesQueue.pushback(resolvedMessage);
	    }
	}
}

void RPLNode::addToMessagesOnTheWay(RPLMessage* message, bool toParent){
	RPLMessage* messageToAdd = new RPLMessage(message);
	messageToAdd->timeInAir = TIME_BETWEEN_HOPS;
	if (toParent){
	    messageToAdd->timeInAir = TIME_BETWEEN_HOPS;
	}
	messagesOnTheWay.pushback(messageToAdd);
}

void RPLNode::moveFromTransmissionQueueToWaitQueue(RPLMessage * message){
	receiveMessage(message);
}

void RPLNode::moveFromTimeoutQueueToWaitQueue(RPLMessage* message){
	receiveMessage(message);
}

void RPLNode::sendTCP_ACKMessage(RPLMessage *tcpAckMessage){
	if (tcpAckMessage->destinationIP == nodeIP){
	    if (DEBUG){
	        string toPrint= "ack matches message";
	        nodeDebugMessage(toPrint);
	    }
		resolveMessage(tcpAckMessage->messageID);
		return;
	}
	int subnetChildIndex = determineIPExistsInSubnet(tcpAckMessage->destinationIP);
	if (subnetChildIndex != -1){
		RPLMessage* tcpAckMessageToAdd = new RPLMessage(tcpAckMessage);
		tcpAckMessageToAdd  -> sourceMAC = this -> nodeMAC;
		tcpAckMessageToAdd -> timeInAir = TIME_BETWEEN_HOPS;
		//children[subnetChildIndex]->receiveMessage(tcpAckMessageToAdd );
		children[subnetChildIndex]->addToMessagesOnTheWay(tcpAckMessageToAdd);
        messagesSent += 1;
		//delete [] (tcpAckMessage);
	}
	else {
		RPLMessage* tcpAckMessageToAdd = new RPLMessage(tcpAckMessage);
		tcpAckMessageToAdd -> sourceMAC = this -> nodeMAC;
		tcpAckMessageToAdd -> destinationMAC = parent->nodeMAC;
		tcpAckMessageToAdd -> timeInAir = 1;
		//parent->receiveMessage(tcpAckMessageToAdd);
		if (DEBUG){
		    string toPrint =  "sending ack message to parent";
		    nodeDebugMessage(toPrint);
		}
		parent-> addToMessagesOnTheWay(tcpAckMessageToAdd, true);
        messagesSent += 1;
		//delete[] (tcpAckMessage);
	}

}

void RPLNode::determineIfMoveFromTransmissionQueueToWaitQueue(){
	RPLMessage* messageToCheck = messagesOnTheWay.head->next;
	while (messageToCheck != messagesOnTheWay.tail){
		if (messageToCheck->timeInAir <= 0){
		    if (DEBUG){
		        string id = std::to_string(messageToCheck-> messageID);
		        string toPrint = ": moving message " +  std::to_string(messageToCheck-> messageID) + " from transmission queue to messageQueue " + " size " + to_string(messagesOnTheWay.queueSize);
		        nodeDebugMessage(toPrint);
		    }
			RPLMessage* messageToPop = messagesOnTheWay.pop();
			receiveMessage(messageToPop);
			messageToCheck = messagesOnTheWay.getFront();
		}
		else {
			messageToCheck -> timeInAir -= 1;
			messageToCheck = messageToCheck->next;
		}
	}
}

void RPLNode::receiveMessage(RPLMessage * message){
	messageQueue.pushback(message);
}

void RPLNode::handleMessage(RPLMessage * message){
	if (message->messageType == MessageType::DIO){
		sendDIOMessage(message);
	}
	else if (message->messageType == MessageType::TCP){
		sendTCPMessage(message);
	}
	else if (message->messageType == MessageType::TCP_ACK){
		sendTCP_ACKMessage(message);
	}
}

void RPLNode::moveTimeStamp(){
	determineIfMoveFromTransmissionQueueToWaitQueue();
	for (int i=0; i< MESSAGES_HANDLED_PER_TIMESTAMP; i++){
		if (messageQueue.getFront() != messageQueue.tail){
		    if (DEBUG){
		        string debug = "messageQueue size " + to_string(messageQueue.queueSize);
		        nodeDebugMessage(debug);
			    string toPrint = " handling message "  + std::to_string(messageQueue.getFront()->messageID) + " message type " +  to_string(messageQueue.getFront()->messageType);
			    nodeDebugMessage(toPrint);
		    }
			handleMessage(messageQueue.pop());
			determineTimeout();
		}
	}
}

bool RPLNode::determineIfNothingToDo(){
    return messageQueue.isEmpty() and timeoutQueue.isEmpty() and messagesOnTheWay.isEmpty();
}

void RPLNode::nodeDebugMessage(string message){
    cout << "Node " << nodeIP << ": " << message << endl;

}

void RPLNode::determineTimeout(){
    RPLMessage * messageToCheck = timeoutQueue.getFront();
    while (messageToCheck != timeoutQueue.tail){
        if (messageToCheck->messageSendTime + TCP_MESSAGE_TIMEOUT <= RPLMessage::currentSimulationTime){
            if (DEBUG){
                nodeDebugMessage("timeout Working");
            }
            messageQueue.pushback(timeoutQueue.pop());
            messageToCheck = timeoutQueue.getFront();
        }
        else {
            messageToCheck = messageToCheck->next;
        }
    }
}

void RPLNode::calculateStatistics(){
    if (resolvedMessagesQueue.queueSize == 0){
        return;
    }
    double handleTimeSum = 0;
    RPLMessage* resolvedMessage = resolvedMessagesQueue.getFront();
    while (resolvedMessage != resolvedMessagesQueue.tail){
        handleTimeSum += double(resolvedMessage->messageResolveTime - resolvedMessage ->messageSendTime);
        resolvedMessage = resolvedMessage->next;
    }
    string toPrint = "average message handle time = " + to_string(handleTimeSum/double(resolvedMessagesQueue.queueSize));
    printStatistics(toPrint);
}

void RPLNode::printStatistics(string toPrint){
    cout << "Node " << nodeIP << " " << toPrint << endl;
}

void RPLNode::printNumberOfMessagesSent(){
    cout << "Node " << nodeIP << " number of messages sent = " << messagesSent << endl;
}
