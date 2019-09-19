/****************************************************************/
/* Includes
/****************************************************************/

#include "val_iter.h"

using namespace ADJZAI001_val_iter;
using namespace std;

int num_states;
float discount_factor;
int converge_count;
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

            while (!temp.eof()) {
                temp >> state;
                temp >> reward;

                transitions.push_back(transition(state-1, reward));
            }

            T.push_back(transitions);
        }
        file.close();
    }
}

float ADJZAI001_val_iter::bellman_value(int state) {
    t_vect transitions = T[state];
    transition max_a = *max_element(transitions.begin(), transitions.end());

    float val = max_a.reward + (discount_factor * V_n[max_a.state]);

    if (V_n[state] == val)  converge_count++;

    return val;
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
                V_np1[i] = bellman_value(i);
                cout << "V[" << i+1 << "] = " << V_np1[i] << endl;
            }
            V_n = V_np1;
            iter++;
            cout << endl;
        }
    }
    else {
        cout << "program call needs to be of format: val_iter [input file name] [output file name] <policy start state> <policy end state>" << endl;
    }
    return 0;
}
