
#include <Framework.h>
#include "GameElements.hpp"
#include <iostream>
#include <string>
#include <random>
#include <time.h>
#include <vector>
#include <Windows.h>
#include <sstream>


#define playerTex "res/Textures/pad1.png"
#define ballTex "res/Textures/ball.png"

#define crosshairTex "res/Textures/crosshair.png"

#define Bg1 "res/Textures/Background-1.png"
#define Bg2 "res/Textures/Background-2.png"
#define Bg3 "res/Textures/Background-3.png"
#define Bg4 "res/Textures/Background-4.png"
#define Bg5 "res/Textures/Background-5.png"
#define Bg6 "res/Textures/Background-6.png"

#define Heart1 "res/Textures/heart.png"
#define Heart2 "res/Textures/heart1.png"

#define ability1 "res/Textures/ability-1.png"
#define ability2 "res/Textures/ability-2.png"

#define soft_board "res/Textures/soft_board.png"
#define soft_board_broken "res/Textures/soft_board_2.png"
#define hard_board "res/Textures/hard_board.png"
#define hard_board_semi_broken "res/Textures/hard_board_2.png"
#define hard_board_broken "res/Textures/hard_board_3.png"
#define weak_board "res/Textures/weak_board.png"
#define bomb_board "res/Textures/bomb_board.png"

#define Flame "res/Textures/flame.png"

#define WallTexture "res/Textures/wall.png"

#define Digit0 "res/Textures/0.png"
#define Digit1 "res/Textures/1.png"
#define Digit2 "res/Textures/2.png"
#define Digit3 "res/Textures/3.png"
#define Digit4 "res/Textures/4.png"
#define Digit5 "res/Textures/5.png"
#define Digit6 "res/Textures/6.png"
#define Digit7 "res/Textures/7.png"
#define Digit8 "res/Textures/8.png"
#define Digit9 "res/Textures/9.png"

#define ScoreStrTexture "res/Textures/score.png"

struct windowDim
{
    int width = 800;
    int height = 600;
}window;


Player* player;
Background* bg;
Ball* ball;
Boards* borders;
Sprite* crosshair, * heart1, * heart2, * explosion, * scoreStringTexture, * wall;


void outOfBounds();
void drawBoards();
void windowRation();
void generateRandomBoards();
void boardCollision();
void padHit();
void restartGame();
void hitBomb(int i);
void handleScore();

float Wedge(std::vector<float> a, std::vector<float> b)
{
    return a[0] * b[0] - a[1] * b[1];
}

std::vector<float> mousePos(2);
std::vector<Boards*> board(65);
std::vector<Ability*> abilities;
std::vector<ScoreTex*> scoreTextures;
std::vector<int> lastPositions(2), randomBoards;


int  counter = 0, totalRandomBoards, nrOfHardBoards, nrOfWeakBoards, nrOfBombBoards;
float fps = 60.0f, targetTicks = fps / 1000.0f, dt;

unsigned int startTicks;
unsigned int score = 0, hits = 0, interval;

bool shoot = false, isGameOver = false, restartReq = false, gameStarted = false, savingWallOn = false, gameWon = false;

bool alreadyExisting(int nr);
bool hitBoard(Boards& board);
bool remainingBoards();
bool getSide(std::vector<float> a, std::vector<float> b, std::vector<float> pt)
{
    std::vector<float> sideVector(2), ptRel(2);

    sideVector[0] = b[0] - a[0];
    sideVector[1] = b[1] - a[1];

    ptRel[0] = pt[0] - a[0];
    ptRel[1] = pt[1] - a[1];

    return Wedge(sideVector, ptRel) > 0;
}


class MyFramework : public Framework {

public:

    virtual void PreInit(int& width, int& height, bool& fullscreen)
    {
        width = window.width;
        height = window.height;
        fullscreen = false;
    }

