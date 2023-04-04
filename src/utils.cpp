#include "utils.h"

void readGraph(Graph* g, string dataset_path) {
    
    ifstream file(dataset_path.c_str());
    if (!file.is_open()) {
        cerr << "Error opening file.\n";
    }
    
	string line;
    while (getline(file, line)) {
        stringstream ss(line);
        int num1, num2;
        char comma;
        if (ss >> num1 >> comma >> num2) {
            g->add_edge(num1, num2);
        }
    }

    file.close();
}
