/*
 * Copyright (C) Mohsen Zohrevandi, 2017
 *
 * Do not share this file with anyone
 */
#include "project2.h"

using namespace std;

Token token;
LexicalAnalyzer lexer;
int var_count= 2;                 // include epsilon and END_OF_FILE
Token universal_variables[200];
vector<i_rule> uni_var;
map<std::string,int> imap;
map<int,int> t_map;
map<int,int> rt_map;
map<int,int> TM;
map<int,int>NTM;
vector<int> terminals;
vector<int> non_terminals;
set<int> order;
bool *generating_array;
int arr_size; // size of the generating array only
bool *task2_print;
bool **Firsts;
bool **Follows;
bool Firsts_Changed;


void printFirstSets()
{

    for (int x = 0; x < terminals.size() + non_terminals.size() + 2; x++)
    {
        for ( int y = 0; y < terminals.size() + 2; y++)
        {
            cout << Firsts[x][y] << " ";
        }
        cout << "" << endl;
    }
}
void printAll()
{
    cout << "*****Printing all terms*****" << endl;
    for(int x = 0; x < non_terminals.size(); x++)
    {
        cout << universal_variables[non_terminals.at(x)].lexeme << " ";
    }
    cout << "|| ";
    for (int x = 0; x < terminals.size(); x++)
    {
        cout << universal_variables[terminals.at(x)].lexeme << " ";
    }
    cout << "\n *****End of all terminals***" << endl;
}
void mapit(Token variable)
{
    string var = variable.lexeme.c_str();
    if (imap[var] == 0)
    {
        imap[var] = var_count;

        universal_variables[var_count] = variable;
        var_count++;
    }
}
int main (int argc, char* argv[])
{
    // required for epsilon and END_OF_FILE
    token.token_type = HASH;
    token.lexeme = "#";
    universal_variables[0] = token;

    token.token_type = DOUBLEHASH;
    token.lexeme = "##";
    universal_variables[1] = token;

    int task;

    parse_ruleList();

    vector<i_rule>::iterator it;
    int rule_pos = 0;
    for (it = uni_var.begin(); it != uni_var.end(); it++)
    {
        it->pos = rule_pos;
        rule_pos++;
    }


    if (argc < 2)
    {
        cout << "Error: missing argument\n";
        return 1;
    }

    task = atoi(argv[1]);

    switch (task) {
        case 1:
            {

                calculateAll();

                set<int>::iterator setiter;
                for (int x = 0; x < non_terminals.size(); x++)
                {
                    order.insert(non_terminals.at(x));
                }

                non_terminals.clear();

                for (setiter = order.begin(); setiter != order.end(); setiter++)
                {
                     non_terminals.push_back(*setiter);
                }

                for (int x = 0; x < non_terminals.size(); x++)
                {
                    cout << universal_variables[non_terminals.at(x)].lexeme << " ";
                }
                for (int x = 0; x < terminals.size(); x++)
                {
                    cout << universal_variables[terminals.at(x)].lexeme << " ";
                }
                cout << endl;
            }
            break;

        case 2:
            {
                //task 1
              calculateAll();

                //  task 2

                calculateUseless();

                for (it = uni_var.begin(); it != uni_var.end(); it++)
                {
                    if (task2_print[it->pos])
                    {
                        cout << universal_variables[it->LHS].lexeme << " -> ";

                        if (it->RHS.at(0) == 0)
                        {
                            cout << "#";
                        }
                        else
                        {
                            for (int x = 0; x < it->RHS.size()-1; x++)
                            {
                                cout << universal_variables[it->RHS.at(x)].lexeme << " ";
                            }
                        }
                        cout << endl;
                    }
                }



            }
            break;

        case 3:
        {
        // starting task 1
             calculateAll();

             set<int>::iterator setiter;
                for (int x = 0; x < non_terminals.size(); x++)
                {
                    order.insert(non_terminals.at(x));
                }

                non_terminals.clear();

                for (setiter = order.begin(); setiter != order.end(); setiter++)
                {
                     non_terminals.push_back(*setiter);
                }
        // end of task 1

            calculateFirstSets();



            bool isPrinted[non_terminals.size() + terminals.size() + 2];
            vector<string> iprint;
            for (int p = 0; p < non_terminals.size() + terminals.size() + 2; p++)
            {
                isPrinted[p] = false;
            }


            for (int q = 0; q < non_terminals.size(); q++)
            {
                if (isPrinted[non_terminals.at(q)] == false)
                {
                    cout << "FIRST(" << universal_variables[non_terminals.at(q)].lexeme << ") = { ";

                    for (int x = 0; x < terminals.size() + 2; x++)
                    {
                        if (Firsts[non_terminals.at(q)][x])
                        {
                            if (x == 0)
                            {
                                iprint.push_back("#");
                            }
                            else if (x == 1)
                            {
                                iprint.push_back("##");
                            }
                            else
                            {
                                iprint.push_back(universal_variables[rt_map[x]].lexeme.c_str());
                            }
                        }
                    }
                    isPrinted[non_terminals.at(q)] = true;



                    for (int y = 0; y < iprint.size(); y++)
                    {

                            if ( y == iprint.size()-1)
                                cout << iprint.at(y) << " ";
                            else
                                cout << iprint.at(y) << ", ";

                    }
                    cout << "}"  << endl;
                }
                    iprint.clear();
          }
        }
            break;

        case 4:
            {
                // starting task 1
                calculateAll();

                set<int>::iterator setiter;
                for (int x = 0; x < non_terminals.size(); x++)
                {
                    order.insert(non_terminals.at(x));
                }

                non_terminals.clear();

                for (setiter = order.begin(); setiter != order.end(); setiter++)
                {
                     non_terminals.push_back(*setiter);
                }


                calculateFirstSets();


                calculateFollowSets();

                bool isPrinted[non_terminals.size() + terminals.size() + 2];
                vector<string> iprint;
                for (int p = 0; p < non_terminals.size() + terminals.size() + 2; p++)
                {
                    isPrinted[p] = false;
                }


                for (int q = 0; q < non_terminals.size(); q++)
                {
                    if (isPrinted[non_terminals.at(q)] == false)
                    {
                        cout << "FOLLOW(" << universal_variables[non_terminals.at(q)].lexeme << ") = { ";

                        for (int x = 0; x < terminals.size() + 2; x++)
                        {
                            if (Follows[non_terminals.at(q)][x])
                            {
                                if (x == 0)
                                {
                                    iprint.push_back("#");
                                }
                                else if (x == 1)
                                {
                                    iprint.push_back("$");
                                }
                                else
                                {
                                    iprint.push_back(universal_variables[rt_map[x]].lexeme.c_str());
                                }
                            }
                        }
                        isPrinted[non_terminals.at(q)] = true;


                        for (int y = 0; y < iprint.size(); y++)
                        {

                                if ( y == iprint.size()-1)
                                    cout << iprint.at(y) << " ";
                                else
                                    cout << iprint.at(y) << ", ";
                        }
                        cout << "}"  << endl;
                    }
                        iprint.clear();
              }
            }

            break;

        case 5:
            // TODO: perform task 5.
            break;

        default:
            cout << "Error: unrecognized task number " << 7 << "\n";
            break;
    }

    return 0;
}
void parse_ruleList()
{
    token = lexer.GetToken();
    lexer.UngetToken(token);

    if (token.token_type == ID)
    {
        parse_rule();
    }
    else
    {
        // throw a syntax error
    }

    token = lexer.GetToken();
    lexer.UngetToken(token);

    if (token.token_type == ID)
    {
        parse_ruleList();
    }
    else if (token.token_type == DOUBLEHASH)
    {
        token = lexer.GetToken();

        if (token.token_type == END_OF_FILE)
        {
            return;
        }
    }
    else
    {
        // throw syntax error
    }

}

