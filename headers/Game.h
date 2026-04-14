#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <memory>
#include <vector>
#include <random>
#include "Paddle.h"
#include "Ball.h"
#include "AIController.h"

class Game {
public:
    Game();
    void run();

private:
    void processEvents();           // events only — no dt
    void update(float dt);          // all dt-based logic lives here
    void render();

    void checkCollisions();
    void clampPaddle(Paddle& paddle);

    void resetBall();
    void resetGame();
    void updateScoreText();
    void playRandomHit(bool isLeft);

private:
    sf::RenderWindow m_window;

    sf::Font                     m_font;
    std::unique_ptr<sf::Text>    m_playerScoreText;
    std::unique_ptr<sf::Text>    m_aiScoreText;
    std::unique_ptr<sf::Text>    m_endGameText;

    std::unique_ptr<Paddle>        m_playerPaddle;
    std::unique_ptr<Paddle>        m_aiPaddle;
    std::unique_ptr<Ball>          m_ball;
    std::unique_ptr<AIController>  m_aiController;
    std::vector<sf::RectangleShape> m_midLine;

    std::vector<sf::SoundBuffer> m_leftHitBuffers;
    std::vector<sf::SoundBuffer> m_rightHitBuffers;
    sf::SoundBuffer              m_scoreBuffer;
    sf::SoundBuffer              m_victoryBuffer;
    sf::SoundBuffer              m_defeatBuffer;
    std::unique_ptr<sf::Sound>   m_hitSound;
    std::unique_ptr<sf::Sound>   m_scoreSound;
    std::unique_ptr<sf::Sound>   m_victorySound;
    std::unique_ptr<sf::Sound>   m_defeatSound;

    int  m_playerScore = 0;
    int  m_aiScore = 0;
    bool m_isGameOver = false;

    static constexpr int m_maxScore = 5;

    std::mt19937 m_rng{ std::random_device{}() };
};
