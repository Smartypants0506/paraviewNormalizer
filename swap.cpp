#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

void swapColumns(const std::string& inputFileName, const std::string& outputFileName) {
    std::ifstream inputFile(inputFileName);
    std::ofstream outputFile(outputFileName);

    if (!inputFile.is_open()) {
        std::cerr << "Error: Unable to open input file." << std::endl;
        return;
    }

    if (!outputFile.is_open()) {
        std::cerr << "Error: Unable to open output file." << std::endl;
        return;
    }

    std::string line;
    while (std::getline(inputFile, line)) {
        std::stringstream ss(line);
        std::vector<std::string> columns;
        std::string cell;

        // Read all columns in the line
        while (std::getline(ss, cell, ',')) {
            columns.push_back(cell);
        }

        // Ensure there are at least 3 columns
        if (columns.size() < 3) {
            std::cerr << "Error: Line with fewer than 3 columns encountered." << std::endl;
            continue;
        }

        // Swap the first two columns
        std::swap(columns[0], columns[1]);

        // Write the modified line to the output file
        for (size_t i = 0; i < columns.size(); ++i) {
            outputFile << columns[i];
            if (i < columns.size() - 1) {
                outputFile << ",";
            }
        }
        outputFile << "\n";
    }

    inputFile.close();
    outputFile.close();
}

int main() {
    std::string inputFileName = "uncAhhDirectory/master.csv"; // Set the input file path here
    std::string outputFileName = "uncAhhDirectory/masterFixed.csv"; // Set the output file path here

    swapColumns(inputFileName, outputFileName);

    std::cout << "Column swapping completed. Check the output file." << std::endl;

    return 0;
}
