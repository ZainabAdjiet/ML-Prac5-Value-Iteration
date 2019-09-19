/****************************************************************/
/* Includes
/****************************************************************/

#include "val_iter.h"

using namespace ADJZAI001_val_iter;
using namespace std;

// data structures used for value iteration algorithm
std::vector<t_vect> T;  // transitions
std::vector<float> V;   // values for states
std::vector<int> PI;    // optimal policy

/****************************************************************/
/* Value iteration Functions
/****************************************************************/

void ADJZAI001_val_iter::load_model(std::string filename) {
    ifstream file(filename);
    string line;
    int num_states, state;
    float reward;

    if (file.is_open()) {
        getline(file, line);
        istringstream iss(line);
        iss >> num_states;

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

// float ADJZAI001_val_iter::bellman_value(int state) {
//     t_vect state_ts = transitions[state];
//     transition max_a = *max_element(state_ts.begin(), state_ts.end());
// }

/****************************************************************/
/* Main Program
/****************************************************************/

int main(int argc, char const *argv[]) {
    
    if (argc > 1) {

        string input = string(argv[1]);
        load_model(input);

        for (int i = 0; i < T.size(); ++i) {
            cout << "state " << i+1 << endl;
            for (int j = 0; j < T[i].size(); ++j) {
                cout << "to " << T[i][j].state + 1 << " R: " << T[i][j].reward << endl;
            }
        }
    }
    else {
        cout << "program call needs to be of format: val_iter [input file name] [output file name] <policy start state> <policy end state>" << endl;
    }
    return 0;
}
