/*
 * Queue.cpp
 *
 *  Created on: Sep 4, 2022
 *      Author: jamal
 */

#include "Queue.h"

Queue::Queue() {
	head = new RPLMessage();
	tail = new RPLMessage();
	head -> next = tail;
	tail -> previous = head;
	head -> previous = nullptr;
	tail -> next = nullptr;
	queueSize = 0;
}

Queue::~Queue() {
	delete [] (head);
	delete [] (tail);
}

void Queue::pushback(RPLMessage * message){
	if (queueSize <= 0){
		head -> next = message;
		message -> previous = head;
		tail -> previous = message;
		message -> next = tail;
		queueSize += 1;
		return;
	}
	tail->previous->next = message;
	message -> previous = tail->previous;
	message -> next = tail;
	tail->previous = message;
	queueSize += 1;
	return;
}
RPLMessage* Queue::popChosen(int messageID){
	RPLMessage* messageToPop = head->next;
	while (messageToPop != tail){
		if (messageToPop->messageID == messageID){
			messageToPop -> previous -> next = messageToPop->next;
			messageToPop -> next -> previous = messageToPop->previous;
			queueSize -= 1;
			return messageToPop;
		}
		messageToPop = messageToPop->next;
	}
	return nullptr;
}
RPLMessage* Queue::pop(){
	if (queueSize == 0){
		return nullptr;
	}
	RPLMessage * toReturn = head->next;
	if (queueSize == 1){
		head -> next = tail;
		tail -> previous = head;
		queueSize -= 1;
		return toReturn;
	}
	head -> next = toReturn -> next;
	queueSize -= 1;
	return toReturn;
}

void Queue::pushFront(RPLMessage *message){
	message->next = head -> next;
	message -> previous = head;
	head -> next = message;
}

RPLMessage* Queue::getFront(){
	return head->next;
}

bool Queue::isEmpty(){
    return (head->next == tail);
}

RPLMessage* Queue::find(int messageID){
    if (queueSize == 0){
        return nullptr;
    }
    RPLMessage* toCheck = head->next;
    while (toCheck != tail){
        if (toCheck->messageID == messageID){
            return toCheck;
        }
        toCheck = toCheck->next;
    }
    return nullptr;
}
