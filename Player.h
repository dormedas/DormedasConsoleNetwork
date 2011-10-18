#pragma once

#include <Windows.h>
#include <time.h>
#include <iostream>

struct PlayerData
{
	char playerChar;
	long int id;
	int x, y;
};

class Player
{
private:
	HANDLE mScreen;
	COORD mPos;
	bool mNetworkPlayer;
	PlayerData mPlayerData;
	char mPlayerChar;
public:
	Player(bool netPlayer, char playerChar);
	PlayerData & getPlayerData() { mPlayerData.x = mPos.X; mPlayerData.y = mPos.Y; return mPlayerData; }
	void clearPosition();
	long int getID() { return mPlayerData.id; }
	void setID(long int newID) { mPlayerData.id = newID; }
	void setPosition(int newX, int newY);
	int update();
};