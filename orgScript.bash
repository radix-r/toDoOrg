#!/bin/bash
echo "Starting to-do organizer"
while true; do  
	echo "compiling" 
	`gcc -o org toDoOrg.c` 
	echo "running"
	`./org toDo.txt`
	
	#sleep 
	echo "Sleeping for 1 hour" 
	sleep 3600 
done
