#pragma once
#include "TileMap.h"
#include <SFML/Graphics.hpp>  // һ��Ҫ�������
#include "GameLevel.h"
#include "PlayState.h"
#include "Kirby.h"
#include "Enemy.h"
#include "BulletManager.h"
#include "BoostItem.hpp"
#include "MainMenu.h"
#include "GameOverScene.h"
#include <vector>
#include <map>
#include <SFML/Audio.hpp>

class Game {
public:
    Game(Kirby& player);
    void loadLevel(PlayState& currentplaystate1, Kirby& player, bool resetToSpawnPoint = true);//���ص�ͼ
    void update(Kirby& player, float deltaTime, sf::RenderWindow& window);//������Ҷ�����λ�õ���Ϣ���Լ��Ƿ�ɹ�/ʧ��
    void draw(sf::RenderWindow& window, Kirby& player);//����ͼ����һ���ȥ
    TileMap& getTileMap();

    void createBoostItems();
    void createEnemies(); // ��������
    bool intersects(const sf::FloatRect& a, const sf::FloatRect& b);

    bool gameOver = false;

    void saveGameState(Kirby& player); // �浵����
    bool loadGameState(Kirby& player); // ��������

    void loadSounds();  //������Ƶ��Դ
private:
    TileMap map;
    sf::Texture tileset;    // ��Ƭ��������ͼ��
    PlayState currentplaystate;
    sf::Vector2f spawnPoint;

    //�����������
    sf::Texture healTexture;
    sf::Texture damageUpTexture;
    sf::Texture invincibleTexture;
    sf::Texture hasteTexture;
    std::vector<BoostItem> boostItems;
    sf::Texture boostTexture;

    sf::Texture enemyTexture;   // ��������
    std::vector<std::unique_ptr<Enemy>> enemies; // ʹ������ָ�����˶���

    sf::SoundBuffer pengBuffer;
    sf::Sound pengSound;

    // ������ð�ť����;���
    sf::Texture settingsButtonTexture;
};