    virtual bool Init()
    {
        std::vector<float> ballStartPos(2);
        std::vector<float> playerStartPos(2);

        bg = new Background(Bg5, window.width, window.height);

        playerStartPos[0] = (float)window.width / 2.0f - 35.0f;
        playerStartPos[1] = (float)window.height - 100.0f;
        player = new Player(playerTex, 80, 10, playerStartPos);

        ballStartPos[0] = playerStartPos[0] + 30.0f;
        ballStartPos[1] = playerStartPos[1] - 17.0f;
        ball = new Ball(ballTex, 15, 15, ballStartPos);

        wall = createSprite(WallTexture);
        setSpriteSize(wall, window.width + 160, 15);

        crosshair = createSprite(crosshairTex);
        setSpriteSize(crosshair, 30, 30);

        scoreStringTexture = createSprite(ScoreStrTexture);
        setSpriteSize(scoreStringTexture, 90, 80);

        heart1 = createSprite(Heart2);
        heart2 = createSprite(Heart1);
        setSpriteSize(heart1, 30, 25);
        setSpriteSize(heart2, 30, 25);

        explosion = createSprite(Flame);
        setSpriteSize(explosion, 30, 30);

        generateRandomBoards();

        for (int i = 0; i < board.size(); i++)
        {

            board[i] = new Boards(soft_board_broken, 40, 20);
            board[i]->setLives(1);
            board[i]->setType(2);

            for (int j = 0; j < totalRandomBoards; j++)
            {
                if (i == randomBoards[j] && (counter >= 0 && (counter < nrOfWeakBoards)))
                {
                    board[i] = new Boards(weak_board, 40, 20);
                    counter++;
                    board[i]->setLives(1);
                    board[i]->setType(2);
                    break;
                }
                if (i == randomBoards[j] && (counter >= nrOfWeakBoards) && (counter < (nrOfWeakBoards + nrOfBombBoards)))
                {
                    board[i] = new Boards(bomb_board, 40, 20);
                    counter++;
                    board[i]->setLives(1);
                    board[i]->setType(3);
                    break;
                }
                if (i == randomBoards[j] && (counter >= (nrOfWeakBoards + nrOfBombBoards) && counter < (nrOfWeakBoards + nrOfBombBoards + nrOfHardBoards)))
                {
                    board[i] = new Boards(hard_board, 40, 20);
                    board[i]->setLives(3);
                    board[i]->setType(1);
                    counter++;
                    break;
                }
            }
        }

        lastPositions[0] = 20;
        lastPositions[1] = 30;

        showCursor(false);

        return 1;

    }

    virtual void Close()
    {
        if (player) delete player;
        if (bg) delete bg;
        if (ball) delete ball;
        if (crosshair) destroySprite(crosshair);
        for (int i = 0; i < board.size(); i++)
            if (board[i])
                delete board[i];
    }

