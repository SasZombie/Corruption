#!/bin/bash
input=$1
output=$2

echo "const char* $(basename $input .fs | sed 's/\./_/g')_shader = R\"(" > $output
cat $input >> $output
echo ")\";" >> $output
