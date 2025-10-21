# Copilot Instructions for Bayesian Network Project

## Project Overview
This project parses and analyzes Bayesian networks described in BIF (Bayesian Interchange Format). The main focus is on reading `.bif` files, extracting variables, their possible values, and the conditional probability tables (CPTs) for each variable.

## Key Components
- `main.cpp`: Entry point. Loads a BIF file (e.g., `gradient.bif`) and prints variable names and values using the parser.
- `parser.h` / `parser.cpp`: Defines the `Variable` struct and implements `parse_bif`, which parses BIF files and returns a vector of `Variable` objects. Handles variable names, values, and (potentially) parent relationships and CPTs.
- `gradient.bif`: Example BIF file with a small Bayesian network (variables, their values, and probability tables).
- `bayes.h` / `bayes.cpp`: (Currently empty) Intended for Bayesian network logic, likely to be implemented for inference, learning, or manipulation of parsed networks.

## Data Flow
1. `main.cpp` calls `parse_bif` with a BIF file path.
2. `parse_bif` reads the file, extracts variables and their values, and returns them.
3. Output is printed to the console.

## Build & Run
- No build scripts or Makefiles are present. To compile, use:
  ```sh
  g++ -std=c++11 -o bayes main.cpp parser.cpp
  ./bayes
  ```
- The code expects `gradient.bif` to be present in the working directory.

## Conventions & Patterns
- Variables are represented by the `Variable` struct (see `parser.h`).
- Parsing logic is in `parser.cpp`, with a focus on extracting variable names and values. CPT and parent parsing may be incomplete or pending.
- Use C++ STL containers (`std::vector`, `std::string`, `std::unordered_map`).
- Error handling is minimal (prints to `std::cerr`).

## Extending the Project
- Implement Bayesian network logic in `bayes.cpp`/`bayes.h`.
- Extend `parse_bif` to fully parse parent relationships and CPTs.
- Add build automation (e.g., Makefile) and tests as needed.

## Example Usage
```cpp
#include "parser.h"
int main() {
    auto vars = parse_bif("gradient.bif");
    // ...
}
```

## Integration Points
- No external libraries or dependencies are used.
- All logic is currently self-contained.

---
For any AI agent: Focus on the parsing logic, the structure of the `Variable` type, and the flow from file input to variable extraction. If implementing inference or learning, use `bayes.cpp`/`bayes.h` for new logic.
