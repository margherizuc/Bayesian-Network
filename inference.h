#ifndef INFERENCE_H
#define INFERENCE_H

#include "parser.h"
#include <unordered_map>
#include <string>
#include <vector>

// Dichiarazioni
double joint_probability(const std::vector<Variable>& vars,
                         const std::unordered_map<std::string,std::string>& assign);

double marginal_probability(const std::vector<Variable>& vars,
                            const std::string& target,
                            const std::string& value);

void generate_parent_assignments(const std::vector<Variable>& vars,
                                 const std::vector<std::string>& parents,
                                 std::vector<std::unordered_map<std::string,std::string>>& assignments,
                                 std::unordered_map<std::string,std::string> current = {},
                                 int i = 0);

#endif
