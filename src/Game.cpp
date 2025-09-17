#include "Game.h"
#include <iostream>
#include "PlayState.h"
#include "BulletManager.h"
#include "Setting.h"

Game::Game(Kirby& player)//���õ���player������
    : map(), currentplaystate(), pengSound(pengBuffer){
    if (!healTexture.loadFromFile("image/item.png")) {
        std::cerr << "���Ƶ����������ʧ��!" << std::endl;
    }
    if (!damageUpTexture.loadFromFile("image/item.png")) {
        std::cerr << "�������������������ʧ��!" << std::endl;
    }
    if (!invincibleTexture.loadFromFile("image/item.png")) {
        std::cerr << "�޵е����������ʧ��!" << std::endl;
    }
    if (!hasteTexture.loadFromFile("image/item.png")) {
        std::cerr << "���ٵ����������ʧ��!" << std::endl;
    }
    //���ص�������
    if (!enemyTexture.loadFromFile("image/enemy.png")) {
        std::cerr << "�����������ʧ��!" << std::endl;
    }
    //���ð�ť
    if (!settingsButtonTexture.loadFromFile("image/settings.png")) {
        std::cerr << "Failed to load settings button texture!" << std::endl;
    }
    
    spawnPoint = sf::Vector2f(20, 10);
    loadSounds();
}
void Game::createBoostItems() {
    boostItems.clear();

    // ȷ�����ᴴ���������
    if (boostItems.capacity() > MAX_ITEMS) {
        boostItems.shrink_to_fit();
    }
    // ���ݹؿ�������ͬ����
    switch (currentplaystate.level) {
    case 1:
        boostItems.push_back(BoostItem(sf::Vector2f(100, 300), healTexture, BoostType::HEAL));
        boostItems.push_back(BoostItem(sf::Vector2f(500, 300), damageUpTexture, BoostType::INVINCIBLE));
        break;
    case 2:
        boostItems.push_back(BoostItem(sf::Vector2f(300, 400), invincibleTexture, BoostType::INVINCIBLE));
        boostItems.push_back(BoostItem(sf::Vector2f(600, 200), hasteTexture, BoostType::HEAL));
        break;
    case 3:
        boostItems.push_back(BoostItem(sf::Vector2f(300, 420), healTexture, BoostType::HEAL));
        boostItems.push_back(BoostItem(sf::Vector2f(600, 170), hasteTexture, BoostType::INVINCIBLE));
        break;
    case 4:
        boostItems.push_back(BoostItem(sf::Vector2f(100, 300), healTexture, BoostType::HEAL));
        boostItems.push_back(BoostItem(sf::Vector2f(500, 300), damageUpTexture, BoostType::INVINCIBLE));
        break;
    default:break;
    }

    // ���óߴ磨�����Ҫ��
    for (auto& item : boostItems) {
        // ��ȡ����ߴ�
        sf::Vector2u textureSize = item.getTexture().getSize();

        // ���þ���ߴ磨����ͳһ���ŵ�40*40��
        float scaleX = 40.0f / textureSize.x;
        float scaleY = 40.0f / textureSize.y;
        item.setScale(scaleX, scaleY);
    }
}
void Game::loadSounds()
{
    if (!pengBuffer.loadFromFile("sound/attck.wav")) {
        std::cerr << "�޷����ص����Ч��" << std::endl;
    }
    else {
        pengSound.setBuffer(pengBuffer);
    }
}
void Game::loadLevel(PlayState& currentplaystate1, Kirby& player, bool resetToSpawnPoint) {
    currentplaystate = currentplaystate1;

    // ���ص�ͼ��ԭ�д��벻�䣩
    switch (currentplaystate.level) {
    case 1:
        if (!tileset.loadFromFile("image/map1.png")) {
            std::cerr << "��ͼ��ͼ image/map1.png ����ʧ�ܣ�" << std::endl;
            return;
        }
        if (!map.loadFromTMX("image/map1.tmx", tileset, 16, 16)) {
            std::cerr << "��ͼ�ļ� map1.tmx ����ʧ�ܣ�" << std::endl;
            return;
        }
        break;
    case 2:
        if (!tileset.loadFromFile("image/map2.png")) {
            std::cerr << "��ͼ��ͼ image/map2.png ����ʧ�ܣ�" << std::endl;
            return;
        }
        if (!map.loadFromTMX("image/map2.tmx", tileset, 16, 16)) {
            std::cerr << "��ͼ�ļ� map1.tmx ����ʧ�ܣ�" << std::endl;
            return;
        }
        break;
    case 3:
        if (!tileset.loadFromFile("image/map3.png")) {
            std::cerr << "��ͼ��ͼ image/map1.png ����ʧ�ܣ�" << std::endl;
            return;
        }
        if (!map.loadFromTMX("image/map3.tmx", tileset, 16, 16)) {
            std::cerr << "��ͼ�ļ� map1.tmx ����ʧ�ܣ�" << std::endl;
            return;
        }

        break;
    case 4:
        if (!tileset.loadFromFile("image/map1.png")) {
            std::cerr << "��ͼ��ͼ image/map1.png ����ʧ�ܣ�" << std::endl;
            return;
        }
        if (!map.loadFromTMX("image/map1.tmx", tileset, 16, 16)) {
            std::cerr << "��ͼ�ļ� map1.tmx ����ʧ�ܣ�" << std::endl;
            return;
        }
        break;
    default: break;
    }

    // ��������ùؿ���ʹ�ó�ʼ������
    if (resetToSpawnPoint) {
        player.setPosition(20, 10);
    }

    createBoostItems();
    player.setMap(&map);
    createEnemies();
}

