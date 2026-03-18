#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <iomanip>
#include <stdexcept>

using namespace std;
struct InstructionInfo {
    string hexCode;
    int size; 
};
struct ParsedLine {
    int lineNumber;
    int locationCounter;
    string label;
    string mnemonic; 
    string operand;  
    int size;
};

class AssemblerException : public runtime_error {
public:
    AssemblerException(int line, const string& msg) 
        : runtime_error("Error on line " + to_string(line) + ": " + msg) {}
};

class Assembler8085 {
private:
    map<string, InstructionInfo> OPTAB; 
    map<string, int> SYMTAB;    
    vector<ParsedLine> intermediateCode;
    int startingAddress = 0x0000;

    string trim(const string& str) {
        size_t first = str.find_first_not_of(" \t");
        if (string::npos == first) return "";
        size_t last = str.find_last_not_of(" \t");
        return str.substr(first, (last - first + 1));
    }

   
    int parseHex(string val, int line) {
        val = trim(val);
        if (val.empty()) throw AssemblerException(line, "Missing operand");
        
        if (val.back() == 'H' || val.back() == 'h') {
            val.pop_back(); 
        }
        
        try {
            size_t processedChars; 
            int result = stoi(val, &processedChars, 16);
            
            if (processedChars != val.length()) {
                throw AssemblerException(line, "Syntax error / Garbage characters found: '" + val.substr(processedChars) + "'");
            }
            return result;
            
        } catch (const invalid_argument&) {
            throw AssemblerException(line, "Invalid syntax or undefined label: '" + val + "'");
        } catch (const out_of_range&) {
            throw AssemblerException(line, "Value out of range: '" + val + "'");
        }
    }

public:
    Assembler8085() {
                
        OPTAB["MOV A,B"] = {"78", 1};
        OPTAB["MOV B,A"] = {"47", 1};
        OPTAB["ADD B"]   = {"80", 1};
        OPTAB["SUB B"]   = {"90", 1};
        OPTAB["HLT"]     = {"76", 1};
        OPTAB["RET"]     = {"C9", 1};        
        OPTAB["MVI A"]   = {"3E", 2};
        OPTAB["MVI B"]   = {"06", 2};
        OPTAB["ADI"]     = {"C6", 2};         
        OPTAB["LDA"]     = {"3A", 3}; 
        OPTAB["STA"]     = {"32", 3}; 
        OPTAB["JMP"]     = {"C3", 3}; 
        OPTAB["JC"]      = {"DA", 3}; 
    }

    void pass1(const vector<string>& sourceCode) {
        int LOCCTR = startingAddress;

        for (int i = 0; i < sourceCode.size(); ++i) {
            string line = sourceCode[i];
            
            size_t commentPos = line.find(';');
            if (commentPos != string::npos) line = line.substr(0, commentPos);
            line = trim(line);
            if (line.empty()) continue;

            ParsedLine parsed;
            parsed.lineNumber = i + 1;
            parsed.locationCounter = LOCCTR;
 
            size_t colonPos = line.find(':');
            if (colonPos != string::npos) {
                parsed.label = trim(line.substr(0, colonPos));
                if (SYMTAB.find(parsed.label) != SYMTAB.end()) {
                    throw AssemblerException(parsed.lineNumber, "Duplicate label '" + parsed.label + "'");
                }
                SYMTAB[parsed.label] = LOCCTR;
                line = trim(line.substr(colonPos + 1));
                if (line.empty()) continue; 
            }

            bool found = false;
            for (auto const& [key, val] : OPTAB) {
                if (line.find(key) == 0) { 
                    parsed.mnemonic = key;
                    parsed.size = val.size;
                    string remainder = trim(line.substr(key.length()));
                    
                    if (remainder.length() > 0 && remainder[0] == ',') {
                         remainder = trim(remainder.substr(1)); 
                    }
                    parsed.operand = remainder;
                    found = true;
                    break;
                }
            }

            if (!found) {
                throw AssemblerException(parsed.lineNumber, "Invalid or unsupported 8085 opcode in line: " + line);
            }

            if (parsed.size > 1 && parsed.operand.empty()) {
                throw AssemblerException(parsed.lineNumber, "Missing operand for " + parsed.mnemonic);
            }
            if (parsed.size == 1 && !parsed.operand.empty()) {
                throw AssemblerException(parsed.lineNumber, "Instruction " + parsed.mnemonic + " does not take an operand");
            }

            intermediateCode.push_back(parsed);
                        
            LOCCTR += parsed.size;
        }
    }

   void pass2() {
        cout << "--- 8085 Assembled Object Code ---" << endl;
        cout << "ADDR\tMACH CODE\tSOURCE" << endl;
        cout << "-------------------------------------------" << endl;

        for (const auto& line : intermediateCode) {
            string machineCode = OPTAB[line.mnemonic].hexCode;

            if (line.size == 2) {
                int data = parseHex(line.operand, line.lineNumber);
                if (data > 0xFF) throw AssemblerException(line.lineNumber, "8-bit overflow in operand");
                
                stringstream ss;
                ss << " " << setfill('0') << setw(2) << hex << uppercase << data;
                machineCode += ss.str();
            } 
            else if (line.size == 3) {
                int address;
                if (SYMTAB.find(line.operand) != SYMTAB.end()) {
                    address = SYMTAB[line.operand];
                } else {
                    address = parseHex(line.operand, line.lineNumber);
                }

                if (address > 0xFFFF) throw AssemblerException(line.lineNumber, "16-bit overflow in operand");

                int lowByte = address & 0xFF;
                int highByte = (address >> 8) & 0xFF;

                stringstream ss;
                ss << " " << setfill('0') << setw(2) << hex << uppercase << lowByte << " ";
                ss << setfill('0') << setw(2) << hex << uppercase << highByte;
                machineCode += ss.str();
            }
  
            cout << right << setfill('0') << setw(4) << hex << uppercase << line.locationCounter 
                 << "\t" 
                 
                 << left << setfill(' ') << setw(10) << machineCode 
                 << "\t" << (line.label.empty() ? "      " : line.label + ":") 
                 << "\t" << line.mnemonic << " " << line.operand << endl;
        }
    }
};

int main() {
    
    vector<string> sourceCode = {
        "START: MVI A, 32H   ; Load 32 Hex into Accumulator (2 bytes)",
        "       MVI B, 14H   ; Load 14 Hex into Register B (2 bytes)",
        "       ADD B        ; Add B to A (1 byte)",
        "       STA 2050H    ; Store result at memory 2050H (3 bytes)",
        "       JMP END      ; Jump to END label (3 bytes)",
        "       MOV A,B      ; Should be skipped by jump",
        "END:   HLT          ; Halt execution (1 byte)"
    };
    Assembler8085 assembler;
    try {
        assembler.pass1(sourceCode);
        assembler.pass2();
    } 
    catch (const AssemblerException& e) {
        cerr << "\n[ASSEMBLER FATAL ERROR] " << e.what() << endl;
    }

    return 0;
}