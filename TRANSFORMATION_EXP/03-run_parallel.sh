#!/bin/sh

for expdir in *; do
    if [ -d "${expdir}" ] && [ -e "./${expdir}/Makefile" ]
    then
        echo "Running experiments in $expdir..."
        cd $expdir
        ./~qsub.sh
#        ./~qdel.sh
        cd ..
    fi
done
