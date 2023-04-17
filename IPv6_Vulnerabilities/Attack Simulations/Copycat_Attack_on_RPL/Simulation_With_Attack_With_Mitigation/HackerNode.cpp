//
// Created by jamal on 12/3/2022.
//

#include "HackerNode.h"
#define HACKER_DIO_MAX 5
HackerNode::HackerNode(RPLNode* parent, string nodeIP, string nodeMAC, bool isRoot) : RPLNode(parent, nodeIP, nodeMAC, isRoot) {
    isHackerNode = true;
}

void HackerNode::sendDIOMessage(RPLMessage *dioMessage) {
    for (int i =0; i < HACKER_DIO_MAX; i++){
       // RPLNode::sendDIOMessage(dioMessage);
        // MessageType type, string sourceIP, string destinationIP, string sourceMAC = "", string destinationMAC =""
        RPLMessage* messageToSend = new RPLMessage (dioMessage->messageID,dioMessage->messageType, nodeIP, dioMessage->destinationIP, nodeMAC, parent->nodeMAC);
        //parent->receiveMessage(messageToSend);
        messageToSend->timeInAir = TIME_BETWEEN_HOPS;
        parent -> addToMessagesOnTheWay(messageToSend, true);
        DIOMessagesSent += 1;
    }
}
