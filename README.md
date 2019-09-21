# VALUE ITERATION ALGORITHM

## How it works:

* This program performs the value iteration algorithm on some given model
* It infers that each state which only can transition back to itself is a terminal state
* After running, the program dislays the model used, the optimal value function and optimal policy
* Optionally, the sequence of steps to take according to optimal policy from start state to end state can be displayed (see below)

## How to run:

* The command format should be:
    ./val_iter [input_file] [output_file] <optimal_policy_start_state> <optimal_policy_end_state>
* If the last 2 arguments are included then the sequence of optimal steps are displayed
* Run 'make' then 'make run' to run command:
    ./val_iter lab_data.txt output.txt 1 3

## Input Data Format:

<< in a text file >>
[number_of_states]
[discount_factor]
<state_1_transition_state> <state_1_transition_state_reward> ...
<state_2_transition_state> <state_2_transition_state_reward> ...
...
