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
#include <cstdlib> //add this
#include "parallel_id.h"


using namespace std;

list<Client*> clients;
bool fileIsCreated;
bool fileIsWritten;
bool serverIsCalled;
pthread_mutex_t mutex3 = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond1 = PTHREAD_COND_INITIALIZER;

	void* createTheNewFile(void* arg)
	{
		pthread_detach(pthread_self());

		// Lock mutex before creating the file
		pthread_mutex_lock(&mutex3);
		fileIsCreated = false;
		
		//Create a new file.
		try {
			ofstream NewFile("/home/collin/Documents/ListOfClients.txt");
			std::cout << "File created\n";
			NewFile.close();
		}
		catch (exception& e) {
			std::cout << "Cannot create file";
		}
		
		fileIsCreated = true;

		// Signal the main thread that file reading is done
    	pthread_cond_signal(&cond1);

    	// Unlock mutex after file reading
    	pthread_mutex_unlock(&mutex3);
		
		pthread_exit(NULL);
	}

	void* writeToTheFile(void* arg)
	{
		pthread_detach(pthread_self());
		
		// Lock mutex before creating the file
		pthread_mutex_lock(&mutex3);
		
		fileIsWritten = false;

		//Append all members of the list to the file.
		ofstream MyFile("/home/collin/Documents/ListOfClients.txt");

		
		list<Client*>::iterator it;

		for (it = clients.begin(); it != clients.end(); it++) {
			MyFile.clear();
			MyFile << (**it).toString() << "\n";
		}
			
		MyFile.close();
		
		fileIsWritten = true;

		// Signal the main thread that file writing is done
    	pthread_cond_signal(&cond1);

    	// Unlock mutex after file writing
		pthread_mutex_unlock(&mutex3);
		
		pthread_exit(NULL);
	}

	void* callServer(void* arg)
	{
		pthread_detach(pthread_self());
		// Lock mutex before creating the file
		pthread_mutex_lock(&mutex3);
		serverIsCalled = false;
		
		Server();

		serverIsCalled = true;

		// Signal the main thread that the server is called.
    	pthread_cond_signal(&cond1);

    	// Unlock mutex after file reading
		pthread_mutex_unlock(&mutex3);
		
		pthread_exit(NULL);
	}

	

	int main(int argc, char* argv[])
	{
		int numberOfThreads = std::atoi(argv[1]); //add this
		setThreads(numberOfThreads);
		
		
		std::srand(static_cast<unsigned int>(std::time(nullptr)));
		
		//create threads 
		pthread_t ptid;
		pthread_t ptid2;
		pthread_t ptid3;

		

		/*Gather the list 
		Written by Nasir Cross-Griffin*/
		for(int i = 0; i < std::atoi(argv[2]); i++) {
		clients.push_back(new Client("Matthew", generate_employee_ids_multithreaded(), 20 + (rand() % 40), "MALE"));
		clients.push_back(new Client("Sarah", generate_employee_ids_multithreaded(), 20 + (rand() % 40), "FEMALE"));
		clients.push_back(new Client("Jason", generate_employee_ids_multithreaded(), 20 + (rand() % 40), "MALE"));
		clients.push_back(new Client("Seth", generate_employee_ids_multithreaded(), 20 + (rand() % 40), "MALE"));
		clients.push_back(new Client("Lisa", generate_employee_ids_multithreaded(), 20 + (rand() % 40), "FEMALE"));
		clients.push_back(new Client("Trina", generate_employee_ids_multithreaded(), 20 + (rand() % 40), "FEMALE"));
		clients.push_back(new Client("Maxwell", generate_employee_ids_multithreaded(), 20 + (rand() % 40), "MALE"));
		clients.push_back(new Client("Roxanne", generate_employee_ids_multithreaded(), 20 + (rand() % 40), "FEMALE"));
		clients.push_back(new Client("Timothy", generate_employee_ids_multithreaded(), 20 + (rand() % 40), "MALE"));
		clients.push_back(new Client("Reece", generate_employee_ids_multithreaded(), 20 + (rand() % 40), "MALE"));
		}
		
		/*Gather the list 
		Written by Nasir Cross-Griffin*/
		/*
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
		*/


		//New Thread created for file creation
		pthread_create(&ptid, NULL, &createTheNewFile, NULL);

		// Lock mutex before checking fileIsCreated flag
        pthread_mutex_lock(&mutex3);

        // Wait until file creation is complete
        while (!fileIsCreated) {
            pthread_cond_wait(&cond1, &mutex3);
        }

        //Unlock mutex after file reading is complete
        pthread_mutex_unlock(&mutex3);

		//New Thread created for appending data to file
		pthread_create(&ptid2, NULL, &writeToTheFile, NULL);

		pthread_mutex_lock(&mutex3);

        // Wait until file creation is complete
        while (!fileIsWritten) {
            pthread_cond_wait(&cond1, &mutex3);
        }

        //Unlock mutex after file reading is complete
        pthread_mutex_unlock(&mutex3);	

		//New Thread created for starting the server
		pthread_create(&ptid3, NULL, &callServer, NULL);

		pthread_mutex_lock(&mutex3);

        // Wait until server is called
        while (!serverIsCalled) {
            pthread_cond_wait(&cond1, &mutex3);
        }

        //Unlock mutex after file reading is complete
        pthread_mutex_unlock(&mutex3);
		//pthread_join(ptid3, NULL);	

		pthread_exit(NULL);

		return 0;
	}


