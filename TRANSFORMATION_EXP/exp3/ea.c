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

#include "params.h"
#include "ca.h"
#include "ea.h"

const UINT unit = 100; // to test rates in % (mutation etc.)
const UINT glength = PROG_INTS;

#ifdef DEBUG
const UINT generations = 0;
#else
const UINT generations = GENERATIONS;
#endif

// evolution searches for a vector of integers from the range
// 0 to MODULUS-1, decoding the chromosomes gives the correct
// values using modulo division by CONDITIONS or STATES.
#define MODULUS (CONDITIONS*STATES)

// pointer to fitness function specifies as FITNESS_F in ea_params.h and defined
// in main.c. variants of FITNESS_F are: fitness_replic or fitness_trans.
extern UINT (*fitness)(GA_chromosome *genome);

extern UINT seed;

time_t tstart, tend; 

GA_chromosome best;	// the best individual found so far
UINT best_ever; // fitness value of the best individual

UINT generation;    // generation counter
GA_chromosome *population;
GA_chromosome *next_population;
GA_chromosome pool1[POPSIZE];
GA_chromosome pool2[POPSIZE];

// STANDARD GENETIC ALGORITHM WITH ELITISM.
// YOU CAN TRY TO TURN THE ELITISM OFF...
int GA() // GA_1st_elit
{
    time(&tstart);

    // inicializace
    generation = 0;
    best.fitness = 0;
    best_ever = 0.0;
	GA_chromosome ind1_new, ind2_new;
	UINT _tour = (TOUR >= 2 ? TOUR : 2);
	UINT i1;
    int stop_flag;
    
	for (UINT i = 0; i < POPSIZE; i++)
	{
		initialize(&pool1[i]);
		pool1[i].evaluate = 1;
	}
	// main evolution cycle
	do
	{
		generation++;
		if (generation & 1)
		{
			population = pool1;
			next_population = pool2;
		}
		else
		{
			population = pool2;
			next_population = pool1;
		}
		// evaluate the population
		for (UINT i = 0; i < POPSIZE; i++)
		{
			if (population[i].evaluate)
			{
				population[i].fitness = fitness(&population[i]);
				if (population[i].fitness >= best.fitness)
				    best = population[i];
				population[i].evaluate = 0;
			}
		}
		// elitism
		next_population[0] = best;
        GA_chromosome mutant = best; mutator(&mutant, unit);
		next_population[1] = mutant;
		// create new population
		for (UINT i = 2; i < POPSIZE; i += 2)
		{
			GA_chromosome *ind1 = NULL, *ind2 = NULL;
			// tournament selection
			for (UINT t = 0; t < _tour; t++)
			{
				i1 = urandom(0, POPSIZE - 1);
				if (ind1 == NULL) ind1 = &population[i1];
				else if (ind2 == NULL) ind2 = &population[i1];
				else if (population[i1].fitness > ind1->fitness)
					ind1 = &population[i1];
				else if (population[i1].fitness > ind2->fitness)
					ind2 = &population[i1];
			}
			// crossover - here not applied (PCROSS=0 in ea_params.h)
			if (urandom(0, unit) < PCROSS)
			{
				crossover(ind1, ind2, &ind1_new, &ind2_new);
				ind1_new.evaluate = 1;
				ind2_new.evaluate = 1;
			}
			else	// if not crossed, pass the parents
			{
				ind1_new = *ind1;
				ind2_new = *ind2;
			}
			// mutation
			if (mutator(&ind1_new, PMUT)) ind1_new.evaluate = 1;
			if (mutator(&ind2_new, PMUT)) ind2_new.evaluate = 1;
			// put the offspring to the new population
			next_population[i] = ind1_new;
			next_population[i + 1] = ind2_new;
		}
		stop_flag = stop();
	} while (!stop_flag);
	
	return stop_flag;
}

UINT urandom(UINT low, UINT high)
{
    return rand() % (high - low + 1) + low;
}

void gprint(GA_chromosome *genome)
{
    for (UINT i = 0; i < glength; i++)
        printf("%d ", genome->prog[i]);
    putchar('\n');
}

void initialize(GA_chromosome *genome)
{
    for (UINT i = 0; i < PROG_INTS; i++)
        genome->prog[i] = urandom(0, MODULUS-1);
}

void crossover(GA_chromosome *parent1, GA_chromosome *parent2,
                GA_chromosome *child1, GA_chromosome *child2)
{
    UINT cpoint = urandom(1, PROG_LEN-1);
    cpoint *= INST_LEN;
    
    for (UINT i = 0; i < glength; i++)
    {
        if (i < cpoint)
        {
            child1->prog[i] = parent1->prog[i];
            child2->prog[i] = parent2->prog[i];
        }
        else
        {
            child1->prog[i] = parent2->prog[i];
            child2->prog[i] = parent1->prog[i];
        }
    }
}

BOOL mutator(GA_chromosome *genome, UINT _pmut)
{
    int mutated = 0;
    
    for (UINT i = 0; i < urandom(1, MUTAGENES); i++)
    {
        // s pravdepodobosti _pmut se zmutuje
        // MUTAGENES nahodne vybranych genu
        if (urandom(0, unit) <= _pmut)
        {
            UINT mpoint = urandom(0, PROG_INTS - 1);
            genome->prog[mpoint] = urandom(0, MODULUS-1);
            mutated = 1;
        }
    }

    return mutated;
}

int stop()
{
    if (best.fitness > best_ever)
    {
        best_ever = best.fitness;
#ifdef DEBUG
        printf("Fitness = %d/%d in generation %d\n", 
                   best_ever, MAX_FIT, generation);
//        gprint(&best);
#endif
    }

    if ((best.fitness==MAX_FIT) || (generations>0 && generation==generations))
    {
        time(&tend);
        double diff = difftime(tend,tstart); 
        
        if (best.fitness == MAX_FIT)
        {
            printf("success; generation=%d, duration=%f, ca_rows=%d, ca_cols=%d,\
                    states=%d, steps=%d, seed=%u\n", generation, diff, CA_ROWS,
                    CA_COLS, STATES, CA_STEPS, seed);
            gprint(&best);

            return 1;   // evolution succeeded
        }
        else printf("failed; generation=%d,     fitness=%d/%d, duration=%f\n",
                                    generation,  best_ever, MAX_FIT,    diff);
        
        return -1;  // evolution failed after passing the generation limit
    }
    
    return 0;   // continue to evolve
}
