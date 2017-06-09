// BiCAS - Bidlo's Cellular Automata System
//
// --------------------------------- Licence -----------------------------------
// BiCAS is provides under "The BSD 3-Clause License" the content of which is
// distributed within the LICENSE file that is a part of this project.
// -----------------------------------------------------------------------------
//
// Author:                           Michal Bidlo
//                          Brno University of Technology
//                        Faculty of Information Technology
//                       IT4Innovations Centre of Excellence
//                    Bozetechova 2, Brno 61266, Czech Republic
//                   E-mail: see http://www.fit.vutbr.cz/~bidlom
//
// -----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <unistd.h>

#include "params.h"
#include "ca.h"
#include "ea.h"

extern CA *cap;
CA ca_copy;

extern UINT generation;
extern GA_chromosome best;

// initial pattern and target pattern (if applicable)
int pattern[CA_ROWS][CA_COLS] = { PATTERN };
int targetp[CA_ROWS][CA_COLS] = { TARGETP };

UINT (*fitness)(GA_chromosome *genome); // assigned in main()

// This fitness function consideres a structure which is the subject of
// replication. The replicas are evaluated in a regular grid with respect
// to the location of the initial structure. The main criterion for
// a successful evolution if to achieve a minimum given number of copies
// of the structure inside the grid.
UINT fitness_replic(GA_chromosome *genome)
{
    UINT fit = 0;
    ca_init(pattern);

    for (int s = 0; s < CA_STEPS; s++)
    {
        ca_step(genome->prog);
        if (s < 8)
            continue;

        UINT pfit = 0;
        UINT replics = 0;
        for (int rg = 1; rg < CA_ROWS; rg += RHEIGHT-1)
        {
            for (int cg = 1; cg < CA_COLS; cg += RWIDTH-1)
            {
                UINT rfit = 0;
                for (int r = 0; r < RHEIGHT; r++)
                {
                    for (int c = 0; c < RWIDTH; c++)
                    {
                        if (// targetp[r][c] == X ||
                            (*cap)[rg+r][cg+c] == targetp[r][c]) rfit++;
                    }
                }
                if (rfit == RWIDTH*RHEIGHT)
                {
                    replics++;
                    pfit += rfit*rfit;
                }
                else pfit += rfit;
            }
        }
        if (replics >= REPLICS_REQ)
        {
            fit = MAX_FIT;
            break;
        }
        else if (pfit > fit) fit = pfit;
    }

    return fit;
}

// This fitness function evaluates reaching a specific given target state from
// an initial CA state which must be of the same size. It is a case of a general
// pattern transformation problem, i.e. from a given initial state the goal is
// to search for CA rules that transform it onto a given target state. The main
// criterion for a successful evolution is the exact coincidence of all cell
// states with the target pattern.
UINT fitness_trans(GA_chromosome *genome)
{
    UINT fit = 0;
    ca_init(pattern);
    for (int s = 1; s <= CA_STEPS; s++)
    {
        ca_step(genome->prog);
        // to save some computation time: it is improbable that the pattern is
        // created after a few initial CA steps, therefore some of them are not
        // evaluated. DEPENDS ON THE PATTERN COMPLEXITY.
        if (s <= 8) continue;

        UINT pfit = 0;
        for (int r = 1; r <= CA_ROWS; r++)
        {
            for (int c = 1; c <= CA_COLS; c++)
                if (// targetp[r-1][c-1] == X ||
                    (*cap)[r][c] == targetp[r-1][c-1]) pfit++;
        }

        if (pfit > fit) fit = pfit;
    }

    return fit < CA_ROWS*CA_COLS ? fit : MAX_FIT;
}

// transformation of the evolced CMRs to the conventional table rules.
// for each row in a file containing a CMR-based transition function of a CA
// a .tab file is generated with the corresponding table rules.
void fread_cmrs_conv_tab(char *fname)
{
    UINT tmp;
    UINT pnum = 0;
    char pstr[6];
    FILE *file = fopen(fname, "r");
    if (file == NULL)
    {
        printf("Unable to open CMR file: %s\n", fname);
        exit(1);
    }

    while (!feof(file))
    {
        UINT cnt = 0;
        while (cnt < PROG_INTS && fscanf(file, "%d", &tmp) != EOF)
        {
            best.prog[cnt] = tmp;
            cnt++;
        }

        if (cnt < PROG_INTS)
        {
            if (cnt > 0)
            {
                printf("Error reading CMR integerss file, cnt = %d\n", cnt);
                exit(0);
            }
            else break; // end of file
        }

        sprintf(pstr, "%02d", pnum);
        ca_init(pattern);
        ca_devel_gt(best.prog, CA_STEPS*CA_STEPS, pnum);
        pnum++;
    }
    fclose(file);
}

UINT seed;
UINT rand_init()
{
    unsigned int seed;
    FILE* fp = fopen("/dev/urandom","rb");
    if (fp == NULL) fp = fopen("/dev/random","rb");
    if (fp != NULL) {
       //printf("Init from /dev/{u}random\n");
       int dummy = fread(&seed, sizeof(seed), 1, fp);
       fclose(fp);
    } else {
       seed = (unsigned int)((unsigned) time(NULL) + getpid());
    }
    srand(seed);
    return seed;
}

int main(int argc, char *argv[])
{

    // file name with evolved CMRs --> generate table rules
    if (argc == 2)
    {
        fread_cmrs_conv_tab(argv[1]);
        return 0;
    }

    // perform the evolution experiment
    fitness = FITNESS_F;
    seed = rand_init();
    GA();

    return 0;
}
