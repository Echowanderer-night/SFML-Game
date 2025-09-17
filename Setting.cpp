#include "Setting.h"
#include <iostream>

using namespace std;

Setting::Setting(sf::RenderWindow& window) :gameOverText(font), retryText(font), menuText(font) {
    if (!font.openFromFile("ELEPHNT.TTF"))
    {
        cerr << "�����ļ��޷���" << endl;
    }

    gameOverText.setFont(font);
    gameOverText.setString("Setting");
    gameOverText.setCharacterSize(90);
    gameOverText.setFillColor(sf::Color::White);
    gameOverText.setOutlineColor(sf::Color(255, 182, 193, 128)); // RGB = ǳ�ۣ�A = ��͸����0~255��
    gameOverText.setOutlineThickness(6.f);
    sf::FloatRect titleBounds = gameOverText.getLocalBounds();
    gameOverText.setOrigin({ titleBounds.size.x / 2.f + titleBounds.position.x, titleBounds.size.y / 2.f + titleBounds.position.y });
    gameOverText.setPosition({ 800/ 2.f, 170.f });

    retryText.setFont(font);
    retryText.setString("Intro");
    retryText.setCharacterSize(60);
    retryText.setFillColor(sf::Color::White);
    retryText.setOutlineColor(sf::Color(0, 0, 0, 128));  // A = ͸���ȣ�0-255����128 �ǰ�͸��
    retryText.setOutlineThickness(4.f);

    menuText.setFont(font);
    menuText.setString("Menu");
    menuText.setCharacterSize(60);
    menuText.setFillColor(sf::Color::White);
    menuText.setOutlineColor(sf::Color(0, 0, 0, 128));  // A = ͸���ȣ�0-255����128 �ǰ�͸��
    menuText.setOutlineThickness(4.f);

    sf::FloatRect yesBounds = retryText.getGlobalBounds();
    sf::FloatRect noBounds = menuText.getGlobalBounds();

    retryText.setOrigin({ yesBounds.size.x / 2.f + yesBounds.position.x, yesBounds.size.y / 2.f + yesBounds.position.y });
    menuText.setOrigin({ noBounds.size.x / 2.f + noBounds.position.x, noBounds.size.y / 2.f + noBounds.position.y });

    retryText.setPosition({ 250.f, 350.f });
    menuText.setPosition({ 550.f, 350.f });

    // ��ť�İ�Χ�򣨿�ѡ��
    sf::FloatRect yesLocalBounds = retryText.getLocalBounds();
    sf::FloatRect noLocalBounds = menuText.getLocalBounds();


    // �����Χ���δ�Сʱ��ֻ�� size������ position
    sf::Vector2f yesBoxSize(yesLocalBounds.size.x + 20.f, yesLocalBounds.size.y + 20.f);
    sf::Vector2f noBoxSize(noLocalBounds.size.x + 20.f, noLocalBounds.size.y + 20.f);

    retryBox.setSize(yesBoxSize);
    menuBox.setSize(noBoxSize);

    // origin����Ϊ����
    retryBox.setOrigin({ yesBoxSize.x / 2.f, yesBoxSize.y / 2.f });
    menuBox.setOrigin({ noBoxSize.x / 2.f, noBoxSize.y / 2.f });

    // ��ť���κ�����ͬλ��
    retryBox.setPosition(retryText.getPosition());
    menuBox.setPosition(menuText.getPosition());

    // ���ð�͸��������ɫ��������ȫ�ڵ�����
    retryBox.setFillColor(sf::Color(50, 50, 50, 150));  // ��͸������ɫ
    menuBox.setFillColor(sf::Color(50, 50, 50, 150));

}

void Setting::run(sf::RenderWindow& window1, MainMenu& menu, sf::RenderWindow& window) {
    while (window1.isOpen()) {
        render(window1);
        handleEvents(window1, menu, window);
        
    }
}

void Setting::handleEvents(sf::RenderWindow& window1, MainMenu& menu, sf::RenderWindow& window) {
    while (window1.isOpen()) {

        while (const std::optional event = window1.pollEvent())//���Ƿ����¼�
        {
            if (event->is<sf::Event::Closed>())
            {
                window1.close();
                return;
            }
            if (event->is<sf::Event::KeyPressed>() && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter))
            {
                window1.close();
                return;
            }
            if (event->is<sf::Event::MouseButtonPressed>())
            {
                if (event->getIf<sf::Event::MouseButtonPressed>()->button == sf::Mouse::Button::Left)
                {
                    sf::Vector2f mousePos(window1.mapPixelToCoords(sf::Mouse::getPosition(window1)));
                    if (retryBox.getGlobalBounds().contains(mousePos)) {
                        sf::Font font;
                        if (!font.openFromFile("ELEPHNT.TTF"))
                        {
                            std::cerr << "�޷���������\n";
                            return;
                        }

                        sf::Text SettingText(font);
                        SettingText.setString("Seting:\nPress Left / Right arrow to move\nPress Spacebar to jump\nPress A to attack\nKirby loses health when touching enemy\nPress S to save your progress\nPress Enter to return");
                        SettingText.setCharacterSize(40);
                        SettingText.setFillColor(sf::Color::White);
                        SettingText.setOutlineColor(sf::Color(250, 100, 100, 128));  // A = ͸���ȣ�0-255����128 �ǰ�͸��
                        SettingText.setOutlineThickness(4.f);
                        SettingText.setPosition({ 0, 0 });

                        sf::Texture SettingTexture;//��������
                        if (!SettingTexture.loadFromFile("image/setting.jpg")) //�ɸ�Ϊ!texture.loadFromFile("image/desk.png", false, sf::IntRect({10, 10}, {32, 32}))
                        {
                            return; // ����ʧ��
                        }
                        sf::Sprite SettingSprite(SettingTexture);//��������
                        sf::Vector2u windowSize = window.getSize();
                        sf::Vector2u textureSize = SettingTexture.getSize();
                        float scaleX = static_cast<float>(windowSize.x) / textureSize.x;
                        float scaleY = static_cast<float>(windowSize.y) / textureSize.y;
                        SettingSprite.setScale({ scaleX, scaleY });
                        SettingSprite.setColor(sf::Color(255, 255, 255, 200)); // 128 �ǰ�͸����0 ��ȫ͸����255 ��ȫ��͸��

                        window1.clear();
                        window1.draw(SettingSprite);
                        window1.draw(SettingText);
                        window1.display();
                    }
                    else if (menuBox.getGlobalBounds().contains(mousePos)) {
                        window.close();
                        menu.showMenu(window1);
                        return;
                    }
                }

            }
        }
 
    }
}



void Setting::render(sf::RenderWindow& window1) {
    sf::Texture gameoverTexture;//��������
    if (!gameoverTexture.loadFromFile("image/setting.jpg")) //�ɸ�Ϊ!texture.loadFromFile("image/desk.png", false, sf::IntRect({10, 10}, {32, 32}))
    {
        return; // ����ʧ��
    }
    sf::Sprite gameoverSprite(gameoverTexture);//��������
    sf::Vector2u windowSize = window1.getSize();
    sf::Vector2u textureSize = gameoverTexture.getSize();
    float scaleX = static_cast<float>(windowSize.x) / textureSize.x;
    float scaleY = static_cast<float>(windowSize.y) / textureSize.y;
    gameoverSprite.setScale({ scaleX, scaleY });
    window1.clear();
    window1.draw(gameoverSprite);
    window1.draw(gameOverText);
    window1.draw(retryBox);
    window1.draw(menuBox);
    window1.draw(retryText);
    window1.draw(menuText);
    window1.display();
}