    virtual bool Tick()
    {
        unsigned int currentTicks = getTickCount();
        dt = (currentTicks - startTicks) * targetTicks;
        if (dt < targetTicks)
            Sleep((DWORD)(targetTicks - dt));

        startTicks = currentTicks;
        if (gameStarted == true)
        {
            if (player->getLives() == 0)
            {
                if (isGameOver == false)
                {
                    bg->setBg(Bg4);
                    isGameOver = true;
                }
                bg->drawBg();
                handleScore();
                drawSprite(crosshair, mousePos[0], mousePos[1]);
                restartReq = true;
            }
            else if (gameWon == true)
            {
                if (isGameOver == false)
                {
                    bg->setBg(Bg6);
                    isGameOver = true;
                }
                bg->drawBg();
                handleScore();
                drawSprite(crosshair, mousePos[0], mousePos[1]);
            }
            else
            {
                bg->drawBg();
                if (savingWallOn == true)
                    drawSprite(wall, -80, player->getPositions()[1] + 20);

                lastPositions[0] = 20;
                lastPositions[1] = 30;

                drawBoards();
                player->drawTexture(dt);

                ball->drawBall(dt);
                if (player->getLives() == 3)
                {
                    drawSprite(heart1, 15, window.height - 50);
                    drawSprite(heart1, 45, window.height - 50);
                    if (savingWallOn == true) drawSprite(heart2, 75, window.height - 50);
                    else drawSprite(heart1, 75, window.height - 50);
                }
                else if (player->getLives() == 2)
                {
                    drawSprite(heart1, 15, window.height - 50);
                    if (savingWallOn == true) drawSprite(heart2, 45, window.height - 50);
                    else drawSprite(heart1, 45, window.height - 50);
                }
                else
                {
                    if (savingWallOn == true)  drawSprite(heart2, 15, window.height - 50);
                    else drawSprite(heart1, 15, window.height - 50);
                }
                drawSprite(crosshair, mousePos[0], mousePos[1]);

                padHit();
                boardCollision();
                outOfBounds();

                if (shoot == true)
                {
                    if (player->getPositions()[0] >= (window.width - 70))
                        player->setSingularPosition(0, player->getPositions()[0] - 15.0f);
                    else if (player->getPositions()[0] <= 0)
                        player->setSingularPosition(0, player->getPositions()[0] + 15.0f);

                    if (player->getSpeed() < player->getVelocity())
                        player->setSpeed(player->getSpeed() + 0.25f);
                    else if (player->getSpeed() > player->getVelocity())
                        player->setSpeed(player->getSpeed() - 0.25f);


                    if (ball->getPos()[0] < 0.0f)
                    {
                        ball->setSingularPosition(0, ball->getPos()[0] + 1.0f);
                        ball->setSingularVelocity(0, -ball->getVelocity()[0]);
                    }
                    else if (ball->getPos()[0] > (float)window.width - 10.0f)
                    {
                        ball->setSingularPosition(0, ball->getPos()[0] - 1.0f);
                        ball->setSingularVelocity(0, -ball->getVelocity()[0]);
                    }
                    else if (ball->getPos()[1] < 0.0f)
                    {
                        ball->setSingularPosition(1, ball->getPos()[1] + 1.0f);
                        ball->setSingularVelocity(1, -ball->getVelocity()[1]);
                    }
                    player->drawTexture(dt);
                }


                if (getTickCount() / 1000 == interval)
                {
                    interval = getTickCount() / 1000 + rand() % 30 + 10;

                    if (rand() % 2 == 0)
                        abilities.push_back(new Ability(ability1, 20, 20, 1));
                    else abilities.push_back(new Ability(ability2, 20, 20, 2));
                    std::vector<float> pos{ (float)(rand() % window.width), (float)(rand() % window.height / 2) };
                    abilities[abilities.size() - 1]->setPositions(pos);
                    abilities[abilities.size() - 1]->setSpeed((float(rand()) / float((RAND_MAX)) * 2.0f + 1.0f));
                }

                if (abilities.size() != 0)
                {
                    for (auto& ability : abilities)
                    {
                        if (ability->isAlive())
                            ability->drawAbility(dt);
                    }
                }
            }
        }
        else
        {
            bg->drawBg();
            drawSprite(crosshair, mousePos[0], mousePos[1]);
            interval = getTickCount() / 1000 + (rand() % 30) + 10;
        }

        return false;
    }

    virtual void onMouseMove(int x, int y, int xrelative, int yrelative)
    {
        mousePos[0] = (float)x;
        mousePos[1] = (float)y;
    }

