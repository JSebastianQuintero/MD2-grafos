#!/bin/bash

# Specify the relative paths where the files are located
relative_path1="grafos"
relative_path2="grafosGrandes"

# Get the absolute paths by combining the current directory with the relative paths
path1="$(pwd)/$relative_path1"
path2="$(pwd)/$relative_path2"

count=0

# Function to process files in a given path
process_files() {
  local path=$1
  local files=$(ls "$path")

  # Iterate over each file
  for file in $files
  do
    # Check if the file is a regular file (not a directory)
    if [ -f "$path/$file" ]; then
      ((count++))
      size=$(du -sh "$path/$file" | cut -f1)
      echo "File: $file, Size: $size"
      # Print the first line of the file
      head -n 1 "$path/$file"
      # Execute the program with the file as input
      start_time=$(date +%s.%N)
      ./program < "$path/$file"

      if [ $? -ne 0 ]; then
        echo "Program failed. Exiting."
        exit 1
      fi

      end_time=$(date +%s.%N)
      elapsed_time=$(echo "$end_time - $start_time" | bc)

      echo "Time elapsed: $elapsed_time seconds"
      echo

    fi
  done

}
clear

# Compile the program
gcc -std=c99 -Wall -Werror -g -o program parte1-chatgpt.c

# Check if compilation was successful
if [ $? -ne 0 ]; then
  echo "Program compilation failed. Exiting."
  exit 1
fi

# Process files in the first path (grafos)
process_files "$path1"

# Process files in the second path (grafosGrandes)
process_files "$path2"

echo "Total executions: $count"