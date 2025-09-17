#include "GameOverScene.h"
#include <iostream>

using namespace std;

GameOverScene::GameOverScene(sf::RenderWindow& window) :gameOverText(font), retryText(font), menuText(font) {
    if (!font.openFromFile("ELEPHNT.TTF"))
    {
        cerr << "�����ļ��޷���" << endl;
    }

    gameOverText.setFont(font);
    gameOverText.setString("Game Over");
    gameOverText.setCharacterSize(90);
    gameOverText.setFillColor(sf::Color::White);
    gameOverText.setOutlineColor(sf::Color(255, 182, 193, 128)); // RGB = ǳ�ۣ�A = ��͸����0~255��
    gameOverText.setOutlineThickness(6.f);
    sf::FloatRect titleBounds = gameOverText.getLocalBounds();
    gameOverText.setOrigin({ titleBounds.size.x / 2.f + titleBounds.position.x, titleBounds.size.y / 2.f + titleBounds.position.y });
    gameOverText.setPosition({ 800 / 2.f, 170.f });

    retryText.setFont(font);
    retryText.setString("Retry");
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

    retryText.setPosition({ 200.f, 350.f });
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
    retryBox.setOrigin({ yesBoxSize.x / 2.f, yesBoxSize.y / 2.f});
    menuBox.setOrigin({ noBoxSize.x / 2.f, noBoxSize.y / 2.f});

    // ��ť���κ�����ͬλ��
    retryBox.setPosition(retryText.getPosition());
    menuBox.setPosition(menuText.getPosition());

    // ���ð�͸��������ɫ��������ȫ�ڵ�����
    retryBox.setFillColor(sf::Color(50, 50, 50, 150));  // ��͸������ɫ
    menuBox.setFillColor(sf::Color(50, 50, 50, 150));

}

void GameOverScene::run(sf::RenderWindow& window, GameLevel& level, MainMenu& menu) {
    if (!winMusic.openFromFile("sound/win.wav")) {
        std::cerr << "Failed to load menu music.\n";
        return;
    }
    winMusic.setLoopPoints({ sf::milliseconds(0), sf::seconds(10) });
    winMusic.setVolume(100);  // ��ѡ����������
    winMusic.play();
    while (window.isOpen()) {
        handleEvents(window, level, menu);
        render(window);
    }
}

void GameOverScene::handleEvents(sf::RenderWindow& window, GameLevel& level, MainMenu& menu) {
    while (const std::optional event = window.pollEvent())//���Ƿ����¼�
    {
        if (event->is<sf::Event::Closed>())
        {
            window.close();
            return;
        }
        if (event->is<sf::Event::MouseButtonPressed>())
        {
            if (event->getIf<sf::Event::MouseButtonPressed>()->button == sf::Mouse::Button::Left)
            {
                sf::Vector2f mousePos(window.mapPixelToCoords(sf::Mouse::getPosition(window)));
                if (retryBox.getGlobalBounds().contains(mousePos)) {
                    winMusic.stop();
                    level.run(window); // ��ʼ������Ϸ
                }
                else if (menuBox.getGlobalBounds().contains(mousePos)) {
                    winMusic.stop();
                    menu.showMenu(window);
                    return;
                }
            }
        }
    }
}



void GameOverScene::render(sf::RenderWindow& window) {
    sf::Texture gameoverTexture;//��������
    if (!gameoverTexture.loadFromFile("image/win.jpg")) //�ɸ�Ϊ!texture.loadFromFile("image/desk.png", false, sf::IntRect({10, 10}, {32, 32}))
    {
        return; // ����ʧ��
    }
    sf::Sprite gameoverSprite(gameoverTexture);//��������
    sf::Vector2u windowSize = window.getSize();
    sf::Vector2u textureSize = gameoverTexture.getSize();
    float scaleX = static_cast<float>(windowSize.x) / textureSize.x;
    float scaleY = static_cast<float>(windowSize.y) / textureSize.y;
    gameoverSprite.setScale({ scaleX, scaleY });
    window.clear();
    window.draw(gameoverSprite);
    window.draw(gameOverText);
    window.draw(retryBox);
    window.draw(menuBox);
    window.draw(retryText);
    window.draw(menuText);
    window.display();
}