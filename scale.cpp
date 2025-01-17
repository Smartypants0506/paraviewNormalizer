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
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << filename << endl;
        exit(EXIT_FAILURE);
    }
    string line;

    while (getline(file, line)) {
        vector<double> row;
        stringstream ss(line);
        string value;

        while (getline(ss, value, ',')) {
            try {
                row.push_back(stod(value));
            } catch (const invalid_argument& e) {
                cerr << "Error: Non-numeric data in file " << filename << endl;
                exit(EXIT_FAILURE);
            }
        }
        if (!row.empty()) grid.push_back(row);
    }

    if (grid.empty()) {
        cerr << "Error: File " << filename << " is empty or contains invalid data." << endl;
        exit(EXIT_FAILURE);
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

            // Check bounds before accessing transformedGrid
            if (newR >= 0 && newR < originalRows && newC >= 0 && newC < originalCols) {
                transformedGrid[newR][newC] = grid[r][c];
            }
        }
    }

    return transformedGrid;
}

int main() {
    string inputFilename = "uncAhhDirectory/Falcon4.csv", outputFilename = "uncAhhDirectory/Falcon4Scaled.csv";
    double scaleX = 0, scaleY = 0;
    int translateX = 5, translateY = -5;

    // Process the CSV file
    Grid grid = readCSV(inputFilename);
    Grid transformedGrid = translateAndScale(grid, scaleX, scaleY, translateX, translateY);

    // Write the transformed grid to the output file
    writeCSV(outputFilename, transformedGrid);

    cout << "Data transformation complete. Output saved to " << outputFilename << endl;

    return 0;
}
