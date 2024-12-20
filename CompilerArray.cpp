#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>

void mergeCSVGrid(
    const std::vector<std::vector<std::string>>& fileGrid,
    const std::string& outputFileName,
    int rowsPerFile,
    int colsPerFile
) {
    int totalRows = rowsPerFile * fileGrid.size(); // Total rows in the final CSV
    int totalCols = colsPerFile * fileGrid[0].size(); // Total columns in the final CSV

    // Create a 2D grid to store the merged data
    std::vector<std::vector<std::string>> mergedData(totalRows, std::vector<std::string>(totalCols));

    for (size_t rowIndex = 0; rowIndex < fileGrid.size(); ++rowIndex) { // Iterate over grid rows
        for (size_t colIndex = 0; colIndex < fileGrid[rowIndex].size(); ++colIndex) { // Iterate over grid cols
            const std::string& fileName = fileGrid[rowIndex][colIndex];
            std::ifstream inputFile(fileName);

            if (!inputFile.is_open()) {
                std::cerr << "Error: Unable to open file: " << fileName << std::endl;
                continue;
            }

            std::string line;
            int localRow = 0; // Row index within the current file
            while (std::getline(inputFile, line) && localRow < rowsPerFile) {
                std::stringstream ss(line);
                std::string value;
                int localCol = 0; // Column index within the current file
                while (std::getline(ss, value, ',') && localCol < colsPerFile) {
                    // Calculate global row and column indices
                    int globalRow = rowIndex * rowsPerFile + localRow;
                    int globalCol = colIndex * colsPerFile + localCol;
                    mergedData[globalRow][globalCol] = value;
                    ++localCol;
                }
                ++localRow;
            }

            inputFile.close();
        }
    }

    // Write the merged data to the output file
    std::ofstream outputFile(outputFileName);
    if (!outputFile.is_open()) {
        std::cerr << "Error: Unable to create output file: " << outputFileName << std::endl;
        return;
    }

    for (const auto& row : mergedData) {
        for (size_t col = 0; col < row.size(); ++col) {
            outputFile << row[col];
            if (col != row.size() - 1) {
                outputFile << ",";
            }
        }
        outputFile << "\n";
    }

    outputFile.close();
    std::cout << "Files merged successfully into: " << outputFileName << std::endl;
}

int main() {
    // Example grid of file names (2x4 grid)
    std::vector<std::vector<std::string>> fileGrid = {
        {"C:/Users/banta/CLionProjects/paraviewNormalizer/tiles/7.csv", "C:/Users/banta/CLionProjects/paraviewNormalizer/tiles/10.csv", "C:/Users/banta/CLionProjects/paraviewNormalizer/tiles/12.csv", "C:/Users/banta/CLionProjects/paraviewNormalizer/tiles/14.csv"},
        {"C:/Users/banta/CLionProjects/paraviewNormalizer/tiles/9.csv", "C:/Users/banta/CLionProjects/paraviewNormalizer/tiles/5.csv", "C:/Users/banta/CLionProjects/paraviewNormalizer/tiles/11.csv", "C:/Users/banta/CLionProjects/paraviewNormalizer/tiles/4.csv"},
        {"C:/Users/banta/CLionProjects/paraviewNormalizer/tiles/13.csv", "C:/Users/banta/CLionProjects/paraviewNormalizer/tiles/6.csv","C:/Users/banta/CLionProjects/paraviewNormalizer/tiles/3.csv", "C:/Users/banta/CLionProjects/paraviewNormalizer/tiles/8.csv"},
        {"C:/Users/banta/CLionProjects/paraviewNormalizer/tiles/1.csv", "C:/Users/banta/CLionProjects/paraviewNormalizer/tiles/2.csv","C:/Users/banta/CLionProjects/paraviewNormalizer/tiles/16.csv", "C:/Users/banta/CLionProjects/paraviewNormalizer/tiles/15.csv"}
    };

    std::string outputFileName = "C:/Users/banta/CLionProjects/paraviewNormalizer/PotentialSolutions/4x4.csv";

    // Dimensions of each CSV file
    int rowsPerFile = 18;
    int colsPerFile = 12;

    mergeCSVGrid(fileGrid, outputFileName, rowsPerFile, colsPerFile);

    return 0;
}
