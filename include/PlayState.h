#pragma once
#include "Kirby.h"
#include "Enemy.h"
#include "BoostItem.hpp"
#include <vector>
#include <string>
#include <fstream>
#include <filesystem>   //�����ļ�ϵͳ��
#include <iostream>
#include "Constants.h"


// PlayState.h
#pragma once
#include <vector>
// Game.h
struct PlayState {
    int level = 1;
    float playerX = 0.f;
    float playerY = 0.f;
    int health = 100;
    bool isInvincible = false;
    float hurtCooldown = 0.f;
};
// ��Ϸ�߼�����
bool isLevelFinished(Kirby& player, TileMap* tileMap);