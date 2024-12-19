#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>

// Function to read a CSV file into a 2D vector
std::vector<std::vector<std::string>> readCSV(const std::string& filename) {
    std::vector<std::vector<std::string>> data;
    std::ifstream file(filename);
    std::string line;

    while (std::getline(file, line)) {
        std::vector<std::string> row;
        std::stringstream ss(line);
        std::string cell;

        while (std::getline(ss, cell, ',')) {
            row.push_back(cell);
        }

        data.push_back(row);
    }

    return data;
}

// Function to rotate a 2D vector 90 degrees counterclockwise
std::vector<std::vector<std::string>> rotateCounterclockwise(const std::vector<std::vector<std::string>>& data) {
    size_t rows = data.size();
    size_t cols = data[0].size();
    std::vector<std::vector<std::string>> rotated(cols, std::vector<std::string>(rows));

    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            rotated[cols - j - 1][i] = data[i][j];
        }
    }

    return rotated;
}

// Function to write a 2D vector to a CSV file
void writeCSV(const std::string& filename, const std::vector<std::vector<std::string>>& data) {
    std::ofstream file(filename);

    for (const auto& row : data) {
        for (size_t i = 0; i < row.size(); ++i) {
            file << row[i];
            if (i < row.size() - 1) {
                file << ',';
            }
        }
        file << '\n';
    }
}

int main() {
    std::string inputFile = "C:/Users/banta/CLionProjects/paraviewNormalizer/input.csv";
    std::string outputFile = "C:/Users/banta/CLionProjects/paraviewNormalizer/output.csv";

    // Read the input CSV
    auto data = readCSV(inputFile);

    // Ensure the input is 12x18
    if (data.size() != 12 || data[0].size() != 18) {
        std::cerr << "Error: Input CSV must be a 12x18 grid." << std::endl;
        return 1;
    }

    // Rotate the data counterclockwise
    auto rotatedData = rotateCounterclockwise(data);

    // Write the rotated data to the output CSV
    writeCSV(outputFile, rotatedData);

    std::cout << "Rotation complete. Output written to " << outputFile << std::endl;

    return 0;
}