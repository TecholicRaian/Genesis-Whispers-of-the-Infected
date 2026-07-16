#include "enemy.h"
#include <cmath>

Enemy::Enemy(double sX, double eX, double startY, EnemyType t) {
    startX = sX;
    endX = eX;
    x = sX;
    y = startY;
    type = t;
    isFacingRight = true;
    animFrame = 0;
    state = ENEMY_PATROL;

    // Apply stats depending on enemy type
    switch (type) {
        case TYPE_RUNNER:
            width = 32;
            height = 54;
            maxHp = 30;
            damage = 10;
            vx = 2.0; // Patrol speed
            break;
        case TYPE_SPITTER:
            width = 40;
            height = 40;
            maxHp = 45;
            damage = 12;
            vx = 0.0; // Stationary
            break;
        case TYPE_ABOMINATION:
            width = 60;
            height = 80;
            maxHp = 150;
            damage = 25;
            vx = 1.0;
            break;
    }
    hp = maxHp;
}

void Enemy::Update(double playerX, double playerY) {
    if (hp <= 0) {
        state = ENEMY_DEAD;
        return;
    }

    double dx = playerX - x;
    double dy = playerY - y;
    double distance = std::sqrt(dx*dx + dy*dy);

    // AI State Decisions
    if (type == TYPE_RUNNER || type == TYPE_ABOMINATION) {
        if (distance < 250.0) { // Chase radius
            state = ENEMY_CHASE;
        } else {
            state = ENEMY_PATROL;
        }

        // Action based on states
        if (state == ENEMY_CHASE) {
            double chaseSpeed = (type == TYPE_RUNNER) ? 3.5 : 1.5;
            if (dx > 0) {
                x += chaseSpeed;
                isFacingRight = true;
            } else {
                x -= chaseSpeed;
                isFacingRight = false;
            }
        } else if (state == ENEMY_PATROL) {
            // Move back and forth between bounds
            double patrolSpeed = (type == TYPE_RUNNER) ? 2.0 : 0.8;
            if (isFacingRight) {
                x += patrolSpeed;
                if (x >= endX) {
                    x = endX;
                    isFacingRight = false;
                }
            } else {
                x -= patrolSpeed;
                if (x <= startX) {
                    x = startX;
                    isFacingRight = true;
                }
            }
        }
    }
}

void Enemy::Render() {
    // Rendered inside game_manager.cpp relative to camera.
}

void Enemy::TakeDamage(int amount) {
    hp -= amount;
    if (hp < 0) hp = 0;
}

bool Enemy::CheckPlayerCollision(double px, double py, int pw, int ph) {
    if (hp <= 0) return false;

    // Simple bounding box overlap (AABB collision check)
    bool collisionX = (x + width >= px) && (px + pw >= x);
    bool collisionY = (y + height >= py) && (py + ph >= y);
    
    return collisionX && collisionY;
}
