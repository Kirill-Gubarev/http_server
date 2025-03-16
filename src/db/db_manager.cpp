#include "db/db_manager.h"

#include <iostream>

db::db_manager::db_manager():connection_ptr(nullptr){}

int db::db_manager::connect(){
	try{
		connection_ptr = std::make_unique<pqxx::connection>(R"(
			dbname=fruits_db
			user=postgres
			password=
			host=localhost
			port=5432
		)");
	}
	catch(const std::exception& ex){
		std::cerr << "Error: " << ex.what() << std::endl;
        return 0;
	}
	return 1;
}
std::string db::db_manager::get_fruits() const{
	if(connection_ptr->is_open()){
		pqxx::work work(*connection_ptr);
		pqxx::result result = work.exec(R"(
			SELECT products.name, products.quantity, products.price, images.link AS image_link
			FROM products
			LEFT JOIN images ON products.image_id = images.id;
		)");
		std::string result_str = "[";
		for (auto it = result.begin(); it != result.end(); ++it) {
			result_str += "{\"name\":\"";    
			result_str += (*it)[0].c_str();
			result_str += "\",\"quantity\":";    
			result_str += (*it)[1].c_str();
			result_str += ",\"price\":";    
			result_str += (*it)[2].c_str();
			result_str += ",\"image\":\"";    
			result_str += (*it)[3].c_str();
			result_str += "\"}";
			
			if (std::next(it) != result.end()) {
				result_str += ",";
			}
		}
		result_str += "]";
		return result_str;
	}
	else{
		return "";
	}
}
int db::db_manager::create_user(std::string login, std::string password, 
                std::string email, std::string fullname, float balance) {
    if (connection_ptr->is_open()) {
        pqxx::work work(*connection_ptr);
        
        pqxx::result result = work.exec_params(R"(
            SELECT 1 FROM users WHERE login = $1
        )", login);
        
        if (!result.empty()) {
            return 0;
        }
        
        // Если логин не занят, создаем нового пользователя
        try {
            work.exec_params(R"(
                INSERT INTO users (login, password, email, fullname, balance)
                VALUES ($1, $2, $3, $4, $5)
            )", login, password, email, fullname, balance);
            work.commit();
            return 1; // Пользователь успешно создан
        } catch (const std::exception& e) {
            // Ошибка при вставке данных
            return 0;
        }
    } else {
        // Если соединение с БД не установлено
        return 0;
    }
}
std::string db::db_manager::get_user(std::string login, std::string password) {
    if (connection_ptr->is_open()) {
        pqxx::work work(*connection_ptr);
        pqxx::result result = work.exec_params(R"(
            SELECT login, email, fullname, balance
            FROM users
            WHERE login = $1 AND password = $2
        )", login, password);

        std::string result_str = "";
        if (!result.empty()) {
            const auto& row = result[0];
            result_str += "{\"success\":true,";
            result_str += "\"login\": \"" + row["login"].as<std::string>() + "\", ";
            result_str += "\"email\": \"" + row["email"].as<std::string>() + "\", ";
            result_str += "\"fullname\": \"" + row["fullname"].as<std::string>() + "\", ";
            result_str += "\"balance\": " + std::to_string(row["balance"].as<double>()) + " ";
            result_str += "}";
        }else{
			result_str = "{\"success\":false}";
		}

        return result_str;
    }
    else {
        return "";
    }
}
