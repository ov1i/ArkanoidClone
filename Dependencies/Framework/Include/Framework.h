#pragma once

#if defined(_WINDOWS)
    #if defined(FRAMEWORK_PROJECT)
        #define FRAMEWORK_API extern "C" __declspec(dllexport)
    #else
        #define FRAMEWORK_API extern "C" __declspec(dllimport)
    #endif
#else
    #define FRAMEWORK_API 
#endif


class Sprite;

FRAMEWORK_API Sprite* createSprite(const char* path);
FRAMEWORK_API void drawSprite(Sprite*, int x, int y);
FRAMEWORK_API void getSpriteSize(Sprite* s, int& w, int &h);
FRAMEWORK_API void setSpriteSize(Sprite* s, int w, int h);
FRAMEWORK_API void destroySprite(Sprite* s);

FRAMEWORK_API void drawTestBackground();
FRAMEWORK_API void getScreenSize(int& w, int &h);

FRAMEWORK_API unsigned int getTickCount();

FRAMEWORK_API void showCursor(bool bShow);

enum class FRKey {
	RIGHT,
	LEFT,
	DOWN,
	UP,
	ENTER,
	COUNT
};

enum class FRMouseButton {
	LEFT,
	MIDDLE,
	RIGHT,
	COUNT
};

class Framework {
public:

	virtual void PreInit(int& width, int& height, bool& fullscreen) = 0;
	virtual bool Init() = 0;

	virtual void Close() = 0;


	virtual bool Tick() = 0;

	virtual void onMouseMove(int x, int y, int xrelative, int yrelative) = 0;
	virtual void onMouseButtonClick(FRMouseButton button, bool isReleased) = 0;

	virtual void onKeyPressed(FRKey k) = 0;
	virtual void onKeyReleased(FRKey k) = 0;

	virtual const char* GetTitle() = 0;

	virtual ~Framework() {};
};


FRAMEWORK_API int run(Framework*);
