#ifndef PLAYER_H
#define PLAYER_H

enum PlayerState { 
    STATE_IDLE, 
    STATE_WALK, 
    STATE_RUN, 
    STATE_JUMP, 
    STATE_ATTACK_MELEE, 
    STATE_HURT, 
    STATE_DEAD 
};

class Player {
public:
    double x, y;
    double vx, vy;
    int width, height;
    
    int hp;
    int maxHp;
    int ammo;
    int medkits;
    
    bool isGrounded;
    bool isFacingRight;
    bool isInvulnerable;
    int invulnerabilityTimer;

    PlayerState state;
    int currentAnimationFrame;
    int frameDelayCounter;

    Player();
    void Initialize(double startX, double startY);
    void Update(bool keys[], bool specialKeys[]);
    void Render();
    void TakeDamage(int damage);
    void AttackMelee();
    void AttackRanged();
    void UseHeal();
};

#endif