    virtual void onMouseButtonClick(FRMouseButton button, bool isReleased)
    {
        if (gameStarted == false && button == FRMouseButton::LEFT && isReleased == true && (mousePos[0] >= 0 && mousePos[0] <= window.width &&
            mousePos[1] >= 0 && mousePos[1] <= window.height))
        {
            int bgID = rand() % 3;
            if (bgID == 0)
                bg->setBg(Bg1);
            else if (bgID == 1)
                bg->setBg(Bg3);
            else bg->setBg(Bg2);

            gameStarted = true;
        }
        else
        {
            if (button == FRMouseButton::LEFT && isReleased == true && mousePos[1] < window.height - 200 && shoot == false)
            {
                shoot = true;

                std::vector<float> targets(2);
                std::vector<float> velocity(2);

                targets = mousePos;

                targets[0] -= ball->getPos()[0];
                targets[1] -= ball->getPos()[1];

                velocity[0] = targets[0] / (float)sqrt(pow(targets[0], 2) + pow(targets[1], 2));
                velocity[1] = targets[1] / (float)sqrt(pow(targets[0], 2) + pow(targets[1], 2));

                ball->setVelocity(velocity);
                ball->setSpeed(5.0f);
            }
            if (restartReq == true && button == FRMouseButton::LEFT && isReleased == true && (mousePos[0] >= 0 && mousePos[0] <= window.width &&
                mousePos[1] >= 0 && mousePos[1] <= window.height))
            {
                restartGame();
                restartReq = false;
            }
            if (gameWon == true && button == FRMouseButton::LEFT && isReleased == true && (mousePos[0] >= 0 && mousePos[0] <= window.width &&
                mousePos[1] >= 0 && mousePos[1] <= window.height))
            {
                restartGame();
                gameWon = false;
            }
        }
    }

    virtual void onKeyPressed(FRKey k)
    {
        if (FRKey::RIGHT == k)
            player->setVelocity(3.0f);
        if (FRKey::LEFT == k)
            player->setVelocity(-3.0f);
    }

    virtual void onKeyReleased(FRKey k)
    {
        if (FRKey::RIGHT == k || FRKey::LEFT == k)
            player->setVelocity(0.0f);
    }

    virtual const char* GetTitle() override
    {
        return "Arkanoid";
    }
};







int main(int argc, char* argv[])
{
    srand((unsigned int)time(NULL));
    std::string input;
    if (argc == 1)
    {
        window.width = 800;
        window.height = 600;
    }
    else
    {
        input = argv[1];
        if (input.find("-window"))
        {
            size_t start = input.find("-window") + 7;
            size_t end = input.find("x") - 1;
            window.width = stoi(input.substr(start, end));


            start = input.find("x") + 1;
            end = std::string::npos;
            window.height = stoi(input.substr(start, end));

        }
        else
        {
            window.width = 800;
            window.height = 600;
        }
    }
    if (window.width < 800)
        window.width = 800;
    if (window.height < 600)
        window.height = 600;


    if (window.width > 800) windowRation();
    if (window.height > 800) ball->setSpeed(ball->getSpeed() + (0.6f * ((window.height - 800.0f) / 100.0f)));

    MyFramework* myFramework = new MyFramework;

    return run(myFramework);
}






void outOfBounds()
{

    if (ball->getPos()[1] + 7.5f > player->getPositions()[1] + player->getHeight() && savingWallOn == true)
    {
        ball->setSingularVelocity(1, (float)-sin(45));
        savingWallOn = false;
    }
    else if (ball->getPos()[1] + 7.5f > player->getPositions()[1] + player->getHeight())
    {
        shoot = false;

        player->setSingularPosition(0, (float)window.width / 2.0f - 35.0f);
        player->setSingularPosition(1, (float)window.height - 100.0f);
        player->setLives(player->getLives() - 1);
        player->setVelocity(0.0f);
        player->setSpeed(0.0f);

        ball->setSpeed(0.0f);
        ball->setSingularPosition(0, player->getPositions()[0] + 30.0f);
        ball->setSingularPosition(1, player->getPositions()[1] - 17.0f);

        hits = 0;
    }
    if (abilities.size() != 0)
    {
        for (auto& ability : abilities)
        {
            if (ability->getPos()[1] >= window.height)
            {
                ability->setLives(0);
            }
        }
    }
}

