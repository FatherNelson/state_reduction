/*Grant James Udstrand
 * 5262811
 * */

#include <iostream>
#include "cstdint";
using namespace std;
/*
 * ' ' is put into the implication table whenver and implication is eliminated
 */
//Set the size of the table in rows here unless someone else can make this have dynamically sized arrays
//This was revised from a define statement because it didn't work below since #define works as a text substitution
//static const is a integer value that will be passed wherever SIZE is asked for. Can be set to any max,
//it just makes sense to keep small to optimize speed
static const int SIZE = 9;

class Row{
private:
    //The name of the state for this row
    char state_name;
    //The state when X = 0
    char state0;
    //The state when X = 1
    char state1;
    //The output of the function (Z)
    int output;
public:
    //Default row constructor
    Row(){
        state_name = 'A';
        state0 = 'C';
        state1 = 'F';
        output = 1;
    }
    //What should be used when we want to create a new row
    Row(char start, char state_zero, char state_one, int out){
        state_name = start;
        state0 = state_zero;
        state1 = state_one;
        output = out;
    }
    //function to display the individual rows
    void disp(){
        cout << state_name << " " << state0 << " " << state1 << " " << output << " " ;
    }
    char getStateName(){
        return state_name;
    };
    char getStateZero(){
        return state0;
    };
    char getStateOne(){
        return state1;
    }
    char getOut(){
        return output;
    }
};