void parse_rule()
{
    token = lexer.GetToken();
    struct i_rule newrule;

    if (token.token_type == ID)
    {
        mapit(token);             // map these non-terminals to the universal array
        newrule.LHS = imap[ token.lexeme.c_str() ]; // this specific variable is now located in var_count--;

        token = lexer.GetToken();
        if (token.token_type == ARROW)
        {
            parse_rhs(newrule);

            token = lexer.GetToken();

            if (token.token_type == HASH)
            {
                newrule.RHS.push_back(0);
                uni_var.push_back(newrule);      //
                return;
            }
        }
        else
        {
            // send an error
        }
    }
    else
    {

        // throw syntax error
    }

}

void parse_rhs(i_rule &nr)
{
    token = lexer.GetToken();
    lexer.UngetToken(token);

    if (token.token_type == ID)
    {
        parse_idList(nr);
    }
    else if (token.token_type == HASH)
    {
        return;
    }
    else
    {
        // throw syntax error
    }
}

void parse_idList(i_rule &nr)
{
    token = lexer.GetToken();

    if (token.token_type == ID)
    {
        mapit(token);
        nr.RHS.push_back(imap[ token.lexeme.c_str() ]);
        parse_idList(nr);
    }
    else if (token.token_type == HASH)
    {
        lexer.UngetToken(token);
        return;
    }
    else
    {
        // throw syntax error
    }
}
void addNT(int x)
{
    bool LHS_is_already_in_Q = false;
    if (non_terminals.size() == 0)
    {
        non_terminals.push_back(x);
        return;
    }

    for (int y = 0; y < non_terminals.size(); y++)
    {
        if (x == non_terminals.at(y))
        {
            LHS_is_already_in_Q = true;
        }
    }

    if (!LHS_is_already_in_Q)
    {
        non_terminals.push_back(x);
    }

}

