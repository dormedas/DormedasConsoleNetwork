#include <iostream>
#include <boost/asio.hpp>
#include "Player.h"
#include <vector>

using boost::asio::ip::udp;

enum { max_length = 1024 };

void server(boost::asio::io_service& io_service, short port)
{
	std::vector<boost::asio::ip::udp::endpoint> clientsVec;
	std::vector<long int> clientsData;

	udp::socket sock(io_service, udp::endpoint(udp::v4(), port));

	system("cls");
	std::cout << "Server successfully bound on port " << port << ". Awaiting packets." << std::endl;

	while(true)
	{
		char data[max_length];
		udp::endpoint sender_endpoint;
		size_t length = sock.receive_from(boost::asio::buffer(data,max_length), sender_endpoint);
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
		bool found = false;
		for(int i = 0; i < clientsVec.size(); i++)
		{
			if(clientsVec[i].address() == sender_endpoint.address())
			{
				found = true;
			}
		}
		PlayerData* temp = reinterpret_cast<PlayerData*>(data);
		if(!found)
		{
			std::cout << "Server: New Player " << sender_endpoint.address() << " ||" << std::endl;
			clientsVec.push_back(sender_endpoint);
			clientsData.push_back(temp->id);
		}
		std::cout << "Server: Player " << sender_endpoint.address() << "[" << temp->playerChar << "] is at (" << temp->x << ", " << temp->y << ")" << std::endl;
		for(int i = 0; i < clientsVec.size(); i++)
		{
			if(clientsVec[i] != sender_endpoint)
				sock.send_to(boost::asio::buffer(data,length), clientsVec[i]);
		}
		//delete temp;
	}
}

int main(int argc, char* argv[])
{
	try
	{
		char port[16];
		{
			using namespace std;
			cout << "Enter port: ";
			cin >> port;
		}

		boost::asio::io_service io_service;

		using namespace std;
		server(io_service, atoi(port));
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
	}
}