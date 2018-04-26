#!/usr/bin/env bash

the_files="$(ls .)"
for file in $the_files
    do
        temp=$(basename $file)
        echo $temp
    done