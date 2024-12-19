#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <cmath>
#include <algorithm>
#include <cfloat>
#include <stdexcept>

using namespace std;

vector<vector<float>> readCSV(const string& filename) {
    ifstream file(filename);
    vector<vector<float>> grid(18, vector<float>(12, 0.0f));

    if (!file) {
        cerr << "Unable to open file " << filename << endl;
        exit(1);
    }

    string line;
    int row = 0;

    while (getline(file, line)) {
        stringstream ss(line);
        string cell;
        int col = 0;

        while (getline(ss, cell, ',')) {
            try {
                cell.erase(remove_if(cell.begin(), cell.end(), ::isspace), cell.end());
                grid[row][col] = stof(cell);
            } catch (const std::invalid_argument& e) {
                cerr << "Invalid number in file " << filename << " at row " << row << " column " << col << ". Skipping." << endl;
                grid[row][col] = 0.0f;
            }
            col++;
        }
        row++;
    }
    file.close();
    return grid;
}

vector<float> extractEdge(const vector<vector<float>>& grid, const string& edgeType) {
    vector<float> edge(12, 0.0f);

    if (edgeType == "top") {
        for (int i = 0; i < 12; i++) {
            edge[i] = grid[0][i];
        }
    } else if (edgeType == "bottom") {
        for (int i = 0; i < 12; i++) {
            edge[i] = grid[17][i];
        }
    } else if (edgeType == "left") {
        for (int i = 0; i < 18; i++) {
            edge[i] = grid[i][0];
        }
    } else if (edgeType == "right") {
        for (int i = 0; i < 18; i++) {
            edge[i] = grid[i][11];
        }
    }

    return edge;
}

float calculateSimilarity(const vector<float>& edge1, const vector<float>& edge2) {
    float similarity = 0.0f;
    for (int i = 0; i < edge1.size(); i++) {
        similarity += abs(edge1[i] - edge2[i]);
    }
    return similarity;
}


int main() {
    string file1 = "C:/Users/banta/CLionProjects/paraviewNormalizer/tiles/1.csv";
    string file2 = "C:/Users/banta/CLionProjects/paraviewNormalizer/tiles/2.csv";


    vector<vector<float>> grid1 = readCSV(file1);
    vector<vector<float>> grid2 = readCSV(file2);


    vector<float> topEdge1 = extractEdge(grid1, "top");
    vector<float> bottomEdge1 = extractEdge(grid1, "bottom");
    vector<float> leftEdge1 = extractEdge(grid1, "left");
    vector<float> rightEdge1 = extractEdge(grid1, "right");

    vector<float> topEdge2 = extractEdge(grid2, "top");
    vector<float> bottomEdge2 = extractEdge(grid2, "bottom");
    vector<float> leftEdge2 = extractEdge(grid2, "left");
    vector<float> rightEdge2 = extractEdge(grid2, "right");

    float minSimilarity = FLT_MAX;
    vector<float> bestEdge1, bestEdge2;
    string bestEdgeType1, bestEdgeType2;
    string edgeTypes[] = {"top", "bottom", "left", "right"};


    for (const string& edgeType1 : edgeTypes) {
        vector<float> edge1 = extractEdge(grid1, edgeType1);

        for (const string& edgeType2 : edgeTypes) {
            if (edgeType1 == edgeType2) {
                continue;
            }

            vector<float> edge2 = extractEdge(grid2, edgeType2);
            float similarity = calculateSimilarity(edge1, edge2);

            if (similarity < minSimilarity) {
                minSimilarity = similarity;
                bestEdge1 = edge1;
                bestEdge2 = edge2;
                bestEdgeType1 = edgeType1;
                bestEdgeType2 = edgeType2;
            }
        }
    }


    cout << "Most similar edges:" << endl;
    cout << "Edge 1 (" << bestEdgeType1 << "): ";
    for (float val : bestEdge1) {
        cout << val << " ";
    }
    cout << endl;

    cout << "Edge 2 (" << bestEdgeType2 << "): ";
    for (float val : bestEdge2) {
        cout << val << " ";
    }
    cout << endl;

    cout << "Similarity Score: " << minSimilarity << endl;

    return 0;
}
