#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

int main()
{
	try {
		boost::asio::io_context ioContext;

		//resolves the host and creates a series of connection strings
		tcp::resolver resolver{ ioContext };

		auto endpoints = resolver.resolve("127.0.0.1", "1337");

		//creates a socket
		tcp::socket socket{ ioContext };

		//connects to the socket
		boost::asio::connect(socket, endpoints);

		while (true) {
			//Listen for messages
			std::array<char, 128> buf{};
			boost::system::error_code error;

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
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}

	return 0;
}
