#include <iostream>
#include <cmath>
#include <vector>

// Define standard frequencies for violin strings
const double G_FREQUENCY = 196.00;
const double D_FREQUENCY = 293.66;
const double A_FREQUENCY = 440.00;
const double E_FREQUENCY = 659.25;

// Function to get the closest string name
std::string getClosestString(double frequency) {
    double minDiff = std::abs(frequency - G_FREQUENCY);
    std::string closestString = "G";

    std::vector<std::pair<std::string, double>> strings = {
        {"D", D_FREQUENCY},
        {"A", A_FREQUENCY},
        {"E", E_FREQUENCY}
    };

    for (const auto& str : strings) {
        double diff = std::abs(frequency - str.second);
        if (diff < minDiff) {
            minDiff = diff;
            closestString = str.first;
        }
    }

    return closestString;
}

// Main function
int main() {
    double frequency;
    std::cout << "Enter the frequency of the note: ";
    std::cin >> frequency;

    std::string closestString = getClosestString(frequency);

    if (std::abs(frequency - G_FREQUENCY) < 1.0 ||
        std::abs(frequency - D_FREQUENCY) < 1.0 ||
        std::abs(frequency - A_FREQUENCY) < 1.0 ||
        std::abs(frequency - E_FREQUENCY) < 1.0) {
        std::cout << "In tune with string: " << closestString << std::endl;
    } else {
        std::cout << "Out of tune, closest string: " << closestString << std::endl;
    }

    return 0;
}