void addT()
{
    bool RHS_is_in_nonterminals = false;                                       // two helper booleans
    bool RHS_was_already_in_terminals = false;
    vector<i_rule>::iterator it;
    for (it = uni_var.begin(); it != uni_var.end(); it++) // this scans over the vector with all the struct rules
    {
        for(int s = 0; s < it->RHS.size(); s++)      // this scans over the vector RHS
        {
            for (int nons = 0; nons < non_terminals.size(); nons++)  // this scans over the vector non terminals
            {
                if ( it->RHS.at(s) == non_terminals.at(nons))  // this compares only ONE RHS s with all the ints inside non terminal vector
                {
                    RHS_is_in_nonterminals = true;                  // sets true if the current s we are examining in RHS vector is in fact already inside nonterminals
                }
            }

            if (!RHS_is_in_nonterminals)                         // if the current s is not a non terminal then we'll check if its a terminal
            {
                for (int term = 0; term < terminals.size(); term++)   // this scans over the terminals Q which should be empty the very first time it runs
                {
                    if (it->RHS.at(s) == terminals.at(term))    // this compares only ONE RHS s with all the ints inside terminal vector
                    {
                        RHS_was_already_in_terminals = true;        // true if the current s we are holding is already inside terminal vector
                    }
                }
                if (!RHS_was_already_in_terminals)                 // this will only result in true if the current s is not in the nonterminal vector & not in the terminal vector
                {
                    if( it->RHS.at(s) != 0)                      // before claiming that the current s is a terminal, we check that its not a HASH
                        terminals.push_back(it->RHS.at(s));
                }
            }
            RHS_is_in_nonterminals = false;                          // resets variables for the next round since we still have to check the rest of the
            RHS_was_already_in_terminals = false;                   // RHS vectors in the rest of the grammar rules
        }
    }
}

void calculateAll(){
  vector<i_rule>::iterator it;

  for (it = uni_var.begin(); it != uni_var.end(); it++)
  {
      addNT(it->LHS);
  }
      addT();

}

