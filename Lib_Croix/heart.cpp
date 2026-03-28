#include <wiringPi.h>
#include <cstdint>
#include <cstring>
#include <cmath>
#include "CroixPharma.h"

CroixPharma croix;
uint8_t bitmap[SIZE][SIZE];

bool in_cross(int x, int y) {
    if (x >= 8 && x < 16 && y >= 0 && y < 24) return true;
    if (y >= 8 && y < 16 && x >= 0 && x < 24) return true;
    return false;
}

void clear_bitmap() {
    memset(bitmap, 0, sizeof(bitmap));
}

// Draw a simple heart shape
// Size ranges from 0 to 4
void draw_heart(int size) {
    clear_bitmap();
    if (size <= 0) return;
    
    // Heart coordinates relative to center (11.5, 11L5)
    // We'll use a simple threshold equation: (x^2 + y^2 - 1)^3 - x^2 * y^3 <= 0
    // But it's easier to just draw it parametrically or with hardcoded frames.
    
    // For small pixel art, hardcoded frames look best.
    const char* frames[5][24] = {
        // Frame 0: Empty
        {0},
        // Frame 1: Small point
        {
            "                        ",
            "                        ",
            "                        ",
            "                        ",
            "                        ",
            "                        ",
            "                        ",
            "                        ",
            "                        ",
            "                        ",
            "       * *   * *        ",
            "      *   * *   *       ",
            "      *         *       ",
            "       *       *        ",
            "        *     *         ",
            "         *   *          ",
            "          * *           ",
            "           *            ",
            "                        ",
            "                        ",
            "                        ",
            "                        ",
            "                        ",
            "                        "
        },
        // Frame 2: Medium
        {
            "                        ",
            "                        ",
            "                        ",
            "                        ",
            "                        ",
            "                        ",
            "                        ",
            "          *   *         ",
            "        *   *   *       ",
            "       *    *    *      ",
            "      *           *     ",
            "      *           *     ",
            "      *           *     ",
            "       *         *      ",
            "        *       *       ",
            "         *     *        ",
            "          *   *         ",
            "           * *          ",
            "            *           ",
            "                        ",
            "                        ",
            "                        ",
            "                        ",
            "                        "
        },
        // Frame 3: Large
        {
            "                        ",
            "                        ",
            "                        ",
            "                        ",
            "                        ",
            "                        ",
            "          ***   ***     ",
            "        **   * *   **   ",
            "       *      *      *  ",
            "      *               * ",
            "      *               * ",
            "      *               * ",
            "       *             *  ",
            "        *           *   ",
            "         *         *    ",
            "          *       *     ",
            "           *     *      ",
            "            *   *       ",
            "             * *        ",
            "              *         ",
            "                        ",
            "                        ",
            "                        ",
            "                        "
        },
        // Frame 4: Max
        {
            "                        ",
            "                        ",
            "                        ",
            "                        ",
            "                        ",
            "                        ",
            "         ***   ***      ",
            "       **   ***   **    ",
            "      *             *   ",
            "     *               *  ",
            "     *               *  ",
            "     *               *  ",
            "      *             *   ",
            "       *           *    ",
            "        *         *     ",
            "         *       *      ",
            "          *     *       ",
            "           *   *        ",
            "            * *         ",
            "             *          ",
            "                        ",
            "                        ",
            "                        ",
            "                        "
        }
    };
    
    if (size > 4) size = 4;
    for (int y = 0; y < 24; y++) {
        if (!frames[size][y]) continue;
        for (int x = 0; x < 24; x++) {
            if (frames[size][y][x] == '*') {
                // translate to center perfectly
                int dx = x - 2;
                if (dx >= 0 && dx < 24 && in_cross(dx, y)) {
                    bitmap[y][dx] = 1;
                }
            }
        }
    }
}

int main() {
    if (wiringPiSetupGpio() < 0) return 1;
    croix.begin();
    
    int sizes[] = {1, 2, 3, 4, 3, 2, 1};
    int current = 0;
    
    while (true) {
        draw_heart(sizes[current]);
        croix.setSide(CroixPharma::BOTH);
        croix.writeBitmap(bitmap);
        
        // Fast beat on max size, slower elsewhere
        if (sizes[current] == 4) delay(100);
        else delay(80);
        
        current = (current + 1) % 7;
        
        // Pause between beats
        if (current == 0) delay(400);
    }
    return 0;
}
