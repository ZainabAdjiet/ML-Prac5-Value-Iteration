/****************************************************************/
/* Includes
/****************************************************************/

#include "val_iter.h"

using namespace ADJZAI001_val_iter;
using namespace std;

/****************************************************************/
/* Transition Functions
/****************************************************************/

transition::transition(int s, float r) : state(s), reward(r) {}

bool transition::operator<(transition other) {
    return this->reward < other.reward;
}

/****************************************************************/
/* Value iteration Functions
/****************************************************************/

/*
* returns if state is terminal state
*/
bool ADJZAI001_val_iter::is_terminal(int state) {
    return find(terminal.begin(), terminal.end(), state) != terminal.end();
}

/*
* loads the model states and action rewards from file
*/
bool ADJZAI001_val_iter::load_model(std::string filename) {
    ifstream file(filename);
    string line;    // hold line in file
    int state;
    float reward;

    if (file.is_open()) {
        // get number of states
        getline(file, line);
        istringstream states(line);
        states >> num_states;

        // get discount factor
        getline(file, line);
        istringstream discount(line);
        discount >> discount_factor;

        // read action rewards in order of states
        for (int i = 0; i < num_states; ++i) {
            getline(file, line);
            istringstream temp(line);

            t_vect transitions;
            bool self_transition = false; // to detect terminal state

            while (!temp.eof()) {
                temp >> state;
                temp >> reward;
                if (i == state-1) self_transition = true;

                transitions.push_back(transition(state-1, reward));
            }

            // if terminal state, add to list of terminal states
            if (self_transition && transitions.size() == 1)
                terminal.push_back(i);
            // add transitions with rewards for state by index
            T.push_back(transitions);
        }

        file.close();
        return true;
    }
    else {
        cout << "could not open " << filename << endl; 
        return false;
    }  
}

/*
* calculates V_{n+1} value using Bellman equation for given state
*/
float ADJZAI001_val_iter::bellman(int state) {
    t_vect transitions = T[state];
    float max_val = -1;

    // get max value of all actions
    for (int j = 0; j < transitions.size(); ++j) {
        transition action = transitions[j];
        float val = action.reward + (discount_factor * V_n[action.state]);
        if (val > max_val)  max_val = val;
    }

    if (abs(V_n[state] - max_val) < 0.00001)  converge_count++; // used to detect convergence

    return max_val;
}

/*
* returns the optimal policy for given state
* optimal policy selected with precedence:
* 1. State has higher value than current state and max value of all actions
* 2. State is terminal
* 3. State has lower value or equal value to current and max value of all actions
*/
int ADJZAI001_val_iter::policy(int state) {
    t_vect transitions = T[state];
    int max_state = -1;
    float val = -1;
    bool found_higher_val = false;  // detect if there is a higher value alternate path
    bool found_terminal = false;    // detect if terminal state can be reached

    for (int j = 0; j < transitions.size(); ++j) {
        int state_t = transitions[j].state;
        // if transition state is terminal and no higher value alternate path, go to terminal
        if (is_terminal(state_t)) {
            if (!found_higher_val) {
                found_terminal = true;
                max_state = state_t;
            }
        }
        /* if transition state value is max so far, only take this path if
        * terminal state cannot be reached from this state, or
        * if a terminal state can be reached, only take path if it has higher value than current state */
        if (V_np1[state_t] > val) {
            if (V_np1[state_t] > V_np1[state])
                found_higher_val = true;

            if (!found_terminal || found_higher_val) {
                max_state = state_t;
                val = V_np1[state_t];
            }
        }
    }

    return max_state;
}

/****************************************************************/
/* Main Program
/****************************************************************/

int main(int argc, char const *argv[]) {
    
    if (argc > 2) {

        string input = string(argv[1]);
        string output = string(argv[2]);
        if (!load_model(input))
            return 0;
        ofstream file_out(output);

        if (file_out.is_open()) {
            // display model

            file_out << "Number of states: " << num_states << endl;
            file_out << "Discount factor: " << discount_factor << endl;
            file_out << "Reward function and Available actions: " << endl;

            for (int i = 0; i < num_states; ++i) {
                t_vect transitions = T[i];
                for (int j = 0; j < transitions.size(); ++j) {
                    file_out << "S" << i+1 << " -> S" << transitions[j].state+1 << " = " << transitions[j].reward << endl;
                }
            }

            // intialise state values to 0
            V_n.resize(num_states, 0);
            V_np1.resize(num_states);

            int iter = 0;

            // perform Bellman calculation for each state until convergence
            while (converge_count < num_states) {
                iter++;
                converge_count = 0;

                for (int i = 0; i < num_states; ++i) {
                    V_np1[i] = bellman(i);
                }
                V_n = V_np1;
            }

            // deduce optimal policy for each state with calculated values
            for (int i = 0; i < num_states; ++i) {
                PI.push_back(policy(i));
            }

            // display output

            file_out << endl << "Iterations to converge: " << iter << endl;

            file_out  << endl << "Optimal values:" << endl;

            for (int i = 0; i < num_states; ++i) {
                file_out << "V*[" << i+1 << "] = " << V_np1[i] << endl;
            }

            file_out  << endl << "Optimal policies:" << endl;

            for (int i = 0; i < num_states; ++i) {
                file_out << "PI[" << i+1 << "] = " << PI[i]+1 << endl;
            }

            if (argc == 5) {
                int start;
                istringstream st(argv[3]);
                st >> start;

                int end;
                istringstream en(argv[4]);
                en >> end;

                start -= 1;
                end -= 1;

                if (is_terminal(end)) {
                    file_out << endl << "Optimal policy for " << start+1 << " to " << end+1 << ": ";
                    int state = start;
                    do {
                        file_out << state+1 << " ";
                        state = PI[state];
                    } while (state != end);
                    file_out << end+1 << endl;
                }
                else {
                    cout << endl << "end state needs to be a terminal state" << endl;
                }
            }
            else if (argc != 3 && argc != 5)
                cout << "program call needs to be of format: val_iter [input file name] [output file name] <policy start state> <policy end state>" << endl;
        }
        else {
            cout << "could not write to " << output << endl;
        }
    }
    else {
        cout << "program call needs to be of format: val_iter [input file name] [output file name] <policy start state> <policy end state>" << endl;
    }
    return 0;
}
