#include "RPLNetwork.h"
#include "Queue.h"
#include <iostream>
using namespace std;
int main (){
	cout << "Start running Copycat Attacks in IPv6-Based Low Power and Lossy Networks simulation\n";
	for (int i = 0; i < 20; i++){
	RPL_Network rplNetwork(i);
    rplNetwork.generateTCPMessageSequence();
	while (!rplNetwork.determineSimulationFinished()){
	   // cout << "###########################" << endl;
	   // cout << "currentSimulationTime = " << RPLMessage::currentSimulationTime << endl;
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
	//cout << "Hacker node detected = Node with IP " << rplNetwork.detectedHacker->nodeIP << endl;
	cout << "Simulation took " << RPLMessage::currentSimulationTime << " timestamps" << endl; 
	cout << "Current Simulation Finished" << endl << "Press any key to continue, to exit press Ctrl+C" << endl; 
	string temp;
	cin >> temp;
	cout << endl << endl;
	RPLMessage::currentSimulationTime =0;
	
	}
	cout << "Finished running Copycat Attacks in IPv6-Based Low Power and Lossy Networks simulation\n";
	return 0;
}
