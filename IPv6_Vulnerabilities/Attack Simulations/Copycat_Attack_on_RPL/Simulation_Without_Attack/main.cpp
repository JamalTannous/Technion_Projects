#include "RPLNetwork.h"
#include "Queue.h"
#include <iostream>
using namespace std;
int main (){
	cout << "Start running Copycat Attacks in IPv6-Based Low Power and Lossy Networks simulation\n";
	vector <RPLMessage*> messagesInScript;
	RPL_Network rplNetwork;
    rplNetwork.generateTCPMessageSequence();
	while (!rplNetwork.determineSimulationFinished()){
	    //cout << "###########################" << endl;
	    //cout << "currentSimulationTime = " << RPLMessage::currentSimulationTime << endl;
	    RPLMessage::currentSimulationTime+=1;
	    rplNetwork.moveTimeStamp();
	    //cout << "###########################" << endl;
	    if (RPLMessage::currentSimulationTime % 5 == 0){
	        rplNetwork.generateTCPMessageSequence();
	    }

	    if (RPLMessage::currentSimulationTime % 21 == 0){
	        rplNetwork.generateDIOMessageSequence();
	    }

    }

	rplNetwork.printStatistics();
	cout << "Simulation took " << RPLMessage::currentSimulationTime << " timestamps" << endl; 
	cout << "Finished running Copycat Attacks in IPv6-Based Low Power and Lossy Networks simulation\n";
	return 0;
}
