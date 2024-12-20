#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>

class Tile {
private:
    std::vector<double> NorthEdge;
    std::vector<double> WestEdge;
    std::vector<double> SouthEdge;
    std::vector<double> EastEdge;
    std::string filepath;

public:
    enum EdgeType { North, East, West, South };

    Tile(const std::string& file) : filepath(file) {
        readCSV();
    }

    void readCSV() {
        std::ifstream file(filepath);
        if (!file) {
            std::cerr << "Unable to open file " << filepath << std::endl;
            exit(1);
        }

        std::vector<std::vector<double>> grid(18, std::vector<double>(12, 0.0));
        std::string line;
        int row = 0;

        while (std::getline(file, line)) {
            std::stringstream ss(line);
            std::string cell;
            int col = 0;

            while (std::getline(ss, cell, ',')) {
                try {
                    cell.erase(std::remove_if(cell.begin(), cell.end(), ::isspace), cell.end());
                    grid[row][col] = std::stod(cell);
                } catch (const std::invalid_argument& e) {
                    std::cerr << "Invalid number in file " << filepath << " at row " << row << " column " << col << ". Skipping." << std::endl;
                    grid[row][col] = 0.0;
                }
                col++;
            }
            row++;
        }
        file.close();

        NorthEdge = grid[0];
        SouthEdge = grid[17];

        for (int i = 0; i < 18; ++i) {
            WestEdge.push_back(grid[i][0]);
            EastEdge.push_back(grid[i][11]);
        }
    }

    void printEdges() const {
        std::cout << "NorthEdge: ";
        for (const auto& val : NorthEdge) std::cout << val << " ";
        std::cout << std::endl;

        std::cout << "WestEdge: ";
        for (const auto& val : WestEdge) std::cout << val << " ";
        std::cout << std::endl;

        std::cout << "SouthEdge: ";
        for (const auto& val : SouthEdge) std::cout << val << " ";
        std::cout << std::endl;

        std::cout << "EastEdge: ";
        for (const auto& val : EastEdge) std::cout << val << " ";
        std::cout << std::endl;
    }

    double calculateDifference(const std::vector<double>& edge1, const std::vector<double>& edge2) const {
        double sum = 0.0;
        for (size_t i = 0; i < edge1.size(); ++i) {
            sum += std::pow(edge1[i] - edge2[i], 2);
        }
        return std::sqrt(sum);
    }

    int mostSimilar(EdgeType edgeType, const std::vector<Tile>& tiles) const {
        const std::vector<double>* currentEdge;

        switch (edgeType) {
        case North:
            currentEdge = &NorthEdge;
            break;
        case East:
            currentEdge = &EastEdge;
            break;
        case West:
            currentEdge = &WestEdge;
            break;
        case South:
            currentEdge = &SouthEdge;
            break;
        default:
            throw std::invalid_argument("Invalid edge type");
        }

        int mostSimilarIndex = -1;
        double smallestDifference = std::numeric_limits<double>::max();

        for (size_t i = 0; i < tiles.size(); ++i) {
            if (&tiles[i] == this) continue; // Skip comparing with itself

            const std::vector<double>* comparisonEdge;

            switch (edgeType) {
            case North:
                comparisonEdge = &tiles[i].SouthEdge;
                break;
            case East:
                comparisonEdge = &tiles[i].WestEdge;
                break;
            case West:
                comparisonEdge = &tiles[i].EastEdge;
                break;
            case South:
                comparisonEdge = &tiles[i].NorthEdge;
                break;
            default:
                throw std::invalid_argument("Invalid edge type");
            }

            double difference = calculateDifference(*currentEdge, *comparisonEdge);

            if (difference < smallestDifference) {
                smallestDifference = difference;
                mostSimilarIndex = i;
            }
        }

        return mostSimilarIndex;
    }
};

int main() {
    std::vector<Tile> tiles;
    for (int i = 1; i <= 16; ++i) {
        std::string filename = "C:/Users/banta/CLionProjects/paraviewNormalizer/tiles/" + std::to_string(i) + ".csv";
        tiles.emplace_back(filename);
    }

    for (size_t i = 1; i <= tiles.size(); ++i) {
        int mostSimilarTile = tiles[i].mostSimilar(Tile::South, tiles);
        if (mostSimilarTile != -1) {
            std::cout << "Tile " << i << "'s South edge is most similar to Tile " << mostSimilarTile + 1 << "'s North edge." << std::endl;
        } else {
            std::cout << "No similar tile found for Tile " << i << "'s South edge." << std::endl;
        }
    }
}
