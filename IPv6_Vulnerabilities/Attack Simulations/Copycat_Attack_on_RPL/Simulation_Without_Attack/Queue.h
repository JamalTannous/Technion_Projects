/*
 * Queue.h
 *
 *  Created on: Sep 4, 2022
 *      Author: jamal
 */

#ifndef QUEUE_H_
#define QUEUE_H_
#include "RPLMessage.h"

class Queue {
public:
	Queue();
	virtual ~Queue();
	RPLMessage* pop();
	RPLMessage* getFront();
	RPLMessage* popChosen (int messageID);
	void pushback(RPLMessage* message);
	void pushFront(RPLMessage * message);
	bool isEmpty();
	RPLMessage* find(int messageID);

public:
RPLMessage* head;
RPLMessage* tail;
int queueSize;
};

#endif /* QUEUE_H_ */
