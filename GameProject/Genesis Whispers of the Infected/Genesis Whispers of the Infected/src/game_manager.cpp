#define _CRT_SECURE_NO_WARNINGS
#include "game_manager.h"
#include "igraphics_declarations.h"
#include <cstdio>
#include <cstring>

GameManager::GameManager() {
    currentState = STATE_MENU;
    score = 0;
    currentLevel = 1;
    isGamePaused = false;
}

void GameManager::Initialize() {
    score = 0;
    currentLevel = 1;
    player.Initialize(100, 150); // Start position
    gameMap.LoadLevel(currentLevel);
    leaderboard.LoadScores();

    // Populate Level 1 enemies
    enemies.clear();
    enemies.push_back(Enemy(400, 700, 150, TYPE_RUNNER));
    enemies.push_back(Enemy(900, 1200, 150, TYPE_RUNNER));
}

void GameManager::Update(bool keys[], bool specialKeys[]) {
    if (currentState == STATE_PLAYING) {
        UpdatePlaying(keys, specialKeys);
    }
}

void GameManager::UpdatePlaying(bool keys[], bool specialKeys[]) {
    // 1. Update Player position and state
    player.Update(keys, specialKeys);

    // 2. Apply collisions with platforms
    const std::vector<Platform>& platforms = gameMap.GetPlatforms();
    player.isGrounded = false;
    
    for (size_t i = 0; i < platforms.size(); ++i) {
        const Platform& p = platforms[i];
        
        // Vertical Collision (Landing on platform)
        if (player.x + player.width > p.x && player.x < p.x + p.width) {
            if (player.vy <= 0 && player.y >= p.y + p.height - 10 && player.y + player.vy <= p.y + p.height) {
                player.y = p.y + p.height;
                player.vy = 0;
                player.isGrounded = true;
            }
        }
    }

    // 3. Update Camera to track player
    gameMap.ApplyCameraTracking(player.x, player.y, 1024, 768);

    // 4. Update Enemies
    for (size_t i = 0; i < enemies.size(); ++i) {
        if (enemies[i].hp > 0) {
            enemies[i].Update(player.x, player.y);
            
            // Check collision with player to deal damage
            if (enemies[i].CheckPlayerCollision(player.x, player.y, player.width, player.height)) {
                player.TakeDamage(enemies[i].damage);
            }
        }
    }

    // Check game over
    if (player.hp <= 0) {
        currentState = STATE_GAMEOVER;
    }
}

void GameManager::Render() {
    switch (currentState) {
        case STATE_MENU:
            RenderMenu();
            break;
        case STATE_PLAYING:
            RenderPlaying();
            break;
        case STATE_DIALOGUE:
            RenderDialogue();
            break;
        case STATE_PAUSED:
            RenderPlaying();
            // Overlay Pause text
            iSetColor(0, 0, 0);
            iFilledRectangle(362, 284, 300, 200);
            iSetColor(255, 255, 255);
            iText(450, 400, "GAME PAUSED", GLUT_BITMAP_TIMES_ROMAN_24);
            iText(410, 340, "Press ESC to Resume", GLUT_BITMAP_HELVETICA_18);
            break;
        case STATE_GAMEOVER:
            RenderGameOver();
            break;
        case STATE_VICTORY:
            RenderVictory();
            break;
        case STATE_LEADERBOARD:
            RenderLeaderboard();
            break;
    }
}

void GameManager::RenderMenu() {
    // Cyberpunk/Post-apocalyptic colored background
    iSetColor(15, 15, 25);
    iFilledRectangle(0, 0, 1024, 768);

    iSetColor(0, 230, 230);
    iText(220, 550, "GENESIS: WHISPERS OF THE INFECTED", GLUT_BITMAP_TIMES_ROMAN_24);

    // Render Buttons
    iSetColor(30, 30, 50);
    iFilledRectangle(362, 380, 300, 50);
    iFilledRectangle(362, 300, 300, 50);
    iFilledRectangle(362, 220, 300, 50);

    iSetColor(255, 255, 255);
    iText(450, 395, "1. PLAY GAME", GLUT_BITMAP_HELVETICA_18);
    iText(420, 315, "2. GENESIS ARCHIVE", GLUT_BITMAP_HELVETICA_18);
    iText(480, 235, "3. EXIT", GLUT_BITMAP_HELVETICA_18);
}

void GameManager::RenderPlaying() {
    double camX = gameMap.GetCameraX();
    double camY = gameMap.GetCameraY();

    // 1. Draw backgrounds (scrolling offsets applied)
    gameMap.RenderBackground(camX);

    // 2. Draw platforms
    gameMap.RenderPlatforms(camX);

    // 3. Draw enemies relative to camera
    for (size_t i = 0; i < enemies.size(); ++i) {
        if (enemies[i].hp > 0) {
            // Transform world coordinate to screen coordinate
            double screenEx = enemies[i].x - camX;
            double screenEy = enemies[i].y - camY;
            
            // Draw placeholder box for enemy
            iSetColor(200, 50, 50);
            iFilledRectangle(screenEx, screenEy, enemies[i].width, enemies[i].height);
            
            // Health bar above enemy
            iSetColor(50, 50, 50);
            iFilledRectangle(screenEx, screenEy + enemies[i].height + 5, enemies[i].width, 6);
            iSetColor(255, 0, 0);
            double hpPercent = (double)enemies[i].hp / enemies[i].maxHp;
            iFilledRectangle(screenEx, screenEy + enemies[i].height + 5, enemies[i].width * hpPercent, 6);
        }
    }

    // 4. Draw Player relative to camera
    double screenPx = player.x - camX;
    double screenPy = player.y - camY;

    iSetColor(0, 200, 0);
    iFilledRectangle(screenPx, screenPy, player.width, player.height);

    // Draw HUD
    iSetColor(20, 20, 30);
    iFilledRectangle(10, 700, 300, 58); // Health panel background

    // HP Bar
    iSetColor(50, 50, 50);
    iFilledRectangle(20, 720, 200, 15);
    iSetColor(0, 255, 100);
    double playerHpPercent = (double)player.hp / player.maxHp;
    iFilledRectangle(20, 720, 200 * playerHpPercent, 15);

    iSetColor(255, 255, 255);
    char hpStr[32];
    sprintf(hpStr, "HP: %d/%d", player.hp, player.maxHp);
    iText(20, 705, hpStr, GLUT_BITMAP_HELVETICA_12);

    char scoreStr[32];
    sprintf(scoreStr, "SCORE: %07d", score);
    iText(850, 730, scoreStr, GLUT_BITMAP_HELVETICA_18);

    char ammoStr[32];
    sprintf(ammoStr, "AMMO: %d/30", player.ammo);
    iText(850, 705, ammoStr, GLUT_BITMAP_HELVETICA_18);
}

