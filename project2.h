#include <iostream>
#include <vector>
#include <map>
#include "lexer.h"
#include <cstdio>
#include <cstdlib>
#include <queue>
#include <string.h>
#include <set>

using namespace std;

struct i_rule
{
int LHS;
vector<int> RHS;
int RHS_Size;
int pos;
int k;
};

void parse_ruleList();
void parse_rule();
void parse_rhs(i_rule &nr);
void parse_idList(i_rule &nr);
void mapit(Token vari);
void addNT(int x);
void addT();
void calculateAll();
void calculateUseless();
void calculateFirstSets();
void calculateFollowSets();
void printFirstSets();
void first_r3(i_rule &r, int symbol);
void first_r4(i_rule &r);
void first_r5(i_rule &r);
void printAll();
void follow_r2(i_rule &r, int symbol);
void follow_r3(i_rule &r);
void follow_r4(i_rule &r);
void follow_r5(i_rule &r);
bool isterminal(int s);
bool hasEpsilon(int symbol);
