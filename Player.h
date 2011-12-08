#pragma once

#include <Windows.h>
#include <time.h>
#include <iostream>


// This struct can be turned into a char array to be sent over 
// a UDP socket, so when we're about to send data, we 
// need to first mirror it in the struct.
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
	// Constructors.
	Player(bool netPlayer, char playerChar);
	PlayerData & getPlayerData() { mPlayerData.x = mPos.X; mPlayerData.y = mPos.Y; return mPlayerData; }

	// Gets
	long int getID() { return mPlayerData.id; }

	// Sets
	void setID(long int newID) { mPlayerData.id = newID; }
	void setPosition(int newX, int newY);

	// Position based methods.
	void clearPosition();
	int update();
};