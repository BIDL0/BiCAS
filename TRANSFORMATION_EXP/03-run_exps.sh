#!/bin/sh

# the number of independent runs you would like to perform for each setup
# (in this case derived from the number of CPU cores available)
EXPS=`exec nproc`

for expdir in *
do
    if [ -d "${expdir}" ] && [ -e "./${expdir}/Makefile" ]
    then
        cd $expdir
        x=1
        while [ "$x" -le "$EXPS" ]
        do
            echo "Running experiment #$x/$EXPS in $expdir..."
            # run on the background (preferred), otherwise modify accordingly
            nice ./2dca_evol > "EXP.$x.out" &
            x=`expr $x + 1`
        done
        cd ..
    fi
done
