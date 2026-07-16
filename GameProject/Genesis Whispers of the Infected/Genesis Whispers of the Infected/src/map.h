#ifndef MAP_H
#define MAP_H

#include <vector>

struct Platform {
    double x, y, width, height;
};

class Map {
private:
    std::vector<Platform> platforms;
    double cameraX;
    double cameraY;
    int levelWidth;
    int levelHeight;

public:
    Map();
    void LoadLevel(int levelNumber);
    void RenderBackground(double cameraX);
    void RenderPlatforms(double cameraX);
    void ApplyCameraTracking(double playerX, double playerY, int screenWidth, int screenHeight);
    
    const std::vector<Platform>& GetPlatforms() const { return platforms; }
    double GetCameraX() const { return cameraX; }
    double GetCameraY() const { return cameraY; }
};

#endif