void Game::draw(sf::RenderWindow& window, Kirby& player)
{
    window.clear();
    window.draw(map);

    // ���Ƶ���
    for (auto& item : boostItems) {
        if (item.getIsAlive())
            item.draw(window);
    }
    for (auto& enemy : enemies) {
        if (enemy->getIsAlive())
        
            enemy->draw(window);
    }
    sf::Sprite settingsButton (settingsButtonTexture);
    settingsButton.setPosition({ 700.f, 0.f }); // ���贰�ڿ���� 800
    settingsButton.setScale({ 0.15, 0.15 });
    // �����ð�ť
    window.draw(settingsButton);
    // ��������ð�ť
    while (const std::optional event = window.pollEvent())
    {
        if (event->is<sf::Event::MouseButtonPressed>())
        {
            if (event->getIf<sf::Event::MouseButtonPressed>()->button == sf::Mouse::Button::Left)
            {
                sf::Vector2f mousePos(window.mapPixelToCoords(sf::Mouse::getPosition(window)));
                if (settingsButton.getGlobalBounds().contains(mousePos))
                {
                    std::cout << "���ð�ť������ˣ�" << std::endl;
                    // ���Ը�Ϊ�������ò˵������л�����
                    sf::RenderWindow window1(sf::VideoMode({ 800, 600 }), "Setting");
                    //���ش��ڱ�������
                    sf::Texture backgroundTexture;//��������
                    if (!backgroundTexture.loadFromFile("image/setting.jpg")) //�ɸ�Ϊ!texture.loadFromFile("image/desk.png", false, sf::IntRect({10, 10}, {32, 32}))
                    {
                        return; // ����ʧ��
                    }
                    sf::Sprite backgroundSprite(backgroundTexture);//��������
                    sf::Vector2u windowSize = window.getSize();
                    sf::Vector2u textureSize = backgroundTexture.getSize();
                    float scaleX = static_cast<float>(windowSize.x) / textureSize.x;
                    float scaleY = static_cast<float>(windowSize.y) / textureSize.y;
                    backgroundSprite.setScale({ scaleX, scaleY });

                    //������һ���˵���
                    MainMenu menu(backgroundSprite);
                    Setting settingmap(window1);
                    settingmap.run(window1, menu, window);
                }
            }
        }
    }
    player.draw(window);
    window.display();
}

