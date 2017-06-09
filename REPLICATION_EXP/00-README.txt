Bidlo's Cellular Automata System (BiCAS) represents an experimental software
for the evolutionary design of 2D cellular automata (CAs) using the concept
of Conditionally Matching Rules (CMRs) as described in the following paper:

Michal Bidlo: "On Routine Evolution of Complex Cellular Automata," IEEE 
Transactions on Evolutionary Computation, vol. 20, no. 5, 2016, pp. 742-754.
URL: http://ieeexplore.ieee.org/document/7377086/

PLEASE CITE THIS PAPER IN CASE OF ANY FURTHER PUBLICATION INSPIRED BY THIS IDEA.

----------------------------------- Licence ------------------------------------
-   BiCAS is provides under "The BSD 3-Clause License" the content of which    -
-   is distributed in the LICENSE file that is a part of this project.         -
--------------------------------------------------------------------------------

Author:                            Michal Bidlo
                           Brno University of Technology
                         Faculty of Information Technology
                        IT4Innovations Centre of Excellence
                     Bozetechova 2, 61266 Brno, Czech Republic
                    E-mail: see http://www.fit.vutbr.cz/~bidlom


User manual:

The system is provided in a form that allows comfortably running less difficult
experiments on common PCs. However, the user is encouraged to adapt it for
supercomputer clusters if available in order to perform more complex exps.

There are several sample experimental settings provided in directories expX,
where the user can specify various parameters of the system. In these examples
only the number of Conditionally Matching Rules are considered as specified by
the parameter PROG_LEN in file params.h for each expX setup. Results of these
experiments have also been included in order to provide a basic insight into
the working of the system and various exsamples of cellular automata evolved
which fulfil the requirements specified by the appropriate fitness functions.
Please note that obtaining results for more complex tasks is computationally
expensive and the user is advised to start with simpler setups (especially if
the experiments are expected to run on a common PC). For this purpose the user
should modify the 03-run_exps.sh script according to specific PC configuration.


FOR A QUICK START (WITHOUT A NEED TO RUN COMPUTATIONAL EVOLUTIONARY EXPERIMENTS)

There have been some pre-evolved results provided as a part of this project,
in order to examine them, run the following:

./02-make_exps.sh       (will recompile all the sources on your architecture)
./04-get_results.sh     (will create RESULTS.* subdirectories with the results
                         prepared for simulation using BiCAS CA simulator)
Go to a RESULTS.* subdirectory, run BiCAS.py and follow the instructions shown.


The following files (with prefix 00-, 01- etc. indicating the order of user
steps) represent the main interface for the user's interaction with the system.
The following scripts are intended to easily compile, run and gather results of
experiments as described in more detail below.

00-README.txt: this file.

01-ca_config.txt:

a configuration file for specifying the CA behaviour which ought to be achieved
by the evolution. The first line of this file MUST contain three integers X Y Z
whose meaning is the following: X is the number of states the CA should work
with, Y is the minimun number of replicas of the given structure required to
emerge from the initial CA state (if it is achieved within a given generation
limit, the evolutionary run is considered successful) and Z represents the
number of steps of the CA evaluated during the evolution.

Then the initial state of the CA is specified as MxN states of each cell of CA.
The last part of this file (separated by a single blank line) differs with
respect to a specific setup of the fitness evaluation which the system offers:

(A) The evolution of replication in CA as described in Section IV of the paper,
specifically for the round loop (results in Section IV-A) and the rectangular
loop (with results in Section IV-B). In this case the last part of the 
01-ca_config.txt specifies just the structure to be replicated (including 
separating "empty" cells). For example:
0 0 0 0 0 0
0 4 1 1 3 0
0 1 0 0 1 0
0 1 0 0 1 0
0 4 1 1 2 0
0 0 0 0 0 0
This structure is assumed to be a part of the CA initial state and its replicas
are expected in a REGULAR GRID with respect to the initial structure. Let S be
the structure to be replicated and R be a subset of cells in the regular grid.
Then the following scheme shows the areas in the CA where the replicas of S
are to be evaluated by the fitness function (see also the example of the CA
initial state provided in the file 01-ca_config.txt):
R R R R
R S R R
R R R R
R R R R
If the exact pattern S is detected in any part R in at least Y instances (copies
including the S itself), then the evolution is finished successfully.

The aforementioned setup is provided as a fully working example of experiment
in REPLICATION_EXP directory. The key feature of this experiment is the use of
fitness_replic() function specified using a parameter FITNESS_F in ea_params.h
and implemented in main.c.

(B) The evolution of CA based on a general pattern transformation problem.
This setup requires to specify an initial CA state and a target state of the
same size in the file 01-ca_config.txt which ought to be reached from the
initial state. The idea of this experiment is described in the aforementioned
paper in Section IV-C and a fully working example of this experiment is provided
in TRANSFORMATION_EXP directory. The key feature of this experiment is the use
of fitness_trans() function specified using a parameter FITNESS_F in ea_params.h
and implemented in main.c.

02-make_exps.sh

After specifying the CA configuration, run 02-make_exps.sh script to build the
experiments.

03-run_exps.sh

The next step after a successful build is to run the experiment. This script is
prepared in order to allow comfortably running some independent runs using
a common multicore PC. All results discovered by the evolution are stored to
files which are then processed during the next step using 04-get_results.sh.

03-run_parallel.sh

Use this script instead of 03-run_exps.sh ONLY IF YOU HAVE A CUPERCOMPUTER
CLUSTER AVAILABLE. This will submit batches of jobs of the given experiments
to be executed in parallel as independent processes using multi-core and/or
distributed environment. In order to do so, you must adapt the ~submit.sh
script to your cluster environment and (maybe) also expX/~qsub.sh. These
scripts have been prepared for Portable Batch System (PBS, specifically PBS
Professional 13.0 by Altair | PBS Works (tm)). PBS is commonly used on many
supercomputer clusters. Please consult your admin and/or documentation for
your system.

04-get_results.sh

After finishing the evolutionary runs the results may be summarised using this
script. It ensures converting the CMR result to files containing conventional
table rules that may be simulated using BiCAS - Bidlo's Cellular Automata
Simulator - which is a part of this system. Please note that the conversion of
the results may be quiet time-consuming (especially for larger CA and/or more
results to process) so be patient and let the script finish. If some successful
results were obtained, there are copied into newly created directories denoted
RESULTS.* for each experiment together with the BiCAS simulator and the
appropriate CA statefile. To run the CA simulation, go to a RESULTS.* directory,
run BiCAS.py and follow the on-screen instructions.

BiCAS.py (Bidlo's Cellular Automata Simulator), included in the directory of the
same name, is a simple Python-based 2D cellular automata simulator by means of
which the results of experiments can be visualised and simulated. BiCAS provides
a sample transition function (BIDLO.tab) that shows a moving pattern development
problem within a uniform 2D CA. In order to experience BiCAS, go to the BiCAS.py
directory, run BiCAS.py and follow the on-screen instructions.

05-del_results.sh, 05-make_clean.sh

Before compiling and running other experiments, old results and/or all files
produced by compiling the previous experiments may be deleted by these scripts.