//The table is composed of rows. A performance tweak may be to take the array strategy that is present here and
//instead replace it by two dimensional arrays. For the time being this works for building the table
class Table{
private:
    int i = 0;
    //The number of rows in the table
    int rows = 0;
    //The array which holds the first states
    char state_names [SIZE];
    //The array which holds the X = 0 states
    char state_0s [SIZE];
    //The array which holds the X = 1 states
    char state_1s [SIZE];
    //The array which holds the outputs of each row
    int outs [SIZE];
    //The array that holds the implication zeroes
    char implication_zeros[SIZE];
    //The array that holds the implication ones
    char implication_ones[SIZE];
    //Build the implication table. We can optimize by removing unnecessary entries. Make private in the final
    //version so as to prevent corruptions
    char* build_implications(char start){
        //The array we are returning each time
        char implication_row[100];
        char implication_name = start;
        char current_state_0;
        char current_state_1;
        bool do_it = true;
        for (int j = 0; j < rows; j++){
            if(start == state_names[j]){
                implication_name = start;
                if(state_names[j] == implication_name) {
                    current_state_0 = state_0s[j];
                    current_state_1 = state_1s[j];
                }
                //These lines are to show that the current state does in fact change
                cout << current_state_0 << '\n';
                cout << current_state_1 << '\n';
                cout << "Found this row in table!" << "\n";
                //This break statement is so we can exit the loop
                break;
            }
            else if (j == rows - 1){
                cout << "No such argument" << "\n";
                do_it = false;
            }
        }
        //Currently this is how we determine whether to print a table or not, this could possibly be improved,
        // but it was simple for prototyping
        if(do_it) {
            for (int i = 0; i < rows; i++) {
                //Currently the indication that the row no longer exists is the filling in of it's implication
                //by ' ', I am open to suggestions on how else to handle this
                implication_row[i*5] = state_names[i];
                implication_row[(i*5)+1] = current_state_0;
                implication_row[(i*5)+2] = state_0s[i];
                implication_row[(i*5)+3] = current_state_1;
                implication_row[(i*5)+4] = state_1s[i];
                if (state_names[i] != start && state_names[i] != ' ') {
                    implication_zeros[i] = state_0s[i];
                    implication_ones[i] = state_1s[i];
                    //These lines have shown that the implication 0 and 1 arrays are made correct for each variable
//                    cout << implication_name << "   " << current_state_0
//                         << "-" << implication_zeros[i] << "   " <<
//                         current_state_1 << "-" << implication_ones[i] << '\n';
                }
                else{
                    implication_zeros[i] = ' ';
                    implication_ones[i] = ' ';
                }
            }
            return implication_row;
        }
        else{
            cout << "Could not compute for" << " " << start << "\n";
        }
    }
public:
    //The constructor function for a new table
    Table(){
        int rows = 0;
        char state_names = state_names;
        char state_0s = state_0s;
        char state_1s = state_1s;
        int outs = outs;
    }
    //Create the objects that show the possible implications. This is done by passing in the complete set of variables,
    // something we can probably do in the private context
    Table imp_for_var(char var_list [SIZE]){
        Table imp_table;
        for(int i = 0; i < SIZE; i++){
            if(var_list[i] != '\0') {
                //build_implications(var_list[i]);
                //From here we need to assemble the implications into a table
                //This value "champ" is going to be the pointer to the array we just built in build implications

                //Considering array of arrays representation instead of straight array and parsing methods
                char* champ = build_implications(var_list[i]);
                char start_of_row = champ[i*5];
                char usual_out_at_0 = champ[(i*5) + 1];
                char hypo_out_at_0 = champ[(i*5) + 2];
                char usual_out_at_1 = champ[(i*5) + 3];
                char hypo_out_at_1 = champ[(i*5) + 4];
                cout << "this is the start of the imp table row: " << start_of_row
                        << " " << usual_out_at_0 << " " << usual_out_at_1;
            }
        }
    }
    //The function to add a new_row. Keep as a public function
    void add_row(Row new_row){
        state_names[i] = new_row.getStateName();
        state_0s[i] = new_row.getStateZero();
        state_1s[i] = new_row.getStateOne();
        outs[i] = new_row.getOut();
        i += 1;
        rows += 1;
    }
    //Double loop to check all the rows of the table and eliminate those that have copies.
    //The next function will get rid of the implication pairs. This might make sense to make private
    //In the final version
    void reduce(){
        for(int j = 0; j < rows; j++){
            char check_row_name = state_names[j];
            char check_row_0 = state_0s[j];
            char check_row_1 = state_1s[j];
            int check_out = outs[j];
            for(int k = j+1; k < rows; k++){
                if(check_row_0 == state_0s[k] && (check_row_1 == state_1s[k]) && (check_out == outs[k])){
                    //We still need to find the way to eliminate the pairs because of this function. Also rename this
                    //to eliminate_pairs. The $ indicates a row to be ignored
                    state_names[k]= ' ';
                    state_0s[k] = ' ';
                    state_1s[k] = ' ';
                    cout << "Found a matching row!" << "\n";
                }
            }
        }
    }
    void disp_imp(char start){
        build_implications(start);
        //Find somewhere to store the implication data
        for (int i = 0; i < rows; i++) {
            //Not quite sure why but using this null notation works unlike the $ that I expected to do the trick
            if(implication_zeros[i] != '\0' && implication_ones[i] != '\0' ) {
                //This double if statement makes the thing look better. Remove this inner if if you want proof that
                //the implication arrays are in fact blank. Still don't get the null members of the array entirely
                if(implication_zeros[i] != ' ' && implication_zeros[i] != ' ') {
                    cout << "Showing the implications for " << start << "  " << i << "\n";
                    cout << implication_zeros[i] << "   ";
                    cout << implication_ones[i] << "   " << "\n";
                }
            }
        }
    }
    void disp() {
        //This first test is to verify the first letter of each state is included. This function will display
        //The built table in its entirety
        for (int i = 0; i < rows; i++) {
            cout << state_names[i] << "   ";
            cout << state_0s[i] << "   ";
            cout << state_1s[i] << "   ";
            cout << outs[i] << "\n";
        }
    }
};
int main() {
    //Created some test rows
    Row A('A', 'C', 'F', 1);
    Row B('B', 'H', 'F', 1);
    Row C('C', 'I', 'H', 1);
    Row D('D', 'A', 'E', 0);
    Row E('E', 'E', 'I', 1);
    Row F('F', 'A', 'C', 0);
    Row H('H', 'A', 'B', 0);
    Row I('I', 'A', 'B', 0);
    Row J('J', 'C', 'I', 1);
    //The display functions to see individual rows
//    A.disp();
//    B.disp();
//    C.disp();
    //Creating a table and the methods to populate the table
    Table one;
    //Adding a row to the table, must make it so I can call these one on top of the other
    one.add_row(A);
    one.add_row(B);
    one.add_row(C);
    one.add_row(D);
    one.add_row(E);
    one.add_row(F);
    one.add_row(H);
    one.add_row(I);
    one.add_row(J);
    //This will show the full table after the rows have been added
    //one.disp();
    //This function will reduce duplicates. Make private function, tested as public in this context
    //one.reduce();
    //These functions show the building of the implications for each variable. Will be a private function
    //make public to observe the test
    //one.build_implications('A');
    //one.build_implications('B');
    //one.build_implications('C');
    //one.build_implications('R');
    //Show the implications for each variable. This is simply a test function
    //one.disp_imp('A');
    //one.disp_imp('C');
    //one.disp_imp('E');
    //Testing the build individual implications function. Must pass in an array of all the implications possible
    char variables[SIZE] = {'H'};
    one.imp_for_var(variables);
    //Fix this so that the disp_imp updates the arrays of implications, need a better strategy to store imps
    return 0;
}