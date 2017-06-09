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

#ifndef PARAMS_H
#define PARAMS_H

// the number of conditions considered for CMRs
#define CONDITIONS 4
// the number of elements - integers of a CMR: 5-neighbourhood + new state = 6
#define INST_LEN 6
// the number of CMRs in a chromosome of the EA
#define PROG_LEN 10

#include "ca_params.h"
#include "ea_params.h"

#endif

