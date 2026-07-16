#ifndef ENEMY_H
#define ENEMY_H

enum EnemyType { 
    TYPE_RUNNER, 
    TYPE_SPITTER, 
    TYPE_ABOMINATION 
};

enum EnemyState { 
    ENEMY_PATROL, 
    ENEMY_CHASE, 
    ENEMY_ATTACK, 
    ENEMY_DEAD 
};

class Enemy {
public:
    double x, y;
    double startX, endX; // Patrol bounds
    double vx;
    int width, height;
    
    int hp;
    int maxHp;
    int damage;
    bool isFacingRight;
    
    EnemyType type;
    EnemyState state;
    int animFrame;

    Enemy(double startX, double endX, double y, EnemyType type);
    void Update(double playerX, double playerY);
    void Render();
    void TakeDamage(int damage);
    bool CheckPlayerCollision(double px, double py, int pw, int ph);
};

#endif
