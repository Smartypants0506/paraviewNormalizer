#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

int main() {
    const int rows = 145;
    const int cols = 96;

    // Input file (replace "input.csv" with your actual file path)
    std::ifstream inputFile("uncAhhDirectory/Falcon5Scaled.csv");

    if (!inputFile.is_open()) {
        std::cerr << "Error: Could not open input file.\n";
        return 1;
    }

    // Vector to hold the reshaped data
    std::vector<std::string> reshapedData;
    std::string line;

    // Read data from the CSV file row by row
    while (std::getline(inputFile, line)) {
        std::stringstream ss(line);
        std::string value;

        // Split the line by commas
        while (std::getline(ss, value, ',')) {
            reshapedData.push_back(value); // Append value to the 1D vector
        }
    }

    inputFile.close();

    // Output file (replace "output.csv" with your desired file path)
    std::ofstream outputFile("uncAhhDirectory/Column5.csv");

    if (!outputFile.is_open()) {
        std::cerr << "Error: Could not open output file.\n";
        return 1;
    }

    // Write the reshaped data to the output file
    for (const auto& value : reshapedData) {
        outputFile << value << "\n";
    }

    outputFile.close();

    std::cout << "Dataset reshaped and written to output.csv successfully.\n";

    return 0;
}
