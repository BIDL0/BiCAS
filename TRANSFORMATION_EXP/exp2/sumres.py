#!/usr/bin/env python

import os, sys

resfile = "resfile.txt"
sumname = "cmrfile.txt"

def sum_results():
    cmd = "cat ./EXP.* > ./" + resfile
    os.system(cmd)
    cmd = "cat ./" + resfile + " | grep -v succ | grep -v fail \
                                 | grep -v kill > ./" + sumname
    os.system(cmd)
    succ = 0
    try:
        caf = open(resfile, "r")
    except IOError:
        print "Unable to open " + resfile
        exit(1)
    with caf:
        for line in caf:
            if "success" in line:
                succ = succ + 1
        if succ > 0:
            print str(succ) + " results in " + os.path.split(os.getcwd())[1]
        caf.close()

    return succ

def main():

    succ = sum_results()
    if succ > 0:
        sys.exit(0) # ok
    else:
        sys.exit(1) # no results found, nothing to process further

if __name__ == "__main__":
    main()
