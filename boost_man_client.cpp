#include <iostream>
#include <vector>
#include <Windows.h>
#include <boost/asio.hpp>
#include "Player.h"

using boost::asio::ip::udp;

enum { max_length = 1024 };

int main(int argc, char* argv[])
{
	system("cls");

	// Holds data for network connection and player display character.
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

	// Ew system commands.
	system("cls");

	// Create an io_service.
	boost::asio::io_service io_service;

	// Create a udp v4 socket on the io_service, "s"
	udp::socket s(io_service, udp::endpoint(udp::v4(), 0));

	// Resolver resolves a query to endpoints.
	udp::resolver resolver(io_service);

	// Set the query type to udp, and try to query the ip and the port as entered above.
	udp::resolver::query query(udp::v4(), ip, port);
	udp::resolver::iterator iterator = resolver.resolve(query); // Do a resolution.

	using namespace std;

	// Creates a new player.
	Player localPlayer = Player(false, playerChar);

	// A vector for all other players.
	vector<Player> otherPlayers;

	boost::system::error_code ec;
	while(1)
	{
		// If we pressed escape, break loop and close program.
		if(GetAsyncKeyState(VK_ESCAPE) != 0) { break; }

		// If we have actually moved (and have something to actually send to the server)
		if(localPlayer.update() == 1)
		{
			// In order to send a datagram, we need its size and its data.
			// This could be rewritten way better.
			PlayerData locData = localPlayer.getPlayerData();
			size_t request_length = sizeof(locData);
			// Send to (a new buffer) with a char-cast version of the struct data, and its size, to the specified iterator. 'ec' is the error code.
			size_t sent = s.send_to(boost::asio::buffer((char*)&localPlayer.getPlayerData(), request_length), *iterator, NULL, ec);
			if(sent < 0)
			{
				// Debug text gets in way of screen update. Enable if needed.
				//cout << "No bytes sent." << endl;
			}
		}
		// Every tick we also take in any data waiting for us. Right now it will only take in one thing and then move on
		// This would be a cool thing to 'fix'

		// reply will hold our char version of the struct data. Endpoint is defined whenever it receives data.
		char reply[max_length];
		udp::endpoint sender_endpoint;
		boost::system::error_code ec;

		// Tells the socket not to block if there's no data waiting to be read in.
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
			// If we have data, reinterpret_cast the char array into a proper PlayerData*
			bool found = false;
			PlayerData* temp = reinterpret_cast<PlayerData*>(reply);

			// Check all other players. If its id matches the one received, then set its position as per the new data.
			for(int i = 0; i < otherPlayers.size(); i++)
			{
				if(otherPlayers[i].getID() == temp->id)
				{
					otherPlayers[i].clearPosition(); // "undraw"
					otherPlayers[i].setPosition(temp->x, temp->y); // Set it's position on screen.
					otherPlayers[i].update(); // "draw"
					found = true;
					break; // No need to do extra stuff.
				}
			}

			// If we didn't find a player in our vector, then we have a new one!
			// Create a new player and push it to the vector.
			if(!found)
			{
				Player otherPlayer = Player(true, temp->playerChar);
				otherPlayer.setID(temp->id);
				otherPlayer.setPosition(temp->x, temp->y);
				otherPlayer.update();
				otherPlayers.push_back(otherPlayer);
			}
		}
		// Sleep so we can actually tell what's happening.
		Sleep(100);
	}
	// Sends a special disconnect packet when someone presses escape.
	char quit[5] = "quit";
	size_t request_length = sizeof(quit);
	size_t sent = s.send_to(boost::asio::buffer((char*)quit, request_length), *iterator, NULL, ec);
	if(sent < 0)
	{
		//cout << "No bytes sent." << endl;
	}

	system("cls");
}