#include <iostream>
#include <vector>
#include <boost/asio.hpp>
#include "Player.h"

using boost::asio::ip::udp;

enum { max_length = 1024 };

void server(boost::asio::io_service& io_service, short port)
{
	// Create a vector with client endpoint data, i.e., their IP addresses.
	std::vector<boost::asio::ip::udp::endpoint> clientsVec;

	// And a vector with the client's 'unique' IDs
	std::vector<long int> clientsData;

	// make a new UDP socket.
	udp::socket sock(io_service, udp::endpoint(udp::v4(), port));

	system("cls");
	// If this succeeds, the catch block in main won't happen, therefore we were successful.
	std::cout << "Server successfully bound on port " << port << ". Awaiting packets." << std::endl;

	while(true)
	{
		// a char to hold the data.
		char data[max_length];
		udp::endpoint sender_endpoint;

		// Receive data from somewhere into the data char.
		size_t length = sock.receive_from(boost::asio::buffer(data,max_length), sender_endpoint);

		// If the length was 5, then it's a special quit packet.
		// There's definitely a more robust way to do this. I just sort've hacked it in for lack of time.
		if(length == 5)
		{
			char* temp = reinterpret_cast<char*>(data);
			if ( temp[0] == 'q' && temp[1] == 'u' )
			{
				for(int i = 0; i < clientsVec.size(); i++)
				{
					if(clientsVec[i].address() == sender_endpoint.address())
					{
						std::cout << "Server: Player " << sender_endpoint.address() << " has disconnected!" << std::endl;
						clientsVec.erase(clientsVec.begin() + i);
						clientsData.erase(clientsData.begin() + i);
					}
				}
				//delete temp; // Here
				continue; 
			}
			//delete temp; // Or here
		}
		// Check to see if this is a client we know.
		bool found = false;
		for(int i = 0; i < clientsVec.size(); i++)
		{
			if(clientsVec[i].address() == sender_endpoint.address())
			{
				found = true;
			}
		}
		// Turn the data into PlayerData*
		PlayerData* temp = reinterpret_cast<PlayerData*>(data);
		if(!found)
		{
			// If we didn't find it, log the new player, put them in the client IP vector, and store their ID.
			std::cout << "Server: New Player " << sender_endpoint.address() << " ||" << std::endl;
			clientsVec.push_back(sender_endpoint);
			clientsData.push_back(temp->id);
		}
		// Write a log to the server indicating where the player is.
		std::cout << "Server: Player " << sender_endpoint.address() << "[" << temp->playerChar << "] is at (" << temp->x << ", " << temp->y << ")" << std::endl;

		// Now send that original data char back out to every client except for the one it came from!
		for(int i = 0; i < clientsVec.size(); i++)
		{
			if(clientsVec[i] != sender_endpoint)
				sock.send_to(boost::asio::buffer(data,length), clientsVec[i]);
		}
	}
}

int main(int argc, char* argv[])
{
	try
	{
		// Just need to bind a port.
		char port[16];
		{ // std scope
			using namespace std;
			cout << "Enter port: ";
			cin >> port;
		}

		boost::asio::io_service io_service;

		using namespace std; // std for atoi.
		server(io_service, atoi(port));
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
	}
}