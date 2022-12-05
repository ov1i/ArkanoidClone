#pragma once
#pragma once
#include <Framework.h>
#include <vector>

struct texAttirbutes
{
	int width, height;
	const char* path;
};




class Player
{
public:
	Player(const char* path, int width, int height, std::vector<float> positions);
	~Player();

	void drawTexture(float dt);

	void setVelocity(float velocity) { m_Velocity = velocity; }
	void setLives(int lives) { m_Lives = lives; }
	void setSpeed(float speed) { m_Speed = speed; }
	void setSingularPosition(int index, float position) { m_Positions[index] = position; }

	int getWidth() { return m_PlayerAttributes.width; }
	int getHeight() { return m_PlayerAttributes.height; }
	int getLives() { return m_Lives; }
	float getVelocity() { return m_Velocity; }
	float getSpeed() { return m_Speed; }
	std::vector<float> getPositions() { return m_Positions; }

private:
	Sprite* m_PlayerTexture;
	texAttirbutes m_PlayerAttributes;

	std::vector<float> m_Positions;
	float m_Velocity;
	float m_Speed;
	int m_Lives;
};






class Boards
{
public:
	Boards(const char* path, int width, int height);
	~Boards();

	void setTexture(const char* path);
	void drawBoard(float x, float y);
	void setLives(int lives) { m_Lives = lives; }
	void setType(int type) { m_Type = type; }

	int getWidth();
	int getHeight();
	int getLives() { return m_Lives; }
	int getType() { return m_Type; }

	bool isAlive();

	float getPos(int i) { return m_Positions[i]; }

private:
	Sprite* m_BoardsTexture;
	texAttirbutes m_BoardsAttributes;

	int m_Lives;
	int m_Type;

	float m_Positions[4];
};






class Background
{
public:
	Background(const char* path, int width, int height)
	{
		m_BackgroundAttributes.height = height;
		m_BackgroundAttributes.width = width;
		m_BackgroundAttributes.path = path;

		m_BackgroundTexture = createSprite(m_BackgroundAttributes.path);
		setSpriteSize(m_BackgroundTexture, m_BackgroundAttributes.width, m_BackgroundAttributes.height);
	}
	void setBg(const char* path)
	{
		destroySprite(m_BackgroundTexture);
		m_BackgroundAttributes.path = path;

		m_BackgroundTexture = createSprite(m_BackgroundAttributes.path);
		setSpriteSize(m_BackgroundTexture, m_BackgroundAttributes.width, m_BackgroundAttributes.height);
	}
	void drawBg()
	{
		drawSprite(m_BackgroundTexture, 0, 0);
	}
	~Background()
	{
		destroySprite(m_BackgroundTexture);
	}

private:
	Sprite* m_BackgroundTexture;
	texAttirbutes m_BackgroundAttributes;
};






class Ball
{
public:
	Ball(const char* path, int width, int height, std::vector<float> positions);
	~Ball();

	void drawBall(float dt);

	void setVelocity(std::vector<float> velocity) { m_Velocity = velocity; }
	void setSpeed(float speed) { m_Speed = speed; } //
	void setSingularVelocity(int index, float velocity) { m_Velocity[index] = velocity; }
	void setSingularPosition(int index, float position) { m_Positions[index] = position; }

	std::vector<float> getPos() { return m_Positions; }
	std::vector<float> getVelocity() { return m_Velocity; }
	float getSpeed() { return m_Speed; }

private:
	Sprite* m_BallTexture;
	texAttirbutes m_BallAttributes;

	std::vector<float> m_Positions;
	std::vector<float> m_Velocity;
	float m_Speed;
};




class Ability
{
public:
	Ability(const char* path, int width, int heigth, const int type);
	~Ability();

	void drawAbility(float dt);

	void setPositions(std::vector<float> pos) { m_Positions = pos; }
	void setLives(int lives) { m_Alive = lives; }
	void setSpeed(float speed) { m_Speed = speed; }

	int getType() { return m_Type; }
	float getSpeed() { return m_Speed; }
	std::vector<float> getPos() { return m_Positions; }
	bool isAlive() { return m_Alive; }

private:
	Sprite* m_AbilityTexture;

	std::vector<float> m_Positions;
	int m_Type;
	float m_Speed;
	bool m_Alive = false;
};


class ScoreTex
{
public:
	ScoreTex(const char* path, int width, int height);
	~ScoreTex();

	void drawScore(int x, int y);
	void setTexutre(const char* path);

private:
	Sprite* m_ScoreTexture;
	texAttirbutes m_ScoreAttributes;


};