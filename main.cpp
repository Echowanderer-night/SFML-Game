#include "MainMenu.h"

int main()
{
    //���ô���
    sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "Kirby Adventure");
    //���ش��ڱ�������
    sf::Texture backgroundTexture;//��������
    if (!backgroundTexture.loadFromFile("image/background.jpg")) //�ɸ�Ϊ!texture.loadFromFile("image/desk.png", false, sf::IntRect({10, 10}, {32, 32}))
    {
        return -1; // ����ʧ��
    }
    sf::Sprite backgroundSprite(backgroundTexture);//��������
    sf::Vector2u windowSize = window.getSize();
    sf::Vector2u textureSize = backgroundTexture.getSize();
    float scaleX = static_cast<float>(windowSize.x) / textureSize.x;
    float scaleY = static_cast<float>(windowSize.y) / textureSize.y;
    backgroundSprite.setScale({ scaleX, scaleY });

    //������һ���˵���
    MainMenu menu(backgroundSprite);

    while (window.isOpen())
    {
        menu.showMenu(window);        
    }
    return 0;
}
