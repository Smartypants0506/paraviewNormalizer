#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

using namespace std;

using Grid = vector<vector<double>>;

// Helper function to read CSV file into a grid
Grid readCSV(const string& filename) {
    Grid grid;
    ifstream file(filename);
    string line;

    while (getline(file, line)) {
        vector<double> row;
        stringstream ss(line);
        string value;

        while (getline(ss, value, ',')) {
            row.push_back(stod(value));
        }
        grid.push_back(row);
    }

    return grid;
}

// Helper function to write grid to CSV file
void writeCSV(const string& filename, const Grid& grid) {
    ofstream file(filename);

    for (const auto& row : grid) {
        for (size_t i = 0; i < row.size(); ++i) {
            file << row[i];
            if (i < row.size() - 1) file << ",";
        }
        file << "\n";
    }
}

// Function to translate and scale the grid
Grid translateAndScale(const Grid& grid, double scaleX, double scaleY, int translateX, int translateY) {
    int originalRows = grid.size();
    int originalCols = grid[0].size();

    // Create a new grid with the same size, initialized to 255.0
    Grid transformedGrid(originalRows, vector<double>(originalCols, 255.0));

    // Apply scaling and translation
    for (int r = 0; r < originalRows; ++r) {
        for (int c = 0; c < originalCols; ++c) {
            int newR = static_cast<int>(r * scaleY) + translateY;
            int newC = static_cast<int>(c * scaleX) + translateX;

            if (newR >= 0 && newR < originalRows && newC >= 0 && newC < originalCols) {
                transformedGrid[newR][newC] = grid[r][c];
            }
        }
    }

    return transformedGrid;
}

int main() {
    string inputFilename = "/home/trey1/Downloads/Falconia1.csv", outputFilename = "/home/trey1/Downloads/Falconia1Scaled.csv";
    double scaleX = 0.95, scaleY = 0.8;
    int translateX = 12, translateY = 9;

    // Process the CSV file
    Grid grid = readCSV(inputFilename);
    Grid transformedGrid = translateAndScale(grid, scaleX, scaleY, translateX, translateY);

    // Write the transformed grid to the output file
    writeCSV(outputFilename, transformedGrid);

    cout << "Data transformation complete. Output saved to " << outputFilename << endl;

    return 0;
}
