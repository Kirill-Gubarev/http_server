#ifndef DB_MANAGER_H
#define DB_MANAGER_H

#include <pqxx/pqxx> 
#include <memory>

namespace db{
	class db_manager{
	private:
		std::unique_ptr<pqxx::connection> connection_ptr;

	public:
		explicit db_manager();
		db_manager(const db_manager& other) = delete;
		db_manager operator=(const db_manager& other) = delete;

		int connect();	
		std::string get_fruits() const;
		std::string get_user(std::string login);
		std::string get_user(std::string login, std::string password);
		int create_user(std::string login, std::string password, 
				std::string email, std::string fullname, float balance);
		int add_product_to_cart(std::string login, std::string product, int quantity);
		int remove_from_cart(std::string login, std::string product);
		std::string get_cart_products(std::string login);
		int add_purchase(std::string login, std::string product, int quantity);
		float add_balance(std::string login, float balance);
		std::string get_purchases(std::string login);
	};
}

#endif//DB_MANAGER_H
