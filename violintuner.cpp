#include <iostream>
#include <cmath>
#include <portaudio.h>
#include <fftw3.h>

// Frequencies for violin strings G, D, A, E, and specific notes
const double G_FREQUENCY = 196.0;
const double D_FREQUENCY = 293.7;
const double A_FREQUENCY = 440.0;
const double E_FREQUENCY = 659.3;

const double TOLERANCE = 1.0; // Frequency tolerance in Hz

// Function to determine color based on frequency difference
std::string determineColor(double frequency, double targetFrequency) {
    double diff = std::abs(frequency - targetFrequency);
    if (diff < TOLERANCE) {
        return "Green";  // Perfectly in tune
    } else if (diff < 3.0) {
        if (frequency < targetFrequency) {
            return "Yellow (bémol)";  // Almost in tune, too low
        } else {
            return "Yellow (diese)";  // Almost in tune, too high
        }
    } else {
        if (frequency < targetFrequency) {
            return "Red (bémol)";  // Too low
        } else {
            return "Red (diese)";  // Too high
        }
    }

}

// Function to calculate the dominant frequency using FFTW
double calculateFrequency(const float* buffer, size_t bufferSize, int sampleRate) {
    fftw_complex* in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * bufferSize);
    fftw_complex* out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * bufferSize);
    fftw_plan p = fftw_plan_dft_1d(bufferSize, in, out, FFTW_FORWARD, FFTW_ESTIMATE);

    for (size_t i = 0; i < bufferSize; ++i) {
        in[i][0] = buffer[i]; // Real part
        in[i][1] = 0.0;       // Imaginary part
    }

    fftw_execute(p);

    int maxIndex = 0;
    double maxAmplitude = 0.0;

    for (size_t i = 0; i < bufferSize / 2; ++i) {
        double real = out[i][0];
        double imag = out[i][1];
        double amplitude = sqrt(real * real + imag * imag);

        if (amplitude > maxAmplitude) {
            maxAmplitude = amplitude;
            maxIndex = i;
        }
    }

    double frequency = (double)maxIndex * sampleRate / bufferSize;

    fftw_destroy_plan(p);
    fftw_free(in);
    fftw_free(out);

    return frequency;
}

int main() {
    // Initialize PortAudio
    Pa_Initialize();

    // Define PortAudio stream parameters
    PaStream *stream;
    Pa_OpenDefaultStream(&stream, 1, 0, paFloat32, 44100, 256, nullptr, nullptr);

    // Start stream
    Pa_StartStream(stream);

    // Placeholder for audio data capture
    float buffer[256];
    int sampleRate = 44100; // Sample rate

    // Dummy loop to capture and analyze audio
    for (int i = 0; i < 100000000; ++i) {
        Pa_ReadStream(stream, buffer, 256);
        double detectedFrequency = calculateFrequency(buffer, 256, sampleRate); // Analyze the input frequency

        std::string noteName;
        double targetFrequency;

        if (detectedFrequency > 554.37) { // above C#5
            noteName = "E";
            targetFrequency = E_FREQUENCY;
        } else if (detectedFrequency > 370) { // between F#4 and C#5
            noteName = "A";
            targetFrequency = A_FREQUENCY;
        } else if (detectedFrequency > 233.08) { // between B3 and F#4
            noteName = "D";
            targetFrequency = D_FREQUENCY;
        } else { // below B3
            noteName = "G";
            targetFrequency = G_FREQUENCY;
        }

        std::string tuningColor = determineColor(detectedFrequency, targetFrequency);
        std::cout << noteName << " string: " << tuningColor << " (Frequency: " << detectedFrequency << " Hz)" << std::endl;
    }

    // Close stream
    Pa_StopStream(stream);
    Pa_CloseStream(stream);
    Pa_Terminate();

    return 0;
}