void Game::update(Kirby& player, float deltaTime, sf::RenderWindow &window) {
    if (gameOver) return;  // �����Ϸ�Ѿ����������ٸ���
    
    if (currentplaystate.level == 4)
    {
        if(!player.isInvincible)    std::cout << "���ȴ����޵йؿ���" << std::endl;
        player.isInvincible = true;    
    }
    // 1. ����������루���緽�������Ծ�ȣ�
    player.handleInput();

    // 2. �������״̬�����ƶ��������ȣ�
    player.update(deltaTime, currentplaystate.level); // ����е�ͼ��ײ���� map �ж�

    // ���µ���
    for (auto& item : boostItems) {
        item.update(deltaTime);
        item.handleCollision(player); // �������ҵ���ײ
    }

    // �Ƴ��ѱ��ռ��ĵ���
    boostItems.erase(std::remove_if(boostItems.begin(), boostItems.end(),
        [](const BoostItem& item) { return !item.getIsAlive(); }),
        boostItems.end());

    // ���µ���
    for (auto& enemy : enemies) {
        if (enemy->getIsAlive())
            enemy->update(deltaTime);

        // �ӵ��������ײ���
        for (auto& bullet : player.getBullets()) {
            if (enemy->getIsAlive() && intersects(bullet.getBounds(), enemy->getGlobalBounds())) {
                enemy->takeHit();        // �����ܻ�
                bullet.dead = true;      // �ӵ����������Ϊfalse��dead8iuw
            }
        }
    }

    // ɾ����������
    enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
        [](const std::unique_ptr<Enemy>& e) { return !e->getIsAlive(); }),
        enemies.end());
    // ���¿���������ȴ��ʱ
    if (player.hurtCooldown > 0.f) {
        player.hurtCooldown -= deltaTime;
    }

    // �����뿨����ײ
    for (auto& enemy : enemies) {
        if (enemy->getIsAlive() &&
            intersects(player.getGlobalBounds(), enemy->getGlobalBounds()) &&
            player.hurtCooldown <= 0.f) {
            // ������ȵ�ǰ���޵�״̬����������
            if (player.isInvincible) {
                std::cout << "�����޵��У�δ�ܵ��˺���\n";
                continue;
            }
            pengSound.play();
            player.takeDamage(10);
            player.hurtCooldown = 1.0f; // ���˺���ȴ 1 ��
            std::cout << "���������ˣ���ǰѪ��: " << player.getHealth() << std::endl;
        }
    }
    // 4. �ж��Ƿ�ͨ��
    if (isLevelFinished(player, &map)) {
        currentplaystate.level += 1;
        std::cout << "��ϲͨ�أ�������һ�� Level " << currentplaystate.level << std::endl;
        if(currentplaystate.level<=4)   loadLevel(currentplaystate, player);
        else
        {
            window.close();
            // �����ֽ��沢�����Ϸ����
            gameOver = true;  // �ؼ������ñ�־
            //���ô���
            sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "Kirby Adventure");
            //���ش��ڱ�������
            sf::Texture backgroundTexture;//��������
            if (!backgroundTexture.loadFromFile("image/background.jpg")) //�ɸ�Ϊ!texture.loadFromFile("image/desk.png", false, sf::IntRect({10, 10}, {32, 32}))
            {
                return; // ����ʧ��
            }
            sf::Sprite backgroundSprite(backgroundTexture);//��������
            sf::Vector2u windowSize = window.getSize();
            sf::Vector2u textureSize = backgroundTexture.getSize();
            float scaleX = static_cast<float>(windowSize.x) / textureSize.x;
            float scaleY = static_cast<float>(windowSize.y) / textureSize.y;
            backgroundSprite.setScale({ scaleX, scaleY });

            //������һ���˵���
            MainMenu menu(backgroundSprite);
            GameLevel gamelevel;
            GameOverScene overscene(window);
            overscene.run(window,gamelevel,menu);
        }
    }
    // 5. ��ѡ���ж���Ϸ�Ƿ�ʧ��
    if (player.getPosition().y > 600) { // ��ҵ���ȥ��������
        std::cout << "�����ȥ�ˣ����¿�ʼ��ǰ�ؿ���" << std::endl;
        loadLevel(currentplaystate, player);
    }
    if (player.getPosition().y < 0 || player.getPosition().x>800 || player.getPosition().x < 0)
    {
        std::cout << "�㲻���ˣ����¿�ʼ��ǰ�ؿ���" << std::endl;
        loadLevel(currentplaystate, player);
    }
    if (player.getHealth()<=0) { // �������
        std::cout << "�㱻���˹���������" << std::endl;
        player.sethealth(100);
        loadLevel(currentplaystate, player);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
        saveGameState(player);
        std::cout << "��Ϸ�Ѵ浵��" << std::endl;
    }
}

TileMap& Game::getTileMap() {
    return map;
}

void Game::createEnemies() {
    enemies.clear(); // ����ɵ���
    
    switch (currentplaystate.level) {
    case 1:
        enemies.push_back(std::make_unique<Enemy>(sf::Vector2f(100.f, 460.f), enemyTexture, &map));
        enemies.push_back(std::make_unique<Enemy>(sf::Vector2f(350.f, 50.f), enemyTexture, &map));
        break;
    case 2:
        enemies.push_back(std::make_unique<Enemy>(sf::Vector2f(250.f, 50.f), enemyTexture, &map));
        enemies.push_back(std::make_unique<Enemy>(sf::Vector2f(350.f, 50.f), enemyTexture, &map));
        break;
    case 3:
        enemies.push_back(std::make_unique<Enemy>(sf::Vector2f(200.f, 50.f), enemyTexture, &map));
        enemies.push_back(std::make_unique<Enemy>(sf::Vector2f(350.f, 50.f), enemyTexture, &map));
        break;
    case 4:
        enemies.push_back(std::make_unique<Enemy>(sf::Vector2f(100.f, 460.f), enemyTexture, &map));
        enemies.push_back(std::make_unique<Enemy>(sf::Vector2f(350.f, 50.f), enemyTexture, &map));
        break;
    default:
        break;
    }

    // ȷ���������������ű���
    for (auto& enemy : enemies) {
        sf::Vector2u textureSize = enemy->getTexture().getSize();
        float scaleX = 40.0f / textureSize.x;
        float scaleY = 40.0f / textureSize.y;
        enemy->setScale(scaleX, scaleY);
    }
}

