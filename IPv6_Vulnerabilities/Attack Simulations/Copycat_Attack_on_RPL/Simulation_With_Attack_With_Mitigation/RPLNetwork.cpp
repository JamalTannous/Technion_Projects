/*
 * RPLNetwork.cpp
 *
 *  Created on: Sep 4, 2022
 *      Author: jamal
 */

#include "RPLNetwork.h"
#include <iostream>
using namespace std;
#define TCP_GENERATE_COUNTER_MAX 200
#define DIO_GENERATE_COUNTER_MAX 20
RPL_Network::RPL_Network(int parentIndex) {
	//RPLNode* parent, string nodeIP, string nodeMAC, bool isRoot
	messagesIDPool = 0;
	tcpGenerateCounter = 0;
	dioGenerateCounter = 0;
	hackerDetected = false;
	detectedHacker = nullptr;
	strikeTwoNode = nullptr;
	strikeOneNode = nullptr;
	RPLNode* a = new RPLNode(nullptr, "A", "A",true);
	root = a;
	nodes.push_back(a);
	RPLNode* b = new RPLNode(a, "B", "B",false);
	RPLNode* c = new RPLNode(a, "C", "C",false);
	RPLNode* d = new RPLNode(a, "D", "D",false);
	a->addChild(b);
	a->addChild(c);
	a->addChild(d);
	nodes.push_back(b);
	nodes.push_back(c);
	nodes.push_back(d);
	RPLNode* e = new RPLNode(b, "E", "E",false);
	RPLNode* f = new RPLNode(b, "F", "F",false);
	b->addChild(e);
	b->addChild(f);
	nodes.push_back(e);
	nodes.push_back(f);
	RPLNode* g = new RPLNode(c, "G", "G",false);
	RPLNode* h = new RPLNode(c, "H", "H",false);
	c->addChild(g);
	c->addChild(h);
	nodes.push_back(g);
	nodes.push_back(h);
	RPLNode* i = new RPLNode(e, "I", "I",false);
	RPLNode* j = new RPLNode(e, "J", "J",false);
	e->addChild(i);
	e->addChild(j);
	nodes.push_back(i);
	nodes.push_back(j);
	RPLNode* k = new RPLNode(f, "K", "K",false);
	f->addChild(k);
	nodes.push_back(k);
	RPLNode* l = new RPLNode(g, "L", "L",false);
	g->addChild(l);
	nodes.push_back(l);
	RPLNode* m = new RPLNode(i, "M", "M",false);
	i->addChild(m);
	nodes.push_back(m);
	RPLNode* n = new RPLNode(k, "N", "N",false);
	RPLNode* o = new RPLNode(k, "O", "O",false);
	k->addChild(n);
	k->addChild(o);
	nodes.push_back(n);
	nodes.push_back(o);
	RPLNode* p = new RPLNode(l, "P", "P",false);
	l->addChild(p);
	nodes.push_back(p);
	RPLNode* q = new RPLNode(o, "Q", "Q",false);
	o->addChild(q);
	nodes.push_back(q);
	RPLNode* r = new RPLNode(p, "R", "R",false);
	p->addChild(r);
	nodes.push_back(r);
	RPLNode* s = new RPLNode(q, "S", "S",false);
	q->addChild(s);
	nodes.push_back(s);
	RPLNode* t = new RPLNode(s, "T", "T",false);
	s->addChild(t);
	nodes.push_back(t);


    /// Here we should add the hacker and pick a place for it.
    /*HackerNode * hackerNode = new HackerNode(t, "X", "X", false);
    t->addChild(hackerNode);
    nodes.push_back(hackerNode);*/
	addHackerNode(nodes[parentIndex]);
}

void RPL_Network::addHackerNode(RPLNode * parent){
	HackerNode * hackerNode = new HackerNode(parent, "X", "X", false);
	parent -> addChild (hackerNode);
	nodes.push_back(hackerNode);
}

