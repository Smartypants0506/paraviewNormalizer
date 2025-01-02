#include <iostream>
#include <fstream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

// Function to load RGB values from a file into a matrix
bool loadValues(const string &filename, Mat &channel) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << filename << endl;
        return false;
    }

    for (int i = 0; i < channel.rows; ++i) {
        for (int j = 0; j < channel.cols; ++j) {
            int value;
            file >> value;
            if (file.fail()) {
                cerr << "Error: Invalid data in file " << filename << endl;
                return false;
            }
            channel.at<uchar>(i, j) = static_cast<uchar>(value);
        }
    }

    file.close();
    return true;
}

int main() {
    const int width = 145;
    const int height = 96;
    const int scaleFactor = 10; // Change this value to scale the image

    // Create matrices for each channel
    Mat redChannel(height, width, CV_8UC1);
    Mat greenChannel(height, width, CV_8UC1);
    Mat blueChannel(height, width, CV_8UC1);

    // Load values from the files
    if (!loadValues("/home/trey1/Downloads/RGB0.txt", redChannel) ||
        !loadValues("/home/trey1/Downloads/RGB3.txt", greenChannel) ||
        !loadValues("/home/trey1/Downloads/RGB0.txt", blueChannel)) {
        return 1;
        }

    // Merge channels into a single image
    vector<Mat> channels = {blueChannel, greenChannel, redChannel};
    Mat image;
    merge(channels, image);

    // Scale up the image
    Mat scaledImage;
    resize(image, scaledImage, Size(image.cols * scaleFactor, image.rows * scaleFactor), 0, 0, INTER_NEAREST);

    // Display the image
    namedWindow("RGB Image", WINDOW_AUTOSIZE);
    imshow("RGB Image", scaledImage);

    // Wait for a key press and then close
    waitKey(0);

    return 0;
}

