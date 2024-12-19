#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;

int main() {
    const int rows = 72; // Number of rows in the input grid
    const int cols = 48; // Number of columns in the input grid
    string inputFile = "C:/Users/banta/CLionProjects/paraviewNormalizer/rawCSV.csv";    // Input file path
    string outputFile = "C:/Users/banta/CLionProjects/paraviewNormalizer/Converted.csv";  // Output file path

    vector<vector<double>> grid(rows, vector<double>(cols)); // 2D grid to store Z values

    // Open the input CSV file
    ifstream inFile(inputFile);
    if (!inFile.is_open()) {
        cerr << "Error: Could not open input file: " << inputFile << endl;
        return 1;
    }

    // Read the CSV file line by line
    string line;
    int row = 0;
    while (getline(inFile, line) && row < rows) {
        stringstream ss(line);
        string value;
        int col = 0;
        while (getline(ss, value, ',') && col < cols) {
            grid[row][col] = stod(value); // Convert string to double and store in grid
            ++col;
        }
        ++row;
    }
    inFile.close();

    // Open the output CSV file
    ofstream outFile(outputFile);
    if (!outFile.is_open()) {
        cerr << "Error: Could not open output file: " << outputFile << endl;
        return 1;
    }

    // Write the header to the output file
    outFile << "X,Y,Z\n";

    // Write the grid data as X, Y, Z values
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            outFile << i << "," << j << "," << grid[i][j] << "\n";
        }
    }

    outFile.close();
    cout << "Successfully processed and written to " << outputFile << endl;

    return 0;
}
