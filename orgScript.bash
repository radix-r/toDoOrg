#!/bin/bash
read -p 'To-Do file to organize: ' fName
echo "Starting to-do organizer on $fName"
while true; do  
	echo "Compiling" 
	`gcc -o org toDoOrg.c` 
	echo "Running"
	`./org $fName`
	
	#sleep 
	echo "Sleeping for 1 hour" 
	sleep 3600 
done