void GameManager::RenderDialogue() {
    RenderPlaying(); // Show gameplay in background
    
    // Transparent dialogue box at bottom
    iSetColor(10, 10, 20);
    iFilledRectangle(50, 40, 924, 150);

    iSetColor(0, 255, 255);
    iRectangle(50, 40, 924, 150);

    iSetColor(255, 255, 255);
    iText(70, 150, "Arin:", GLUT_BITMAP_HELVETICA_18);
    iText(70, 100, "\"I promise you, Luna, I will find you no matter what it takes.\"", GLUT_BITMAP_HELVETICA_18);
    iText(800, 55, "Press ENTER to continue", GLUT_BITMAP_HELVETICA_12);
}

void GameManager::RenderGameOver() {
    iSetColor(20, 10, 10);
    iFilledRectangle(0, 0, 1024, 768);

    iSetColor(255, 0, 0);
    iText(420, 450, "GAME OVER", GLUT_BITMAP_TIMES_ROMAN_24);

    iSetColor(255, 255, 255);
    iText(400, 380, "Press ENTER to return to Menu", GLUT_BITMAP_HELVETICA_18);
}

void GameManager::RenderVictory() {
    iSetColor(10, 25, 10);
    iFilledRectangle(0, 0, 1024, 768);

    iSetColor(0, 255, 100);
    iText(440, 450, "VICTORY ACHIEVED", GLUT_BITMAP_TIMES_ROMAN_24);

    iSetColor(255, 255, 255);
    iText(360, 380, "You rescued Luna and saved the project files.", GLUT_BITMAP_HELVETICA_18);
    iText(400, 300, "Press ENTER to return to Menu", GLUT_BITMAP_HELVETICA_18);
}

void GameManager::RenderLeaderboard() {
    iSetColor(15, 15, 25);
    iFilledRectangle(0, 0, 1024, 768);

    iSetColor(0, 255, 255);
    iText(400, 650, "GENESIS ARCHIVE - LEADERBOARD", GLUT_BITMAP_TIMES_ROMAN_24);

    const std::vector<ScoreEntry>& entries = leaderboard.GetEntries();
    iSetColor(255, 255, 255);
    
    if (entries.empty()) {
        iText(420, 400, "No records found.", GLUT_BITMAP_HELVETICA_18);
    } else {
        for (size_t i = 0; i < entries.size() && i < 5; ++i) {
            char row[128];
            sprintf(row, "%d.  %-20s   Score: %07d", (int)(i + 1), entries[i].name, entries[i].score);
            iText(350, 500 - (i * 60), row, GLUT_BITMAP_HELVETICA_18);
        }
    }

    iSetColor(0, 250, 150);
    iText(410, 100, "Press ESC to Return to Menu", GLUT_BITMAP_HELVETICA_18);
}

void GameManager::HandleKeyPress(unsigned char key) {
    if (currentState == STATE_MENU) {
        if (key == '1') {
            currentState = STATE_PLAYING;
            Initialize(); // Fresh start
        } else if (key == '2') {
            currentState = STATE_LEADERBOARD;
        } else if (key == '3') {
            exit(0);
        }
    } else if (currentState == STATE_PLAYING) {
        if (key == 27) { // ESC Key
            currentState = STATE_PAUSED;
        } else if (key == 'j' || key == 'J') {
            player.AttackMelee();
        } else if (key == 'k' || key == 'K') {
            player.AttackRanged();
        } else if (key == 'h' || key == 'H') {
            player.UseHeal();
        }
    } else if (currentState == STATE_PAUSED) {
        if (key == 27) {
            currentState = STATE_PLAYING;
        }
    } else if (currentState == STATE_DIALOGUE) {
        if (key == 13) { // Enter key
            currentState = STATE_PLAYING;
        }
    } else if (currentState == STATE_GAMEOVER || currentState == STATE_VICTORY) {
        if (key == 13) { // Enter key
            currentState = STATE_MENU;
        }
    } else if (currentState == STATE_LEADERBOARD) {
        if (key == 27) {
            currentState = STATE_MENU;
        }
    }
}

void GameManager::HandleSpecialKeyPress(unsigned char key) {
    // Handled in player.cpp directly using the keysPressed / specialKeysPressed arrays
}

void GameManager::HandleMouseClick(int button, int mx, int my) {
    // Menu interactions can go here based on coordinates
}

void GameManager::HandleMouseMove(int mx, int my) {
    // Mouse hover/tracking features
}

void GameManager::AddScore(int amount) {
    score += amount;
}
