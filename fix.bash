#!/bin/bash

cat kauai.txt | while read line
do
  ./fixer t6/${line} -2 commit
done

cat home.txt | while read line
do
  ./fixer t6/${line} 1 commit
done
