#!/bin/bash
read -p 'To-Do file to organize: ' fName
echo "Starting to-do organizer on $fName"

#create reference file. If target differers from reference run
`touch ref.txt`
#in case something goes wrong keep the previous version
`touch prev.txt`   



while true; do  
	bool_run=0;
	
	#use makefile
	echo "Compiling" 
	`make > make.out` 
	
	d=$(diff make.out makeComp.txt)
	
	#echo "$d"
	
	if [ ! -z $d ]; then
		echo "Change to source code detected"
		bool_run=1
	fi
	
	
	# compare ref and $fName. if they differ run
	d=$(diff ref.txt $fName)
	
	if [[ ! -z $d ]]; then
		echo "Target file changed"
		bool_run=1
	fi
	
	if [ $bool_run -eq 1 ]; then
		# Update prev.txt
		`cat $fName > prev.txt`
		
		echo "Running"
		`./org $fName`
		
		# update ref.txt
		`cat $fName > ref.txt`
	fi
	
	#sleep 
	echo "Sleeping for 1 hour" 
	sleep 3600 
done
