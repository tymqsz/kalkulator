
#!/bin/bash

# Initialize a total line count variable
total_lines=0

# Loop through all .c files in the current directory
for file in *.c; do
  # Check if there are any .c files
  if [[ -f "$file" ]]; then
    # Count lines in the file
    lines_in_file=$(wc -l < "$file")
    echo "$file: $lines_in_file lines"
    
    # Add to total line count
    total_lines=$((total_lines + lines_in_file))
  fi
done

# Print the total line count
echo "Total lines in all .c files: $total_lines"
