#!/bin/bash

# Set nice value (lower values = higher priority)
NICE_VALUE=-5

# Run gen.py with increased priority
echo "Running gen.py with nice value $NICE_VALUE..."
sudo nice -n $NICE_VALUE python3 gen.py

# Check if gen.py ran successfully
if [ $? -ne 0 ]; then
    echo "gen.py encountered an error."
    exit 1
fi

# Run test.py with increased priority
echo "Running test.py with nice value $NICE_VALUE..."
sudo nice -n $NICE_VALUE python3 test.py

# Check if test.py ran successfully
if [ $? -ne 0 ]; then
    echo "test.py encountered an error."
    exit 1
fi

# Run the main program with increased priority
echo "Running ./main with nice value $NICE_VALUE..."
sudo nice -n $NICE_VALUE ./main

# Check if the main program ran successfully
if [ $? -ne 0 ]; then
    echo "./main encountered an error."
    exit 1
fi

# Run diff
echo "Comparing c_out.txt and output.txt..."
diff c_out.txt output.txt

# Check if diff ran successfully
if [ $? -eq 0 ]; then
    echo "Files are identical."
else
    echo "Files differ."
fi

echo "Script completed."
