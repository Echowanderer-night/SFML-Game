#include "BulletManager.h"

void BulletManager::spawnBullet(sf::Vector2f pos, float dir, bool fasingRight) {
    bullets.emplace_back(pos, dir, fasingRight);//����һ���µ� Bullet ʵ��������ӵ� bullets ����
}

void BulletManager::update(float dt) {
    for (auto& b : bullets)
        b.update(dt);// ����ÿ���ӵ���λ�ú�״̬

    bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
        [](const Bullet& b) { return b.isDead(); }), bullets.end());//�Ƴ��������������ӵ�
}

void BulletManager::draw(sf::RenderWindow& window) {
    for (auto& b : bullets)
        b.draw(window);
}

const std::vector<Bullet>& BulletManager::getBullets() const {
    return bullets;
}
std::vector<Bullet>& BulletManager::getBullets() {
    return bullets;
}
