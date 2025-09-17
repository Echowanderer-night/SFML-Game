#include "Kirby.h"

// ���������ʵ��
void Kirby::loadTextures() {
    // ���Լ�����������
    if (!runTextures[0].loadFromFile("image/kirbyImage/kirby1.png")) {
        throw std::runtime_error("Failed to load kirby1.png");
    }
    if (!runTextures[1].loadFromFile("image/kirbyImage/kirby2.png")) {
        throw std::runtime_error("Failed to load kirby2.png");
    }
    if (!runTextures[2].loadFromFile("image/kirbyImage/kirby3.png")) {
        throw std::runtime_error("Failed to load kirby2.png");
    }
    if (!runTextures[3].loadFromFile("image/kirbyImage/kirby4.png")) {
        throw std::runtime_error("Failed to load kirby2.png");
    }
    if (!runTextures[4].loadFromFile("image/kirbyImage/kirby5.png")) {
        throw std::runtime_error("Failed to load kirby2.png");
    }
    if (!runTextures[5].loadFromFile("image/kirbyImage/kirby6.png")) {
        throw std::runtime_error("Failed to load kirby2.png");
    }
    if (!runTextures[6].loadFromFile("image/kirbyImage/kirby7.png")) {
        throw std::runtime_error("Failed to load kirby2.png");
    }
    // ... Ϊ����7������������ƴ��� ...
}

Kirby::Kirby()
    : 
    runTextures(), // ��ʼ����������
    texture_zero("image/kirbyImage/kirby1.png"),
    body(texture_zero), // ʹ�õ�һ�������ʼ������
    onGround(false),
    climbing(false),
    dead(false),
    tileMap(nullptr),
    iswin(false),
    currentFrame(0),
    frameTime(0.0f),
    frameDuration(0.08f),
    facingRight(true),
    attackSound(attackBuffer)
{
    // �������������ھ����ʼ����
    loadTextures();
    loadSounds();  // ������Ч
    // ��������������ȷ��ʹ�ü��غ������
    body.setTexture(runTextures[0]);

    // ��������
    body.setScale({ scaleFactor, scaleFactor });
    body.setPosition({ 0.f, 0.f });

    // ��ʼ�ٶ�
    velocity = { 0.f, 0.f };
}
void Kirby::loadSounds()
{
    if (!attackBuffer.loadFromFile("sound/attck.wav")) {
        std::cerr << "�޷����ص����Ч��" << std::endl;
    }
    else {
        attackSound.setBuffer(attackBuffer);
    }
}
void Kirby::setMap(TileMap* map) {
    tileMap = map;
}

void Kirby::setPosition(float x, float y) {
    body.setPosition({ x, y });
}

sf::Vector2f Kirby::getPosition() {
    return body.getPosition();
}

sf::Vector2f Kirby::getSize() const
{
    sf::FloatRect bounds = body.getGlobalBounds();
    return { bounds.size.x, bounds.size.y };
}


void Kirby::handleInput() {
    // ÿ֡����״̬
    onGround = isOnGround();
    climbing = isOnLadder();

    velocity.x = 0.f;

    // �����ƶ�
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
        velocity.x = -100.f;
        facingRight = false; // ������У����ó���
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
        velocity.x = 100.f;
        facingRight = true; // ������У����ó���
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
        attack();
        attackSound.play();
    }
    // ����״̬�������ƶ�
    if (climbing) {
        velocity.y = 0.f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) {
            velocity.y = -80.f;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) {
            velocity.y = 80.f;
        }
    }
    else {
        // ��Ծ��ֻ���ڵ�������
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) && onGround) {
            velocity.y = -250.f;
            onGround = false;
        }
    }
}

void Kirby::applyGravity(float dt) {
    if (!onGround && !climbing) {
        const float gravity = 600.f;  // �������ٶ�
        velocity.y += gravity * dt;   // �����������ٶ�
        if (velocity.y > 600.f) velocity.y = 600.f; // ������������ٶ�
    }
}

