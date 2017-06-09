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

#ifndef EA_PARAMS_H
#define EA_PARAMS_H

// to see the evolution progress on stdout, use DEBUG.
// Do not use DEBUG to run final experiments, it does not
// limit the maximal number of evolutionary generations.
// #define DEBUG

// ---------------------- PARAMETRY EVOLUCNIHO ALGORITMU ----------------------
#define PMUT 100
#define MUTAGENES 4
#define PCROSS 0
#define TOUR 4
#define POPSIZE 8
#define GENERATIONS 3000000
#define FITNESS_F fitness_trans

#define TERM_HEIGHT 23

#endif

