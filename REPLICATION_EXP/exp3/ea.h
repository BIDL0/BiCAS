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

#ifndef EA_H
#define EA_H

typedef struct {
    PROG prog;
    UINT fitness;
    BOOL evaluate;
    
    UINT step;
} GA_chromosome;

UINT urandom(UINT low, UINT high);

void initialize(GA_chromosome *g);
void crossover(GA_chromosome *parent1, GA_chromosome *parent2, 
                GA_chromosome *child1, GA_chromosome *child2);
BOOL mutator(GA_chromosome *genome, UINT _pmut);
extern UINT fitness_fn(GA_chromosome *g);

void gprint(GA_chromosome *genome);
int GA(); // 1st_elit
int stop();

extern UINT (*fitness)(GA_chromosome*);

#endif
