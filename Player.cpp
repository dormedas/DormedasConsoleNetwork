#include "Player.h"

/*
	Constructors
*/
Player::Player(bool netPlayer, char playerChar)
{
	mScreen = GetStdHandle(STD_OUTPUT_HANDLE);

	mNetworkPlayer = netPlayer;

	// Make a really big random number. REALLY low chances of the same one being repeated.
	// This is in lieu of a formal uuid system.
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
/*
	Set
*/
void Player::setPosition(int newX, int newY)
{
	mPos.X = newX;
	mPos.Y = newY;
	return;
}

/*
	Position based methods
*/

void Player::clearPosition()
{
	SetConsoleCursorPosition(mScreen, mPos);
	std::cout << " ";
}

int Player::update()
{
	SetConsoleCursorPosition(mScreen, mPos);
	std::cout << " ";

	bool moved = false;

	// network players do nothing

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