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
	};
}

#endif//DB_MANAGER_H
