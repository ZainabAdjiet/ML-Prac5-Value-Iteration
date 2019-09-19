#ifndef MAIN_H
#define MAIN_H

/****************************************************************/
/* Includes
/****************************************************************/

#include <iostream>
#include <string>
#include <vector>

/****************************************************************/
/* Functions
/****************************************************************/

namespace ADJZAI001_val_iter {

    struct transition {
        int state_num;
        float reward;

        bool operator<(transition other);
    };

    // data structures used for value iteration algorithm
    std::vector< std::vector<transition> > transitions;
    std::vector<float> V;
    std::vector<int> PI;

    // value iteration methods
    float bellman_value(int state);
    int next_max_state(int state);
}

#endif
