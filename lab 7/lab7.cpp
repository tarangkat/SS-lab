#include <bits/stdc++.h>
using namespace std;


struct Production
{
   string lhs;
   string rhs;
};


struct Item
{
   string lhs;
   string rhs;
   int dot;


   bool operator<(const Item &other) const
   {
       if (lhs != other.lhs)
           return lhs < other.lhs;
       if (rhs != other.rhs)
           return rhs < other.rhs;
       return dot < other.dot;
   }


   bool operator==(const Item &other) const
   {
       return lhs == other.lhs &&
              rhs == other.rhs &&
              dot == other.dot;
   }
};
vector<Production> grammar = {
   {"E'", "E"},
   {"E", "TT"},
   {"T", "aT"},
   {"T", "b"}};


set<char> terminals = {'a', 'b', '$'};
set<char> nonTerminals = {'E', 'T'};


map<char, set<char>> FOLLOW;


vector<set<Item>> C;
map<pair<int, char>, int> transitions;


map<int, map<char, string>> ACTION;
map<int, map<char, int>> GOTO_TABLE;


void computeFOLLOW()
{
   FOLLOW['E'].insert('$');


   bool changed = true;


   while (changed)
   {
       changed = false;


       for (auto &p : grammar)
       {
           for (int i = 0; i < p.rhs.size(); i++)
           {
               char B = p.rhs[i];


               if (nonTerminals.count(B))
               {
                  
                   if (i + 1 < p.rhs.size())
                   {
                       char next = p.rhs[i + 1];


                      
                       if (terminals.count(next))
                       {
                           if (!FOLLOW[B].count(next))
                           {
                               FOLLOW[B].insert(next);
                               changed = true;
                           }
                       }
                       else if (nonTerminals.count(next))
                       {
                           for (auto &prod : grammar)
                           {
                               if (prod.lhs[0] == next)
                               {
                                   char firstChar = prod.rhs[0];
                                   if (!FOLLOW[B].count(firstChar))
                                   {
                                       FOLLOW[B].insert(firstChar);
                                       changed = true;
                                   }
                               }
                           }
                       }
                   }
                   else
                   {
                       for (char f : FOLLOW[p.lhs[0]])
                       {
                           if (!FOLLOW[B].count(f))
                           {
                               FOLLOW[B].insert(f);
                               changed = true;
                           }
                       }
                   }
               }
           }
       }
   }
}


set<Item> closure(set<Item> I)
{


   bool changed = true;


   while (changed)
   {
       changed = false;


       vector<Item> items(I.begin(), I.end());


       for (auto &item : items)
       {


           if (item.dot < item.rhs.size())
           {


               char symbol = item.rhs[item.dot];


               if (nonTerminals.count(symbol))
               {


                   for (auto &p : grammar)
                   {
                       if (p.lhs[0] == symbol)
                       {
                           Item newItem = {p.lhs, p.rhs, 0};


                           if (!I.count(newItem))
                           {
                               I.insert(newItem);
                               changed = true;
                           }
                       }
                   }
               }
           }
       }
   }


   return I;
}



set<Item> GOTO_func(set<Item> I, char X)
{


   set<Item> J;


   for (auto &item : I)
   {
       if (item.dot < item.rhs.size() && item.rhs[item.dot] == X)
       {
           J.insert({item.lhs, item.rhs, item.dot + 1});
       }
   }


   return closure(J);
}




void buildCanonicalCollection()
{


   set<Item> start;
   start.insert({"E'", "E", 0});
   start = closure(start);


   C.push_back(start);


   queue<int> q;
   q.push(0);


   while (!q.empty())
   {


       int i = q.front();
       q.pop();


       set<char> symbols = terminals;
       symbols.insert(nonTerminals.begin(), nonTerminals.end());


       for (char X : symbols)
       {


           set<Item> gotoSet = GOTO_func(C[i], X);


           if (gotoSet.empty())
               continue;


           int index = -1;
           for (int k = 0; k < C.size(); k++)
           {
               if (C[k] == gotoSet)
               {
                   index = k;
                   break;
               }
           }
           if (index == -1)
           {
               C.push_back(gotoSet);
               index = C.size() - 1;
               q.push(index);
           }


           transitions[{i, X}] = index;
       }
   }
}


void buildSLRTable()
{

   for (int i = 0; i < C.size(); i++)
   {
    for (auto &item : C[i])
       {
           if (item.dot < item.rhs.size())
           {
               char a = item.rhs[item.dot];

               if (terminals.count(a))
               {
                   int j = transitions[{i, a}];
                   ACTION[i][a] = "S" + to_string(j);
               }

               if (nonTerminals.count(a))
               {
                   int j = transitions[{i, a}];
                   GOTO_TABLE[i][a] = j;
               }
           }
           else
           {
               if (item.lhs == "E'")
               {
                   ACTION[i]['$'] = "ACC";
               }
               else
               {
                   for (int p = 0; p < grammar.size(); p++)
                   {
                       if (grammar[p].lhs == item.lhs &&
                           grammar[p].rhs == item.rhs)
                       {


                           for (char a : FOLLOW[item.lhs[0]])
                           {
                               ACTION[i][a] = "R" + to_string(p);
                           }
                       }
                   }
               }
           }
       }
   }
}

void parse(string input)
{

   input += "$";
   stack<int> st;
   st.push(0);
   int i = 0;
   cout << "\nStack\tInput\tAction\n";
   while (true)
   {
       int state = st.top();
       char lookahead = input[i];

       cout << state << "\t" << input.substr(i) << "\t";

       if (ACTION[state].find(lookahead) == ACTION[state].end())
       {
           cout << "ERROR\n";
           cout << " Rejected\n";
           return;
       }
       string action = ACTION[state][lookahead];
       cout << action << "\n";
       if (action == "ACC")
       {
           cout << " Accepted\n";
           return;
       }
       else if (action[0] == 'S')
       {
           int nextState = stoi(action.substr(1));
           st.push(nextState);
           i++;
       }

       else if (action[0] == 'R')
       {
           int prodIndex = stoi(action.substr(1));
           int len = grammar[prodIndex].rhs.size();

           while (len--)
               st.pop();

           int topState = st.top();
           st.push(GOTO_TABLE[topState][grammar[prodIndex].lhs[0]]);
       }
   }
}

int main()
{
   computeFOLLOW();
   buildCanonicalCollection();
   buildSLRTable();
   cout << "Parsing aabb\n";
   parse("aabb");
   cout << "\n------------------\n";
   cout << "Parsing babb\n";
   parse("babb");
   return 0;
}