void Kirby::checkCollisions() {
    if (!tileMap) return;

    sf::Vector2f pos = body.getPosition();
    sf::Vector2f size = getSize();

    int leftTile = static_cast<int>(pos.x) / tileMap->getTileWidth();
    int rightTile = static_cast<int>(pos.x + size.x - 1) / tileMap->getTileWidth();
    int topTile = static_cast<int>(pos.y) / tileMap->getTileHeight();
    int bottomTile = static_cast<int>(pos.y + size.y - 1) / tileMap->getTileHeight();

    // ˮƽ��ײ
    if (velocity.x < 0) {
        for (int y = topTile; y <= bottomTile; y++) {
            TileType t = tileMap->getTileType(y, leftTile);
            if (t == TileType::ground || t == TileType::obstacle) {
                velocity.x = 0;
                body.setPosition(sf::Vector2f((leftTile + 1) * tileMap->getTileWidth(), pos.y));
                break;
            }
        }
    }
    else if (velocity.x > 0) {
        for (int y = topTile; y <= bottomTile; y++) {
            TileType t = tileMap->getTileType(y, rightTile);
            if (t == TileType::ground || t == TileType::obstacle) {
                velocity.x = 0;
                body.setPosition(sf::Vector2f(rightTile * tileMap->getTileWidth() - size.x, pos.y));
                break;
            }
        }
    }

    // ��ֱ��ײ
    if (velocity.y > 0) {
        // ����
        int footY = static_cast<int>(pos.y + size.y + velocity.y * 0.016f) / tileMap->getTileHeight();
        for (int x = leftTile; x <= rightTile; x++) {
            TileType t = tileMap->getTileType(footY, x);
            if (t == TileType::ground || t == TileType::obstacle) {
                velocity.y = 0;
                body.setPosition(sf::Vector2f(pos.x, footY * tileMap->getTileHeight() - size.y));
                onGround = true;
                break;
            }
        }
    }
    else if (velocity.y < 0) {
        // ����
        int headY = static_cast<int>(pos.y + velocity.y * 0.016f) / tileMap->getTileHeight();
        for (int x = leftTile; x <= rightTile; x++) {
            TileType t = tileMap->getTileType(headY, x);
            if (t == TileType::ground || t == TileType::obstacle) {
                velocity.y = 0;
                body.setPosition(sf::Vector2f(pos.x, (headY + 1) * tileMap->getTileHeight()));
                break;
            }
        }
    }
}

bool Kirby::isOnGround() {
    sf::Vector2f pos = body.getPosition();
    sf::Vector2f size = getSize();

    int left = static_cast<int>(pos.x) / tileMap->getTileWidth();
    int right = static_cast<int>(pos.x + size.x - 1) / tileMap->getTileWidth();
    int bottom = static_cast<int>(pos.y + size.y + 1) / tileMap->getTileHeight();

    for (int x = left; x <= right; ++x) {
        TileType t = tileMap->getTileType(bottom, x);
        if (t == TileType::ground || t == TileType::obstacle) {
            return true;
        }
    }
    return false;
}
bool Kirby::isOnLadder() const {
    sf::Vector2f pos = body.getPosition();
    sf::Vector2f size = getSize();

    int left = static_cast<int>(pos.x) / tileMap->getTileWidth();
    int right = static_cast<int>(pos.x + size.x - 1) / tileMap->getTileWidth();
    int centerY = static_cast<int>(pos.y + size.y / 2) / tileMap->getTileHeight();

    for (int x = left; x <= right; ++x) {
        TileType t = tileMap->getTileType(centerY, x);
        if (t == TileType::ladder) {
            return true;
        }
    }
    return false;
}



void Kirby::update(float dt, int gamelevel) {
    if (!tileMap) return;

    handleInput();
    checkCollisions();
    applyGravity(dt);

    if (bulletCooldown > 0.f)
        bulletCooldown -= dt;

    for (auto& bullet : bullets) {
        bullet.update(dt);
    }

    // ɾ���Ѿ��������ӵ���bullet.dead == true��
    bullets.erase(
        std::remove_if(bullets.begin(), bullets.end(), [](const Bullet& b) {
            return b.isDead();
            }),
        bullets.end()
    );

    // ����֡�л��߼�
    if (onGround && std::abs(velocity.x) > 0.1f) {
        frameTime += dt;
        if (frameTime >= frameDuration) {
            frameTime = 0.0f;
            currentFrame = (currentFrame + 1) % runTextures.size();
            body.setTexture(runTextures[currentFrame]);
        }
    }
    else if (currentFrame != 0) {
        currentFrame = 0;
        body.setTexture(runTextures[0]);
    }

    // ���ó��������
    if (facingRight) {
        body.setScale({ scaleFactor, scaleFactor });
        body.setOrigin({ 0, 0 });
    }
    else {
        body.setScale({ -scaleFactor, scaleFactor });
        body.setOrigin({ body.getLocalBounds().size.x, 0 });
    }

    // ���½�ɫλ��
    sf::Vector2f pos = body.getPosition();
    pos += velocity * dt;
    body.setPosition(pos);

    if (gamelevel != 4)
    {
        if (isInvincible && invincibilityTimer.getElapsedTime().asSeconds() > 3.0f) {
            isInvincible = false;
            std::cout << "�޵�״̬������\n";
        }
    }
}

