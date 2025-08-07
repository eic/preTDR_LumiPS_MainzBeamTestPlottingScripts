#!/bin/bash

for file in *25-08-01*Tagger18*; do
  new_name="${file//Tagger18/Tagger26}"
  mv -- "$file" "$new_name"
  echo "Renamed: $file -> $new_name"
done
