#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <tiffio.h>

// Function to read the CSV file and return a 2D vector of grayscale values
std::vector<std::vector<uint8_t>> readCSV(const std::string& filename) {
    std::vector<std::vector<uint8_t>> data;
    std::ifstream file(filename);

    if (!file.is_open()) {
        throw std::runtime_error("Unable to open CSV file");
    }

    std::string line;
    while (std::getline(file, line)) {
        std::vector<uint8_t> row;
        std::stringstream ss(line);
        std::string value;

        while (std::getline(ss, value, ',')) {
            row.push_back(static_cast<uint8_t>(std::stoi(value)));
        }
        data.push_back(row);
    }

    file.close();
    return data;
}

// Function to save the 2D vector as a greyscale TIFF file
void saveAsTIFF(const std::string& filename, const std::vector<std::vector<uint8_t>>& data) {
    const size_t height = data.size();
    const size_t width = data[0].size();

    TIFF* tiff = TIFFOpen(filename.c_str(), "w");
    if (!tiff) {
        throw std::runtime_error("Unable to open TIFF file for writing");
    }

    TIFFSetField(tiff, TIFFTAG_IMAGEWIDTH, static_cast<uint32_t>(width));
    TIFFSetField(tiff, TIFFTAG_IMAGELENGTH, static_cast<uint32_t>(height));
    TIFFSetField(tiff, TIFFTAG_SAMPLESPERPIXEL, 1);
    TIFFSetField(tiff, TIFFTAG_BITSPERSAMPLE, 8);
    TIFFSetField(tiff, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT);
    TIFFSetField(tiff, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
    TIFFSetField(tiff, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);

    for (size_t row = 0; row < height; ++row) {
        if (TIFFWriteScanline(tiff, const_cast<uint8_t*>(data[row].data()), static_cast<uint32_t>(row), 0) == -1) {
            TIFFClose(tiff);
            throw std::runtime_error("Error writing scanline to TIFF file");
        }
    }

    TIFFClose(tiff);
}

int main() {
    const std::string inputCSV = "uncAhhDirectory/Falcon2ScaledNoRotate.csv"; // Replace with your input CSV file path
    const std::string outputTIFF = "TIFF/test.tiff"; // Replace with your output TIFF file path

    try {
        // Read the CSV file
        auto data = readCSV(inputCSV);

        // Ensure all rows have the same width
        size_t width = data[0].size();
        for (const auto& row : data) {
            if (row.size() != width) {
                throw std::runtime_error("Inconsistent row sizes in CSV file");
            }
        }

        // Save as TIFF file
        saveAsTIFF(outputTIFF, data);
        std::cout << "TIFF file saved successfully: " << outputTIFF << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
