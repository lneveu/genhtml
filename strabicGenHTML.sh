#!/bin/bash
if [ $# -ne 2 ]
then
	echo "Error, wrong parameters !"
	echo "Usage: ./strabicGenHTML [graph_file] [html_out_file]"
	exit 1 
else
  lua constellationGen.lua $1 $2
fi 