void calculateUseless()
{
        arr_size = non_terminals.size() + terminals.size() + 2;
        generating_array = new bool[arr_size];
        bool uni_rules[uni_var.size()];

        for(int x = 0; x < arr_size; x++)
        {
            generating_array[x] = false;
        }
        for (int x = 0; x < terminals.size(); x++)
        {
            generating_array[terminals.at(x)] = true;
        }
            generating_array[0] = true;
        for (int x = 0; x < uni_var.size(); x++)
        {
            uni_rules[x] = false;
        }

        bool isChanged = true;
        vector<i_rule>::iterator it;

        while(isChanged)
        {
            isChanged = false;
            for (it = uni_var.begin(); it != uni_var.end(); it++)
            {
                if ( !generating_array[it->LHS] )
                {
                    bool all_are_generating = true;
                    for ( int RHS_terms = 0; RHS_terms < it->RHS.size(); RHS_terms++)
                    {
                        all_are_generating = all_are_generating && generating_array[it->RHS.at(RHS_terms)];
                    }
                    if (all_are_generating == true)
                    {
                        generating_array[it->LHS] = true;
                        isChanged = true;
                    }

                }
            }
        }
        vector<i_rule>::iterator it2;

        for (it2  = uni_var.begin(); it2 != uni_var.end(); it2++)
        {
            bool generating = true;
            for(int symb = 0; symb < it2->RHS.size(); symb++)
            {
                generating = generating && generating_array[it2->RHS.at(symb)];
            }

            if(generating)
            {
                uni_rules[it2->pos] = true;
            }
        }


        bool reachable_rules[uni_var.size()];
        bool isReachable[arr_size];

        for(int x = 0; x < uni_var.size(); x++)
        {
            reachable_rules[x] = false;
        }
        for (int x = 0; x < arr_size; x++)
        {
            isReachable[x] = false;
        }

        vector<i_rule>::iterator it3;
        isReachable[non_terminals.at(0)] = true;

            for(it3 = uni_var.begin(); it3 != uni_var.end(); it3++)
            {
                if ( uni_rules[it3->pos] )
                {
                    if (isReachable[it3->LHS])
                    {
                        for(int x = 0; x < it3->RHS.size(); x++)
                        {
                            isReachable[it3->RHS.at(x)] = true;
                            reachable_rules[it3->pos] = true;
                        }
                    }
                }
            }


            task2_print = new bool[uni_var.size()];

            for (int uni = 0; uni < uni_var.size(); uni++)
            {
                task2_print[uni] = reachable_rules[uni] && uni_rules[uni];
            }




}

void calculateFirstSets()
{

    int firstsize = non_terminals.size() + terminals.size() + 2;
    Firsts = new bool*[firstsize];

    for ( int x = 0; x < firstsize; x++ )
    {
        Firsts[x] = new bool[terminals.size() + 2];
    }

    for (int x =0; x < firstsize; x++)
    {
        for (int y = 0; y < terminals.size() + 2; y++)
        {
            Firsts[x][y] = false;
        }
    }

    int terminals_new_position =  2;
    for(int x = 0; x < terminals.size(); x++)
    {
        t_map[terminals.at(x)] = terminals_new_position;
        rt_map[terminals_new_position] = terminals.at(x);
        terminals_new_position++;
    }

    for (int x = 0; x < terminals.size(); x++)
    {
        Firsts[terminals.at(x)][t_map[terminals.at(x)]] = true;
    }
        Firsts[0][0] = true;

    Firsts_Changed = true;
    int its = 0;
    vector<i_rule>::iterator it;

    while (Firsts_Changed)
    {

        for (it = uni_var.begin(); it != uni_var.end(); it++)
        {
            first_r3(*it, it->RHS.at(0));
            first_r4(*it);
            first_r5(*it);
        }

        if (its == 30)
            Firsts_Changed = false;
         its++;


    }


}
void first_r3(i_rule &r, int s)
{
    if (s == 0) // +++
        return; // +++

    i_rule *rule = &r;

    for (int x = 1; x < terminals.size() + 2; x++) // we start at one to exclude epsilon
    {
        Firsts[rule->LHS][x] = Firsts[rule->LHS][x] ||  Firsts[s][x];
    }
}
void first_r4(i_rule &r)
{
    i_rule *rule = &r;
    vector<int> *p = &(rule->RHS);

    if (rule->RHS.size()  <= 2) // if the rule only has epsilon then return +++
        return; // +++

    vector<int>::iterator it;
    bool exit; // +++

    for (it = p->begin(); it != p->end(); it++) // iterating RHS to find epsilons
    {
         exit = true; // +++

        if (Firsts[*it][0] && (it != p->end()-1)) // if the current s has epsilon and if its not the last s in Q
        {
            first_r3(*rule, *(it+1));
            exit = false; // +++
        }

        if (exit == true) //+++
            return; //+++
    }
}
void first_r5(i_rule &r)
{
    i_rule *rule = &r;
    bool all_epsilon = true;
    vector<int>::iterator it;

    for (it = rule->RHS.begin(); it != rule->RHS.end(); it++)
    {
        all_epsilon = all_epsilon && Firsts[*it][0];
    }

    if (all_epsilon)
    {
        Firsts[rule->LHS][0] = true;
    }
}

