#pragma once

#include <iostream>
#include <vector>
#include <ctime>
#include <sstream>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
/*
Class that acts as the game engine.
Wrapper class.
*/
class Game
{
private:
	//Variables
	//Window
	sf::RenderWindow* window;
	sf::VideoMode videoMode;
	sf::Event eve;

	//Mouse positions
	sf::Vector2i mousePosWindow;
	sf::Vector2f mousePosView;

	//Resources
	sf::Font font;
	sf::Texture enemyTexture;
	sf::Texture backgroundTexture;
	sf::Sprite backgroundSprite;

	//Text
	sf::Text uiText;

	//Game logic
	bool endGame;
	unsigned points;
	int health;
	float enemySpawnTimer;
	float enemySpawnTimerMax;
	float enemyDirectionX;
	float enemySpeedY;
	int maxEnemies;
	bool canEnemyDirectionChange;
	bool hasEnemyDirectionChanged;
	bool hasWonGame;
	sf::Clock clock;
	float elapsedTime;

	//Sounds
	sf::SoundBuffer hitSoundBuffer;
	sf::SoundBuffer winSoundBuffer;
	sf::SoundBuffer whooshSoundBuffer;
	sf::SoundBuffer slidingSoundBuffer;
	sf::Sound hitSound;
	sf::Sound winSound;
	sf::Sound whooshSound;
	sf::Sound slidingSound;

	//Player parameters
	float playerX = 50;
	float playerY = 500;
	float playerXSpeed = 0;
	float playerYSpeed = 1;

	//Game objects
	std::vector<sf::Sprite> enemies;
	sf::Sprite enemy;
	sf::RectangleShape leftBorder;
	sf::RectangleShape rightBorder;
	sf::CircleShape player;

	//Private functions
	void initVariables();
	void initSounds();
	void initWindow();
	void initTextures();
	void initSprites();
	void initEnemies();
	void initBorders();
	void initPlayer();
	void initFonts();
	void initText();
	

public:
	//Constructors / Destructors
	Game();
	virtual ~Game();

	//Accessors
	const bool running() const;	
	const bool getEndGame() const;

	//Functions
	void spawnEnemy();
	void pollEvents();	
	void updateMousePos();
	void updateText();
	void updateEnemies();
	void updatePlayer();
	void updateBorders();
	void updateTime();
	void update();

	//Render functions
	void renderBackground(sf::RenderTarget& target);
	void renderBorders(sf::RenderTarget& target);
	void renderPlayer(sf::RenderTarget& target);
	void renderText(sf::RenderTarget& target);
	void renderEnemies(sf::RenderTarget& target);
	void render();
};