RPL_Network::~RPL_Network() {
	// TODO Auto-generated destructor stub
}

void RPL_Network::moveTimeStamp(){
	for (int i=0; i< nodes.size(); i++){
	    if (detectedHacker == nodes[i]){
            continue;
	    }
		nodes[i]->moveTimeStamp();
	}
}

bool RPL_Network::determineSimulationFinished(){
    for (int i=0; i<nodes.size(); i++){
        if (nodes[i] != detectedHacker) {
            if (!nodes[i]->determineIfNothingToDo()) {
                return false;
            }
        }
    }
    return true;
}


void RPL_Network::generateTCPMessageSequence(){
        	///RPLMessage(int messageID,MessageType type, string sourceIP, string destinationIP, string sourceMAC = "", string destinationMAC ="")
        if (tcpGenerateCounter >= TCP_GENERATE_COUNTER_MAX){
            return;
        }

        /// Nodes Identifiers for easier understanding of the code
        RPLNode * A = root;
        RPLNode * B = root->children[0];
        RPLNode * C = root->children[1];
        RPLNode * D = root->children[2];
        RPLNode * E = B->children[0];
        RPLNode * F = B->children[1];
        RPLNode * G = C->children[0];
        RPLNode * H = C->children[1];
        RPLNode * I = E->children[0];
        RPLNode * J = E->children[1];
        RPLNode * K = F->children[0];
        RPLNode * L = G->children[0];
        RPLNode * M = I->children[0];
        RPLNode * N = K->children[0];
        RPLNode * O = K->children[1];
        RPLNode * P = L->children[0];
        RPLNode * Q = O->children[0];
        RPLNode * R = P->children[0];
        RPLNode * S = Q->children[0];
        RPLNode * T = S->children[0];

        /// Message 1 A->E
        RPLNode* sourceNode = root;
        RPLNode* destinationNode = root->children[0]->children[0];
        RPLMessage* tcpMessage = new RPLMessage(messagesIDPool,MessageType::TCP, sourceNode->nodeIP, destinationNode->nodeIP, sourceNode->nodeMAC, destinationNode->nodeMAC);
        root->addToMessagesOnTheWay(tcpMessage);
        messagesIDPool += 1;

        /// Message 2 A->B
        sourceNode = root;
        destinationNode = root->children[0];
        tcpMessage = new RPLMessage(messagesIDPool,MessageType::TCP, sourceNode->nodeIP, destinationNode->nodeIP, sourceNode->nodeMAC, destinationNode->nodeMAC);
        sourceNode->addToMessagesOnTheWay(tcpMessage);
        messagesIDPool += 1;

        /// Message 3 B->D
        sourceNode = root->children[0];
        destinationNode = root->children[2];
        tcpMessage = new RPLMessage(messagesIDPool,MessageType::TCP, sourceNode->nodeIP, destinationNode->nodeIP, sourceNode->nodeMAC, destinationNode->nodeMAC);
        root->addToMessagesOnTheWay(tcpMessage);
        messagesIDPool += 1;

        /// Message 4 P->T
        sourceNode = root->children[1]->children[0]->children[0]->children[0];
        destinationNode = root->children[0]->children[1]->children[0]->children[1]->children[0]->children[0]->children[0];
        tcpMessage = new RPLMessage(messagesIDPool,MessageType::TCP, sourceNode->nodeIP, destinationNode->nodeIP, sourceNode->nodeMAC, destinationNode->nodeMAC);
        sourceNode->addToMessagesOnTheWay(tcpMessage);
        messagesIDPool += 1;

        /// Message 5 N -> D
        sourceNode = root->children[0]->children[1]->children[0]->children[0];
        destinationNode = root->children[2];
        tcpMessage = new RPLMessage(messagesIDPool,MessageType::TCP, sourceNode->nodeIP, destinationNode->nodeIP, sourceNode->nodeMAC, destinationNode->nodeMAC);
        sourceNode->addToMessagesOnTheWay(tcpMessage);
        messagesIDPool += 1;

        /// Message 6 J-> C
        sourceNode = root->children[0]->children[0]->children[1];
        destinationNode = root->children[1];
        tcpMessage = new RPLMessage(messagesIDPool,MessageType::TCP, sourceNode->nodeIP, destinationNode->nodeIP, sourceNode->nodeMAC, destinationNode->nodeMAC);
        sourceNode->addToMessagesOnTheWay(tcpMessage);
        messagesIDPool += 1;

        /// Message E->L
        sourceNode = E;
        destinationNode = L;
        tcpMessage = new RPLMessage(messagesIDPool,MessageType::TCP, sourceNode->nodeIP, destinationNode->nodeIP, sourceNode->nodeMAC, destinationNode->nodeMAC);
        sourceNode->addToMessagesOnTheWay(tcpMessage);
        messagesIDPool += 1;

        /// Message M->J
        tcpMessage = new RPLMessage(messagesIDPool,MessageType::TCP, M->nodeIP, J->nodeIP, M->nodeMAC, J->nodeMAC);
        sourceNode->addToMessagesOnTheWay(tcpMessage);
        messagesIDPool += 1;

        /// Message K->A
        sourceNode = K;
        destinationNode = A;
        tcpMessage = new RPLMessage(messagesIDPool,MessageType::TCP, sourceNode->nodeIP, destinationNode->nodeIP, sourceNode->nodeMAC, destinationNode->nodeMAC);
        sourceNode->addToMessagesOnTheWay(tcpMessage);
        messagesIDPool += 1;

        /// Message C->K
         sourceNode = C;
         destinationNode = K;
         tcpMessage = new RPLMessage(messagesIDPool,MessageType::TCP, sourceNode->nodeIP, destinationNode->nodeIP, sourceNode->nodeMAC, destinationNode->nodeMAC);
         sourceNode->addToMessagesOnTheWay(tcpMessage);
         messagesIDPool += 1;

        /// Message I->H
        sourceNode = I;
        destinationNode = H;
        tcpMessage = new RPLMessage(messagesIDPool,MessageType::TCP, sourceNode->nodeIP, destinationNode->nodeIP, sourceNode->nodeMAC, destinationNode->nodeMAC);
        sourceNode->addToMessagesOnTheWay(tcpMessage);
        messagesIDPool += 1;

        /// Message H -> F
        sourceNode = H;
        destinationNode = F;
        tcpMessage = new RPLMessage(messagesIDPool,MessageType::TCP, sourceNode->nodeIP, destinationNode->nodeIP, sourceNode->nodeMAC, destinationNode->nodeMAC);
        sourceNode->addToMessagesOnTheWay(tcpMessage);
        messagesIDPool += 1;

        /// Message F -> L
        sourceNode = F;
        destinationNode = L;
        tcpMessage = new RPLMessage(messagesIDPool,MessageType::TCP, sourceNode->nodeIP, destinationNode->nodeIP, sourceNode->nodeMAC, destinationNode->nodeMAC);
        sourceNode->addToMessagesOnTheWay(tcpMessage);
        messagesIDPool += 1;

        /// Message  Q -> P
        sourceNode = Q;
        destinationNode = P;
        tcpMessage = new RPLMessage(messagesIDPool,MessageType::TCP, sourceNode->nodeIP, destinationNode->nodeIP, sourceNode->nodeMAC, destinationNode->nodeMAC);
        sourceNode->addToMessagesOnTheWay(tcpMessage);
        messagesIDPool += 1;

        /// Message O -> Q
        sourceNode = O;
        destinationNode = Q;
        tcpMessage = new RPLMessage(messagesIDPool,MessageType::TCP, sourceNode->nodeIP, destinationNode->nodeIP, sourceNode->nodeMAC, destinationNode->nodeMAC);
        sourceNode->addToMessagesOnTheWay(tcpMessage);
        messagesIDPool += 1;

        /// Message R -> O
        sourceNode = R;
        destinationNode = O;
        tcpMessage = new RPLMessage(messagesIDPool,MessageType::TCP, sourceNode->nodeIP, destinationNode->nodeIP, sourceNode->nodeMAC, destinationNode->nodeMAC);
        sourceNode->addToMessagesOnTheWay(tcpMessage);
        messagesIDPool += 1;

        /// Message S -> R
        sourceNode = S;
        destinationNode = R;
        tcpMessage = new RPLMessage(messagesIDPool,MessageType::TCP, sourceNode->nodeIP, destinationNode->nodeIP, sourceNode->nodeMAC, destinationNode->nodeMAC);
        sourceNode->addToMessagesOnTheWay(tcpMessage);
        messagesIDPool += 1;

        /// Message T->M
        sourceNode = T;
        destinationNode = M;
        tcpMessage = new RPLMessage(messagesIDPool,MessageType::TCP, sourceNode->nodeIP, destinationNode->nodeIP, sourceNode->nodeMAC, destinationNode->nodeMAC);
        sourceNode->addToMessagesOnTheWay(tcpMessage);
        messagesIDPool += 1;

        /// Message G->N
        sourceNode = G;
        destinationNode = N;
        tcpMessage = new RPLMessage(messagesIDPool,MessageType::TCP, sourceNode->nodeIP, destinationNode->nodeIP, sourceNode->nodeMAC, destinationNode->nodeMAC);
        sourceNode->addToMessagesOnTheWay(tcpMessage);
        messagesIDPool += 1;

        /// Message D->G
        sourceNode = D;
        destinationNode = G;
        tcpMessage = new RPLMessage(messagesIDPool,MessageType::TCP, sourceNode->nodeIP, destinationNode->nodeIP, sourceNode->nodeMAC, destinationNode->nodeMAC);
        sourceNode->addToMessagesOnTheWay(tcpMessage);
        messagesIDPool += 1;

        /// Message L -> I
        sourceNode = L;
        destinationNode = I;
        tcpMessage = new RPLMessage(messagesIDPool,MessageType::TCP, sourceNode->nodeIP, destinationNode->nodeIP, sourceNode->nodeMAC, destinationNode->nodeMAC);
        sourceNode->addToMessagesOnTheWay(tcpMessage);
        messagesIDPool += 1;

    tcpGenerateCounter +=1;
}

