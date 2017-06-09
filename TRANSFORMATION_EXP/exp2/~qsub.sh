#!/bin/sh

rm -f *.o
rm -f EXP.*
echo -ne "qdel " > "~qdel.sh"
ident=`qsub ../~submit.sh`
echo $ident >> "~qdel.sh"
chmod +x "~qdel.sh"
echo -ne "qstat -a " > "~qstat.sh"
ident="${ident/'[]'/[1-6]}"
echo $ident >> "~qstat.sh"
chmod +x "~qstat.sh"