void drawBoards()
{

    for (int i = 0; i < board.size(); i++)
    {
        if (board[i]->isAlive())
            board[i]->drawBoard(lastPositions[0], lastPositions[1]);

        if (board[i]->getPos(0) + board[i]->getWidth() >= window.width - 40)
        {
            lastPositions[1] += board[i]->getHeight() * 2;
            lastPositions[0] = 20;
        }
        else
            lastPositions[0] += board[i]->getWidth() + 20;
    }
}

void windowRation()
{
    int ratio = 0;
    {
        ratio = (int)(window.width - 800) / 60;
        board.resize(65 + ((ratio + 1) * 5));
    }
}

void generateRandomBoards()
{
    nrOfBombBoards = rand() % 5;
    nrOfWeakBoards = rand() % 8;
    nrOfHardBoards = rand() % 20;
    totalRandomBoards = nrOfBombBoards + nrOfWeakBoards + nrOfHardBoards;

    int randomNr;

    for (int i = 0; i < totalRandomBoards; i++)
    {
        do
        {
            randomNr = rand() % (board.size() - (window.width / 60) - 1);
        } while (alreadyExisting(randomNr));
        randomBoards.push_back(randomNr);
    }
}

bool alreadyExisting(int nr)
{
    for (int i = 0; i < randomBoards.size(); i++)
    {
        if (nr == randomBoards[i])
        {
            return true;
        }
    }
    return false;
}
void boardCollision()
{
    for (int i = 0; i < board.size(); i++)
    {
        if (hitBoard(*board[i]) == true)
        {
            if (board[i]->isAlive())
            {
                board[i]->setLives(board[i]->getLives() - 1);
                hits++;
                hitBomb(i);

                for (auto& b : board)
                {
                    if (b->getType() == 1 && b->getLives() == 2)
                        b->setTexture(hard_board_semi_broken);
                    if (b->getType() == 1 && b->getLives() == 1)
                        b->setTexture(hard_board_broken);
                }

                if (ball->getPos()[0] + 7.5f >= board[i]->getPos(0) - 3.0f && ball->getPos()[0] + 7.5f <= board[i]->getPos(2) + 3.0f && ball->getPos()[1] + 7.5f >= board[i]->getPos(1) + board[i]->getHeight() / 2.0f)
                {
                    if (ball->getPos()[0] + 7.5f >= board[i]->getPos(0) - 3.0f && ball->getPos()[0] + 7.5f <= board[i]->getPos(0) + board[i]->getWidth() / 2.0f)
                    {
                        ball->setSingularVelocity(0, (float)-cos(45));
                        ball->setSingularVelocity(1, (float)sin(45));
                    }
                    else if (ball->getPos()[0] + 7.5f >= board[i]->getPos(0) + board[i]->getWidth() / 2.0f && ball->getPos()[0] + 7.5f <= board[i]->getPos(2) + 3.0f)
                    {
                        ball->setSingularVelocity(0, (float)cos(45));
                        ball->setSingularVelocity(1, (float)sin(45));
                    }
                }
                else if (ball->getPos()[0] + 7.5f >= board[i]->getPos(0) - 3.0f && ball->getPos()[0] + 7.5f <= board[i]->getPos(2) + 3.0f && ball->getPos()[1] + 7.5f < board[i]->getPos(1) + board[i]->getHeight() / 2.0f)
                {
                    if (ball->getPos()[0] + 7.5f >= board[i]->getPos(0) - 3.0f && ball->getPos()[0] + 7.5f <= board[i]->getPos(0) + board[i]->getWidth() / 2.0f)
                    {
                        ball->setSingularVelocity(0, (float)-cos(45));
                        ball->setSingularVelocity(1, (float)-sin(45));
                    }
                    else if (ball->getPos()[0] + 7.5f >= board[i]->getPos(0) + board[i]->getWidth() / 2.0f && ball->getPos()[0] + 7.5f <= board[i]->getPos(2) + 3.0f)
                    {
                        ball->setSingularVelocity(0, (float)cos(45));
                        ball->setSingularVelocity(1, (float)-sin(45));
                    }
                }
                else if (ball->getPos()[0] + 7.5f >= board[i]->getPos(2) - 3.0f && ball->getPos()[1] + 7.5f >= board[i]->getPos(1) - 3.0f && ball->getPos()[1] + 7.5f <= board[i]->getPos(3) + 3.0f)
                    ball->setSingularVelocity(0, (float)cos(45));
                else if (ball->getPos()[0] + 7.5f <= board[i]->getPos(0) + 3.0f && ball->getPos()[1] + 7.5f >= board[i]->getPos(1) - 3.0f && ball->getPos()[1] + 7.5f <= board[i]->getPos(3) + 3.0f)
                    ball->setSingularVelocity(0, (float)-cos(45));

                score += 250;
                if (hits >= 3)
                    score += 3 * 250;
            }
        }
    }
}
void padHit()
{
    std::vector<float> a(2), b(2), c(2), d(2), pt(2);


    a[0] = player->getPositions()[0]; a[1] = player->getPositions()[1];
    b = a; b[0] += player->getWidth();
    c = b; b[1] += player->getHeight();
    d = a; d[1] += player->getHeight();

    pt = ball->getPos();
    pt[0] += 10;
    pt[1] += 25;

    bool ab = getSide(a, b, pt);
    bool bc = getSide(b, c, pt);
    bool cd = getSide(c, d, pt);
    bool da = getSide(d, a, pt);


    if (ab == bc && bc == cd && cd == da)
    {
        hits = 0;
        if (remainingBoards() == false)
            gameWon = true;

        if (ball->getPos()[0] + 7.5f >= player->getPositions()[0] && ball->getPos()[0] + 7.5f <= player->getPositions()[0] + player->getWidth() / 4.0f)
            ball->setSingularVelocity(0, (float)-cos(45));
        if (ball->getPos()[0] + 7.5f >= player->getPositions()[0] + player->getWidth() / 4.0f && ball->getPos()[0] + 7.5f <= player->getPositions()[0] + (3 * player->getWidth() / 4.0f))
            ball->setSingularVelocity(1, -ball->getVelocity()[1]);
        if (ball->getPos()[0] + 7.5f > player->getPositions()[0] + (3 * player->getWidth() / 4.0f) && ball->getPos()[0] + 7.5f <= player->getPositions()[0] + player->getWidth())
            ball->setSingularVelocity(0, (float)cos(45));

        ball->setSingularVelocity(1, (float)-sin(45));
    }

    if (abilities.size() != 0)
    {
        for (auto& ability : abilities)
        {
            if (ability->isAlive())
            {
                a[0] = player->getPositions()[0]; a[1] = player->getPositions()[1];
                b = a; b[0] += player->getWidth();
                c = b; b[1] += player->getHeight();
                d = a; d[1] += player->getHeight();

                pt = ability->getPos();
                pt[0] += 10;
                pt[1] += 25;



                bool ab = getSide(a, b, pt);
                bool bc = getSide(b, c, pt);
                bool cd = getSide(c, d, pt);
                bool da = getSide(d, a, pt);


                if (ab == bc && bc == cd && cd == da)
                {
                    ability->setLives(0);
                    if (ability->getType() == 2)
                        player->setLives(player->getLives() - 1);
                    else savingWallOn = true;
                }
            }
        }
    }
}

