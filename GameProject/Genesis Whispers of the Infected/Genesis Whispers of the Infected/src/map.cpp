#include "map.h"
#include "igraphics_declarations.h"

Map::Map() {
    cameraX = 0;
    cameraY = 0;
    levelWidth = 3000;
    levelHeight = 768;
}

void Map::LoadLevel(int levelNumber) {
    platforms.clear();

    if (levelNumber == 1) {
        // Floor platform
        platforms.push_back({0, 0, 3000, 150});

        // Floating platforms for exploration/combat
        platforms.push_back({300, 300, 200, 20});
        platforms.push_back({600, 420, 250, 20});
        platforms.push_back({1000, 300, 180, 20});
        platforms.push_back({1300, 450, 300, 20});
        platforms.push_back({1800, 300, 200, 20});
        platforms.push_back({2100, 420, 250, 20});
    }
}

void Map::RenderBackground(double camX) {
    // Parallax Layer 1 (Distant Sky) - Scrolling at 10% speed
    iSetColor(10, 10, 25);
    iFilledRectangle(0, 0, 1024, 768);

    // Draw some simple stars/moon as distant structures
    iSetColor(40, 40, 60);
    iFilledCircle(900 - (camX * 0.05), 600, 60); // Moon

    // Parallax Layer 2 (Silhouette Mountains/Buildings) - Scrolling at 30% speed
    iSetColor(20, 20, 35);
    for (int i = 0; i < 4; ++i) {
        double bx = (i * 400) - (camX * 0.3);
        iFilledRectangle(bx, 150, 300, 300);
    }
}

void Map::RenderPlatforms(double camX) {
    iSetColor(45, 45, 60); // Dark stone platform color

    for (size_t i = 0; i < platforms.size(); ++i) {
        double screenX = platforms[i].x - camX;
        double screenY = platforms[i].y; // Camera only scrolls horizontally for this level
        
        // Draw the platform on screen if it is visible
        if (screenX + platforms[i].width >= 0 && screenX <= 1024) {
            iFilledRectangle(screenX, screenY, platforms[i].width, platforms[i].height);
            
            // Draw a highlighted top edge
            iSetColor(0, 200, 200);
            iFilledRectangle(screenX, screenY + platforms[i].height - 4, platforms[i].width, 4);
            iSetColor(45, 45, 60); // Reset color
        }
    }
}

void Map::ApplyCameraTracking(double playerX, double playerY, int screenWidth, int screenHeight) {
    // Keep player centered in the middle third of the screen
    double targetCameraX = playerX - (screenWidth / 2.0);

    // Smooth camera interpolation
    cameraX += (targetCameraX - cameraX) * 0.1;

    // Bounds checking: Keep camera within level boundaries
    if (cameraX < 0) cameraX = 0;
    if (cameraX > (levelWidth - screenWidth)) cameraX = levelWidth - screenWidth;
    
    // Y camera lock
    cameraY = 0;
}
