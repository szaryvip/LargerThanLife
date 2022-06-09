#ifndef RANDOM_H
#define RANDOM_H
#include <random>
#include <vector>
#include <chrono>
#include <map>

using namespace std;

/* Generates vector of ints with numbers of necessary alive neighbours
for birth or survival of the cell. Vector can have from
1 to 10 elements with numbers from 0 to 9 */
vector<int> generate_birth_survive_cond();

/* Generates number of states from 2 to 255*/
int generate_number_of_states();

/* Generates range of neighbourhood from 1 to 10 */
int generate_range();

/* Generates if middle is included in neighbourhood calculations */
bool generate_middle_included();

/* Generates type of neighbourhood where m is Moore type and n is
Neumann type */
char generate_neighbourhood();

#endif
