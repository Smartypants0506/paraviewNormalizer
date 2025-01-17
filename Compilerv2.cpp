#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

void appendCSV(const std::string& file1, const std::string& file2, const std::string& outputFile) {
    std::ifstream inputCSV(file1);
    std::ifstream singleColumnFile(file2);
    std::ofstream outputCSV(outputFile);

    if (!inputCSV.is_open() || !singleColumnFile.is_open()) {
        std::cerr << "Error: Unable to open input files." << std::endl;
        return;
    }
    if (!outputCSV.is_open()) {
        std::cerr << "Error: Unable to open output file." << std::endl;
        return;
    }

    std::string csvLine, singleColumnValue;
    while (std::getline(inputCSV, csvLine) && std::getline(singleColumnFile, singleColumnValue)) {
        // Append the value from the single-column file to the current CSV line
        outputCSV << csvLine << "," << singleColumnValue << "\n";
    }

    // Check if there are mismatched rows
    if (inputCSV.good() || singleColumnFile.good()) {
        std::cerr << "Error: Files have a different number of rows." << std::endl;
    }

    inputCSV.close();
    singleColumnFile.close();
    outputCSV.close();

    std::cout << "Files merged successfully into " << outputFile << std::endl;
}

int main() {
    // File paths can be passed here
    std::string file1 = "uncAhhDirectory/master.csv"; // Replace with the path to your first CSV file
    std::string file2 = "uncAhhDirectory/Column5.csv"; // Replace with the path to your single-column file
    std::string outputFile = "uncAhhDirectory/master5.csv";   // Replace with the desired output file path

    appendCSV(file1, file2, outputFile);

    return 0;
}
