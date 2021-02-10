#!/bin/bash

source backup_restore_lib.sh

A=$1
B=$2
C=$3

validate_restore_params $A $B $C

if [ -d $A ]; then
	if [ -d $B ]; then
		restore $A $B $C
	fi
fi
unset A B C


