#!/bin/bash

source ./backup_restore_lib.sh

A=$1	#original file directory
B=$2	#backup file directory
c=$3	#password

validate_backup_params $A $B $C		#call function to validate parameters

if [ -d $A ]; then		#check if directories do exist before calling the backup function
	if [ -d $B ]; then
	backup $A $B $C		#call backup unction
	fi	
fi

unset A B C
