#include "Game.h"
#include <iostream>

Game::Game()
	: m_window(sf::VideoMode(sf::Vector2u(800, 600)), "C++ Pong Test Assignment") {

	m_playerPaddle = std::make_unique<Paddle>(30.f, m_windowHeight / 2.f);
	m_aiPaddle = std::make_unique<Paddle>(m_windowWidth - 30.f, m_windowHeight / 2.f);
	m_ball = std::make_unique<Ball>(m_windowWidth / 2.f, m_windowHeight / 2.f);

	m_aiController = std::make_unique<AIController>(*m_aiPaddle, *m_ball);


	//TEXT AND SCORE INITIALIZING

	if (!m_font.openFromFile("assets/Aria.ttf")) {
		std::cout << "FAILED TO LOAD FONT\n";   //exception
	}

	m_playerScoreText = std::make_unique<sf::Text>(m_font);
	m_aiScoreText = std::make_unique<sf::Text>(m_font);

	m_playerScoreText->setFont(m_font);
	m_playerScoreText->setCharacterSize(50);
	m_playerScoreText->setFillColor(sf::Color::White);
	m_playerScoreText->setPosition({ m_windowWidth / 4.f, 20.f });

	*m_aiScoreText = *m_playerScoreText;
	m_aiScoreText->setPosition({ m_windowWidth * 0.75f, 20.f });

	updateScoreText();



	m_endGameText = std::make_unique<sf::Text>(m_font);
	m_endGameText->setCharacterSize(40);
	m_endGameText->setFillColor(sf::Color::Yellow);
	m_endGameText->setStyle(sf::Text::Bold);


	//CENTRAL VERTICAL LINE INITIALIZING

	float dashHeight = 20.f;
	float gap = 15.f;
	for (float y = 0; y < m_windowHeight; y += dashHeight + gap) {
		sf::RectangleShape dash(sf::Vector2f(5.f, dashHeight));
		dash.setFillColor(sf::Color(255, 255, 255, 100));
		dash.setOrigin({ 2.5f, 0.f });
		dash.setPosition({ m_windowWidth / 2.f, y });
		m_midLine.push_back(dash);
	}

	//SOUND INITIALIZING

	for (int i = 1; i <= 3; ++i) {
		sf::SoundBuffer buf;
		if (buf.loadFromFile("assets/sfx/hit_left_" + std::to_string(i) + ".mp3")) {
			m_leftHitBuffers.push_back(std::move(buf));
		}
	}
	if (m_leftHitBuffers.empty()) {
		std::cout << "FAILED TO LOAD LEFT SOUNDS\n"; //exception
	}

	for (int i = 1; i <= 2; ++i) {
		sf::SoundBuffer buf;
		if (buf.loadFromFile("assets/sfx/hit_right_" + std::to_string(i) + ".mp3")) {
			m_rightHitBuffers.push_back(std::move(buf));
		}
	}
	if (m_rightHitBuffers.empty()) {
		std::cout << "FAILED TO LOAD RIGHT SOUNDS\n"; //exception
	}

	if (!m_scoreBuffer.loadFromFile("assets/sfx/goal.mp3")) {
		std::cout << "FAILED TO LOAD goal.mp3\n"; //exception
	}

	if (!m_victoryBuffer.loadFromFile("assets/sfx/victory.mp3")) {
		std::cout << "FAILED TO LOAD victory.mp3\n"; //exception
	}

	if (!m_defeatBuffer.loadFromFile("assets/sfx/defeat.mp3")) {
		std::cout << "FAILED TO LOAD defeat.mp3\n"; //exception
	}


	m_defeatSound = std::make_unique<sf::Sound>(m_defeatBuffer);
	m_victorySound = std::make_unique<sf::Sound>(m_victoryBuffer);
	m_scoreSound = std::make_unique<sf::Sound>(m_scoreBuffer);
	m_hitSound = std::make_unique<sf::Sound>(m_rightHitBuffers[0]);
}

void Game::run() {
	sf::Clock clock;
	while (m_window.isOpen()) {
		sf::Time deltaTime = clock.restart();
		float dt = deltaTime.asSeconds();

		processEvents(dt);
		update(dt);
		render();
	}
}

void Game::processEvents(float dt) {
	while (const std::optional event = m_window.pollEvent()) {

		if (event->is<sf::Event::Closed>()) {
			m_window.close();
		}

		if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
			if (keyPressed->code == sf::Keyboard::Key::Escape)
				m_window.close();


			if (m_isGameOver && keyPressed->code == sf::Keyboard::Key::Enter) {
				m_playerScore = 0;
				m_aiScore = 0;
				m_isGameOver = false;
				updateScoreText();
				m_ball->reset(m_windowWidth / 2, m_windowHeight / 2);
			}
		}
	}


	if (!m_isGameOver) {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
			m_playerPaddle->moveUp(dt);

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
			m_playerPaddle->moveDown(dt);
	}


}

void Game::update(float dt) {
	if (m_isGameOver) return;

	m_ball->update(dt);
	m_playerPaddle->update(dt);
	m_aiController->update(dt);

	checkCollisions();

	bool scoreChanged = false;
	if (m_ball->getBounds().position.x < 0) {
		m_aiScore++;
		scoreChanged = true;
	}
	else if (m_ball->getBounds().position.x > m_windowWidth) {
		m_playerScore++;
		scoreChanged = true;
	}

	if (scoreChanged) {
		updateScoreText();
		m_ball->reset(m_windowWidth / 2, m_windowHeight / 2);

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

			sf::FloatRect textBounds = m_endGameText->getLocalBounds();
			m_endGameText->setOrigin({ textBounds.size.x / 2.f, textBounds.size.y / 2.f });
			m_endGameText->setPosition({ m_windowWidth / 2.f, m_windowHeight / 2.f });
		}
		else {
			m_scoreSound->play();
		}
	}
}

void Game::checkCollisions() {
	if (m_ball->getBounds().position.y < 0 ||
		m_ball->getBounds().position.y + m_ball->getBounds().size.y > m_windowHeight) {

		m_ball->bounceY();

	}

	if (m_ball->getBounds().findIntersection(m_playerPaddle->getBounds()) && m_ball->getVelocity().x < 0) {
		m_ball->bounceX();
		playRandomHit(true);
	}

	if (m_ball->getBounds().findIntersection(m_aiPaddle->getBounds()) && m_ball->getVelocity().x > 0) {
		m_ball->bounceX();
		playRandomHit(false);
	}

}

void Game::render() {
	m_window.clear(sf::Color::Black);

	for (const auto& dash : m_midLine) {
		m_window.draw(dash);
	}

	m_playerPaddle->draw(m_window);
	m_aiPaddle->draw(m_window);
	m_ball->draw(m_window);

	if (m_playerScoreText) m_window.draw(*m_playerScoreText);
	if (m_aiScoreText) m_window.draw(*m_aiScoreText);

	if (m_isGameOver && m_endGameText) {
		m_window.draw(*m_endGameText);
	}

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
	size_t randomIndex = dist(m_rng);

	m_hitSound->setBuffer(buffers[randomIndex]);
	m_hitSound->play();
}