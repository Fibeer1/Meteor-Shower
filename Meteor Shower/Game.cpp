#include "Game.h"
#include <iostream>

/*
	CREDITS:
	Suraj Sharma for the template code
	Pixabay.com and SoundJay.com for the sounds


*/

//private functions
void Game::initVariables()
{
	this->window = nullptr;

	//Game logic
	this->endGame = false;
	this->points = 0;
	this->health = 10;
	this->enemySpawnTimerMax = 9.5f;
	this->enemySpawnTimer = this->enemySpawnTimerMax;
	this->maxEnemies = 20;
	this->enemyDirectionX = 0;
	this->enemySpeedY = 5.f;
	this->canEnemyDirectionChange = true;
	this->clock;
	this->elapsedTime;
	this->hasEnemyDirectionChanged = false;
	this->hasWonGame = false;

}
void Game::initWindow()
{
	this->videoMode.width = 800;
	this->videoMode.height = 600;	
	this->window = new sf::RenderWindow(this->videoMode, "Meteor Shower", sf::Style::Titlebar | sf::Style::Close);
	this->window->setFramerateLimit(60);
}
void Game::initEnemies()
{
	this->enemy.setPosition(25.0f,25.0f);
}
void Game::initBorders()
{
	this->leftBorder.setPosition(5, 0);
	this->leftBorder.setSize(sf::Vector2f(-700.0f, 600.0f));
	this->leftBorder.setFillColor(sf::Color::Black);
	this->rightBorder.setPosition(795, 0);
	this->rightBorder.setSize(sf::Vector2f(700.0f, 600.0f));
	this->rightBorder.setFillColor(sf::Color::Black);
}
void Game::initPlayer()
{
	this->player.setRadius(20.f);
	this->player.setOrigin(20, 40);
	this->player.setPosition(0, 0);
	this->player.setPosition(playerX, playerY);
	this->player.setFillColor(sf::Color::Yellow);
}
void Game::initFonts()
{
	if (!this->font.loadFromFile("Assets/Fonts/PixellettersFull.ttf"))
	{
		std::cout << "ERROR::GAME::INITFONTS::Failed to load font!" << "\n";
	}
	
}
void Game::initText()
{
	this->uiText.setFont(this->font);
	this->uiText.setCharacterSize(24);
	this->uiText.setPosition(20, 0);
	this->uiText.setOutlineThickness(1);
	this->uiText.setFillColor(sf::Color::White);
	this->uiText.setString("NONE");
}
void Game::initTextures()
{
	if (!this->backgroundTexture.loadFromFile("Assets/Images/background.png"))
	{
		std::cout << "ERROR::GAME::INITTEXTURES::Failed to load texture!" << "\n";
	}
	if (!this->enemyTexture.loadFromFile("Assets/Images/meteorite.png"))
	{
		std::cout << "ERROR::GAME::INITTEXTURES::Failed to load texture!" << "\n";
	}
	backgroundTexture.setSmooth(true);
	enemyTexture.setSmooth(true);
}
void Game::initSprites()
{
	backgroundSprite.setTexture(backgroundTexture);
	enemy.setTexture(enemyTexture);
}
void Game::initSounds()
{
	if (!this->hitSoundBuffer.loadFromFile("Assets/Sounds/HitSound.wav"))
	{
		std::cout << "ERROR::GAME::INITSOUNDS::Failed to load sound!" << "\n";
	}
	if (!this->winSoundBuffer.loadFromFile("Assets/Sounds/WinSound.wav"))
	{
		std::cout << "ERROR::GAME::INITSOUNDS::Failed to load sound!" << "\n";
	}
	if (!this->whooshSoundBuffer.loadFromFile("Assets/Sounds/whoosh.wav"))
	{
		std::cout << "ERROR::GAME::INITSOUNDS::Failed to load sound!" << "\n";
	}
	if (!this->slidingSoundBuffer.loadFromFile("Assets/Sounds/HeavyObjectSliding.wav"))
	{
		std::cout << "ERROR::GAME::INITSOUNDS::Failed to load sound!" << "\n";
	}
	hitSound.setBuffer(hitSoundBuffer);
	winSound.setBuffer(winSoundBuffer);
	whooshSound.setBuffer(whooshSoundBuffer);
	slidingSound.setBuffer(slidingSoundBuffer);
}

