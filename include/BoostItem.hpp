#pragma once

#include "Entity.hpp"
#include <iostream>

class Kirby;

enum class BoostType {
	HEAL,  // ���ƣ�����һ��Ѫ��
	INVINCIBLE,  // �޵�
};

class BoostItem : public Entity
{
protected:
	BoostType type = BoostType::HEAL;
	float size = 10.f;   //��ռ�����С

	//ʵ�ָ���
	sf::Clock floatClock;
	float floatAmplitude = 5.0f; // �����������ȣ������ܹ� 10 ���أ�
	float floatSpeed = 2.0f;    // �������ٶȣ�Խ�󸡶�Խ�죩
	float baseY;   //��׼Y����

	float scaleFactor = 0.0125f;
public:
	BoostItem(sf::Vector2f spawnPos, const sf::Texture& texture, BoostType tp);

	BoostType getType() const { return type; }
	void boost(Kirby* kirby);

	void update(float deltaTime) override;
	void draw(sf::RenderWindow& window) override;
	void handleCollision(Kirby& kirby);
	void setSpriteTextureRect(const sf::IntRect& rect) override;

	// ������������ȡ����
	const sf::Texture& getTexture() const {
		// ȷ������ָ����Ч
		const sf::Texture* texturePtr = &sprite.getTexture();
		if (texturePtr) {
			return *texturePtr; // ������ָ�뷵����������
		}

	}

	// ������������������
	void setScale(float scaleX, float scaleY) {
		sprite.setScale(sf::Vector2f(scaleX, scaleY));
		// ������ײ��
		boundingBox = sprite.getGlobalBounds();
	}

	// �������������óߴ磨ͳһ�ߴ磩
	void setSize(float size) {
		this->size = size;
		// ���������С��������
		const sf::Texture* texturePtr = &sprite.getTexture(); // ��ȡ����ָ��
		if (texturePtr) {
			sf::Vector2u texSize = texturePtr->getSize();
			if (texSize.x > 0 && texSize.y > 0) {
				float scaleX = size / texSize.x;
				float scaleY = size / texSize.y;
				sprite.setScale({ scaleX, scaleY });
				boundingBox = sprite.getGlobalBounds();
			}
		}
	}
};
