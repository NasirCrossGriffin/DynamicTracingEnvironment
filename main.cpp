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
#include <pthread.h>
using namespace std;

list<Client*> clients;

	void* createTheNewFile(void* arg)
	{
		pthread_detach(pthread_self());
		//Create a new file.
		try {
			ofstream NewFile("/Users/nas/Documents/ListOfClients.txt");
			std::cout << "File created\n";
			NewFile.close();
		}
		catch (exception& e) {
			std::cout << "Cannot create file";
		}
		pthread_exit(NULL);
	}

	void* writeToTheFile(void* arg)
	{
		pthread_detach(pthread_self());
		//Append all members of the list to the file.
		ofstream MyFile("/Users/nas/Documents/ListOfClients.txt");

		
		list<Client*>::iterator it;

		for (it = clients.begin(); it != clients.end(); it++) {
			MyFile.clear();
			MyFile << (**it).toString() << "\n";
		}
			
		MyFile.close();
		pthread_exit(NULL);
	}

	void* callServer(void* arg)
	{
		pthread_detach(pthread_self());
		Server();
		pthread_exit(NULL);
	}

	int main(int argc, char* argv[])
	{
		std::srand(static_cast<unsigned int>(std::time(nullptr)));
		
		//create threads 
		pthread_t ptid;
		pthread_t ptid2;
		pthread_t ptid3;
		

		/*Gather the list 
		Written by Nasir Cross-Griffin*/
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
		

		//New Thread created for file creation
		pthread_create(&ptid, NULL, &createTheNewFile, NULL);

		//New Thread created for appending data to file
		pthread_create(&ptid2, NULL, &writeToTheFile, NULL);	

		//New Thread created for starting the server
		pthread_create(&ptid3, NULL, &callServer, NULL);
		//pthread_join(ptid3, NULL);	

		pthread_exit(NULL);

		return 0;
	}