void restartGame()
{
    size_t size = board.size();
    for (int i = 0; i < board.size(); i++)
        delete board[i];
    randomBoards.clear();
    abilities.clear();
    scoreTextures.clear();
    counter = 0;

    int bgID = rand() % 3;
    if (bgID == 0)
        bg->setBg(Bg1);
    else if (bgID == 1)
        bg->setBg(Bg3);
    else bg->setBg(Bg2);

    generateRandomBoards();

    for (int i = 0; i < size; i++)
    {
        board[i] = new Boards(soft_board_broken, 40, 20);
        board[i]->setLives(1);
        board[i]->setType(2);
        for (int j = 0; j < totalRandomBoards; j++)
        {
            if (i == randomBoards[j] && (counter >= 0 && (counter < nrOfWeakBoards)))
            {
                board[i] = new Boards(weak_board, 40, 20);
                counter++;
                board[i]->setLives(1);
                board[i]->setType(2);
                break;
            }
            if (i == randomBoards[j] && (counter >= nrOfWeakBoards) && (counter < (nrOfWeakBoards + nrOfBombBoards)))
            {
                board[i] = new Boards(bomb_board, 40, 20);
                counter++;
                board[i]->setLives(1);
                board[i]->setType(3);
                break;
            }
            if (i == randomBoards[j] && (counter >= (nrOfWeakBoards + nrOfBombBoards) && counter < (nrOfWeakBoards + nrOfBombBoards + nrOfHardBoards)))
            {
                board[i] = new Boards(hard_board, 40, 20);
                board[i]->setLives(3);
                board[i]->setType(1);
                counter++;
                break;
            }
        }
    }

    player->setLives(3);
    interval = (int)(getTickCount() / 1000 + (rand() % 30 + 10));

    player->setSingularPosition(0, (float)window.width / 2.0f - 35.0f);
    player->setSingularPosition(1, (float)window.height - 100.0f);

    ball->setSingularPosition(0, player->getPositions()[0] + 30.0f);
    ball->setSingularPosition(1, player->getPositions()[1] - 17.0f);
    ball->setSpeed(0.0f);

    hits = 0;
    score = 0;
    isGameOver = false;
    shoot = false;
    savingWallOn = false;
}

