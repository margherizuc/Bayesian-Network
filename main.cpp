#include "parser.h"
#include "inference.h"
#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <algorithm> // Necessario per std::find_if

int main() {
    auto vars = parse_bif("gradient.bif");

    for (const auto& v : vars) {
        if (v.parents.empty()) {
            // --- Nodi senza genitori (radici) ---
            // Le probabilità sono già nella prima (e unica) riga della CPT.
            // È molto più efficiente leggerle direttamente che ricalcolare la marginale.
            std::cout << "P(" << v.name << "):\n";
            for (size_t i = 0; i < v.values.size(); ++i) {
                std::cout << "  " << v.values[i] << " -> " << v.cpt[0][i] << "\n";
            }
            std::cout << "\n";
        } else {
            // --- Nodi con genitori ---
            std::cout << "P(" << v.name << " | ";
            for (size_t i = 0; i < v.parents.size(); ++i) {
                if (i > 0) std::cout << ", ";
                std::cout << v.parents[i];
            }
            std::cout << "):\n";

            std::vector<std::unordered_map<std::string, std::string>> parentAssigns;
            generate_parent_assignments(vars, v.parents, parentAssigns);

            for (const auto& assign : parentAssigns) {
                std::cout << "  ";
                for (size_t i = 0; i < v.parents.size(); ++i) {
                    if (i > 0) std::cout << ", ";
                    std::cout << v.parents[i] << "=" << assign.at(v.parents[i]);
                }
                std::cout << ": ";

                // Calcola la riga corretta nella CPT basandoti sull'assegnazione dei genitori.
                // Questa logica è la stessa usata in joint_probability per trovare la riga.
                int row_idx = 0;
                for (const auto& parent_name : v.parents) {
                    const auto& parent_val = assign.at(parent_name);
                    
                    // Trova la variabile genitore per sapere quanti valori ha
                    const auto& parentVar = *std::find_if(vars.begin(), vars.end(),
                        [&](const Variable& var_to_find){ return var_to_find.name == parent_name; });

                    // Trova l'indice del valore del genitore
                    int parent_val_idx = std::find(parentVar.values.begin(), parentVar.values.end(), parent_val) - parentVar.values.begin();
                    
                    row_idx = row_idx * parentVar.values.size() + parent_val_idx;
                }

                // Stampa le probabilità direttamente dalla riga calcolata della CPT
                for (size_t col_idx = 0; col_idx < v.values.size(); ++col_idx) {
                    std::cout << v.values[col_idx] << " -> " << v.cpt[row_idx][col_idx];
                    if (col_idx < v.values.size() - 1) std::cout << ", ";
                }
                std::cout << "\n";
            }
            std::cout << "\n";
        }
    }

    return 0;
}