void RPL_Network::generateDIOMessageSequence(){
    if (dioGenerateCounter > DIO_GENERATE_COUNTER_MAX){
        return;
    }
    RPLMessage* message;
    for (int i =0; i< nodes.size(); i++){
        if (nodes[i]->isHackerNode){
            continue;
        }
        message = new RPLMessage(messagesIDPool,MessageType::DIO, nodes[i]->nodeIP, "", nodes[i]->nodeMAC, "");
        nodes[i]->addToMessagesOnTheWay(message);
        messagesIDPool += 1;
    }

    dioGenerateCounter += 1;
}

void RPL_Network::printStatistics(){
    for (int i = 0; i<nodes.size();i++){
        nodes[i]->calculateStatistics();
    }
}

void RPL_Network::analyzeAndCheckForHacker(){
    RPLNode * suspicious = root;
    double minimalRatio = suspicious->calculateRatioBetweenDIOAndRegularMessages();
    for (int i =0; i<nodes.size(); i++){
        if (nodes[i]->calculateRatioBetweenDIOAndRegularMessages() <= minimalRatio){
            suspicious = nodes[i];
        }
    }
    if (strikeOneNode == strikeTwoNode and strikeOneNode == suspicious){
        hackerDetected = true;
        detectedHacker = suspicious;
    }
    strikeOneNode = strikeTwoNode;
    strikeTwoNode = suspicious;
}
