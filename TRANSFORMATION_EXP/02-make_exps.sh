#!/bin/sh

./~proc_states.py

for expdir in *
do
    if [ -d "$expdir" ] && [ -e "./$expdir/Makefile" ]
    then
        echo "Making in $expdir..."
        cd $expdir
        make
        cd ..
    fi
done
