#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;

int main() {
    const int rows = 8*18;
    const int cols = 8*12;
    string inputFile = "/home/trey1/CLionProjects/paraviewNormalizer/io/rawCSV.csv";
    string outputFile = "/home/trey1/CLionProjects/paraviewNormalizer/io/Converted.csv";

    vector<vector<double>> grid(rows, vector<double>(cols));

    ifstream inFile(inputFile);
    if (!inFile.is_open()) {
        cerr << "Error: Could not open input file: " << inputFile << endl;
        return 1;
    }

    string line;
    int row = 0;
    while (getline(inFile, line) && row < rows) {
        stringstream ss(line);
        string value;
        int col = 0;
        while (getline(ss, value, ',') && col < cols) {
            grid[row][col] = stod(value);
            ++col;
        }
        ++row;
    }
    inFile.close();

    ofstream outFile(outputFile);
    if (!outFile.is_open()) {
        cerr << "Error: Could not open output file: " << outputFile << endl;
        return 1;
    }

    outFile << "X,Y,Z\n";

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            outFile << i << "," << j << "," << grid[i][j] << "\n";
        }
    }

    outFile.close();
    cout << "Successfully processed and written to " << outputFile << endl;

    return 0;
}
