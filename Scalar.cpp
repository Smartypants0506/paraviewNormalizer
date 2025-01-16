#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

// Function to read a CSV file into a 2D vector
std::vector<std::vector<double>> readCSV(const std::string &filename) {
    std::ifstream file(filename);
    std::vector<std::vector<double>> data;
    std::string line;

    while (std::getline(file, line)) {
        std::vector<double> row;
        std::stringstream ss(line);
        std::string value;

        while (std::getline(ss, value, ',')) {
            row.push_back(std::stod(value));
        }

        data.push_back(row);
    }

    return data;
}

// Function to write a 2D vector to a CSV file
void writeCSV(const std::string &filename, const std::vector<std::vector<double>> &data) {
    std::ofstream file(filename);

    for (const auto &row : data) {
        for (size_t i = 0; i < row.size(); ++i) {
            file << row[i];
            if (i < row.size() - 1) file << ",";
        }
        file << "\n";
    }
}

// Function to scale the dataset using nearest-neighbor interpolation
std::vector<std::vector<double>> scaleDataset(const std::vector<std::vector<double>> &input, int newRows, int newCols) {
    int oldRows = input.size();
    int oldCols = input[0].size();

    // Initialize the output dataset
    std::vector<std::vector<double>> output(newRows, std::vector<double>(newCols));

    // Scale each value based on the nearest-neighbor interpolation
    for (int i = 0; i < newRows; ++i) {
        for (int j = 0; j < newCols; ++j) {
            int oldRow = i * oldRows / newRows;
            int oldCol = j * oldCols / newCols;
            output[i][j] = input[oldRow][oldCol];
        }
    }

    return output;
}

int main() {
    std::string inputFile = "/home/trey1/CLionProjects/paraviewNormalizer/io/input.csv";
    std::string outputFile = "/home/trey1/CLionProjects/paraviewNormalizer/io/output.csv";

    // Read the input CSV file
    std::vector<std::vector<double>> dataset = readCSV(inputFile);

    // Ensure the dataset has the expected dimensions
    if (dataset.size() != 72 || dataset[0].size() != 48) {
        std::cerr << "Error: Input dataset must be 72 x 48." << std::endl;
        return 1;
    }


    std::vector<std::vector<double>> scaledDataset = scaleDataset(dataset, 145, 96);

    // Write the scaled dataset to the output CSV file
    writeCSV(outputFile, scaledDataset);

    std::cout << "Dataset successfully scaled and written to " << outputFile << std::endl;
    return 0;
}
