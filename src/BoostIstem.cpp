#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <iostream> // ���ڵ������
#include <optional> // ���� SFML 3.0.0 ���¼�����
#include <algorithm>
#include <cmath>

#include "BoostItem.hpp"
#include "Kirby.h"

class Kirby;

BoostItem::BoostItem(sf::Vector2f spawnPos, const sf::Texture& texture, BoostType tp) :
    Entity(spawnPos.x, spawnPos.y, texture),
    type(tp),
    baseY(spawnPos.y)  // ��� baseY �洢��ʼ Y λ��
{
    // ��ȡ����������ߴ�
    sf::Vector2u textureSize = texture.getSize();

    // ���þ���ʹ����������
    sprite.setTextureRect(sf::IntRect({ 0, 0 }, { static_cast<int>(textureSize.x), static_cast<int>(textureSize.y) }));

    // ������ʵ����ű���
    float desiredWidth = 10.0f;
    scaleFactor = desiredWidth / static_cast<float>(textureSize.x);  // ʹ�ó�Ա����

    // Ӧ������
    sprite.setScale(sf::Vector2f(scaleFactor, scaleFactor));

    // ���ó�ʼλ��
    sprite.setPosition(position);

    // ������ײ��
    boundingBox = sprite.getGlobalBounds();

    // �������
    std::cout << "BoostItem created. Scaled size: "
        << boundingBox.size.x << "x" << boundingBox.size.y << "\n";
}

//���û���Ե��ͻ��ƹ�ʵ
void BoostItem::draw(sf::RenderWindow& window) {
    if (isAlive) {
        window.draw(sprite);
    }
}

//���һ�����¸����Ķ���
void BoostItem::update(float deltaTime) {
    float elapsedTime = floatClock.getElapsedTime().asSeconds();
    float yOffset = std::sin(elapsedTime * floatSpeed) * floatAmplitude;

    // ʹ�� baseY ��Ϊ����λ��
    float currentY = baseY + yOffset;
    sprite.setPosition(sf::Vector2f(position.x, currentY));

    // ������ײ��
    boundingBox = sprite.getGlobalBounds();

    // ͬ��λ�ã���ѡ�������Ҫ����λ�ø��£�
    position.y = currentY;
}

void BoostItem::handleCollision(Kirby& kirby) {
    if (kirby.isDead()) return;

    Kirby* kirbyPtr = &kirby;
    if (kirbyPtr && !kirbyPtr->isDead()) {
        sf::FloatRect itemBounds = getGlobalBounds();
        sf::FloatRect kirbyBounds = kirbyPtr->getGlobalBounds();

        // ʹ��ʵ�ʳߴ�������
        float itemArea = itemBounds.size.x * itemBounds.size.y;

        if (std::optional<sf::FloatRect> intersection = itemBounds.findIntersection(kirbyBounds)) {
            float intersectionArea = intersection->size.x * intersection->size.y;
            boost(&kirby);
            // ʹ��ʵ�����������ֵ
            if (intersectionArea >= itemArea * 0.1f) { // 30% �ص�
                boost(kirbyPtr);
                setAlive(false);
            }
        }
    }
}

//�����������
void BoostItem::setSpriteTextureRect(const sf::IntRect& rect) {
    sprite.setTextureRect(rect);
    // ÿ��������θı�����Kirby���Ӿ��ߴ���Ҫ���ֲ��䣬��Ҫ���µ������Ż�ԭ��
    // �������Ǽ���Kirby�Ĵ�С��shape������sprite��Ҫ��Ӧ��
    if (sprite.getLocalBounds().size.x > 0 && sprite.getLocalBounds().size.y > 0) {
        sprite.setScale(sf::Vector2f(scaleFactor, scaleFactor));
    }
    // ������ײ����ƥ���µľ���ߴ磨����������Ӱ���˳ߴ磩
    boundingBox = sprite.getGlobalBounds();
}


void BoostItem::boost(Kirby* kirby)
{
    if (!isAlive) {
        return;
    }

    std::cout << "Kirby ���������ʵ: ";

    switch (type) { // ���� BoostItem ���������ж�
    case BoostType::HEAL:
        // ����Ч�������� Kirby �ĵ�ǰ����ֵ�����������������ֵ
        kirby->health = std::min(kirby->health + 25, kirby->maxHealth);
        std::cout << "���ƣ�����ֵС��100��Ե�ǰ����ֵ��25 " << kirby->health << std::endl;
        break;

    case BoostType::INVINCIBLE:
        // �޵�Ч�������� Kirby Ϊ�޵�״̬����������ʱ��
        kirby->isInvincible = true;
        kirby->invincibilityTimer.restart(); // �����޵м�ʱ��
        std::cout << "����޵�Ч����" << std::endl;
        break;
    }
    // �����ʵ�����Ե����󣬽�����Ϊ����Ծ���Ա����Ϸ�������Ƴ�
    setAlive(false);
}


