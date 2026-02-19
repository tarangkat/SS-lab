#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <map>

using namespace std;
 
struct Line {
    string label;   
    string opcode;  
    string operand;  
};

 
map<string, int> opcodeTable = {
    {"LOAD", 1},
    {"ADD", 2},
    {"STORE", 3},
    {"JMP", 4},
    {"DATA", 0}  
};

map<string, int> symbolTable;

Line parseLine(string rawLine) {
    Line l;
    stringstream ss(rawLine);
    string token;
    
    if (ss >> token) {
                  
        if (token.back() == ':') {
            token.pop_back();     
            l.label = token;
            ss >> l.opcode;   
        } else {
            l.opcode = token;  
        }
        ss >> l.operand; 
    }
    return l;
}

int main() {

 
    vector<string> sourceCode = {
        "START: LOAD A",
        "       ADD B",
        "       STORE C",
        "       JMP START",
        "A:     DATA 10",
        "B:     DATA 20",
        "C:     DATA 0"
    };

    vector<Line> parsedProgram;
    int LC = 0; 

    cout << "--- PASS 1: SYMBOL TABLE GENERATION ---\n";

    for (const string& rawLine : sourceCode) {
        Line line = parseLine(rawLine);
        parsedProgram.push_back(line); 
        
        if (!line.label.empty()) {

            symbolTable[line.label] = LC;
            cout << "Found Label: " << line.label << " at Address " << LC << endl;
        }

        if (!line.opcode.empty()) {
            LC++;
        }
    }

    cout << "\n--- PASS 2: CODE GENERATION ---\n";
    cout << "Addr\tOpcode\tOperand\n";
    cout << "----\t------\t-------\n";

    LC = 0; 
    
    for (const Line& line : parsedProgram) {
        if (line.opcode.empty()) continue;

        int opValue = 0;
        int operandValue = 0;

      
        if (opcodeTable.find(line.opcode) != opcodeTable.end()) {
            opValue = opcodeTable[line.opcode];
        } else {
            cout << "Error: Unknown Opcode " << line.opcode << endl;
            continue;
        }


        if (line.opcode == "DATA") {
          
            operandValue = stoi(line.operand); 
        } 
        else if (!line.operand.empty()) {
            
            if (symbolTable.find(line.operand) != symbolTable.end()) {
                operandValue = symbolTable[line.operand];
            } else {
                cout << "Error: Undefined Label " << line.operand << endl;
            }
        }

        cout << LC << "\t" << opValue << "\t" << operandValue << endl;

        LC++;
    }

    return 0;
}