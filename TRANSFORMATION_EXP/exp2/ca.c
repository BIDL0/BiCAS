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
#include <string.h>

#include "ca.h"
#include "ea.h"

// kazda bunka CZ nabyva hodnot stavu (0..STATES-1) a hodnoty STATES, ktera
// zachovava stav dane bunky stejny jako byl v predchazejicim kroku.
// efektivni rozmer CA je dan konstantou CA_DIM, + 2 bunky nulovych okr. podm.
CA ca1 = {0}, tca1 = {0};
CA *cap = &ca1, *tcap = &tca1;

void ca_init(int pattern[][CA_COLS])
{
    for (UINT r = 0; r < CA_ROWS + 2; r++)
        for (UINT c = 0; c < CA_COLS + 2; c++)
            (*cap)[r][c] = r >= 1 && r <= CA_ROWS &&
                           c >= 1 && c <= CA_COLS ? pattern[r - 1][c - 1] : 0;
}

// duplikuje stav CA do dst
void ca_dup(CA dst)
{
    for (UINT i = 1; i <= CA_ROWS; i++)
        for (UINT j = 1; j <= CA_COLS; j++)
            dst[i][j] = (*cap)[i][j];
}

// zjisti shodnost stavu aktualniho CA a dst ve vsech bunkach
int ca_cmp(CA dst)
{
    for (UINT i = 1; i <= CA_ROWS; i++)
        for (UINT j = 1; j <= CA_COLS; j++)
            if (dst[i][j] != (*cap)[i][j])
                return 0;
    
    return 1;
}

void ca_print()
{
    for (UINT r = 1; r <= CA_ROWS; r++)
    {
        for (UINT s = 1; s <= CA_COLS; s++)
            if ((*cap)[r][s] == 0) printf(". ");
            else printf("%d ", (*cap)[r][s]);
        putchar('\n');
    }
    
    for (UINT i = 0; i < TERM_HEIGHT - CA_ROWS; i++)
        putchar('\n');
}

// vykona program, ktery z aktualni kopie sousedstvi vypocita novy stav bunky
UINT next_state(UINT row, UINT col, PROG prog)
{
//    printf("next in\n");
    //    N
    //  W C  E
    //    S
    // vytvorime kopii Moorova 9-okoli
    UINT neigh[5];  //          [0]-N
                    // [1]- W   [2]-C   [3]-E
                    //          [4]-S
#ifdef SDCYC
    // cyclic boundary conditions if needed
    neigh[0] = row-1 > 0 ? (*cap)[row-1][col] : (*cap)[CA_ROWS][col];
    neigh[1] = col-1 > 0 ? (*cap)[row][col-1] : (*cap)[row][CA_COLS];
    neigh[2] = (*cap)[row][col];
    neigh[3] = col+1 <= CA_COLS ? (*cap)[row][col+1] : (*cap)[row][1];
    neigh[4] = row+1 <= CA_ROWS ? (*cap)[row+1][col] : (*cap)[1][col];
#else
    neigh[0] = (*cap)[row-1][col];
    neigh[1] = (*cap)[row][col-1];
    neigh[2] = (*cap)[row][col];
    neigh[3] = (*cap)[row][col+1];
    neigh[4] = (*cap)[row+1][col];
#endif

    // evaluate RMRs from a chromosome (array prog[])
    for (UINT i = 0; i < PROG_INTS; i += INST_LEN)
    {
        int match = 1;
        for (UINT e = 0; match && e < 5; e++) // for 5-neighbourhood
        {
            // HERE THE CONDITIONS ARE EVALUATED AND "MATCHED" - SEE THE PAPER
            if (prog[i+e] < STATES)
                match = (neigh[e] == 0);
            else if (prog[i+e] < 2*STATES)
                match = (neigh[e] != 0);
            else if (prog[i+e] < 3*STATES)
                match = (neigh[e] <= (prog[i+e]%STATES));
            else if (prog[i+e] < 4*STATES)
                match = (neigh[e] >= (prog[i+e]%STATES));
            else
//                match = 1; // I recommend not to use this,
                // otherwise comment out the following { }
                // AND THE SAME MUST BE DONE IN THE FUNCTION next_state_gt BELOW!!!
                { printf("Something is wrong with the CMRs!\n"); exit(1); }
        }
        if (match) // rule matched, return its new state
            return (prog[i+5] % STATES);
    }
    // if no rule matched, then return the old cell state
    return neigh[2];
}

void ca_step(PROG prog)
{
    static CA *tmp;
    
    for (UINT r = 1; r <= CA_ROWS; r++)
        for (UINT s = 1; s <= CA_COLS; s++)
            (*tcap)[r][s] = next_state(r, s, prog);

    tmp = cap; cap = tcap; tcap = tmp;
}

void ca_develop(PROG prog, UINT steps)
{
    for (UINT s = 0; s < steps; s++)
    {
        ca_step(prog);
        ca_print();
    }
}

FILE *tabf; // soubor pro zpracovani tabulkove prechodove funkce
UINT dcount; // pocet pravidel tabulkove prechodove funkce s duplicitami
UINT rcount; // pocet pravidel bez duplicit

