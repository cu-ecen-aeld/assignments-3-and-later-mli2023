#!/bin/bash

filesdir=$1
searchstr=$2

if [ -z "$filesdir" ] || [ -z "$searchstr" ]; then
    echo "Usage: $0 <filesdir> <searchstr>"
    exit 1
fi

if [ ! -d "$filesdir" ]; then
    echo "Error: Directory $filesdir does not exist."
    exit 1
fi

# Find files in the specified directory that match the search string
matching_files=$(grep -rl "$searchstr" "$filesdir") 
if [ -z "$matching_files" ]; then
    echo "No files found containing the string '$searchstr' in directory '$filesdir'."
    exit 0
fi

# Print the matching files
#echo "$matching_files"
grep -c "$searchstr" $matching_files

# Count the number of matching files
num_files=$(echo "$matching_files" | wc -l)
# Count the number of matching lines in all files
num_matching_lines=$(grep -o "$searchstr" $matching_files | wc -l)
echo "The number of files are $num_files and the number of matching lines are $num_matching_lines"

exit 0