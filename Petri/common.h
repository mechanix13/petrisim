#pragma once

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm> 
#include <functional>
#include <cctype>
#include <ctime>
#include <locale>
#include <iomanip>

#include "common_types.h"
#include "State.h"
#include "Gate.h"
#include "IfEqGate.h"
#include "IfNEqGate.h"
#include "AssignGate.h"
#include "AlwaysGate.h"
#include "ActionGate.h"

const char* dumpFile = "output.vcd";
const char codes[10] = "!\"#$%&'()";
std::string moduleName;

struct token {
    std::string item;
    size_t      line,
                line_orig,
                pos,
                pos_orig;
};

State* stateBuffer;
Gate*  gateBuffer;
std::vector<token> tokens;

std::vector<Gate*> gates;
std::vector<State*> states;
std::vector<State*> inputStates;
std::vector<State*> outputStates;

State*      in;
State*      out;
AlwaysGate* action;
State* TrueCondition;
State* FalseCondition;

State* findState(std::string name)
{
    for (int i = 0; i < states.size(); i++)
    {
        if (states[i]->Name == name)
        {
            return states[i];
        }
    }

    return NULL;
}
void readAssign(int& i, const char* name, State* inputSignal)
{
    State* lhs = findState(tokens[i].item);
    bool inv = false;

    if (tokens[i + 2].item == "!")
    {
        inv = true;
        i++;
    }
    if (tokens[i + 2].item == "0")
    {
        stateBuffer = FalseCondition;
    }
    else if (tokens[i + 2].item == "1")
    {
        stateBuffer = TrueCondition;
    }
    else
    {
        stateBuffer = findState(tokens[i + 2].item);
    }

    states.push_back(new State(name));
    gateBuffer = new AssignGate(
        lhs,
        stateBuffer,
        inputSignal,
        states[states.size() - 1],
        inv);
    gates.push_back(gateBuffer);
}

void readIf(int& i, State* signal)
{
    i += 2;

    State* TrueSignal = new State("ifTrue");
    states.push_back(TrueSignal);
    State* FalseSignal = new State("ifFalse");
    states.push_back(FalseSignal);

    if (tokens[i + 2].item == "0")
    {
        stateBuffer = FalseCondition;
    }
    else if (tokens[i + 2].item == "1")
    {
        stateBuffer = TrueCondition;
    }
    else
    {
        stateBuffer = findState(tokens[i + 2].item);
    }

    if (tokens[i + 1].item == "==")
    {						
        gateBuffer = new IfEqGate(
            findState(tokens[i].item),
            stateBuffer,
            signal,
            states[states.size() - 2],
            states[states.size() - 1]
        );
    }

    if (tokens[i + 1].item == "!=")
    {
        gateBuffer = new IfNEqGate(
            findState(tokens[i].item),
            stateBuffer,
            signal,
            states[states.size() - 2],
            states[states.size() - 1]
        );
    }
    gates.push_back(gateBuffer);
    i += 4;
    if (tokens[i].item == "then")
    {
        if (tokens[i + 1].item != "begin")
        {
            i++;
            if (tokens[i + 1].item == "=")
            {
                readAssign(i, "thenAssign", TrueSignal);
                i += 3;
            }
            if (tokens[i].item == "if")
            {
                readIf(i, TrueSignal);
            }
            if (tokens[i + 1].item == "else")
            {
                i += 1;
            }
        }
    }
    if (tokens[i].item == "else")
    {
        if (tokens[i + 1].item != "begin")
        {
            i++;
            if (tokens[i + 1].item == "=")
            {
                readAssign(i, "elseAssign", FalseSignal);
                i += 3;
            }
            if (tokens[i + 1].item == "if")
            {
                i++;
                readIf(i, FalseSignal);
            }
        }
    }
}

void initDumpFile(std::fstream& dump)
{
    if (!dump.is_open())
    {
        return;
    }

    time_t currentTime;
    time(&currentTime);

    dump << "$date\n\t" << ctime(&currentTime) << "$end\n";
    dump << "$version\n\t" << "Petri Behavioral Simulation" << "\n$end\n";
    dump << "$timescale\n\t" << "1ns" << "\$end\n";
    dump << "$scope module " << moduleName << " $end\n";

    for (int i = 0; i < inputStates.size(); i++)
    {
        if (inputStates[i]->Name == "@")
        {
            continue;
        }
        dump << "$var wire 1 " << codes[i] << " " << inputStates[i]->Name << " $end\n";
    }
    for (int i = 0; i < outputStates.size(); i++)
    {
        dump << "$var wire 1 " << codes[inputStates.size() + i] << " " << outputStates[i]->Name << " $end\n";
    }
    
    dump << "$upscope $end\n";
    dump << "$enddefinitions $end\n";
}

