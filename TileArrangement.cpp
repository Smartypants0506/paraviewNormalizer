#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <limits>
#include <cmath>
#include <iomanip>

using namespace std;

string trim(const string& str) {
    size_t first = str.find_first_not_of(" \t");
    if (first == string::npos) return "";
    size_t last = str.find_last_not_of(" \t");
    return str.substr(first, (last - first + 1));
}

vector<vector<double>> readCSV(const string& filename) {
    vector<vector<double>> data;
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        exit(1);
    }

    string line;
    while (getline(file, line)) {
        vector<double> row;
        stringstream ss(line);
        string token;

        while (getline(ss, token, ',')) {
            token = trim(token);
            if (!token.empty()) {
                try {
                    row.push_back(stod(token));
                } catch (const invalid_argument& e) {
                    cerr << "Invalid value in file '" << filename << "': " << token << endl;
                }
            }
        }
        if (!row.empty()) {
            data.push_back(row);
        }
    }
    file.close();
    return data;
}

struct Edges {
    vector<double> top, bottom, left, right;
};

Edges getEdges(const vector<vector<double>>& data) {
    int rows = data.size();
    int cols = data[0].size();
    Edges edges;


    edges.top = data[0];
    edges.bottom = data[rows - 1];


    for (int i = 0; i < rows; i++) {
        edges.left.push_back(data[i][0]);
        edges.right.push_back(data[i][cols - 1]);
    }
    return edges;
}


double calculateSimilarity(const vector<double>& edge1, const vector<double>& edge2) {
    double sum = 0;
    int n = edge1.size();
    for (int i = 0; i < n; i++) {
        sum += fabs(edge1[i] - edge2[i]);
    }
    return sum;
}


struct PlacedFile {
    string name;
    Edges edges;
    bool placed;
};


int findBestStartingFile(const vector<PlacedFile>& files) {
    int bestIdx = -1;
    double maxIsolation = -1;

    for (int i = 0; i < files.size(); i++) {
        double isolation = 0;

        for (int j = 0; j < files.size(); j++) {
            if (i == j) continue;
            isolation += calculateSimilarity(files[i].edges.top, files[j].edges.bottom) +
                         calculateSimilarity(files[i].edges.bottom, files[j].edges.top) +
                         calculateSimilarity(files[i].edges.left, files[j].edges.right) +
                         calculateSimilarity(files[i].edges.right, files[j].edges.left);
        }

        if (isolation > maxIsolation) {
            maxIsolation = isolation;
            bestIdx = i;
        }
    }
    return bestIdx;
}

pair<int, string> findBestMatch(int currentIdx, const vector<PlacedFile>& files, const string& edgeToMatch) {
    int bestIdx = -1;
    double bestScore = numeric_limits<double>::max();
    string bestEdge;

    for (int i = 0; i < files.size(); i++) {
        if (files[i].placed || i == currentIdx) continue;

        double score;
        if (edgeToMatch == "right") {
            score = calculateSimilarity(files[currentIdx].edges.right, files[i].edges.left);
            if (score < bestScore) {
                bestScore = score;
                bestIdx = i;
                bestEdge = "left";
            }
        }
    }

    return {bestIdx, bestEdge};
}


void arrangeFiles(vector<PlacedFile>& files, vector<vector<string>>& grid) {
    grid.resize(4, vector<string>(4, ""));


    int startIdx = findBestStartingFile(files);
    files[startIdx].placed = true;
    grid[0][0] = files[startIdx].name;

    int currentIdx = startIdx;
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            if (r == 0 && c == 0) continue;

            pair<int, string> result = findBestMatch(currentIdx, files, "right");
            int bestIdx = result.first;
            string bestEdge = result.second;

            if (bestIdx != -1) {
                grid[r][c] = files[bestIdx].name;
                files[bestIdx].placed = true;
                currentIdx = bestIdx;
            } else {
                cerr << "No suitable match found!" << endl;
                return;
            }
        }
    }
}


int main() {
    const int NUM_FILES = 16;

    vector<PlacedFile> files;
    for (int i = 1; i <= NUM_FILES; i++) {
        string filename = "C:/Users/banta/CLionProjects/paraviewNormalizer/tiles/" + to_string(i) + ".csv";
        vector<vector<double>> data = readCSV(filename);
        files.push_back({filename, getEdges(data), false});
    }


    vector<vector<string>> grid;
    arrangeFiles(files, grid);

    cout << "Final 4x4 Grid of Files:" << endl;
    for (const auto& row : grid) {
        for (const auto& file : row) {
            cout << setw(20) << file << " ";
        }
        cout << endl;
    }

    return 0;
}