void hitBomb(int i)
{
    if (board[i]->getType() == 3)
    {
        drawSprite(explosion, board[i]->getPos(0) + (float)board[i]->getWidth() / 3.0f, board[i]->getPos(1) + (float)board[i]->getHeight() / 2.0f);
        if (i > 0)
        {
            if (board[i - 1]->getPos(1) == board[i]->getPos(1) && board[i - 1]->isAlive())
            {
                board[i - 1]->setLives(board[i - 1]->getLives() - 1);
                hits++;
                if (hits >= 3)
                    score += 3 * 250;
                else score += 250;
                if (board[i - 1]->getType() == 3) hitBomb(i - 1);
            }
        }
        if (board[i + 1]->getPos(1) == board[i]->getPos(1) && board[i + 1]->isAlive())
        {
            board[i + 1]->setLives(board[i + 1]->getLives() - 1);
            hits++;
            if (hits >= 3)
                score += 3 * 250;
            else score += 250;
            if (board[i + 1]->getType() == 3) hitBomb(i + 1);
        }
        for (int j = 0; j < board.size(); j++)
        {
            if (board[j]->isAlive())
            {
                if (board[j]->getPos(0) == board[i]->getPos(0) && board[j]->getPos(1) == board[i]->getPos(1) - board[i]->getHeight() - 20)
                {
                    board[j]->setLives(board[j]->getLives() - 1);
                    hits++;
                    if (hits >= 3)
                        score += 3 * 250;
                    else score += 250;
                    if (board[j]->getType() == 3) hitBomb(j);
                }
                else if (board[j]->getPos(0) == board[i]->getPos(0) && board[j]->getPos(1) == board[i]->getPos(3) + 20)
                {
                    board[j]->setLives(board[j]->getLives() - 1);
                    hits++;
                    if (hits >= 3)
                        score += 3 * 250;
                    else score += 250;
                    if (board[j]->getType() == 3) hitBomb(j);
                }
                else if (board[j]->getPos(0) == board[i]->getPos(0) - board[i]->getWidth() - 20 && board[j]->getPos(1) == board[i]->getPos(1) - board[i]->getHeight() - 20)
                {
                    board[j]->setLives(board[j]->getLives() - 1);
                    hits++;
                    if (hits >= 3)
                        score += 3 * 250;
                    else score += 250;
                    if (board[j]->getType() == 3) hitBomb(j);
                }
                else if (board[j]->getPos(0) == board[i]->getPos(2) + 20 && board[j]->getPos(1) == board[i]->getPos(1) - board[i]->getHeight() - 20)
                {
                    board[j]->setLives(board[j]->getLives() - 1);
                    hits++;
                    if (hits >= 3)
                        score += 3 * 250;
                    else score += 250;
                    if (board[j]->getType() == 3) hitBomb(j);
                }
                else if (board[j]->getPos(0) == board[i]->getPos(2) + 20 && board[j]->getPos(1) == board[i]->getPos(3) + 20)
                {
                    board[j]->setLives(board[j]->getLives() - 1);
                    hits++;
                    if (hits >= 3)
                        score += 3 * 250;
                    else score += 250;
                    if (board[j]->getType() == 3) hitBomb(j);
                }
                else if (board[j]->getPos(0) == board[i]->getPos(0) - board[i]->getWidth() - 20 && board[j]->getPos(1) == board[i]->getPos(3) + 20)
                {
                    board[j]->setLives(board[j]->getLives() - 1);
                    hits++;
                    if (hits >= 3)
                        score += 3 * 250;
                    else score += 250;
                    if (board[j]->getType() == 3) hitBomb(j);
                }
            }
        }
    }
}

