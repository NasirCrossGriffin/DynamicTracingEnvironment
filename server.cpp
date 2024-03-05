#include <iostream>
#include <boost/asio.hpp>
#include "server.h"

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
				boost::system::error_code error;

				boost::asio::write(socket, boost::asio::buffer(hello_message), error);
			}
		}
		catch (std::exception e) {
			std::cerr << e.what() << std::endl;
		}
	}