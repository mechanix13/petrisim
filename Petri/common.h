#pragma once

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm> 
#include <functional>
#include <cctype>
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

// TODO: global true and false state nodes

struct token {
    std::string item;
    size_t      line,
			    line_orig,
			    pos,
			    pos_orig;
};

std::vector<Gate*> gates;
std::vector<State*> states;

State*      in;
State*      out;
AlwaysGate* action;
State* TrueCondition;
State* FalseCondition;

void ltrim(std::string &s)
{
    s.erase(s.begin(), std::find_if(s.begin(), s.end(),
	    std::not1(std::ptr_fun<int, int>(std::isspace))));
}

void clear_at_left(std::string &line) {
    size_t pos = line.find_first_not_of(" \t\n");

    if(!pos)
        return;

    if(pos == std::string::npos) {
        line.erase();
        return;
    }

    line.erase(line.begin(), line.begin() + pos);
}

void clear_at_right(std::string &line) {
    size_t pos = line.find_last_not_of(" \t\n");

    if(pos == std::string::npos) {
        line.erase();
        return;
    }

    if(pos != line.length())
        line.erase(line.begin() + pos + 1, line.end());
}

std::vector<token> tokenize(const char* fileName) {
    std::vector<token> tokens;

    FILE *p_file = fopen(fileName, "rt");
    if(!p_file)
        return tokens;
    
    char buffer[4096];
    std::string line;
    token       tk;
    
    size_t line_no = 0, token_no = 0, pos = 0;
    
    while(!feof(p_file)) {
        fgets(buffer, 4095, p_file);
        line = buffer;
        
        ++line_no;
        token_no = 0;
        
        clear_at_left(line);
        clear_at_right(line);
        
        if(line.empty())
            continue;
        
        while(!line.empty()) {
            clear_at_left(line);
            
            ++token_no;
            tk.line = tk.line_orig = line_no;
            tk.pos = tk.pos_orig = token_no;
            pos = line.find_first_of(" \t\n\"()+-*/=.,[]{}#@!~;`\\");
            if(pos == std::string::npos) {
                tk.item = line;
                tokens.push_back(tk);
                break;
            }
            if(pos == 0) {
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
    for(size_t i = 0; i < tokens.size() - 1; ++i) {
        // TopGun:
        // Here we should also check line numbers and so on to be sure that this is a comment
        // But it's not implemented yet.
        // This solution has a huge bug: I can't find any difference between '//' and '/ /' :(
        if(tokens[i].item == "/" && tokens[i + 1].item == "/") {
            tokens[i + 1].item = "//";
            tokens.erase(tokens.begin() + i);
        }
        if(tokens[i].item == "/" && tokens[i + 1].item == "*") {
            tokens[i + 1].item = "/*";
            tokens.erase(tokens.begin() + i);
        }
        if(tokens[i].item == "*" && tokens[i + 1].item == "/") {
            tokens[i + 1].item = "*/";
            tokens.erase(tokens.begin() + i);
        }
        if(tokens[i].item == "=" && tokens[i + 1].item == "=") {
            tokens[i + 1].item = "==";
            tokens.erase(tokens.begin() + i);
        }
        if(tokens[i].item == "&" && tokens[i + 1].item == "&") {
            tokens[i + 1].item = "&&";
            tokens.erase(tokens.begin() + i);
        }
        if(tokens[i].item == "|" && tokens[i + 1].item == "|") {
            tokens[i + 1].item = "||";
            tokens.erase(tokens.begin() + i);
        }
        if(tokens[i].item == ">" && tokens[i + 1].item == "=") {
            tokens[i + 1].item = ">=";
            tokens.erase(tokens.begin() + i);
        }
        if(tokens[i].item == "<" && tokens[i + 1].item == "=") {
            tokens[i + 1].item = "<=";
            tokens.erase(tokens.begin() + i);
        }
        if(tokens[i].item == "!" && tokens[i + 1].item == "=") {
            tokens[i + 1].item = "!=";
            tokens.erase(tokens.begin() + i);
        }
    }
    
    // Remove comments from file
    for(size_t i = 0; i < tokens.size(); ++i) {
        if(tokens[i].item == "//") {
            line_no = tokens[i].line;
            while(tokens[i].line == line_no && i <= tokens.size())
                tokens.erase(tokens.begin() + i);
            --i;
            continue;
        }
        if(tokens[i].item == "/*") {
            while(tokens[i].item != "*/" && i <= tokens.size())
                tokens.erase(tokens.begin() + i);
            --i;
            continue;
        }
    }
    
    // Merge strings by "\" at the end of line
    for (size_t i = 0; i < tokens.size(); ++i) {
        if (tokens[i].item != "\\")
            continue;
        if (tokens[i].line == tokens[i + 1].line)
            continue;
        size_t line_1 = tokens[i].line;
        size_t line_2 = tokens[i + 1].line;
        size_t pos = tokens[i].pos;

        size_t j = i + 1;
        while (line_2 == tokens[j].line) {
            tokens[j].line = line_1;
            tokens[j].pos = pos++;
            ++j;
        }
        tokens.erase(tokens.begin() + i);
        i = j - 2;
    }

    return tokens;
}

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

void ReadFile(const char* fileName)
{
    State* stateBuffer;
    Gate*  gateBuffer;
    std::vector<token> tokens = tokenize(fileName);
    for (int i = 0; i < tokens.size(); i++)
    {
	    std::cout << "(" << tokens[i].line << ":" << tokens[i].pos << ") : " << tokens[i].item << std::endl;
    }
	
    // TODO: walk around tokens
    for (int i = 0; i < tokens.size(); i++)
    {
	    if (tokens[i].item == "input")
	    {
		    stateBuffer = new State(tokens[i + 1].item);
		    states.push_back(stateBuffer);
	    }
	    if (tokens[i].item == "output")
	    {
		    stateBuffer = new State(tokens[i + 1].item);
		    states.push_back(stateBuffer);
	    }
	    if (tokens[i].item == "initial")
	    {
		    findState(tokens[i + 1].item)->Condition = atoi(tokens[i + 3].item.c_str());
	    }
	    if (tokens[i].item == "always")
	    {
		    i += 2;
		    stateBuffer = new State("alwaysOut");
		    states.push_back(stateBuffer);
		    gateBuffer = new AlwaysGate(findState(tokens[i].item), stateBuffer);
		    i += 2;
			
		    if (tokens[i].item == "begin")
		    {
			    i++;
			    while(tokens[i].item != "end")
			    {
				    if (tokens[i].item == "if")
				    {
					    i += 2;
					    states.push_back(new State("ifTrue"));
					    states.push_back(new State("ifFalse"));

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
							    states[states.size() - 2],
							    states[states.size() - 1]
						    );
					    }

					    if (tokens[i + 1].item == "!=")
					    {
						    gateBuffer = new IfNEqGate(
							    findState(tokens[i].item),
							    stateBuffer,
							    states[states.size() - 2],
							    states[states.size() - 1]
						    );
					    }
					    gates.push_back(gateBuffer);
                        i += 3;
                        if (tokens[i].item == "then")
                        {
                            if (tokens[i + 1].item != "begin")
                            {
                                i++;
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

                                gateBuffer = new AssignGate(
                                    findState(tokens[i].item),
                                    stateBuffer,
                                    gates[gates.size() - 1]->TrueSignal);
                            }
                        }
                        if (tokens[i].item == "else")
                        {
                            if (tokens[i + 1].item != "begin")
                            {
                                i++;
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

                                gateBuffer = new AssignGate(
                                    findState(tokens[i].item),
                                    stateBuffer,
                                    gates[gates.size() - 1]->FalseSignal);
                            }
                        }
				    }
				    i++;
			    }
		    }
	    }
    }
}

void Model(int modelTime, int timeStep)
{
    action->InitState();
    std::cout << std::setw(3) << "in " << std::setw(3) << "out" << std::endl;
    std::cout << "______" << std::endl;
    for (int i = 1; i <= modelTime; i += timeStep)
    {
	    if (i % 10 == 0)
	    {
		    in->Condition = !in->Condition;
	    }

	    action->UpdateState();
	    std::cout << std::setw(3) << in->Condition << std::setw(3) << out->Condition << std::endl;
    }
}
