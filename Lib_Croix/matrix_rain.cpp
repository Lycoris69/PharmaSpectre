#include <wiringPi.h>
#include <cstdint>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include "CroixPharma.h"

CroixPharma croix;
uint8_t bitmap[SIZE][SIZE];

// Drops array: for each column, where is the head of the drop
int drop_y[SIZE];
// How long is the tail
int drop_len[SIZE];

// The map of the valid cross pixels
bool in_cross(int x, int y) {
    if (x >= 8 && x < 16 && y >= 0 && y < 24) return true;
    if (y >= 8 && y < 16 && x >= 0 && x < 24) return true;
    return false;
}

void init_matrix() {
    for (int x = 0; x < SIZE; x++) {
        drop_y[x] = -(rand() % 15); // Start off-screen randomly
        drop_len[x] = 3 + (rand() % 5);
    }
}

void step_matrix() {
    memset(bitmap, 0, sizeof(bitmap));
    for (int x = 0; x < SIZE; x++) {
        int head_y = drop_y[x];
        
        for (int i = 0; i < drop_len[x]; i++) {
            int draw_y = head_y - i;
            if (draw_y >= 0 && draw_y < SIZE) {
                // If the drop is outside the cross, don't draw it
                if (in_cross(x, draw_y)) {
                    // We only have 0/1 bits in bitmap. 
                    // But maybe we want the tail to skip pixels? Let's just draw 1s.
                    // For a tail effect, we can dot it.
                    if (i == 0 || (rand() % 3 != 0)) {
                        bitmap[draw_y][x] = 1;
                    }
                }
            }
        }
        
        drop_y[x]++;
        if (drop_y[x] - drop_len[x] > SIZE) {
            drop_y[x] = -(rand() % 5);
            drop_len[x] = 3 + (rand() % 5);
        }
    }
}

int main() {
    if (wiringPiSetupGpio() < 0) return 1;
    croix.begin();
    srand(time(NULL));
    init_matrix();
    
    while (true) {
        step_matrix();
        croix.setSide(CroixPharma::BOTH);
        croix.writeBitmap(bitmap);
        
        delay(60); // fast Matrix rain speed
    }
    return 0;
}
