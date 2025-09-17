#pragma once
#include <SFML/Graphics.hpp>
#include "TileMap.h" // ��ͼ�࣬������ͼ���ݺ͸�����Ϣ
#include "Bullet.h"
#include <iostream>
#include <SFML/Audio.hpp>
#include <stdexcept> // �����쳣����

class Kirby {
public:
    Kirby(); // ���캯������ʼ����ɫ����

    void setMap(TileMap* map);           // �󶨵�ͼָ�룬���ڲ�ѯ��ͼ��������
    void setPosition(float x, float y);  // ���ý�ɫ�ĳ�ʼλ��
    void handleInput();                  // ����������룬�ı��ɫ�ٶ�
    void update(float dt,int gamelevel=1);               // ÿ֡���½�ɫ״̬��dtΪʱ����
    void draw(sf::RenderWindow& window); // ���ƽ�ɫ������
    sf::Vector2f getPosition();          // ��ȡ��ɫ��ǰλ��
    sf::Vector2f getSize() const;

    int getHealth() const { return health; }
    void takeDamage(int amount) 
    {
        health -= amount;
        if (health <= 0) {
            dead = true;
            health = 0;
        }
    }
    float hurtCooldown = 0.f; // ������ȴ
    void sethealth(int amount)
    {
        health = amount;
    }
    void attack();
    bool isDead() const;                 // ��ѯ��ɫ�Ƿ�����
    void setLives(int live)
    {
        dead = !live;
    }
    // �������жϽ�ɫ�ŵ��Ƿ���ڵ��涥��
    bool isOnGround();
    bool isOnLadder() const;
    //�Զ��䵽������
    void fixInitialPosition();

    bool isOnWaterSurface(); // �ж��Ƿ����·�ĳ����ˮ
    TileType getCurrentTileType();

    sf::FloatRect getGlobalBounds() const {
        return body.getGlobalBounds();
    }

    const std::vector<Bullet>& getBullets() const { return bullets; }
    std::vector<Bullet>& getBullets() { return bullets; }
    friend class BoostItem;

    bool hasAttackBoost = false;
    sf::Clock attackBoostTimer;

    bool isInvincible = false;
    sf::Clock invincibilityTimer;

    bool hasSpeedBoost = false;
    sf::Clock speedBoostTimer;

    void loadSounds();  //������Ƶ��Դ
private:
    std::array<sf::Texture, 7> runTextures = {
        sf::Texture("image/kirbyImage/kirby1.png"), sf::Texture("image/kirbyImage/kirby2.png"), sf::Texture("image/kirbyImage/kirby3.png"),
        sf::Texture("image/kirbyImage/kirby4.png"), sf::Texture("image/kirbyImage/kirby5.png"), sf::Texture("image/kirbyImage/kirby6.png"),
        sf::Texture("image/kirbyImage/kirby7.png")
    };
    sf::Texture texture_zero;
    sf::Sprite body;
    sf::Vector2f velocity = { 0.f,0.f };               // ��ɫ��ǰ�ٶȣ�ˮƽ�ʹ�ֱ��
    bool onGround;                      // ��ɫ�Ƿ�վ�ڵ��棨�ŵײ��ŵر�
    bool climbing;                      // ��ɫ�Ƿ���������״̬
    bool dead;                         // ��ɫ�Ƿ�������״̬
    bool iswin;
    TileMap* tileMap;                  // ָ���ͼʵ����ָ�룬�����ѯ��������

    void applyGravity(float dt);        // ����ɫӦ��������Ӱ�촹ֱ�ٶ�
    void checkCollisions();             // ��ײ��⣬�����ɫ���ͼ�Ľ���
    // ��������ķ���
    void loadTextures();

    int currentFrame = 0;
    float frameTime = 0.1f; // ÿ 0.1 ���л�һ֡
    float frameDuration = 0.08f;
    bool isRunning = false;
    bool facingRight = true;
    float scaleFactor = 0.075f; // �����������

    int health = 100;
    int maxHealth = 100;
    std::vector<Bullet> bullets;
    float bulletCooldown = 0.f;

    sf::SoundBuffer attackBuffer;
    sf::Sound attackSound;
};
