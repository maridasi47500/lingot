#include <iostream>
#include <cmath>
#include <vector>
#include <fftw3.h>
#include <portaudio.h>

const double G_FREQUENCY = 196.00;
const double D_FREQUENCY = 293.66;
const double A_FREQUENCY = 440.00;
const double E_FREQUENCY = 659.25;

// Function to calculate the frequency (placeholder logic)
double calculateFrequency(float const* buffer, unsigned long bufferSize, int sampleRate) {
    // Placeholder frequency calculation logic
    return 440.0;  // Example frequency, replace with actual logic
}

std::string determineColor(double detectedFrequency, double targetFrequency) {
    double diff = std::abs(detectedFrequency - targetFrequency);
    if (diff < 5) {
        return "green";  // In tune
    } else if (diff < 15) {
        return "yellow"; // Almost in tune
    } else {
        return "red";    // Out of tune
    }
}

int main() {
    Pa_Initialize();
    PaStream *stream;
    Pa_OpenDefaultStream(&stream, 1, 0, paFloat32, 44100, 256, nullptr, nullptr);
    Pa_StartStream(stream);

    float buffer[256];
    int sampleRate = 44100;

    for (int i = 0; i < 100; ++i) {
        Pa_ReadStream(stream, buffer, 256);
        double detectedFrequency = calculateFrequency(buffer, 256, sampleRate);

        std::string noteName;
        double targetFrequency;
        if (detectedFrequency > 554.37) {
            noteName = "E";
            targetFrequency = E_FREQUENCY;
        } else if (detectedFrequency > 370) {
            noteName = "A";
            targetFrequency = A_FREQUENCY;
        } else if (detectedFrequency > 233.08) {
            noteName = "D";
            targetFrequency = D_FREQUENCY;
        } else {
            noteName = "G";
            targetFrequency = G_FREQUENCY;
        }

        std::string tuningColor = determineColor(detectedFrequency, targetFrequency);
        std::cout << noteName << " string: " << tuningColor << " (Frequency: " << detectedFrequency << " Hz)" << std::endl;
    }

    Pa_StopStream(stream);
    Pa_CloseStream(stream);
    Pa_Terminate();
    return 0;
}