void handleScore()
{


    while (score > 0)
    {
        int digit = score % 10;

        switch (digit)
        {
        case 0:
            scoreTextures.push_back(new ScoreTex(Digit0, 20, 20));
            break;
        case 1:
            scoreTextures.push_back(new ScoreTex(Digit1, 20, 20));
            break;
        case 2:
            scoreTextures.push_back(new ScoreTex(Digit2, 20, 20));
            break;
        case 3:
            scoreTextures.push_back(new ScoreTex(Digit3, 20, 20));
            break;
        case 4:
            scoreTextures.push_back(new ScoreTex(Digit4, 20, 20));
            break;
        case 5:
            scoreTextures.push_back(new ScoreTex(Digit5, 20, 20));
            break;
        case 6:
            scoreTextures.push_back(new ScoreTex(Digit6, 20, 20));
            break;
        case 7:
            scoreTextures.push_back(new ScoreTex(Digit7, 20, 20));
            break;
        case 8:
            scoreTextures.push_back(new ScoreTex(Digit8, 20, 20));
            break;
        case 9:
            scoreTextures.push_back(new ScoreTex(Digit9, 20, 20));
            break;
        default:
            break;
        }
        score /= 10;
    }
    int k = 0;
    for (int i = scoreTextures.size() - 1; i >= 0; i--)
    {
        scoreTextures[i]->drawScore(window.width - (85 - (k * 10)), window.height - 100);
        k++;
    }
    drawSprite(scoreStringTexture, window.width - (140 - (k * 10)), window.height - 170);
}

// 0 - x
// 1 - y
// 2 - x + width
// 3 - y + height
bool hitBoard(Boards& board)
{
    float boardX = board.getPos(0);
    float boardY = board.getPos(1);
    float boardXW = board.getPos(2);
    float boardYH = board.getPos(3);
    if (((ball->getPos()[0] + 7.5f > boardX - 3 && ball->getPos()[0] + 7.5f < boardXW + 3)) &&
        ((ball->getPos()[1] + 7.5f < boardYH + 3 && ball->getPos()[1] + 7.5f > boardY - 3)))
        return true;
    return false;
}

bool remainingBoards()
{
    for (auto& b : board)
    {
        if (b->isAlive())
            return true;
    }
    return false;
}