//constructors /destructors
Game::Game()
{
	this->initTextures();
	this->initSounds();
	this->initSprites();
	this->initVariables();
	this->initWindow();
	this->initEnemies(); //the order matters
	this->initBorders();
	this->initPlayer();
	this->initFonts();
	this->initText();
}
Game::~Game()
{
	delete this->window;
}

//Accessors
const bool Game::running() const
{
	return this->window->isOpen();
}
const bool Game::getEndGame() const
{
	return this->endGame;
}

//Functions
void Game::spawnEnemy()
{
	/*
	@return void
	Spawns enemies and sets their colors and positions
	-Sets a random position
	-Sets a random color
	-Adds enemy to the vector
	*/
	this->enemy.setPosition(
		static_cast<float>(rand()%static_cast<int>(this->window->getSize().x - this->enemy.getScale().x)),
		-30.f);

	//Spawn the enemy
	this->enemies.push_back(this->enemy);
}
void Game::pollEvents()
{
	//Event polling
	while (this->window->pollEvent(this->eve))
	{
		switch (this->eve.type)
		{
		case sf::Event::Closed:
		{
			this->window->close();
			break;
		}		
		case sf::Event::KeyPressed:
		{
			if (this->eve.key.code == sf::Keyboard::Escape)
			{
				this->window->close();
			}				
		}
		}
	}
}
void Game::updateMousePos()
{
	/*
		@return void
		Updates the mouse positions:
		- Mouse position relative to the window (Vector2i)
	*/
	this->mousePosWindow = sf::Mouse::getPosition(*this->window);
	this->mousePosView = this->window->mapPixelToCoords(this->mousePosWindow);
}
void Game::updateText()
{
	std::stringstream ss;	
	if (!this->endGame)
	{
		ss << "Points: " << this->points << "\n" << "Health: " << this->health << "\n";
		this->uiText.setString(ss.str());
	}
	else
	{
		if (hasWonGame)
		{
			
			ss << "Congratulations!" << "\n" << "You won!" << "\n" << "Points gained: " << this->points << "\n" << "Remaining health: " << this->health << "\n" << "Press ESC to quit" << "\n";
			this->uiText.setCharacterSize(50);
			this->uiText.setPosition(250, 300);
			this->uiText.setString(ss.str());
		}
		else
		{
			ss << "Game over" << "\n" << "Points gained: " << this->points << "\n" << "Press ESC to quit" << "\n";
			this->uiText.setCharacterSize(50);
			this->uiText.setPosition(250, 400);
			this->uiText.setString(ss.str());
		}
	}
	
}
void Game::updateEnemies()
{
	/*
	@return void
	Updates the enemy spawn timer and spawns enemies when the total amount of enemies is lower than the max
	moves enemies downwards
	removes the enemies at the edge of the screen
	damages the player upon collision
	*/

	elapsedTime = clock.getElapsedTime().asSeconds();

	//updating the timer for enemy spawning
	if (this->enemies.size() < this->maxEnemies)
	{
		if (this->enemySpawnTimer >= this->enemySpawnTimerMax)
		{
			//spawn and reset the enemy timer
			this->spawnEnemy();
			this->enemySpawnTimer = 0.f;
		}
		else
		{
			this->enemySpawnTimer += 1.f;
		}
	}
	//Move and update the enemies	
	for (int i = 0; i < this->enemies.size(); i++)
	{
				
		bool deleted = false;
		this->enemies[i].move(enemyDirectionX, enemySpeedY);
		this->enemies[i].rotate(0.5f);
		if (enemyDirectionX > 0)
		{
			this->enemies[i].rotate(1);
		}
		if (enemyDirectionX < 0)
		{
			this->enemies[i].rotate(-1);
		}
		if (this->enemies[i].getPosition().y > this->window->getSize().y + 30)
		{
			deleted = true;
			points++;
		}
		if (player.getGlobalBounds().intersects(enemies[i].getGlobalBounds()))
		{			
			deleted = true;
			hitSound.play();
			health--;
			if (points < 5)
			{
				points = 0;
			}			
			else
			{
				points -= 5;
			}
		}
		if (deleted)
		{
			this->enemies.erase(this->enemies.begin() + i);
		}

	}
	canEnemyDirectionChange = false;
}
void Game::updatePlayer()
{
	//Player's bouncing and movement mechanics
	this->player.move(playerXSpeed, playerYSpeed);
	playerX = player.getPosition().x;
	playerY = player.getPosition().y;
	playerYSpeed++;
	if (playerYSpeed < -10)
	{
		playerYSpeed = -10;
	}
	if (player.getPosition().y > this->window->getSize().y)
	{
		playerYSpeed = -playerYSpeed;
		playerY = 600;
		player.setPosition(playerX, playerY);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		playerXSpeed = -5;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		playerXSpeed = 5;
	}
	else
	{
		playerXSpeed = 0;
	}
}
void Game::updateBorders()
{
	//Barriers which prevent the player from going out of bounds
	if (leftBorder.getGlobalBounds().intersects(player.getGlobalBounds()))
	{
		playerX = leftBorder.getPosition().x + 30;
		player.setPosition(playerX, playerY);
	}
	if (rightBorder.getGlobalBounds().intersects(player.getGlobalBounds()))
	{
		playerX = rightBorder.getPosition().x - 30;
		player.setPosition(playerX, playerY);
	}
	elapsedTime = clock.getElapsedTime().asSeconds();
	if (elapsedTime > 60 && this->leftBorder.getPosition().x != 400 && this->rightBorder.getPosition().x != 500)
	{
		this->leftBorder.move(1, 0);
		this->rightBorder.move(-1, 0);				
	}
	if (elapsedTime > 59.9f && elapsedTime < 60.f)
	{
		slidingSound.play();
	}
}
void Game::updateTime()
{
	//make stuff happen depending on the time
	elapsedTime = clock.getElapsedTime().asSeconds();
	if (elapsedTime > 1)
	{
		if (static_cast<int>(elapsedTime) % 5 == 0 && !hasEnemyDirectionChanged)
		{
			
			canEnemyDirectionChange = true;
			hasEnemyDirectionChanged = true;
			whooshSound.play();
		}
		if (canEnemyDirectionChange)
		{
			//Change the direction and speed of the enemies every 5 seconds
			enemySpeedY = 4.f + (rand() % 5);
			enemyDirectionX = -3.f + (rand() % 5);
			canEnemyDirectionChange = false;
		}
		if (static_cast<int>(elapsedTime) % 9 == 0 && static_cast<int>(elapsedTime) != 45 && hasEnemyDirectionChanged)
		{
			//Check if the enemies are ready to change direction
			hasEnemyDirectionChanged = false;
		}
	}
	if (elapsedTime > 70)
	{
		this->hasWonGame = true;
		this->endGame = true;
	}
	if (elapsedTime > 69.9f)
	{
		this->winSound.play();
	}
	if (elapsedTime > 65)
	{
		this->maxEnemies = 7;
		enemyDirectionX = 0;
	}
}

