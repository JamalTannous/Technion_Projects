//
// Created by jamal on 12/3/2022.
//

#ifndef CLIONPROJECT_HACKERNODE_H
#define CLIONPROJECT_HACKERNODE_H

#include "RPLNode.h"
class HackerNode : public  RPLNode{
public:
    HackerNode(RPLNode* parent, string nodeIP, string nodeMAC, bool isRoot);
    void sendDIOMessage(RPLMessage* dioMessage) override;
};


#endif //CLIONPROJECT_HACKERNODE_H
