#include "player.h"

// Define special key codes standard in GLUT
#define KEY_LEFT 100
#define KEY_UP 101
#define KEY_RIGHT 102
#define KEY_DOWN 103

Player::Player() {
    width = 40;
    height = 64;
    maxHp = 100;
    Initialize(100, 150);
}

void Player::Initialize(double startX, double startY) {
    x = startX;
    y = startY;
    vx = 0;
    vy = 0;
    hp = maxHp;
    ammo = 15;
    medkits = 2;
    isGrounded = false;
    isFacingRight = true;
    isInvulnerable = false;
    invulnerabilityTimer = 0;
    state = STATE_IDLE;
    currentAnimationFrame = 0;
    frameDelayCounter = 0;
}

void Player::Update(bool keys[], bool specialKeys[]) {
    // 1. Horizontal Movement Physics
    // In iGraphics, holding Shift yields uppercase character codes (A, D, etc.)
    bool isRunning = keys['A'] || keys['D'] || keys['r'] || keys['R'];
    double speed = isRunning ? 6.0 : 4.0;
    vx = 0;

    if (keys['a'] || keys['A'] || specialKeys[KEY_LEFT]) {
        vx = -speed;
        isFacingRight = false;
        state = STATE_WALK;
    }
    if (keys['d'] || keys['D'] || specialKeys[KEY_RIGHT]) {
        vx = speed;
        isFacingRight = true;
        state = STATE_WALK;
    }

    if (vx == 0) {
        state = STATE_IDLE;
    }

    // 2. Jump Physics
    if ((keys['w'] || keys['W'] || keys[' '] || specialKeys[KEY_UP]) && isGrounded) {
        vy = 10.0; // Jump force
        isGrounded = false;
        state = STATE_JUMP;
    }

    // Apply gravity
    if (!isGrounded) {
        vy -= 0.45; // Gravity acceleration
        if (vy < -12) vy = -12; // Terminal velocity
    }

    // Update position
    x += vx;
    y += vy;

    // Boundary constraints (Level bounds)
    if (x < 0) x = 0;
    if (y < 0) {
        y = 0;
        vy = 0;
        isGrounded = true;
    }

    // 3. Invulnerability ticks
    if (isInvulnerable) {
        invulnerabilityTimer--;
        if (invulnerabilityTimer <= 0) {
            isInvulnerable = false;
        }
    }

    // 4. Animation frame counter
    frameDelayCounter++;
    if (frameDelayCounter > 8) { // update animation state every 8 game ticks
        frameDelayCounter = 0;
        currentAnimationFrame = (currentAnimationFrame + 1) % 4; // Assuming 4 frames per animation
    }
}

void Player::Render() {
    // Player is rendered in game_manager.cpp relative to camera.
}

void Player::TakeDamage(int damage) {
    if (isInvulnerable) return;

    hp -= damage;
    if (hp < 0) hp = 0;

    isInvulnerable = true;
    invulnerabilityTimer = 60; // 1 second invincibility at 60 FPS
}

void Player::AttackMelee() {
    state = STATE_ATTACK_MELEE;
}

void Player::AttackRanged() {
    if (ammo > 0) {
        ammo--;
    }
}

void Player::UseHeal() {
    if (medkits > 0 && hp < maxHp) {
        medkits--;
        hp += 35;
        if (hp > maxHp) hp = maxHp;
    }
}