void Game::update()
{
	this->pollEvents();
	if (!this->endGame)
	{
		this->updateEnemies();		
		this->updateMousePos();
		this->updateBorders();
		this->updatePlayer();
		this->updateTime();
	}
	this->updateText();

	//End game condition
	if (this->health <= 0)
	{
		this->endGame = true;
	}
}
void Game::renderBackground(sf::RenderTarget& target)
{
	target.draw(this->backgroundSprite);
}
void Game::renderBorders(sf::RenderTarget& target)
{
	target.draw(this->leftBorder);
	target.draw(this->rightBorder);
}
void Game::renderPlayer(sf::RenderTarget& target)
{
	target.draw(this->player);
}
void Game::renderText(sf::RenderTarget& target)
{
	target.draw(this->uiText);
}
void Game::renderEnemies(sf::RenderTarget& target)
{
	//Rendering the enemies
	for (auto& e : this->enemies)
	{
		target.draw(e);
	}
}
void Game::render()
{
	/*
		@return void
		-clear old frame
		-render new objects
		-display frame in window
		Renders the game objects.
	*/
	this->window->clear();
	//Draw game objects

	this->renderBackground(*this->window);

	this->renderPlayer(*this->window);

	this->renderEnemies(*this->window);

	this->renderBorders(*this->window);

	this->renderText(*this->window);

	this->window->display();
}