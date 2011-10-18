#include "Player.h"

Player::Player(bool netPlayer, char playerChar)
{
	mScreen = GetStdHandle(STD_OUTPUT_HANDLE);

	mNetworkPlayer = netPlayer;
	if(!netPlayer)
	{
		srand(time(NULL));
		mPlayerData.id = rand() % 2000000;
	}

	mPlayerData.playerChar = playerChar;
	mPlayerChar = playerChar;

	mPos.X = 1;
	mPos.Y = 1;
	mPlayerData.x = 1;
	mPlayerData.y = 1;
}

void Player::clearPosition()
{
	SetConsoleCursorPosition(mScreen, mPos);
	std::cout << " ";
}

void Player::setPosition(int newX, int newY)
{
	mPos.X = newX;
	mPos.Y = newY;
	return;
}

int Player::update()
{
	SetConsoleCursorPosition(mScreen, mPos);
	std::cout << " ";

	bool moved = false;

	if(!mNetworkPlayer)
	{
		if(GetAsyncKeyState(VK_LEFT) != 0) 
		{ 
			if(mPos.X > 0)
				mPos.X--; 
			moved = true; 
		}
		if(GetAsyncKeyState(VK_RIGHT) != 0) 
		{ 
			if(mPos.X < 80)
				mPos.X++; 
			moved = true; 
		}
		if(GetAsyncKeyState(VK_UP) != 0)
		{ 
			if(mPos.Y > 0)
				mPos.Y--; 
			moved = true; 
		}
		if(GetAsyncKeyState(VK_DOWN) != 0) 
		{ 
			if(mPos.Y < 25)
				mPos.Y++; 
			moved = true; 
		}
	}

	SetConsoleCursorPosition(mScreen, mPos);
	
	std::cout << mPlayerChar;

	if (moved)
		return 1;
	else
		return 0;
}