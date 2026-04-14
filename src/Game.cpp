#include "Game.h"
#include <iostream>
#include <algorithm>

Game::Game()
    : m_window(sf::VideoMode(sf::Vector2u(WINDOW_WIDTH, WINDOW_HEIGHT)), "C++ Pong") {

    // --- Entities ---
    m_playerPaddle = std::make_unique<Paddle>();
    m_playerPaddle->setPosition({ 30.f, WINDOW_HEIGHT / 2.f });

    m_aiPaddle = std::make_unique<Paddle>();
    m_aiPaddle->setPosition({ WINDOW_WIDTH - 30.f, WINDOW_HEIGHT / 2.f });

    m_ball = std::make_unique<Ball>();
    resetBall();

    m_aiController = std::make_unique<AIController>(*m_aiPaddle, *m_ball);

    // --- Font & score text ---
    if (!m_font.openFromFile("assets/Aria.ttf"))
        std::cout << "FAILED TO LOAD FONT\n";

    m_playerScoreText = std::make_unique<sf::Text>(m_font);
    m_playerScoreText->setCharacterSize(50);
    m_playerScoreText->setFillColor(sf::Color::White);
    m_playerScoreText->setPosition({ WINDOW_WIDTH / 4.f, 20.f });

    m_aiScoreText = std::make_unique<sf::Text>(*m_playerScoreText);
    m_aiScoreText->setPosition({ WINDOW_WIDTH * 0.75f, 20.f });

    updateScoreText();

    m_endGameText = std::make_unique<sf::Text>(m_font);
    m_endGameText->setCharacterSize(40);
    m_endGameText->setFillColor(sf::Color::Yellow);
    m_endGameText->setStyle(sf::Text::Bold);

    // --- Centre dashed line ---
    const float dashH = 20.f;
    const float gap = 15.f;
    for (float y = 0.f; y < WINDOW_HEIGHT; y += dashH + gap) {
        sf::RectangleShape dash({ 5.f, dashH });
        dash.setFillColor(sf::Color(255, 255, 255, 100));
        dash.setOrigin({ 2.5f, 0.f });
        dash.setPosition({ WINDOW_WIDTH / 2.f, y });
        m_midLine.push_back(dash);
    }

    // --- Sounds ---
    for (int i = 1; i <= 3; ++i) {
        sf::SoundBuffer buf;
        if (buf.loadFromFile("assets/sfx/hit_left_" + std::to_string(i) + ".mp3"))
            m_leftHitBuffers.push_back(std::move(buf));
    }
    if (m_leftHitBuffers.empty())
        std::cout << "FAILED TO LOAD LEFT SOUNDS\n";

    for (int i = 1; i <= 2; ++i) {
        sf::SoundBuffer buf;
        if (buf.loadFromFile("assets/sfx/hit_right_" + std::to_string(i) + ".mp3"))
            m_rightHitBuffers.push_back(std::move(buf));
    }
    if (m_rightHitBuffers.empty())
        std::cout << "FAILED TO LOAD RIGHT SOUNDS\n";

    if (!m_scoreBuffer.loadFromFile("assets/sfx/goal.mp3"))
        std::cout << "FAILED TO LOAD goal.mp3\n";
    if (!m_victoryBuffer.loadFromFile("assets/sfx/victory.mp3"))
        std::cout << "FAILED TO LOAD victory.mp3\n";
    if (!m_defeatBuffer.loadFromFile("assets/sfx/defeat.mp3"))
        std::cout << "FAILED TO LOAD defeat.mp3\n";

    m_scoreSound = std::make_unique<sf::Sound>(m_scoreBuffer);
    m_victorySound = std::make_unique<sf::Sound>(m_victoryBuffer);
    m_defeatSound = std::make_unique<sf::Sound>(m_defeatBuffer);
    m_hitSound = std::make_unique<sf::Sound>(
        m_rightHitBuffers.empty() ? m_scoreBuffer : m_rightHitBuffers[0]);
}

// ---------------------------------------------------------------------------

void Game::run() {
    sf::Clock clock;
    while (m_window.isOpen()) {
        float dt = clock.restart().asSeconds();
        processEvents();
        update(dt);
        render();
    }
}

// ---------------------------------------------------------------------------
// processEvents: window/keyboard events only — NO dt, NO movement
// ---------------------------------------------------------------------------
void Game::processEvents() {
    while (const std::optional event = m_window.pollEvent()) {
        if (event->is<sf::Event::Closed>())
            m_window.close();

        if (const auto* key = event->getIf<sf::Event::KeyPressed>()) {
            if (key->code == sf::Keyboard::Key::Escape)
                m_window.close();

            if (m_isGameOver && key->code == sf::Keyboard::Key::Enter)
                resetGame();
        }
    }
}

