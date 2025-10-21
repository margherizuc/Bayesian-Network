#include "parser.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

std::vector<std::vector<int>> adj;
std::unordered_map<std::string,int> name_to_id;

// pulizia stringa: rimuove spazi iniziali/finali
std::string clean_name(const std::string& s) {
    std::string res = s;
    res.erase(std::remove_if(res.begin(), res.end(), ::isspace), res.end());
    return res;
}

std::vector<Variable> parse_bif(const std::string& filename) {
    std::ifstream file(filename);
    if(!file.is_open()) { std::cerr << "Errore apertura file\n"; return {}; }

    std::vector<Variable> vars;
    Variable current;
    bool inVariable = false, inProbability = false;
    std::string line;
    std::string target;
    std::vector<std::string> parents;

    while(std::getline(file, line)) {
        // Rimuove spazi iniziali/finali
        line.erase(0, line.find_first_not_of(" \t"));
        line.erase(line.find_last_not_of(" \t") + 1);

        // --- Blocchi variable ---
        if(line.find("variable") != std::string::npos) {
            inVariable = true;
            current = Variable{};
            std::istringstream iss(line);
            std::string tmp; iss >> tmp >> current.name;
            current.name = clean_name(current.name);
        }
        else if(inVariable && line.find("type discrete") != std::string::npos) {
            size_t open = line.find('{'), close = line.find('}');
            if(open != std::string::npos && close != std::string::npos) {
                std::string inside = line.substr(open+1, close-open-1);
                std::istringstream iss(inside);
                std::string val;
                while(std::getline(iss, val, ',')) {
                    current.values.push_back(clean_name(val));
                }
            }
        }
        else if(inVariable && line.find("}") != std::string::npos) {
            name_to_id[current.name] = vars.size();
            vars.push_back(current);
            inVariable = false;
        }

        // --- Blocchi probability ---
        else if(line.find("probability") != std::string::npos) {
            inProbability = true;
            parents.clear();
            size_t paren_open = line.find('('), paren_close = line.find(')');
            if(paren_open != std::string::npos && paren_close != std::string::npos) {
                std::string inside = line.substr(paren_open+1, paren_close-paren_open-1);
                std::istringstream iss(inside);
                std::string token;
                std::getline(iss, token, '|');
                target = clean_name(token);

                if(inside.find('|') != std::string::npos) {
                    std::getline(iss, token);
                    std::istringstream iss2(token);
                    while(std::getline(iss2, token, ',')) {
                        parents.push_back(clean_name(token));
                    }
                }

                if(name_to_id.find(target) != name_to_id.end())
                    vars[name_to_id[target]].parents = parents;
            }
        }
        else if(inProbability && line.find("}") != std::string::npos) {
            inProbability = false;
        }
        else if(inProbability) {
            if(line.empty()) continue;
            std::vector<double> probs;
            std::istringstream iss(line);
            std::string val;
            while(iss >> val) {
                val.erase(std::remove_if(val.begin(), val.end(),
                    [](char c){ return c=='('||c==')'||c==','||c==';'; }), val.end());
                if(!val.empty() && (isdigit(val[0]) || val[0]=='.')) {
                probs.push_back(std::stod(val));
                }
            }
            if(!probs.empty() && name_to_id.find(target) != name_to_id.end())
                vars[name_to_id[target]].cpt.push_back(probs);
        }
    }

    // costruzione DAG
    adj.resize(vars.size());
    for(int i=0;i<vars.size();++i)
        for(auto& p : vars[i].parents)
            adj[name_to_id[p]].push_back(i);

    return vars;
}

