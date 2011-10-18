#include <iostream>
#include <boost/asio.hpp>
#include <Windows.h>
#include <vector>
#include "Player.h"

using boost::asio::ip::udp;

enum { max_length = 1024 };

int main(int argc, char* argv[])
{
	system("cls");

	char ip[16];
	char port[16];
	char playerChar;

	{ // std scope
		using namespace std;
		cout << "Enter IP Address: ";
		cin >> ip;
		cout << "Enter Port: ";
		cin >> port;
		cout << "Enter a keyboard character to represent you: ";
		cin >> playerChar;
	}

	system("cls");

	boost::asio::io_service io_service;

	udp::socket s(io_service, udp::endpoint(udp::v4(), 0));

	udp::resolver resolver(io_service);
	udp::resolver::query query(udp::v4(), ip, port);
	udp::resolver::iterator iterator = resolver.resolve(query);

	using namespace std;

	Player localPlayer = Player(false, playerChar);

	vector<Player> otherPlayers;

	boost::system::error_code ec;
	while(1)
	{
		if(GetAsyncKeyState(VK_ESCAPE) != 0) { break; }

		if(localPlayer.update() == 1)
		{
			PlayerData locData = localPlayer.getPlayerData();
			size_t request_length = sizeof(locData);
			size_t sent = s.send_to(boost::asio::buffer((char*)&localPlayer.getPlayerData(), request_length), *iterator, NULL, ec);
			if(sent < 0)
			{
				//cout << "No bytes sent." << endl;
			}
		}
		char reply[max_length];
		udp::endpoint sender_endpoint;
		boost::system::error_code ec;
		s.non_blocking(true);
		size_t reply_length = s.receive_from(boost::asio::buffer(reply, max_length), sender_endpoint, 0, ec);
		if(reply_length <= 0)
		{
			//cout << "No bytes received" << endl;
		}
		else if (reply_length == 5)
		{
		}
		else
		{
			bool found = false;
			PlayerData* temp = reinterpret_cast<PlayerData*>(reply);
			for(int i = 0; i < otherPlayers.size(); i++)
			{
				if(otherPlayers[i].getID() == temp->id)
				{
					otherPlayers[i].clearPosition();
					otherPlayers[i].setPosition(temp->x, temp->y);
					otherPlayers[i].update();
					found = true;
					break;
				}
			}
			if(!found)
			{
				Player otherPlayer = Player(true, temp->playerChar);
				otherPlayer.setID(temp->id);
				otherPlayer.setPosition(temp->x, temp->y);
				otherPlayer.update();
				otherPlayers.push_back(otherPlayer);
			}
		}
		Sleep(100);
	}
	char quit[5] = "quit";
	size_t request_length = sizeof(quit);
	size_t sent = s.send_to(boost::asio::buffer((char*)quit, request_length), *iterator, NULL, ec);
	if(sent < 0)
	{
		//cout << "No bytes sent." << endl;
	}

	system("cls");
}