#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>

using namespace std;

const int TILE_ROWS = 18; // Rows per CSV tile
const int TILE_COLS = 12; // Columns per CSV tile
const int GRID_SIZE = 4;  // 4x4 grid of CSV files

vector<vector<string>> readCSV(const string& filename) {
    vector<vector<string>> data(TILE_ROWS, vector<string>(TILE_COLS));
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << filename << endl;
        exit(1);
    }

    string line;
    for (int i = 0; i < TILE_ROWS; ++i) {
        if (!getline(file, line)) {
            cerr << "Error: Unexpected end of file in " << filename << " at row " << i << endl;
            exit(1);
        }
        stringstream ss(line);
        string cell;
        for (int j = 0; j < TILE_COLS; ++j) {
            if (!getline(ss, cell, ',')) {
                cerr << "Error: Unexpected number of columns in file " << filename << " at row " << i << ", column " << j << endl;
                exit(1);
            }
            data[i][j] = cell;
        }
    }
    file.close();
    return data;
}

void writeCSV(const string& filename, const vector<vector<string>>& data) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Could not open output file " << filename << endl;
        exit(1);
    }

    for (const auto& row : data) {
        for (size_t i = 0; i < row.size(); ++i) {
            file << row[i];
            if (i < row.size() - 1) file << ",";
        }
        file << "\n";
    }
    file.close();
    cout << "Output written to " << filename << endl;
}

int main() {
    vector<vector<string>> mergedMap(GRID_SIZE * TILE_ROWS, vector<string>(GRID_SIZE * TILE_COLS));

    // Iterate over the 4x4 grid of files
    for (int gridRow = 0; gridRow < GRID_SIZE; ++gridRow) {
        for (int gridCol = 0; gridCol < GRID_SIZE; ++gridCol) {
            // Construct the filename (e.g., tile_0_0.csv, tile_0_1.csv, etc.)
            string filename = "C:/Users/banta/CLionProjects/paraviewNormalizer/PotentialSolutions/" + to_string(gridRow) + "_" + to_string(gridCol) + ".csv";
            vector<vector<string>> tileData = readCSV(filename);

            // Copy tile data to the appropriate position in the merged map
            for (int i = 0; i < TILE_ROWS; ++i) {
                for (int j = 0; j < TILE_COLS; ++j) {
                    mergedMap[gridRow * TILE_ROWS + i][gridCol * TILE_COLS + j] = tileData[i][j];
                }
            }
        }
    }

    // Write the merged map to output.csv
    writeCSV("C:/Users/banta/CLionProjects/paraviewNormalizer/PotentialSolutions/final1", mergedMap);
    return 0;
}