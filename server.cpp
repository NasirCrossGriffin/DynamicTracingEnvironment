#include <iostream>
#include <boost/asio.hpp>
#include "server.h"
#include <iostream>
#include <fstream>
#include <boost/dll/runtime_symbol_info.hpp>
#include <boost/filesystem.hpp>
#include <unistd.h>

using boost::asio::ip::tcp;


	void Server()
	{
		try {
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
				std::string hello_message = "Welcome to testing enviroment server!\n";
				std::ifstream MyReadFile("/home/collin/Documents/ListOfClients.txt");
				// Use a while loop together with the getline() function to read the file line by line
				//while (getline(MyReadFile, myText)) {
					// Output the text from the file
					//std::cout << myText;
				//}

				std::stringstream buffer;
				buffer << MyReadFile.rdbuf();

				MyReadFile.close();
				boost::system::error_code error;

				//boost::asio::write(socket, boost::asio::buffer(hello_message), error);
				boost::asio::write(socket, boost::asio::buffer(buffer.str()), error);
			}
		}
		catch (std::exception e) {
			std::cerr << e.what() << std::endl;
		}
	}


