#!/bin/bash
g++ tester.cpp rwlock-reader-pref.cpp -o rwlock-reader-pref -lpthread
g++ tester.cpp rwlock-writer-pref.cpp -o rwlock-writer-pref -lpthread

echo "Generating output in outputs-reader-pref"
rm -rf outputs-reader-pref
mkdir outputs-reader-pref
./rwlock-reader-pref 5 0 > outputs-reader-pref/output_5_0
./rwlock-reader-pref 5 1 > outputs-reader-pref/output_5_1
./rwlock-reader-pref 5 3 > outputs-reader-pref/output_5_3
./rwlock-reader-pref 0 4 > outputs-reader-pref/output_0_4

echo "Grading output in outputs-reader-pref"
python autograder-reader-pref.py 

echo "Generating output in outputs-writer-pref"
rm -rf outputs-writer-pref
mkdir outputs-writer-pref
./rwlock-writer-pref 5 0 > outputs-writer-pref/output_5_0
./rwlock-writer-pref 5 1 > outputs-writer-pref/output_5_1
./rwlock-writer-pref 5 3 > outputs-writer-pref/output_5_3
./rwlock-writer-pref 0 4 > outputs-writer-pref/output_0_4

echo "Grading output in outputs-writer-pref"
python autograder-writer-pref.py 
