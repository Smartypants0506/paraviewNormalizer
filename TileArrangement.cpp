#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <limits>
#include <cmath>
#include <iomanip>

using namespace std;

// Helper function to trim leading and trailing whitespace
string trim(const string& str) {
    size_t first = str.find_first_not_of(" \t");
    if (first == string::npos) return "";
    size_t last = str.find_last_not_of(" \t");
    return str.substr(first, (last - first + 1));
}

// Function to read a CSV file into a 2D vector
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

// Function to extract edges from a 2D vector
struct Edges {
    vector<double> top, bottom, left, right;
};

Edges getEdges(const vector<vector<double>>& data) {
    int rows = data.size();
    int cols = data[0].size();
    Edges edges;

    // Top and bottom edges
    edges.top = data[0];
    edges.bottom = data[rows - 1];

    // Left and right edges
    for (int i = 0; i < rows; i++) {
        edges.left.push_back(data[i][0]);
        edges.right.push_back(data[i][cols - 1]);
    }
    return edges;
}

// Function to calculate similarity between two edges (sum of absolute differences)
double calculateSimilarity(const vector<double>& edge1, const vector<double>& edge2) {
    double sum = 0;
    int n = edge1.size();
    for (int i = 0; i < n; i++) {
        sum += fabs(edge1[i] - edge2[i]);
    }
    return sum;
}

// Structure to represent a placed CSV file
struct PlacedFile {
    string name;
    Edges edges;
    bool placed;
};

// Function to find the most "isolated" file to use as the starting file
int findBestStartingFile(const vector<PlacedFile>& files) {
    int bestIdx = -1;
    double maxIsolation = -1;

    for (int i = 0; i < files.size(); i++) {
        double isolation = 0;

        // Compare the edges of the current file to every other file
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

// Function to find the best matching edge and position
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
        // Expand for other edges if needed
    }

    return {bestIdx, bestEdge};
}

// Function to arrange the files into a 4x4 grid
void arrangeFiles(vector<PlacedFile>& files, vector<vector<string>>& grid) {
    grid.resize(4, vector<string>(4, ""));

    // Find the best starting file dynamically
    int startIdx = findBestStartingFile(files);
    files[startIdx].placed = true;
    grid[0][0] = files[startIdx].name;

    int currentIdx = startIdx;
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            if (r == 0 && c == 0) continue; // Skip the first file

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

// Main function
int main() {
    const int NUM_FILES = 16;

    // Step 1: Read all files
    vector<PlacedFile> files;
    for (int i = 1; i <= NUM_FILES; i++) {
        string filename = "C:/Users/banta/CLionProjects/paraviewNormalizer/tiles/" + to_string(i) + ".csv";
        vector<vector<double>> data = readCSV(filename);
        files.push_back({filename, getEdges(data), false});
    }

    // Step 2: Arrange files into a 4x4 grid
    vector<vector<string>> grid;
    arrangeFiles(files, grid);

    // Step 3: Output the grid
    cout << "Final 4x4 Grid of Files:" << endl;
    for (const auto& row : grid) {
        for (const auto& file : row) {
            cout << setw(20) << file << " ";
        }
        cout << endl;
    }

    return 0;
}