void dumpInitValues(std::fstream& dump)
{
    dump << "#0\n$dumpvars\n";

    for (int i = 0; i < inputStates.size(); i++)
    {
        if (inputStates[i]->Name == "@")
        {
            continue;
        }

        dump << (int)inputStates[i]->Condition << codes[i] << "\n";
    }
    for (int i = 0; i < outputStates.size(); i++)
    {
        dump << (int)outputStates[i]->Condition << codes[inputStates.size() + i] << "\n";
    }

    dump << "$end\n";
}

void clear_at_left(std::string &line)
{
    size_t pos = line.find_first_not_of(" \t\n");

    if(!pos)
        return;

    if(pos == std::string::npos)
    {
        line.erase();
        return;
    }

    line.erase(line.begin(), line.begin() + pos);
}

void clear_at_right(std::string &line)
{
    size_t pos = line.find_last_not_of(" \t\n");

    if(pos == std::string::npos)
    {
        line.erase();
        return;
    }

    if(pos != line.length())
        line.erase(line.begin() + pos + 1, line.end());
}

std::vector<token> tokenize(const char* fileName)
{
    std::vector<token> tokens;

    FILE *p_file = fopen(fileName, "rt");
    if(!p_file)
        return tokens;
    
    char buffer[4096];
    std::string line;
    token       tk;
    
    size_t line_no = 0, token_no = 0, pos = 0;
    
    while(!feof(p_file))
    {
        fgets(buffer, 4095, p_file);
        line = buffer;
        
        ++line_no;
        token_no = 0;
        
        clear_at_left(line);
        clear_at_right(line);
        
        if(line.empty())
            continue;
        
        while(!line.empty())
        {
            clear_at_left(line);
            
            ++token_no;
            tk.line = tk.line_orig = line_no;
            tk.pos = tk.pos_orig = token_no;
            pos = line.find_first_of(" \t\n\"()+-*/=.,[]{}#@!~;`\\");
            if(pos == std::string::npos)
            {
                tk.item = line;
                tokens.push_back(tk);
                break;
            }
            if(pos == 0)
            {
                tk.item = line.substr(0, 1);
                tokens.push_back(tk);
                line.erase(line.begin(), line.begin() + 1);
                continue;
            }
            tk.item = line.substr(0, pos);
            tokens.push_back(tk);
            line.erase(line.begin(), line.begin() + pos);
        }
    }
    fclose(p_file);
    if(tokens[tokens.size() - 1].item == tokens[tokens.size() - 2].item)
        tokens.pop_back();
    
    // Find composite operands
    for(size_t i = 0; i < tokens.size() - 1; ++i)
    {
        // TopGun:
        // Here we should also check line numbers and so on to be sure that this is a comment
        // But it's not implemented yet.
        // This solution has a huge bug: I can't find any difference between '//' and '/ /' :(
        if(tokens[i].item == "/" && tokens[i + 1].item == "/")
        {
            tokens[i + 1].item = "//";
            tokens.erase(tokens.begin() + i);
        }
        if(tokens[i].item == "/" && tokens[i + 1].item == "*")
        {
            tokens[i + 1].item = "/*";
            tokens.erase(tokens.begin() + i);
        }
        if(tokens[i].item == "*" && tokens[i + 1].item == "/")
        {
            tokens[i + 1].item = "*/";
            tokens.erase(tokens.begin() + i);
        }
        if(tokens[i].item == "=" && tokens[i + 1].item == "=")
        {
            tokens[i + 1].item = "==";
            tokens.erase(tokens.begin() + i);
        }
        if(tokens[i].item == "&" && tokens[i + 1].item == "&")
        {
            tokens[i + 1].item = "&&";
            tokens.erase(tokens.begin() + i);
        }
        if(tokens[i].item == "|" && tokens[i + 1].item == "|")
        {
            tokens[i + 1].item = "||";
            tokens.erase(tokens.begin() + i);
        }
        if(tokens[i].item == ">" && tokens[i + 1].item == "=")
        {
            tokens[i + 1].item = ">=";
            tokens.erase(tokens.begin() + i);
        }
        if(tokens[i].item == "<" && tokens[i + 1].item == "=")
        {
            tokens[i + 1].item = "<=";
            tokens.erase(tokens.begin() + i);
        }
        if(tokens[i].item == "!" && tokens[i + 1].item == "=")
        {
            tokens[i + 1].item = "!=";
            tokens.erase(tokens.begin() + i);
        }
    }
    
    // Remove comments from file
    for(size_t i = 0; i < tokens.size(); ++i)
    {
        if(tokens[i].item == "//")
        {
            line_no = tokens[i].line;
            while(tokens[i].line == line_no && i <= tokens.size())
                tokens.erase(tokens.begin() + i);
            --i;
            continue;
        }
        if(tokens[i].item == "/*")
        {
            while(tokens[i].item != "*/" && i <= tokens.size())
                tokens.erase(tokens.begin() + i);
            --i;
            continue;
        }
    }
    
    // Merge strings by "\" at the end of line
    for (size_t i = 0; i < tokens.size(); ++i)
    {
        if (tokens[i].item != "\\")
            continue;
        if (tokens[i].line == tokens[i + 1].line)
            continue;
        size_t line_1 = tokens[i].line;
        size_t line_2 = tokens[i + 1].line;
        size_t pos = tokens[i].pos;

        size_t j = i + 1;
        while (line_2 == tokens[j].line)
        {
            tokens[j].line = line_1;
            tokens[j].pos = pos++;
            ++j;
        }
        tokens.erase(tokens.begin() + i);
        i = j - 2;
    }

    return tokens;
}

