#ifndef MAIN_H
#define MAIN_H

/****************************************************************/
/* Includes
/****************************************************************/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>

/****************************************************************/
/* Functions
/****************************************************************/

namespace ADJZAI001_val_iter {

    struct transition {
        int state;
        float reward;

        transition(int s, float r);
        bool operator<(transition other);
    };

    typedef std::vector<transition> t_vect;

    int num_states;
    float discount_factor;
    int converge_count;

    std::vector<int> terminal;
    std::vector<t_vect> T;  // transitions
    std::vector<float> V_n;   // values for states
    std::vector<float> V_np1;
    std::vector<int> PI;    // optimal policy

    // value iteration methods
    void load_model(std::string filename);
    int max_action(int state);
    float bellman(int state);
    int policy(int state);
}

#endif
