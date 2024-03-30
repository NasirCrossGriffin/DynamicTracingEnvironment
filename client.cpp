#include <string>
#include <list>

//Written by Nasir Cross-Griffin

class Client
{
	private:
		
		//Data Members
		std::string name;

		int id;

		int age;

		std::string gender;

	public:

		//Member Functions
		Client(std::string name, int id, int age, std::string gender)
		{
			this->name = name;
			this->id = id;
			this->age = age;
			this->gender = gender;
		}
		
		
		void setName() {

		}

		std::string getName() {
			return this->name;
		}

		void setID() {

		}
		

		int getID() {
			return this->id;
		}

		
		void setAge() {

		}

		int getAge() {
			return this->age;
		}

		std::string getGender()
		{
			return this->gender;
		}
		

		bool equals(Client other)
		{
			if (other.getID() == this->getID())
			{
				return true;
			}
			else
			{
				return false;
			}

		}

		std::string toString()
		{
			return ("Name: " + getName() + ", ID: " + std::to_string(getID()) + ", Age: " + std::to_string(getAge()) + ", Gender: " + getGender());
		}

		bool compareClients(Client client1, Client client2)
		{
			return (client1.getID() < client2.getID());
		}
};
