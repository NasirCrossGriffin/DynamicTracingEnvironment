#include <list>
#include <iostream>
#include <limits.h>
#include <cstdlib>
#include <iostream>
#include <boost/asio.hpp>
#include "server.h"
#include <fstream>
using namespace std;

	int main(int argc, char* argv[])
	{
		/*Gather the list and sort it
		Written by Nasir Cross-Griffin*/

		list<int> NewList;

		int i = 0;
		while (i < 10)
		{
			NewList.push_back(rand());
			i++;
		}

		NewList.sort();

		//Create a new file
		try {
			fstream NewFile("ListValues.txt");
			std::cout << "File created\n";
		}
		catch (exception& e) {
			std::cout << "Cannot create file";
		}

		//Print out all members of the list (Will be changed to append to file)
		list<int>::iterator it = NewList.begin();

		/*for (int i; i < NewList.size(); i++)
		{
			cout << it.next();
		} */

		Server();

		return 0;


	}
