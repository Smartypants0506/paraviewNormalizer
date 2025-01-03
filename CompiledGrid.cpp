#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>

using namespace std;

const int TILE_ROWS = 18;
const int TILE_COLS = 12;
const int GRID_SIZE = 4;

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

    for (int gridRow = 0; gridRow < GRID_SIZE; ++gridRow) {
        for (int gridCol = 0; gridCol < GRID_SIZE; ++gridCol) {
            string filename = "C:/Users/banta/CLionProjects/paraviewNormalizer/PotentialSolutions/" + to_string(gridRow) + "_" + to_string(gridCol) + ".csv";
            vector<vector<string>> tileData = readCSV(filename);

            for (int i = 0; i < TILE_ROWS; ++i) {
                for (int j = 0; j < TILE_COLS; ++j) {
                    mergedMap[gridRow * TILE_ROWS + i][gridCol * TILE_COLS + j] = tileData[i][j];
                }
            }
        }
    }

    writeCSV("C:/Users/banta/CLionProjects/paraviewNormalizer/PotentialSolutions/final2", mergedMap);
    return 0;
}