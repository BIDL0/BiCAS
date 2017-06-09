#!/bin/sh

for expdir in *
do
    if [ -d "$expdir" ]
    then
        if [ "$expdir" != "BiCAS.py" ]
        then
            echo "Cleaning in $expdir"
            cd $expdir
            rm -f *.tab
            rm -f EXP.*
            rm -f resfile.txt
            rm -f cmrfile.txt
            rm -f default.cas
            rm -f BiCAS.py
            cd ..
            if [ -z "$(ls -A $expdir)" ]; then
                rmdir $expdir
            fi
        fi
    fi
done
