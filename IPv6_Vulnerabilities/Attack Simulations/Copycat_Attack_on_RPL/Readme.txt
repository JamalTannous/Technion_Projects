Here we have three simulations:
	1) One for simulating a normal behaviour of an RPL network (Not under attack).
	2) One for simulating a behaviour of an RPL network under a Copycat attack, without a mitigation mechanism, each iteration the attacker is connected under a different node.
	3) One for simulating a behaviour of an RPL network under a Copycat attack, using a mitigation mechanism, each iteration the attacker is connected under a different node.

To run each program, first you should use the shell script "build_sim.sh", which compiled the program and outputs the binary "copycat_sim".
The "copycat_sim" file includes the binary for the simulation.

Regarding each class in the simulation:
	1) "RPLMessage" represents messages that get sent by the nodes (TCP, DIO).
	2) "Queue" represents the FIFO message queue for each node.
	3) "RPLNode" represents each node connected to the network, each node has a parent and children.
	4) "RPLNetwork" represents, implements and monitors the RPL network, which has the topolgy "Topology.jpg".
	5) "HackerNode" inherets the "RPLNode" class and overrides certain methods to make them unique to an 	   
	   attacker's behaviour.
	
