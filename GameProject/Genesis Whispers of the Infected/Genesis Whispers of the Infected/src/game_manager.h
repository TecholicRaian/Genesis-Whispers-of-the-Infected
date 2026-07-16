#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include "player.h"
#include "map.h"
#include "enemy.h"
#include "leaderboard.h"
#include <vector>

enum GameState {
    STATE_MENU,
    STATE_PLAYING,
    STATE_DIALOGUE,
    STATE_PAUSED,
    STATE_GAMEOVER,
    STATE_VICTORY,
    STATE_LEADERBOARD
};

class GameManager {
private:
    GameState currentState;
    Player player;
    Map gameMap;
    Leaderboard leaderboard;
    std::vector<Enemy> enemies;

    int score;
    int currentLevel;
    bool isGamePaused;

    // Helper functions for localized state updates/rendering
    void UpdatePlaying(bool keys[], bool specialKeys[]);
    void RenderPlaying();
    void RenderMenu();
    void RenderLeaderboard();
    void RenderDialogue();
    void RenderGameOver();
    void RenderVictory();

public:
    GameManager();
    
    void Initialize();
    void Update(bool keys[], bool specialKeys[]);
    void Render();

    // Input hooks from iGraphics
    void HandleKeyPress(unsigned char key);
    void HandleSpecialKeyPress(unsigned char key);
    void HandleMouseClick(int button, int mx, int my);
    void HandleMouseMove(int mx, int my);

    // Score and Level utility
    void AddScore(int amount);
    void TransitionToLevel(int levelNum);
    
    GameState GetCurrentState() const { return currentState; }
    void SetCurrentState(GameState state) { currentState = state; }
};

#endif
