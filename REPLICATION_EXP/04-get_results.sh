#!/bin/sh

resdir=RESULTS

for expdir in *
do
    results=0
    if [ -d "${expdir}" ] && [ -e "./${expdir}/sumres.py" ]
    then
        cd $expdir
        ./sumres.py
        if [ $? -eq 0 ] # ok, some results found,
        then            # then process them furtner
            ./2dca_evol cmrfile.txt
            for ftab in *
            do
                case "$ftab" in
                    *.tab)
                    mkdir -p "../${resdir}.${expdir}"
                    cp -n $ftab "../${resdir}.${expdir}"
                    results=`expr $results + 1`
                    ;;
                esac
            done
            if [ $results -gt 0 ]
            then
                cp -n "../default.cas" "../${resdir}.${expdir}"
                cp -n "../BiCAS.py/BiCAS.py" "../${resdir}.${expdir}"
            fi
        else
            echo "No results found in $expdir."
        fi
        cd ..
    fi
done
