#include "Enemy.h"

Enemy::Enemy(sf::Vector2f position, const sf::Texture& texture, TileMap* map)
    : texture(texture), tileMap(map), sprite(texture) {
    sprite.setTexture(this->texture);
    sprite.setPosition(position);
    direction = sf::Vector2f(1.f, 0.f); // Ĭ������
}

void Enemy::update(float deltaTime) {
    if (!isAlive) return;

    sf::Vector2f pos = sprite.getPosition();
    sf::FloatRect bounds = sprite.getGlobalBounds();

    // ====== ���������߼� ======
    float gravity = 300.f;
    float fallSpeed = gravity * deltaTime;

    int colLeft = static_cast<int>(pos.x) / tileMap->getTileWidth();
    int colRight = static_cast<int>(pos.x + bounds.size.x - 1) / tileMap->getTileWidth();
    int rowBelow = static_cast<int>(pos.y + bounds.size.y + fallSpeed) / tileMap->getTileHeight();

    bool isStandingOnGround =
        tileMap->getTileType(rowBelow, colLeft) == TileType::ground ||
        tileMap->getTileType(rowBelow, colRight) == TileType::ground;

    if (!isStandingOnGround) {
        // ���µ���
        sprite.move({ 0.f, fallSpeed });
        return; // �ڿ��в�����ˮƽ�ƶ�
    }

    // ====== ˮƽ�ƶ��߼� ======
    timeSinceSwitch += deltaTime;
    if (timeSinceSwitch >= switchTime) {
        direction.x = -direction.x;
        timeSinceSwitch = 0.f;
    }

    handleTileCollision(); // �ж�ǰ���Ƿ�ײǽ

    float dx = direction.x * moveSpeed * deltaTime;
    sprite.move({ dx, 0.f });
}


void Enemy::handleTileCollision() {
    sf::Vector2f pos = sprite.getPosition();
    sf::FloatRect bounds = sprite.getGlobalBounds();

    int rowTop = static_cast<int>(pos.y) / tileMap->getTileHeight();
    int rowBottom = static_cast<int>(pos.y + bounds.size.y - 1) / tileMap->getTileHeight();

    // �жϡ�ǰ��������
    int colFront = direction.x > 0 ?
        static_cast<int>(pos.x + bounds.size.x) / tileMap->getTileWidth() :
        static_cast<int>(pos.x - 1) / tileMap->getTileWidth();

    // �����������ǰ���Ƿ����ϰ�
    for (int row = rowTop; row <= rowBottom; ++row) {
        if (tileMap->getTileType(row, colFront) == TileType::ground) {
            direction.x = -direction.x; // ײ���ϰ�����ת
            timeSinceSwitch = 0.f; // ���÷�תʱ��
            break;
        }
    }
}


void Enemy::draw(sf::RenderWindow& window) {
    if (isAlive)
        window.draw(sprite);
}

void Enemy::takeHit() {
    hitCount++;
    if (hitCount >= maxHits) {
        isAlive = false;
    }
}

bool Enemy::getIsAlive() const {
    return isAlive;
}

sf::FloatRect Enemy::getGlobalBounds() const {
    return sprite.getGlobalBounds();
}


void Enemy::setScale(float x, float y) {
    sprite.setScale({ x, y });
}

const sf::Texture& Enemy::getTexture() const {
    return texture;
}
