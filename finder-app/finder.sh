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

#https://stackoverflow.com/questions/18135451/what-is-the-difference-between-var-var-and-var-in-the-bash-shell
var="foo bar"
for i in "${var}dddd"; do # Expands to 'for i in "foo bar"; do...'
    echo $i         #   so only runs the loop once
done

#array example
foo=(a b c)
# Returns number of elements in array.
$ echo ${#foo[*]}
echo ${foo}
#a

echo -e "\n"
echo ${foo[@]}
# a b c

echo -e "\n"
echo $foo[@]
# a[@]

foo=("the first" "the second")
for i in "${foo[@]}"; do # Expands to 'for i in "the first" "the second"; do...'
    echo $i              #   so the loop runs twice
done
# the first
# the second

echo -e "\n"
foo=("the first" "the second")
for i in ${foo[@]}; do # Expands to 'for i in the first the second; do...'
    echo $i            #   so the loop runs four times!
done
# the
# first
# the
# second