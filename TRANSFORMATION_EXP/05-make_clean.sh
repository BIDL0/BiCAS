#!/bin/sh

rm -f sca_params.h
rm -f states.h
rm -f default.cas

for expdir in *
do
    if [ -d "$expdir" ] && [ -e "./$expdir/Makefile" ]
    then
        echo "Cleaning in $expdir..."
        cd $expdir
        make clean
        cd ..
    fi
done
