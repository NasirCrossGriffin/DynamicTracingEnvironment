#include <list>
#include <iostream>
#include <limits.h>
#include <cstdlib>
#include <iostream>
#include <boost/asio.hpp>
#include "server.h"
#include <fstream>
#include <thread>
#include <string>
#include "client.cpp"
#include <boost/dll/runtime_symbol_info.hpp>
#include <boost/filesystem.hpp>
#include <unistd.h>
using namespace std;

	int main(int argc, char* argv[])
	{
		/*Gather the list and sort it
		Written by Nasir Cross-Griffin*/

		list<Client*> clients;

		int i = 0;
		while (i < 10)
		{
			clients.push_back(new Client("Matthew", 1000 + (rand() % 8999), 20 + (rand() % 40), "MALE"));
			clients.push_back(new Client("Sarah", 1000 + (rand() % 8999), 20 + (rand() % 40), "FEMALE"));
			clients.push_back(new Client("Jason", 1000 + (rand() % 8999), 20 + (rand() % 40), "MALE"));
			clients.push_back(new Client("Seth", 1000 + (rand() % 8999), 20 + (rand() % 40), "MALE"));
			clients.push_back(new Client("Lisa", 1000 + (rand() % 8999), 20 + (rand() % 40), "FEMALE"));
			clients.push_back(new Client("Trina", 1000 + (rand() % 8999), 20 + (rand() % 40), "FEMALE"));
			clients.push_back(new Client("Maxwell", 1000 + (rand() % 8999), 20 + (rand() % 40), "MALE"));
			clients.push_back(new Client("Roxanne", 1000 + (rand() % 8999), 20 + (rand() % 40), "FEMALE"));
			clients.push_back(new Client("Timothy", 1000 + (rand() % 8999), 20 + (rand() % 40), "MALE"));
			clients.push_back(new Client("Reece", 1000 + (rand() % 8999), 20 + (rand() % 40), "MALE"));
			
			i++;
		}



		//sort(clients.begin(), clients.end(), Client::compareClients);

		//Create a new file
		try {
			ofstream NewFile("/Users/nas/Documents/ListOfClients.txt");
			std::cout << "File created\n";
			NewFile.close();
		}
		catch (exception& e) {
			std::cout << "Cannot create file";
		}

		

		ofstream MyFile("/Users/nas/Documents/ListOfClients.txt");

		//Print out all members of the list (Will be changed to append to file)
		list<Client*>::iterator it;

		for (it = clients.begin(); it != clients.end(); it++)
			MyFile << (**it).toString() << "\n";

		MyFile.close();
		/*for (int i; i < NewList.size(); i++)
		{
			cout << it.next();
		} */

		Server();

		return 0;


	}
