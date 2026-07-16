#include "../iGraphics.h"
#include "game_manager.h"

// Instantiate the global Game Manager
GameManager g_gameManager;

// Global array to track pressed keys for smooth fluid movement
bool keysPressed[256] = { false };
bool specialKeysPressed[256] = { false };

// Track key transitions to detect single-press events (e.g. Menu presses)
bool prevKeys[512] = { false };
bool prevSpecialKeys[512] = { false };

/*
  iDraw() is called continuously by the iGraphics loop to render scenes.
  All drawing commands (e.g., iShowBMP, iSetColor, iFilledRectangle) must be inside this function or functions called by it.
*/
void iDraw() {
    iClear();
    g_gameManager.Render();
}

/*
  iMouseMove() is called when the mouse is dragged (moved with a button pressed).
*/
void iMouseMove(int mx, int my) {
    g_gameManager.HandleMouseMove(mx, my);
}

/*
  iMouse() is called when a mouse button is clicked (pressed or released).
*/
void iMouse(int button, int state, int mx, int my) {
    if (state == GLUT_DOWN) {
        g_gameManager.HandleMouseClick(button, mx, my);
    }
}

/*
  iKeyboard() is called when a standard ASCII key is pressed (e.g., letters, space, enter).
*/
void iKeyboard(unsigned char key) {
    keysPressed[key] = true;
    g_gameManager.HandleKeyPress(key);
}

/*
  iKeyboardUp() is called when a standard ASCII key is released.
  This allows for continuous movement tracking.
*/
void iKeyboardUp(unsigned char key) {
    keysPressed[key] = false;
}

/*
  iSpecialKeyboard() is called when non-ASCII special keys (arrows, F1-F12) are pressed.
*/
void iSpecialKeyboard(unsigned char key) {
    specialKeysPressed[key] = true;
    g_gameManager.HandleSpecialKeyPress(key);
}

/*
  iSpecialKeyboardUp() is called when non-ASCII special keys are released.
*/
void iSpecialKeyboardUp(unsigned char key) {
    specialKeysPressed[key] = false;
}

/*
  iPassiveMouseMove() is called when the mouse moves without any button pressed.
*/
void iPassiveMouseMove(int mx, int my) {
    // Empty implementation
}

/*
  fixedUpdate() is called by the keypressHandler timer in iGraphics.h.
*/
void fixedUpdate() {
    // 1. Detect standard key press transitions (Press event for Menus/Dialogues)
    for (int i = 0; i < 256; ++i) {
        bool isPressed = (keyPressed[i] != 0);
        if (isPressed && !prevKeys[i]) {
            g_gameManager.HandleKeyPress(i);
        }
        prevKeys[i] = isPressed;
    }

    // 2. Detect special key press transitions (Press event)
    for (int i = 0; i < 256; ++i) {
        bool isPressed = (specialKeyPressed[i] != 0);
        if (isPressed && !prevSpecialKeys[i]) {
            g_gameManager.HandleSpecialKeyPress(i);
        }
        prevSpecialKeys[i] = isPressed;
    }
}

/*
  Physics and game tick update function.
  This timer callback is triggered by iSetTimer in main() to run physics independently of frame-rate.
*/
void GameTick() {
    // Sync iGraphics keyboard states to our boolean arrays for gameplay physics updates
    for (int i = 0; i < 256; ++i) {
        keysPressed[i] = (keyPressed[i] != 0);
        specialKeysPressed[i] = (specialKeyPressed[i] != 0);
    }
    g_gameManager.Update(keysPressed, specialKeysPressed);
}

int main() {
    // Initialize the Game Manager
    g_gameManager.Initialize();

    // Set a timer for game updates (approx 60 ticks per second: 1000ms / 60 ≈ 16ms)
    iSetTimer(16, GameTick);

    // Initialize the window: size 1024x768, title "Genesis: Whispers of the Infected"
    iInitialize(1024, 768, "Genesis: Whispers of the Infected");

    // iStart() starts the rendering and event handling loop (blocking call)
    iStart(); 
    return 0;
}
