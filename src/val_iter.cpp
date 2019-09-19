/****************************************************************/
/* Includes
/****************************************************************/

#include "val_iter.h"

using namespace ADJZAI001_val_iter;

/****************************************************************/
/* Transition Functions
/****************************************************************/

transition::transition(int s, float r) : state(s), reward(r) {}

bool transition::operator<(transition other) {
    return this->reward < other.reward;
}