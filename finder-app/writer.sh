#!/bin/bash


writefile=$1
writestr=$2
if [ -z "$writefile" ] || [ -z "$writestr" ]; then
    echo "Usage: $0 <writefile> <writestr>"
    exit 1
fi

# Get the directory of the file
writefile_dir=$(dirname "$writefile")
# Check if the directory exists, if not create it
if [ ! -d "$writefile_dir" ]; then
    mkdir -p "$writefile_dir"
    if [ $? -ne 0 ]; then
        echo "Error: Could not create directory $writefile_dir."
        exit 1
    fi
fi

# create file and write the string to it
echo "$writestr" > "$writefile"

# Check if the file was created and written to successfully
if [ ! -f "$writefile" ]; then
    echo "Error: File $writefile was not created."
    exit 1
fi