// vvv blok generovani tabulkove fitness pro 2DCA vvv
// vv funkce pro von Neumanovo 5-okoli vv
// vykona program, ktery z aktualni kopie sousedstvi vypocita novy stav bunky
UINT next_state_gt(UINT row, UINT col, PROG prog)
{
//    printf("next in\n");
    //    N
    //  W C  E
    //    S
    // vytvorime kopii von Neumanova 5-okoli
    UINT neigh[5];  //          [0]-N
                    // [1]- W   [2]-C   [3]-E
                    //          [4]-S
#ifdef SDCYC
    // cyklicne okrajove podminky - pouze pro 5-okoli !!!
    neigh[0] = row-1 > 0 ? (*cap)[row-1][col] : (*cap)[CA_ROWS][col];
    neigh[1] = col-1 > 0 ? (*cap)[row][col-1] : (*cap)[row][CA_COLS];
    neigh[2] = (*cap)[row][col];
    neigh[3] = col+1 <= CA_COLS ? (*cap)[row][col+1] : (*cap)[row][1];
    neigh[4] = row+1 <= CA_ROWS ? (*cap)[row+1][col] : (*cap)[1][col];
#else
    neigh[0] = (*cap)[row-1][col];
    neigh[1] = (*cap)[row][col-1];
    neigh[2] = (*cap)[row][col];
    neigh[3] = (*cap)[row][col+1];
    neigh[4] = (*cap)[row+1][col];
#endif
    
    // evaluate RMRs from a chromosome (array prog[])
    for (UINT i = 0; i < PROG_INTS; i += INST_LEN)
    {
        int match = 1;
        for (UINT e = 0; match && e < 5; e++) // for 5-neighbourhood
        {
            // HERE THE CONDITIONS ARE EVALUATED AND "MATCHED" - SEE THE PAPER
            if (prog[i+e] < STATES)
                match = (neigh[e] == 0);
            else if (prog[i+e] < 2*STATES)
                match = (neigh[e] != 0);
            else if (prog[i+e] < 3*STATES)
                match = (neigh[e] <= (prog[i+e]%STATES));
            else if (prog[i+e] < 4*STATES)
                match = (neigh[e] >= (prog[i+e]%STATES));
            else
//                match = 1; // I recommend not to use this,
                // otherwise comment out the following { }
                { printf("V CMR neco blbe!\n"); exit(1); }
        }
        if (match) // generating table rules from matching CMRs
        {
            // only rules modifying the cell state are generated
            if (neigh[2] != prog[i+5] % STATES)
            {
                for (UINT q = 0; q < 5; q++)    // for 5-neighbourhood
                    fprintf(tabf, "%d ", neigh[q]);
                fprintf(tabf, "%d\n", prog[i+5] % STATES);
                dcount++;
            }
            return (prog[i+5] % STATES);
        }
    }
    return neigh[2];
}

void ca_step_gt(PROG prog)
{
    static CA *tmp;
    
    for (UINT r = 1; r <= CA_ROWS; r++)
        for (UINT s = 1; s <= CA_COLS; s++)
            (*tcap)[r][s] = next_state_gt(r, s, prog);
    
    tmp = cap; cap = tcap; tcap = tmp;
}

// to sort the table rules according to the combinations of neighbourhood states
static int compare_str(const void * a, const void * b)
{
    return strcmp(*(const char **) a, *(const char **) b);
}

void ca_devel_gt(PROG prog, UINT steps, UINT id)
{
    char fname[16];
    UINT s; // index

    // here the table rules with possible duplicities are generated...
    sprintf(fname, "%02d.tab", id);
    tabf = fopen(fname, "w");
    fprintf(tabf, "%d\n", STATES);
    dcount = 0;
    for (s = 0; s < steps; s++)
        ca_step_gt(prog);
    fclose(tabf);

    // allocate array of strings for the generated table rules
    char** rlist = (char**)malloc(dcount * sizeof(char*));
    for(s = 0; s < dcount; s++)
        rlist[s] = (char*) malloc(40); // 40 MUST be enough

    // read the rules from a file
    s = 0; // counter
    tabf = fopen(fname, "r");
    rlist[s] = fgets(rlist[s], 40, tabf); // head is discarded
    while (s < dcount && (rlist[s] = fgets(rlist[s], 40, tabf)) != NULL)
        s++;
    fclose(tabf);

    // sort the rules and write them to a file, NO DUPLICITIES
    qsort(rlist, dcount, sizeof (const char *), compare_str);
    int eq;
    tabf = fopen(fname, "w");
    fprintf(tabf, "%d\n", STATES);
    for(s = 0; s < dcount-1; s++)
        // eliminate duplicities
        if ((eq = strcmp(rlist[s], rlist[s+1])) != 0)
            fprintf(tabf, "%s", rlist[s]);
    fprintf(tabf, "%s", rlist[dcount-1]); // the last rule must always be written
    fclose(tabf);
}

// calculates base**exp
UINT ipow(UINT base, UINT exp)
{
    UINT result = 1;
    while (exp)
    {
        if (exp & 1)
            result *= base;
        exp >>= 1;
        base *= base;
    }

    return result;
}
