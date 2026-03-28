#include <wiringPi.h>
#include <cstdint>
#include <cstring>
#include <cmath>
#include <vector>
#include <complex>
#include <iostream>
#include "CroixPharma.h"

#define PI 3.14159265358979323846
#define FFT_SIZE 1024
#define BANDS 8

CroixPharma croix;
uint8_t bitmap[SIZE][SIZE];

// Simple Radix-2 FFT implementation
void fft(std::vector<std::complex<double>>& a) {
    int n = a.size();
    if (n <= 1) return;

    std::vector<std::complex<double>> a0(n / 2), a1(n / 2);
    for (int i = 0; i * 2 < n; i++) {
        a0[i] = a[i * 2];
        a1[i] = a[i * 2 + 1];
    }
    fft(a0);
    fft(a1);

    double ang = 2 * PI / n;
    std::complex<double> w(1), wn(cos(ang), sin(ang));
    for (int i = 0; i * 2 < n; i++) {
        a[i] = a0[i] + w * a1[i];
        a[i + n / 2] = a0[i] - w * a1[i];
        w *= wn;
    }
}

void draw_specter(const std::vector<double>& bands) {
    memset(bitmap, 0, sizeof(bitmap));

    for (int i = 0; i < BANDS; i++) {
        int height = (int)(bands[i] * 24); // Max 24 pixels for full vertical height
        if (height > 24) height = 24;

        for (int h = 0; h < height; h++) {
            // Full vertical bar: x[8-15], y grows from 23 up to 0
            int y = 23 - h;
            if (y >= 0 && y < 24) {
                bitmap[y][8 + i] = 1;
            }
        }

        // Horizontal bars (Low intensity: 30% of max width, starting from edges)
        int horiz_width = (int)(bands[i] * 8 * 0.4); // 8 pixels max width for a horizontal branch
        if (horiz_width > 8) horiz_width = 8;

        for (int w = 0; w < horiz_width; w++) {
            // Left branch: x grows from 0 to 7
            bitmap[8 + i][w] = 1;

            // Right branch: x grows from 23 down to 16
            bitmap[8 + i][23 - w] = 1;
        }
    }

    croix.setSide(CroixPharma::BOTH);
    croix.writeBitmap(bitmap);
}

#define SENSITIVITY 0.3

int main() {
    if (wiringPiSetupGpio() < 0) return 1;
    croix.begin();

    std::vector<int16_t> audio_buffer(FFT_SIZE);
    std::vector<std::complex<double>> fft_input(FFT_SIZE);
    std::vector<double> window(FFT_SIZE);

    // Hamming window
    for (int i = 0; i < FFT_SIZE; i++) {
        window[i] = 0.54 - 0.46 * cos(2 * PI * i / (FFT_SIZE - 1));
    }

    while (std::cin.read((char*)audio_buffer.data(), FFT_SIZE * sizeof(int16_t))) {
        // Prepare FFT input
        for (int i = 0; i < FFT_SIZE; i++) {
            fft_input[i] = std::complex<double>(audio_buffer[i] * window[i] / 32768.0, 0);
        }

        fft(fft_input);

        // Group into 8 logarithmic bands
        std::vector<double> bands(BANDS, 0);
        int bin = 1; // Skip DC
        for (int i = 0; i < BANDS; i++) {
            int next_bin = bin * 2;
            if (next_bin > FFT_SIZE / 2) next_bin = FFT_SIZE / 2;
            
            double sum = 0;
            int count = 0;
            for (; bin < next_bin; bin++) {
                sum += std::abs(fft_input[bin]);
                count++;
            }
            if (count > 0) bands[i] = (sum / count) * SENSITIVITY * (1.0 + i * 0.4);
        }

        draw_specter(bands);
    }

    return 0;
}
