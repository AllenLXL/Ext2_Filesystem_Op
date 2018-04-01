#!/usr/bin/env bash

# --- Now do the diff ---

the_files="$(ls self-tester/results)"
for file in $the_files
    do
        temp=$(basename $file)
        echo "============================ doing test for $temp ==============================="
        diff self-tester/results/$temp self-tester/solution-results/$temp
    done