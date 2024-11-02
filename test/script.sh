#!/bin/bash

# Set nice value (lower values = higher priority)
NICE_VALUE=-5

sudo nice -n $NICE_VALUE python3 gen.py

if [ $? -ne 0 ]; then
    echo "gen.py encountered an error."
    exit 1
fi

sudo nice -n $NICE_VALUE python3 test.py

if [ $? -ne 0 ]; then
    echo "test.py encountered an error."
    exit 1
fi

sudo nice -n $NICE_VALUE "../source/./main"

if [ $? -ne 0 ]; then
    echo "./main encountered an error."
    exit 1
fi

diff "../assets/c_out.txt" "../assets/output.txt"

if [ $? -eq 0 ]; then
    echo "Files are identical."
else
    echo "Files differ."
fi

echo "Script completed."