void calculateFollowSets()
{
    vector<i_rule>::iterator it;

    int followsize = non_terminals.size() + terminals.size() + 2;
    Follows = new bool*[followsize];

    for ( int x = 0; x < followsize; x++ )
    {
        Follows[x] = new bool[terminals.size() + 2];
    }

    for (int x = 0; x < followsize; x++)
    {
        for (int y = 0; y < terminals.size() + 2; y++)
        {
            Follows[x][y] = false;
        }
    }
    int newposi = 2;
    for (int x = 0; x < terminals.size(); x++)
    {
        TM[terminals.at(x)] = newposi;
        newposi++;
    }

    Follows[2][1] = true;

    bool ChangeMade = true;
    int iterations = 0;

    while (ChangeMade)
    {
        for (it = uni_var.begin(); it != uni_var.end(); it++)
        {
            if (it->RHS.size() > 1)
                follow_r2(*it, it->RHS.at(it->RHS.size()-2));
            if (it->RHS.size() > 2)
                follow_r3(*it);
            if (it->RHS.size() >= 3){
              follow_r4(*it);
              follow_r5(*it);
            }


        }
        iterations++;
        if(iterations == 20){
          ChangeMade = false;
        }
    }

}
void follow_r2(i_rule &r, int s)
{
    if ( isterminal(s) )
        return;

    i_rule *rule = &r;

    for (int x = 0; x < terminals.size() + 2; x++)
    {
        Follows[s][x] = Follows[s][x] || Follows[rule->LHS][x];
    }

}
void follow_r3(i_rule &r)
{
    i_rule *rule = &r;
    vector<int>::iterator it;
    bool ex;

    for (it = rule->RHS.end()-2; it > rule->RHS.begin(); it--)
    {
        ex = true;
        if (Firsts[*it][0])
        {
            follow_r2(*rule, *(it-1));
            ex = false;
        }

        if (ex == true)
            return;
    }

}

void follow_r4(i_rule &r)
{
    i_rule *rule = &r;
    vector<int>::iterator it;

    for (it = rule->RHS.begin(); it != rule->RHS.end() -2; it++)
    {
        for (int y = 1; y < terminals.size() + 2; y++)
        {
                Follows[*it][y] = Follows[*it][y] || Firsts[*(it+1)][y];
        }
    }

}

void follow_r5(i_rule &r)
{

    i_rule *rule = &r;
    bool exi;

    for (int i = 0; i < rule->RHS.size(); i++)
    {
        for (int j = i +1; j < rule->RHS.size() -1; j++)
        {

            if ( hasEpsilon(rule->RHS.at(j)) )
            {
                if ( !isterminal(rule->RHS.at(j+1)) )
                {
                    for (int p = 2; p < terminals.size() + 2; p++)
                        Follows[rule->RHS.at(i)][p] = Follows[rule->RHS.at(i)][p] || Firsts[rule->RHS.at(j+1)][p];
                }
            }
            else
            {
                break;
            }
        }
    }

}

bool isterminal(int s)
{
    bool contains = false;
    for (int x = 0 ; x < terminals.size(); x++)
    {
        if (s == terminals.at(x))
            contains = true;
    }

    return contains;
}

bool hasEpsilon(int s)
{
    bool epsilon = false;

    if (Firsts[s][0])
        epsilon = true;

    return epsilon;
}
