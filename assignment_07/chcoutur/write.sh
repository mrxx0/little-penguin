#!/bin/bash

i=0
while :
do
	i=$((i+1))
	echo $i > /sys/kernel/debug/fortytwo/foo
done
