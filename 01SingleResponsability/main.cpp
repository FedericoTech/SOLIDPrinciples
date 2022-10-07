/**
 * The principle of single responsibility.
 * a class should have one and only one responsibility.
 */

#include <string>
#include <iostream>
#include <vector>
#include <functional>

/**
 * This class simulates a database connection
 */
struct Database {
	//this simulates the table users
	std::vector<std::string> table_users;

	//this method receives queries in a fancy way
	Database &operator<<(const std::string &query){
		//we discard that string with the query and we only accept the params
		if(query.rfind("INSERT", 0) == std::string::npos
			&& query.rfind("SELECT", 0) == std::string::npos
			&& query.rfind("UPDATE", 0) == std::string::npos
		){
			table_users.push_back(query);
		}

		return *this;
	}

	//this method returns results also in a fancy way.
	Database &operator>>(std::function<void(std::string &)> lambda){

		for(auto username: table_users){
			lambda(username);
		}
		return *this;
	}
};

const std::string kPrefix = "user-";

//#define BAD_CODE

#ifdef BAD_CODE

	/**
	 * This class manages user operations
	 * The problem with this class is that it is also responsible for the queries and for the username formatting.
	 * so this class has more than a single responsibility.
	 */
	struct UserManager {
		UserManager(Database& database): db{database} {}

		void createUser(std::string username)
		{
			//if user name doesn't start with
			if(username.rfind(kPrefix, 0) == std::string::npos){
				username = kPrefix + username;
			}

			db << "INSERT INTO USERS (name) VALUES (?);" << username;
		}

		std::vector<std::string> getUsersReport()
		{
			std::vector<std::string> users;

			db << "SELECT name FROM users" >> [&users](std::string &user) {
				users.push_back(user.erase(0, kPrefix.length()));
			};

			return users;
		}

	protected:
		Database &db;
	};

#else //NOW THE GOOD CODE

	/**
	 * we have this class that takes care of the name format
	 */
	struct UsernameFormater {
		~UsernameFormater() = default;

		std::string format(std::string username){
			//if user name doesn't start with
			if(username.rfind(kPrefix, 0) == std::string::npos){
				return kPrefix + username;
			}

			return username;
		}

		std::string getReadableName(std::string input){
			return input.erase(0, kPrefix.length());
		}
	};

	/**
	 * we have this class that takes care of the SQL queries
	 * it also makes use of the UsernameFormater class instead of formating the name by itself.
	 */

	struct UserModel {
		UserModel(Database& database): db{database} {}

		~UserModel() = default;

		void addUser(std::string usarname){
			db << "INSERT INTO USERS (name) VALUES (?);" << uf.format(usarname);
		};

		std::vector<std::string> getAllUsers()
		{
			std::vector<std::string> users;

			db << "SELECT name FROM users" >> [&users, this](std::string &user) {
				users.push_back(this->uf.getReadableName(user));
			};

			return users;
		}

	protected:
		UsernameFormater uf;
		Database &db;
	};

	/**
	 * Now UserManager only does its own concern whilst offering the same interface.
	 */
	struct UserManager {

		UserManager(Database& database): um(UserModel{database}) {}

		~UserManager(){}

		void createUser(std::string username)
		{
			um.addUser(username);
		}

		std::vector<std::string> getUsersReport()
		{
			return um.getAllUsers();
		}

	protected:
		UserModel um;
	};

#endif //THE GOOD CODE

int main()
{

	Database db;
	UserManager um(db);

	um.createUser("user1");
	um.createUser("user-user2");

	std::cout << "printing:" << std::endl;

	for(auto user: um.getUsersReport()){
		std::cout << user << '\n';
	}

	return 0;
}


