#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

void flipCSV(const std::string& inputFile, const std::string& outputFile) {
    std::ifstream inFile(inputFile);
    if (!inFile) {
        std::cerr << "Error opening input file: " << inputFile << std::endl;
        return;
    }

    std::ofstream outFile(outputFile);
    if (!outFile) {
        std::cerr << "Error opening output file: " << outputFile << std::endl;
        return;
    }

    std::vector<std::vector<std::string>> data;
    std::string line, cell;

    while (std::getline(inFile, line)) {
        std::istringstream lineStream(line);
        std::vector<std::string> row;
        while (std::getline(lineStream, cell, ',')) {
            row.push_back(cell);
        }
        data.push_back(row);
    }

    for (int i = data.size() - 1; i >= 0; --i) {
        for (int j = 0; j < data[i].size(); ++j) {
            outFile << data[i][j];
            if (j < data[i].size() - 1) {
                outFile << ",";
            }
        }
        outFile << "\n";
    }

    inFile.close();
    outFile.close();
}

int main() {
    std::string inputFile = "C:/Users/banta/CLionProjects/paraviewNormalizer/io/input.csv";
    std::string outputFile = "C:/Users/banta/CLionProjects/paraviewNormalizer/io/output.csv";

    flipCSV(inputFile, outputFile);

    std::cout << "Flipped CSV file created: " << outputFile << std::endl;
    return 0;
}
