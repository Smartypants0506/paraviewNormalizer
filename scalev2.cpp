#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <cmath>

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

// Function to rotate the grid by an arbitrary angle
Grid rotateGrid(const Grid& grid, double angle) {
    int rows = grid.size();
    int cols = grid[0].size();
    double radians = angle * M_PI / 180.0;
    double cosAngle = cos(radians);
    double sinAngle = sin(radians);

    // Calculate the dimensions of the new grid
    int newRows = static_cast<int>(abs(rows * cosAngle) + abs(cols * sinAngle));
    int newCols = static_cast<int>(abs(rows * sinAngle) + abs(cols * cosAngle));
    Grid rotatedGrid(newRows, vector<double>(newCols, 255.0));

    // Center coordinates for rotation
    int centerX = rows / 2;
    int centerY = cols / 2;
    int newCenterX = newRows / 2;
    int newCenterY = newCols / 2;

    // Perform rotation
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            int x = r - centerX;
            int y = c - centerY;

            int newX = static_cast<int>(x * cosAngle - y * sinAngle + newCenterX);
            int newY = static_cast<int>(x * sinAngle + y * cosAngle + newCenterY);

            if (newX >= 0 && newX < newRows && newY >= 0 && newY < newCols) {
                rotatedGrid[newX][newY] = grid[r][c];
            }
        }
    }

    return rotatedGrid;
}

int main() {
    string inputFilename = "uncAhhDirectory/Falcon5.csv", outputFilename = "uncAhhDirectory/Falcon5Scaled.csv";
    double scaleX = 1, scaleY = 1;
    int translateX = 0, translateY = 0;
    double rotationAngle = 0; // Rotation angle in degrees (increments of 5)

    // Process the CSV file
    Grid grid = readCSV(inputFilename);

    // Apply transformations
    Grid transformedGrid = translateAndScale(grid, scaleX, scaleY, translateX, translateY);
    transformedGrid = rotateGrid(transformedGrid, rotationAngle);

    // Write the transformed grid to the output file
    writeCSV(outputFilename, transformedGrid);

    cout << "Data transformation complete. Output saved to " << outputFilename << endl;

    return 0;
}
