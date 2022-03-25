#Building a compact, relatively cheap, battery powered pet microchip reader, which scans the pet's microchip and sends its id to a database that connects to an Android application through which we can access the pet's data, take pictures and modify the data in the cloud very easily.


Lots of pets today have microchips inserted in their body.
Each one of those microchips has an id which consists of 16 bytes.
In order to scan the pet’s microchip, the user needs an RFID chip scanner.

Most interfaces that deal with pet chip readers are hard to operate, the user needs to place the pet in a certain position with the help of extra personnel in order to manage to scan the microchip placed inside the pet.
The hardships do not end here, because after scanning the microchip, the doctor must insert the chip’s number manually on a separate computer in order to access the relevant data. 

It is also noticeable that most of these pet scanners are relatively very expensive.

In this project we have managed to design a compact, battery powered pet chip scanner. 
The scanner transmits the serial number to a connected phone, using what is called a real time database.
Afterwards the user can access the pet’s data using an application which reads from a cloud storage dedicated to store information about the pets that are/were treated in the doctor’s clinic.
In case the pet is being treated for the first time, then adding the information about the pet is very easy and intuitive; the user should fill out the fields using the same application.
