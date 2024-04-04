#include <iostream>
#include <boost/asio.hpp>
#include "server.h"
#include <iostream>
#include <fstream>
#include <boost/dll/runtime_symbol_info.hpp>
#include <boost/filesystem.hpp>
#include <unistd.h>

using boost::asio::ip::tcp;

	std::stringstream buffer;
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
bool fileReadComplete = false;

void* fileToBuffer(void* arg)
{
    // Lock mutex before reading the file
    pthread_mutex_lock(&mutex1);

    //creates a buffer
    std::ifstream MyReadFile("/Users/nas/Documents/ListOfClients.txt");
    buffer << MyReadFile.rdbuf();
    MyReadFile.close();

    // Set the flag to indicate file reading is complete
    fileReadComplete = true;

    // Signal the main thread that file reading is done
    pthread_cond_signal(&cond);

    // Unlock mutex after file reading
    pthread_mutex_unlock(&mutex1);

    pthread_exit(NULL);
}


	void Server()
	{
		try {
			pthread_t ptid;
			//Opens an IO context that boost uses to keep input and output on the system
			boost::asio::io_context io_context;

			//This acceptor accepts connections on port 1337
			tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 1337));

			while (true) {
				//Waits for a connections and stores that connection in the socket
				std::cout << "Accepting connections on port 1337!\n";
				tcp::socket socket(io_context);
				acceptor.accept(socket);

				std::cout << "Client connected! Sending data!\n";
				boost::system::error_code error;

			pthread_create(&ptid, NULL, &fileToBuffer, NULL);

            // Lock mutex before checking fileReadComplete flag
            pthread_mutex_lock(&mutex1);

            // Wait until file reading is complete
            while (!fileReadComplete) {
                pthread_cond_wait(&cond, &mutex1);
            }

            // Unlock mutex after file reading is complete
            pthread_mutex_unlock(&mutex1);

				std::cout << buffer.str();

				//boost::asio::write(socket, boost::asio::buffer(hello_message), error);
				boost::asio::write(socket, boost::asio::buffer(buffer.str()), error);
			/*
				//Print out the sorted list
				while (true) {
					//Listen for messages
					std::array<char, 128> buf{};
						
					boost::system::error_code error;

					//Read data from socket and write to buffer
					size_t en = socket.read_some(boost::asio::buffer(buf), error);

						
					if (error == boost::asio::error::eof) {
						// Clean connection cut off
						break;
					}
					else if (error)
					{
						throw boost::system::system_error(error);
					}
					

					std::cout.write(buf.data(), buf.size());
					
				}
				*/
			}
		}
		catch (std::exception e) {
			std::cerr << e.what() << std::endl;
		}
	}