void ReadFile(const char* fileName)
{
    State* stateBuffer;
    Gate*  gateBuffer;
    tokens = tokenize(fileName);
    State* lastSignal;
    
    for (int i = 0; i < tokens.size(); i++)
    {
        if (tokens[i].item == "module")
        {
            moduleName = tokens[i + 1].item;
        }
        if (tokens[i].item == "input")
        {
            stateBuffer = new State(tokens[i + 1].item);
            in = stateBuffer;
            states.push_back(stateBuffer);
            inputStates.push_back(in);
        }
        if (tokens[i].item == "output")
        {
            stateBuffer = new State(tokens[i + 1].item);
            out = stateBuffer;
            states.push_back(stateBuffer);
            outputStates.push_back(out);
        }
        if (tokens[i].item == "initial")
        {
            findState(tokens[i + 1].item)->Condition = atoi(tokens[i + 3].item.c_str());
        }
        if (tokens[i].item == "always")
        {
            i += 2;

            int alwaysMode = MODE_CHANGE;
            if (tokens[i].item == "posedge")
            {
                alwaysMode = MODE_POSEDGE;
                i++;
            }
            if (tokens[i].item == "negedge")
            {
                alwaysMode = MODE_NEGEDGE;
                i++;
            }

            if (tokens[i].item == "@")
            {
                stateBuffer = new State("@");
                states.push_back(stateBuffer);
                inputStates.push_back(stateBuffer);
            }

            stateBuffer = new State("alwaysOut");
            states.push_back(stateBuffer);

            gateBuffer = new AlwaysGate(findState(tokens[i].item), stateBuffer, alwaysMode);
            gates.push_back(gateBuffer);
            i += 2;
            
            lastSignal = stateBuffer;
            if (tokens[i].item == "begin")
            {
                i++;
                while(tokens[i].item != "end")
                {
                    if (tokens[i].item == "if")
                    {
                       readIf(i, lastSignal);
                    }

                    if (tokens[i].item == "=")
                    {
                        i--;
                        readAssign(i, "assignation", lastSignal);
                        i += 3;
                    }

                    lastSignal = gates[gates.size() - 1]->OutputState;
                    i++;
                }
            }
        }
    }
}


void init()
{
    for (int i = 0; i < gates.size(); i++)
    {
        gates[i]->InitState();
    }
}

void update()
{
    for (int i = 0; i < gates.size(); i++)
    {
        gates[i]->UpdateState();
    }
}

void Model(int modelTime, int timeStep)
{
    init();

    std::fstream dump;
    dump.open(dumpFile, std::ios_base::out);

    initDumpFile(dump);
    dumpInitValues(dump);

    std::cout << "Driving simulation..." << std::endl;

    for (int i = 1; i <= modelTime; i += timeStep)
    {
        dump << "#" << i << "\n";
        for (int k = 0; k < inputStates.size(); k++)
        {
            if (inputStates[k]->Name == "@")
            {
                inputStates[k]->Condition = !inputStates[k]->Condition;
            }
        }
        if (i % 3 == 0)
        {
            inputStates[0]->Condition = !inputStates[0]->Condition;
        }
        if (i % 6 == 0)
        {
            inputStates[1]->Condition = !inputStates[1]->Condition;
        }
        update();
        for (int i = 0; i < inputStates.size(); i++)
        {
            if (inputStates[i]->Name == "@")
            {
                continue;
            }
            dump << (int)inputStates[i]->Condition << codes[i] << "\n";
        }
        for (int i = 0; i < outputStates.size(); i++)
        {
            dump << (int)outputStates[i]->Condition << codes[inputStates.size() + i] << "\n";
        }
    }
    std::cout << "Done" << std::endl;
    dump.close();
}
