#include "GameElements.hpp"





Player::Player(const char* path, int width, int height, std::vector<float> positions) : m_Positions(positions), m_Speed(0.0f), m_Velocity(0.0f), m_Lives(3)
{
	m_PlayerAttributes.height = height;
	m_PlayerAttributes.width = width;
	m_PlayerAttributes.path = path;

	m_PlayerTexture = createSprite(m_PlayerAttributes.path);
	setSpriteSize(m_PlayerTexture, m_PlayerAttributes.width, m_PlayerAttributes.height);
}

Player::~Player()
{
	destroySprite(m_PlayerTexture);
}

void Player::drawTexture(float dt)
{
	drawSprite(m_PlayerTexture, m_Positions[0], m_Positions[1]);
	m_Positions[0] += m_Speed * dt;
}






Ball::Ball(const char* path, int width, int height, std::vector<float> positions) : m_Speed(0.0f), m_Velocity(2, 0.0f), m_Positions(positions)
{
	m_BallAttributes.path = path;
	m_BallAttributes.height = height;
	m_BallAttributes.width = width;

	m_BallTexture = createSprite(m_BallAttributes.path);
	setSpriteSize(m_BallTexture, m_BallAttributes.width, m_BallAttributes.height);
}

Ball::~Ball()
{
	destroySprite(m_BallTexture);
}

void Ball::drawBall(float dt)
{
	drawSprite(m_BallTexture, m_Positions[0], m_Positions[1]);
	m_Positions[0] += (m_Velocity[0] * m_Speed) * dt;
	m_Positions[1] += (m_Velocity[1] * m_Speed) * dt;
}







Boards::Boards(const char* path, int width, int height) : m_Lives(0), m_Type(0), m_Positions()
{
	m_BoardsAttributes.path = path;
	m_BoardsAttributes.height = height;
	m_BoardsAttributes.width = width;


	m_BoardsTexture = createSprite(m_BoardsAttributes.path);
	setSpriteSize(m_BoardsTexture, m_BoardsAttributes.width, m_BoardsAttributes.height);
}

Boards::~Boards()
{
	destroySprite(m_BoardsTexture);
}

void Boards::drawBoard(float x, float y)
{
	m_Positions[0] = x;
	m_Positions[1] = y;
	m_Positions[2] = x + (float)m_BoardsAttributes.width;
	m_Positions[3] = y + (float)m_BoardsAttributes.height;
	drawSprite(m_BoardsTexture, x, y);
}

int Boards::getWidth()
{
	return m_BoardsAttributes.width;
}

int Boards::getHeight()
{
	return m_BoardsAttributes.height;
}

bool Boards::isAlive()
{
	if (m_Lives > 0)
		return true;
	return false;
}

void Boards::setTexture(const char* path)
{
	destroySprite(m_BoardsTexture);

	m_BoardsTexture = createSprite(path);
	setSpriteSize(m_BoardsTexture, m_BoardsAttributes.width, m_BoardsAttributes.height);
}






Ability::Ability(const char* path, int width, int heigth, const int type) : m_Type(type), m_Alive(true), m_Speed(0)
{
	m_AbilityTexture = createSprite(path);
	setSpriteSize(m_AbilityTexture, width, heigth);
}

Ability::~Ability()
{
	destroySprite(m_AbilityTexture);
}

void Ability::drawAbility(float dt)
{
	drawSprite(m_AbilityTexture, m_Positions[0], m_Positions[1]);
	m_Positions[1] += m_Speed * dt;
}






ScoreTex::ScoreTex(const char* path, int width, int height)
{
	m_ScoreAttributes.path = path;
	m_ScoreAttributes.width = width;
	m_ScoreAttributes.height = height;

	m_ScoreTexture = createSprite(m_ScoreAttributes.path);
	setSpriteSize(m_ScoreTexture, m_ScoreAttributes.width, m_ScoreAttributes.height);
}

ScoreTex::~ScoreTex()
{
	destroySprite(m_ScoreTexture);
}

void ScoreTex::drawScore(int x, int y)
{
	drawSprite(m_ScoreTexture, x, y);
}

void ScoreTex::setTexutre(const char* path)
{
	m_ScoreAttributes.path = path;

	destroySprite(m_ScoreTexture);
	m_ScoreTexture = createSprite(m_ScoreAttributes.path);
	setSpriteSize(m_ScoreTexture, m_ScoreAttributes.width, m_ScoreAttributes.height);
}

