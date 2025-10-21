#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <vector>
#include <unordered_map>

struct Variable {
    std::string name;                     // nome della variabile
    std::vector<std::string> values;      // valori possibili
    std::vector<std::string> parents;     // nomi dei genitori
    std::vector<std::vector<double>> cpt; // CPT
};

// Parsing rete bayesiana da file BIF
std::vector<Variable> parse_bif(const std::string& filename);

// DAG globale
extern std::vector<std::vector<int>> adj;
extern std::unordered_map<std::string,int> name_to_id;

#endif
