#pragma once
//��ģ���ṩ RenderWindow��Text��RectangleShape ���࣬������Ⱦ�˵�����Ϸ��
#include <string>//<string>: �����ڲ˵���ʹ�� std::string ����ʾ�ı������磬��Kirby Adventure����
#include <iostream>
#include <optional>
#include <vector>
#include "introAnimation.h"
#include <SFML/Audio.hpp>
#include "PlayState.h"
using namespace std;

enum class GameState { Menu, Gameplay, LoadGame, Settings, Help, Exit };//����һ���Զ������� GameState ��׷����Ϸ�ĵ�ǰ״̬��
//�г��˿��ܵ�ֵ��Menu, Gameplay, Exit���������ڿ�����Ϸ�Ƿ���ʾ�˵���������Ϸ�淨���˳���

class MainMenu
{
private:
    sf::Font font1;//�洢���ڱ��������
    sf::Font font2;//�洢���ڿ�ʼ��Ϸ������
    sf::Text title;//��ʾ��Ϸ����
    sf::Text startButton;//��ʼ��ť
    sf::RectangleShape buttonBox;//��ʼ��ť����
    GameState state;//��ʾ��Ϸ״̬
    sf::Sprite backgroundSprite;
    vector<sf::Text> buttons;             // ��ť����
    vector<sf::RectangleShape> buttonBoxes; // ��ť������
    vector<GameState> buttonActions;      // ÿ����ť��Ӧ�Ķ�

    sf::Music menuMusic;
    sf::SoundBuffer clickBuffer;
    sf::Sound clickSound;

public:
    MainMenu(const sf::Sprite& bg);
    void showGameplay(sf::RenderWindow& window);
    GameState getState();    
    void showLoadGame(sf::RenderWindow& window);
    void showSettings(sf::RenderWindow& window);
    void showHelp(sf::RenderWindow& window);
    void showMenu(sf::RenderWindow& window);
    void handleInput(sf::RenderWindow& window);
    void draw(sf::RenderWindow& window);

    void loadSounds();  //������Ƶ��Դ
};