bool Game::intersects(const sf::FloatRect& a, const sf::FloatRect& b) {
    return a.position.x < b.position.x + b.position.x &&
        a.position.x + a.size.x > b.position.x &&
        a.position.y < b.position.y + b.size.y &&
        a.position.y + a.size.y > b.position.y;
}

// Game.cpp
void Game::saveGameState(Kirby& player) {
    // ���µ�ǰ��Ϸ״̬
    currentplaystate.playerX = player.getPosition().x;
    currentplaystate.playerY = player.getPosition().y;
    currentplaystate.health = player.getHealth();
    currentplaystate.isInvincible = player.isInvincible;
    currentplaystate.hurtCooldown = player.hurtCooldown;

    // ���浽�ļ�
    std::ofstream outFile("savegame.dat", std::ios::binary);
    if (outFile) {
        // ���������Ϸ״̬
        outFile.write(reinterpret_cast<const char*>(&currentplaystate), sizeof(currentplaystate));

        // �����������
        size_t enemyCount = enemies.size();
        outFile.write(reinterpret_cast<const char*>(&enemyCount), sizeof(enemyCount));

        // ����ÿ�����˵�λ�úʹ��״̬
        for (const auto& enemy : enemies) {
            sf::Vector2f pos = enemy->getPosition();
            bool isAlive = enemy->getIsAlive();

            outFile.write(reinterpret_cast<const char*>(&pos.x), sizeof(pos.x));
            outFile.write(reinterpret_cast<const char*>(&pos.y), sizeof(pos.y));
            outFile.write(reinterpret_cast<const char*>(&isAlive), sizeof(isAlive));
        }

        // �����������
        size_t itemCount = boostItems.size();
        outFile.write(reinterpret_cast<const char*>(&itemCount), sizeof(itemCount));

        // ����ÿ�����ߵĴ��״̬
        for (const auto& item : boostItems) {
            bool isAlive = item.getIsAlive();
            outFile.write(reinterpret_cast<const char*>(&isAlive), sizeof(isAlive));
        }

        std::cout << "��Ϸ�Ѵ浵��" << std::endl;
    }
    else {
        std::cerr << "�޷�������Ϸ��" << std::endl;
    }
}

// Game.cpp
bool Game::loadGameState(Kirby& player) {
    std::ifstream inFile("savegame.dat", std::ios::binary);
    if (!inFile) {
        std::cerr << "û���ҵ��浵�ļ���" << std::endl;
        return false;
    }

    // ��ȡ������Ϸ״̬
    inFile.read(reinterpret_cast<char*>(&currentplaystate), sizeof(currentplaystate));

    // �ȼ��ص�ͼ
    loadLevel(currentplaystate, player, false); // false��ʾ�����õ���ʼ��

    // �������״̬
    player.setPosition(currentplaystate.playerX, currentplaystate.playerY);
    player.sethealth(currentplaystate.health);
    player.isInvincible = currentplaystate.isInvincible;
    player.hurtCooldown = currentplaystate.hurtCooldown;

    // ��ȡ��������
    size_t enemyCount;
    inFile.read(reinterpret_cast<char*>(&enemyCount), sizeof(enemyCount));

    // �ؽ�����
    enemies.clear();
    for (size_t i = 0; i < enemyCount; ++i) {
        float x, y;
        bool isAlive;

        inFile.read(reinterpret_cast<char*>(&x), sizeof(x));
        inFile.read(reinterpret_cast<char*>(&y), sizeof(y));
        inFile.read(reinterpret_cast<char*>(&isAlive), sizeof(isAlive));

        if (isAlive) {
            auto enemy = std::make_unique<Enemy>(sf::Vector2f(x, y), enemyTexture, &map);
            // ���õ������ű���
            sf::Vector2u textureSize = enemy->getTexture().getSize();
            float scaleX = 40.0f / textureSize.x;
            float scaleY = 40.0f / textureSize.y;
            enemy->setScale(scaleX, scaleY);
            enemies.push_back(std::move(enemy)); // ע������ʹ�� std::move
        }
    }

    // ��ȡ��������
    size_t itemCount;
    inFile.read(reinterpret_cast<char*>(&itemCount), sizeof(itemCount));

    // �ؽ�����״̬
    boostItems.clear();
    createBoostItems(); // �ȴ������е���

    for (size_t i = 0; i < itemCount && i < boostItems.size(); ++i) {
        bool isAlive;
        inFile.read(reinterpret_cast<char*>(&isAlive), sizeof(isAlive));
        boostItems[i].setAlive(isAlive);
    }

    std::cout << "��Ϸ�Ѽ��أ�" << std::endl;
    return true;
}