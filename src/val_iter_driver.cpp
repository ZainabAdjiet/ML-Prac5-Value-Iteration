/****************************************************************/
/* Includes
/****************************************************************/

#include "val_iter.h"

using namespace ADJZAI001_val_iter;
using namespace std;

int num_states;
float discount_factor;
int converge_count;

vector<int> terminal;
vector<t_vect> T;  // transitions
vector<float> V_n;   // values for states
vector<float> V_np1;
vector<int> PI;    // optimal policy

/****************************************************************/
/* Value iteration Functions
/****************************************************************/

void ADJZAI001_val_iter::load_model(std::string filename) {
    ifstream file(filename);
    string line;
    int state;
    float reward;

    if (file.is_open()) {
        getline(file, line);
        istringstream states(line);
        states >> num_states;

        getline(file, line);
        istringstream discount(line);
        discount >> discount_factor;

        for (int i = 0; i < num_states; ++i) {
            getline(file, line);
            istringstream temp(line);

            t_vect transitions;
            bool self_transition = false;

            while (!temp.eof()) {
                temp >> state;
                temp >> reward;
                if (i == state-1) self_transition = true;

                transitions.push_back(transition(state-1, reward));
            }

            if (self_transition && transitions.size() == 1)
                terminal.push_back(i);
            T.push_back(transitions);
        }
        file.close();
    }
}

float ADJZAI001_val_iter::bellman(int state) {
    t_vect transitions = T[state];
    float max_val = -1;

    for (int j = 0; j < transitions.size(); ++j) {
        transition action = transitions[j];
        float val = action.reward + (discount_factor * V_n[action.state]);
        if (val > max_val)  max_val = val;
    }

    if (abs(V_n[state] - max_val) < 0.00001)  converge_count++;

    return max_val;
}

int ADJZAI001_val_iter::policy(int state) {
    t_vect transitions = T[state];
    int max_state = -1;
    float val = -1;
    bool found_higher_val = false;
    bool found_terminal = false;

    for (int j = 0; j < transitions.size(); ++j) {
        int state_t = transitions[j].state;
        if (find(terminal.begin(), terminal.end(), state_t) != terminal.end()) {
            if (!found_higher_val) {
                found_terminal = true;
                max_state = state_t;
            }
        }
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
    
    if (argc > 1) {

        string input = string(argv[1]);
        load_model(input);

        V_n.resize(num_states, 0);
        V_np1.resize(num_states);

        int iter = 1;

        while (converge_count < num_states) {
            converge_count = 0;
            cout << "iteration " << iter << endl;

            for (int i = 0; i < num_states; ++i) {
                V_np1[i] = bellman(i);
                cout << "V[" << i+1 << "] = " << V_np1[i] << endl;
            }
            V_n = V_np1;
            iter++;
            cout << endl;
        }

        for (int i = 0; i < num_states; ++i) {
            PI.push_back(policy(i));
            cout << "PI[" << i+1 << "] = " << PI[i] + 1 << endl;
        }
    }
    else {
        cout << "program call needs to be of format: val_iter [input file name] [output file name] <policy start state> <policy end state>" << endl;
    }
    return 0;
}