// ---------------------------------------------------------------------------
// update: all dt-based logic lives here
// ---------------------------------------------------------------------------
void Game::update(float dt) {
    if (m_isGameOver) return;

    // 1. Player input & paddle velocity
    sf::Vector2f playerVel = { 0.f, 0.f };
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
        playerVel.y -= Paddle::SPEED;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
        playerVel.y += Paddle::SPEED;
    m_playerPaddle->setVelocity(playerVel);

    // 2. AI controller sets AI paddle velocity
    m_aiController->update();

    // 3. Move all entities (velocity * dt happens inside each update)
    m_playerPaddle->update(dt);
    m_aiPaddle->update(dt);
    m_ball->update(dt);

    // 4. Clamp paddles inside the field
    clampPaddle(*m_playerPaddle);
    clampPaddle(*m_aiPaddle);

    // 5. Ball collisions (walls + paddles)
    checkCollisions();

    // 6. Scoring
    sf::FloatRect bb = m_ball->getBounds();
    bool scoreChanged = false;

    if (bb.position.x + bb.size.x < 0.f) {
        ++m_aiScore;
        scoreChanged = true;
    }
    else if (bb.position.x > WINDOW_WIDTH) {
        ++m_playerScore;
        scoreChanged = true;
    }

    if (scoreChanged) {
        updateScoreText();
        resetBall();

        if (m_playerScore >= m_maxScore || m_aiScore >= m_maxScore) {
            m_isGameOver = true;

            if (m_playerScore >= m_maxScore) {
                m_endGameText->setString("VICTORY!\nPress Enter to Restart");
                m_victorySound->play();
            }
            else {
                m_endGameText->setString("DEFEAT\nPress Enter to Restart");
                m_defeatSound->play();
            }

            sf::FloatRect tb = m_endGameText->getLocalBounds();
            m_endGameText->setOrigin({ tb.size.x / 2.f, tb.size.y / 2.f });
            m_endGameText->setPosition({ WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f });
        }
        else {
            m_scoreSound->play();
        }
    }
}

// ---------------------------------------------------------------------------
// checkCollisions: bounce logic — sets velocity, never calls update()
// ---------------------------------------------------------------------------
void Game::checkCollisions() {
    sf::FloatRect bb = m_ball->getBounds();
    sf::Vector2f  vel = m_ball->getVelocity();

    // --- Top/bottom walls ---
    // Use std::abs to enforce direction after bounce, preventing double-trigger
    // if the ball penetrates the wall by more than one frame.
    if (bb.position.y < 0.f) {
        m_ball->setPosition({ m_ball->getPosition().x, Ball::RADIUS });
        m_ball->setVelocity({ vel.x, std::abs(vel.y) });   // force downward
        return; // re-evaluate next frame with corrected position
    }
    if (bb.position.y + bb.size.y > static_cast<float>(WINDOW_HEIGHT)) {
        m_ball->setPosition({ m_ball->getPosition().x,
                               static_cast<float>(WINDOW_HEIGHT) - Ball::RADIUS });
        m_ball->setVelocity({ vel.x, -std::abs(vel.y) });  // force upward
        return;
    }

    // --- Paddles ---
    if (bb.findIntersection(m_playerPaddle->getBounds()) && vel.x < 0.f) {
        vel = m_ball->getVelocity();
        vel.x = std::abs(vel.x) * 1.08f;   // force rightward + speed up
        vel.y *= 1.08f;
        m_ball->setVelocity(vel);
        playRandomHit(true);
    }
    else if (bb.findIntersection(m_aiPaddle->getBounds()) && vel.x > 0.f) {
        vel = m_ball->getVelocity();
        vel.x = -std::abs(vel.x) * 1.08f;  // force leftward + speed up
        vel.y *= 1.08f;
        m_ball->setVelocity(vel);
        playRandomHit(false);
    }
}

// ---------------------------------------------------------------------------

void Game::clampPaddle(Paddle& paddle) {
    sf::Vector2f pos = paddle.getPosition();
    const float  halfH = Paddle::HEIGHT / 2.f;
    pos.y = std::clamp(pos.y, halfH, static_cast<float>(WINDOW_HEIGHT) - halfH);
    paddle.setPosition(pos);
}

void Game::resetBall() {
    m_ball->setPosition({ WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f });
    m_ball->setVelocity({ Ball::INITIAL_SPEED, Ball::INITIAL_SPEED * 0.5f });
}

void Game::resetGame() {
    m_playerScore = 0;
    m_aiScore = 0;
    m_isGameOver = false;
    updateScoreText();
    resetBall();
}

// ---------------------------------------------------------------------------

void Game::render() {
    m_window.clear(sf::Color::Black);

    for (const auto& dash : m_midLine)
        m_window.draw(dash);

    m_playerPaddle->draw(m_window);
    m_aiPaddle->draw(m_window);
    m_ball->draw(m_window);

    if (m_playerScoreText) m_window.draw(*m_playerScoreText);
    if (m_aiScoreText)     m_window.draw(*m_aiScoreText);
    if (m_isGameOver && m_endGameText)
        m_window.draw(*m_endGameText);

    m_window.display();
}

void Game::updateScoreText() {
    m_playerScoreText->setString(std::to_string(m_playerScore));
    m_aiScoreText->setString(std::to_string(m_aiScore));
}

void Game::playRandomHit(bool isLeft) {
    const auto& buffers = isLeft ? m_leftHitBuffers : m_rightHitBuffers;
    if (buffers.empty()) return;

    std::uniform_int_distribution<size_t> dist(0, buffers.size() - 1);
    m_hitSound->setBuffer(buffers[dist(m_rng)]);
    m_hitSound->play();
}
