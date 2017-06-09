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

#ifndef CA_PARAMS_H
#define CA_PARAMS_H

// cyclic boundary conditions - usually does not matter!
#define SDCYC
// don't care state - not used
#define X -1

#include "../states.h"
#include "../sca_params.h"

// just a virtual constant identifying the maximal fitness.
// in this experiment the maximal fitness is given if the CA
// fulfils a condition -- here the emergence of a given number
// of replicas specified by the second number on the first line
// in the file ../ca_from_to.txt.
#define MAX_FIT (~0)

#endif
