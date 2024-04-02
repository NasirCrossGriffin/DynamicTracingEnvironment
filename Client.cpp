#include <iostream>
#include <boost/asio.hpp>
#include "../client.cpp"
#include <fstream>
#include <list>

using boost::asio::ip::tcp;

list<Client*> clients;

std::stringstream clientString;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void* conversion(void* arg)
{
    pthread_mutex_lock(&mutex);

    std::stringstream clientSS;
    std::string line;
    int i = 0;
    std::string current;
    std::string name;
    int id;
    int age;
    std::string gender;

    while (std::getline(clientString, line))
    {
        clientSS.clear();
        clientSS.str(line);
        i = 0;
        while (clientSS >> current) {
            switch (i)
            {
                case 0:
                    break;
                case 1:
                    name = current;
                    break;
                case 3:
                    id = std::stoi(current);
                    break;
                case 5:
                    age = std::stoi(current);
                    break;
                case 7:
                    gender = current;
                    break;
                default:
                    ;
            }
            current.clear();
            i++;
        }
        clients.push_back(new Client(name, id, age, gender));
    }

    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);
    pthread_exit(NULL);
}

void* sort(void* arg)
{
    pthread_mutex_lock(&mutex);
    pthread_cond_wait(&cond, &mutex);

    clients.sort(Client::compareClients);

    for (auto it = clients.begin(); it != clients.end(); it++)
        std::cout << (**it).toString() << "\n";

    pthread_mutex_unlock(&mutex);
    pthread_exit(NULL);
}

void* createAndAppend(void* arg)
{
    pthread_mutex_lock(&mutex);
    pthread_cond_wait(&cond, &mutex);

    try {
        std::ofstream NewFile("/Users/nas/Documents/ListOfSortedClients.txt");
        std::cout << "\nFile created\n";
        NewFile.close();
    }
    catch (std::exception& e) {
        std::cout << "Cannot create file";
    }

    std::ofstream MyFile("/Users/nas/Documents/ListOfSortedClients.txt");

    for (auto it = clients.begin(); it != clients.end(); it++) {
        MyFile.clear();
        MyFile << (**it).toString() << "\n";
    }

    MyFile.close();

    pthread_mutex_unlock(&mutex);
    pthread_exit(NULL);
}

int main()
{
	pthread_t ptid;
	pthread_t ptid2;
	pthread_t ptid3;

	try {
		boost::asio::io_context ioContext;

		//resolves the host and creates a series of connection strings
		tcp::resolver resolver{ ioContext };

		auto endpoints = resolver.resolve("127.0.0.1", "1337");

		//creates a socket
		tcp::socket socket{ ioContext };

		//connects to the socket
		boost::asio::connect(socket, endpoints);

		//Create list of clients from file data
			

		while (true) {
			//Listen for messages
			std::array<char, 128> buf{};
			
			boost::system::error_code error;

			//Read data from socket and write to buffer
			size_t en = socket.read_some(boost::asio::buffer(buf), error);

			
			if (error == eof) {
				// No more data to read
				break;
			}
			else if (error)
			{
				throw boost::system::system_error(error);
			}
		
			std::cout.write(buf.data(), buf.size());
			
			for (char c : buf)
			{
				 clientString << c;
			}
		}

		//Convert buffer text to object data
  		pthread_create(&ptid, NULL, &conversion, NULL);
		std::cout << "\n\n" << "Sorting..." << "\n\n";
			
			
		
		//sort the new list
		list<Client*>::iterator it;
		pthread_create(&ptid2, NULL, &sort, NULL);
		//Create a new file and append all the data to the file
		pthread_create(&ptid3, NULL, &createAndAppend, NULL);
			
        // Wait for all threads to finish
        pthread_join(ptid, NULL);
        pthread_join(ptid2, NULL);
        pthread_join(ptid3, NULL);


		std::ifstream MyReadFile("/Users/nas/Documents/ListOfSortedClients.txt");

		std::stringstream buffer;
		buffer << MyReadFile.rdbuf();
			
		MyReadFile.close();
			
		boost::system::error_code error;

		//Write to the socket
		boost::asio::write(socket, boost::asio::buffer(buffer.str()), error);
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}

	return 0;
}
