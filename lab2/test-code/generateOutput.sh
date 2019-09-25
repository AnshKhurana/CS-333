#!/bin/bash
if [ "$#" -ne 1 ]; then 
    echo "Usage: ./generateOutput.sh outputdir"
    exit
fi
echo "Output: $1"
OUTDIR=$1

rm -f xv6-public
tar -zxvf xv6-public.tgz
tar -zxvf xv6-public-patch.tgz
cp xv6-public-patch/* xv6-public/
cp instructor_modified_files/* xv6-public/
cp student_modified_files/* xv6-public/
cp testcases/* xv6-public/

python3 helper.py

mkdir $OUTDIR -p
cp xv6-public/out*.txt $OUTDIR
