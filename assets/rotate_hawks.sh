#!/bin/bash
for file in hawk_*.bmp; do
  # Extract the number part, e.g., 000, 001
#   number=$(echo "$file" | grep -oP '(?<=hawk_)\d+(?=\.bmp)')
  number=${file#hawk_}
  number=${number%.bmp}
  if [[ -n "$number" ]]; then
    output="hawkL_${number}.bmp"
    echo "Processing $file -> $output"
    convert "$file" -flop "$output"
  else
    echo "Skipping $file: does not match pattern"
  fi
done
