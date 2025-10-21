#include "inference.h"
#include <algorithm>
#include <functional>
#include <iostream>

double joint_probability(const std::vector<Variable>& vars,
                         const std::unordered_map<std::string,std::string>& assign)
{
    double p = 1.0;
    for(const auto& var : vars) {
        int row = 0;
        int nParents = var.parents.size();
        if(nParents > 0 && var.cpt.size() > 1) {
            int idx = 0;
            for(int i=0;i<nParents;++i){
                const auto& parent = var.parents[i];
                const auto& parentVal = assign.at(parent);
                const auto& parentVar = *std::find_if(vars.begin(), vars.end(),
                    [&](const Variable& v){ return v.name == parent; });
                int parentIndex = std::find(parentVar.values.begin(), parentVar.values.end(), parentVal) - parentVar.values.begin();
                idx = idx * parentVar.values.size() + parentIndex;
            }
            row = idx;
        }
        const auto& varValue = assign.at(var.name);
        int col = std::find(var.values.begin(), var.values.end(), varValue) - var.values.begin();
        p *= var.cpt[row][col];
    }
    return p;
}

double marginal_probability(const std::vector<Variable>& vars,
                            const std::string& target,
                            const std::string& value)
{
    std::vector<std::unordered_map<std::string,std::string>> allAssignments;

    std::function<void(int,std::unordered_map<std::string,std::string>)> dfs =
    [&](int i,std::unordered_map<std::string,std::string> current){
        if(i==vars.size()){
            if(current[target]==value)
                allAssignments.push_back(current);
            return;
        }
        for(auto& v : vars[i].values){
            current[vars[i].name]=v;
            dfs(i+1,current);
        }
    };

    dfs(0,{});

    double total = 0.0;
    for(auto& a : allAssignments)
        total += joint_probability(vars,a);

    return total;
}

void generate_parent_assignments(const std::vector<Variable>& vars,
                                 const std::vector<std::string>& parents,
                                 std::vector<std::unordered_map<std::string,std::string>>& assignments,
                                 std::unordered_map<std::string,std::string> current,
                                 int i)
{
    if(i==parents.size()){
        assignments.push_back(current);
        return;
    }

    const auto& parentVar = *std::find_if(vars.begin(), vars.end(),
        [&](const Variable& v){ return v.name==parents[i]; });

    for(auto& val : parentVar.values){
        current[parents[i]]=val;
        generate_parent_assignments(vars,parents,assignments,current,i+1);
    }
}