void Kirby::draw(sf::RenderWindow& window) {
    sf::Font font;
    font.openFromFile("ELEPHNT.TTF"); // ��ȷ���ļ�����

    sf::Text healthText(font);
    healthText.setCharacterSize(24);
    healthText.setFillColor(sf::Color::White);

    sf::RectangleShape healthBox({ 150, 40 });
    healthBox.setFillColor(sf::Color(50, 50, 50, 200));
    healthBox.setPosition({ 10, 10 });

    // ÿ֡����
    healthText.setString("HP: " + std::to_string(getHealth()));
    healthText.setPosition({ 15, 15 });

    // ����
    for (auto& b : bullets)
        b.draw(window);
    window.draw(healthBox);
    window.draw(healthText);
    window.draw(body);
}

bool Kirby::isDead() const {
    return dead;
}

void Kirby::fixInitialPosition() {
    // �ý�ɫ�ӵ�ǰλ�����¼�⣬ֱ��վ�ڵ���
    if (!tileMap) return;

    sf::Vector2f pos = body.getPosition();
    sf::Vector2f size = getSize();

    int maxFallTiles = 10;  // �������10���ֹ��ѭ��
    for (int i = 0; i < maxFallTiles; i++) {
        int footTile = static_cast<int>(pos.y + size.y) / tileMap->getTileHeight();
        bool onGroundNow = false;

        int leftTile = static_cast<int>(pos.x) / tileMap->getTileWidth();
        int rightTile = static_cast<int>(pos.x + size.x - 1) / tileMap->getTileWidth();

        for (int x = leftTile; x <= rightTile; x++) {
            TileType t = tileMap->getTileType(footTile, x);
            if (t == TileType::ground || t == TileType::obstacle) {
                onGroundNow = true;
                break;
            }
        }

        if (onGroundNow) {
            // վ���ˣ�����λ�ò�����
            body.setPosition(sf::Vector2f(pos.x, footTile * tileMap->getTileHeight() - size.y));
            onGround = true;
            velocity.y = 0;
            return;
        }

        pos.y += tileMap->getTileHeight(); // ����һ��
    }
}

bool Kirby::isOnWaterSurface() {
    if (!tileMap) return false;

    sf::Vector2f pos = body.getPosition();
    sf::Vector2f size = getSize();

    int footTile = static_cast<int>(pos.y + size.y) / tileMap->getTileHeight();
    int leftTile = static_cast<int>(pos.x) / tileMap->getTileWidth();
    int rightTile = static_cast<int>(pos.x + size.x - 1) / tileMap->getTileWidth();

    for (int x = leftTile; x <= rightTile; x++) {
        TileType t = tileMap->getTileType(footTile, x);
        if (t == TileType::water) {
            return true;
        }
    }
    return false;
}

TileType Kirby::getCurrentTileType() {
    if (!tileMap) return TileType::empty;

    sf::Vector2f pos = body.getPosition();
    int centerX = static_cast<int>(pos.x + getSize().x / 2) / tileMap->getTileWidth();
    int centerY = static_cast<int>(pos.y + getSize().y / 2) / tileMap->getTileHeight();

    return tileMap->getTileType(centerY, centerX);
}

void Kirby::attack() {
    if (bulletCooldown > 0.f) return;

    sf::Vector2f pos = body.getPosition();
    sf::Vector2f size = getSize();
    float spacing = size.y / 4;

    for (int i = 0; i < 5; i++) {
        float y = pos.y + i * spacing;
        bullets.emplace_back(sf::Vector2f(facingRight ? pos.x + size.x : pos.x, y), 5.f, facingRight);
    }

    bulletCooldown = 0.5f; // ������ȴ
}
