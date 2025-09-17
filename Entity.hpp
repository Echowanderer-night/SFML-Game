#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <iostream> // ���ڵ������
#include <optional> // ���� SFML 3.0.0 ���¼�����

//�������Entity�����ж�̬����Ļ���������
//�����������ԣ�λ�á��ٶȣ��Ӿ����ԣ����ơ���������������ײ��
class Entity
{
protected:
	sf::Sprite sprite; //ͼ�����
	sf::Vector2f position;
	sf::Vector2f velocity; //�ٶȣ�����x��y����
	sf::FloatRect boundingBox; //��ײ���
	//AnimationController* animation; //AnimationController�ཫ�������ļ�����
	bool isAlive;
public:
	Entity(float x, float y, const sf::Texture& texture);
	virtual ~Entity() = default;     // ����������
	bool getIsAlive() const { return isAlive; }
	void setAlive(bool alive) { isAlive = alive; }
	virtual void setSpriteTextureRect(const sf::IntRect& rect) = 0; //���þ�����������
	virtual void update(float deltaTime) = 0; //deltaTimeָʱ��֡
	virtual void draw(sf::RenderWindow& window) = 0;
	virtual sf::FloatRect getGlobalBounds() const; //ȷ����ײ�߽�
	virtual void handleCollision(Entity& other) {}; //��ײ������CollisionManager�п��ܻ����
};