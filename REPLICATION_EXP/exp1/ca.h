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

#ifndef CA_H
#define CA_H

#include "params.h"

// ------------------------Makra pro implementaci CA------------------------
// the number of integers of a chromosome is given by the number of elements
// of a CMR times the number of CMRs in a chromsome.
#define PROG_INTS (INST_LEN * PROG_LEN)

typedef unsigned int UINT;
typedef unsigned char UCHAR;
typedef int BOOL;

typedef UINT PROG[PROG_INTS];  // a type for a GA chromosome with CMRs

// +2 is here for the possibility to easily use zero boundary conditions
typedef UINT CA[CA_ROWS + 2][CA_COLS + 2]; // 2DCA s nulovymi okrajovymi podminkami

void ca_init(int pattern[][CA_COLS]);
UINT next_state(UINT row, UINT col, PROG prog);

void ca_dup(CA dst);
int ca_cmp(CA dst);
void ca_print();
void ca_step(PROG prog);
void ca_develop(PROG prog, UINT steps);
void ca_devel_gt(PROG prog, UINT steps, UINT id);

UINT ipow(UINT base, UINT exp);

#endif
