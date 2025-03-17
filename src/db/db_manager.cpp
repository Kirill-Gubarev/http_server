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
int db::db_manager::add_product_to_cart(std::string login, std::string product, int quantity) {
    if (connection_ptr->is_open()) {
        pqxx::work work(*connection_ptr);

        // Проверка, существует ли пользователь
        pqxx::result result = work.exec_params(R"(
            SELECT id FROM users WHERE login = $1
        )", login);

        if (result.empty()) {
            return 0; // Пользователь не найден
        }

        int user_id = result[0][0].as<int>();

        // Проверка, существует ли продукт
        result = work.exec_params(R"(
            SELECT id FROM products WHERE name = $1
        )", product);

        if (result.empty()) {
            return 0; // Продукт не найден
        }

        int product_id = result[0][0].as<int>();

        // Проверка, существует ли уже этот товар в корзине
        result = work.exec_params(R"(
            SELECT id FROM user_cart WHERE id_user = $1 AND id_product = $2
        )", user_id, product_id);

        if (!result.empty()) {
            // Товар уже в корзине, обновим его количество
            work.exec_params(R"(
                UPDATE user_cart SET quantity = $1 WHERE id_user = $2 AND id_product = $3
            )", quantity, user_id, product_id);
        } else {
            // Товара нет в корзине, добавим новый
            work.exec_params(R"(
                INSERT INTO user_cart (id_user, id_product, quantity) VALUES ($1, $2, $3)
            )", user_id, product_id, quantity);
        }

        try {
            work.commit();
            return 1; // Успешно добавлено
        } catch (const std::exception& e) {
            // Ошибка при вставке данных
            return 0;
        }
    } else {
        return 0;
    }
}
std::string db::db_manager::get_cart_products(std::string login){
    if (connection_ptr->is_open()) {
        pqxx::work work(*connection_ptr);

        pqxx::result result = work.exec_params(R"(
            SELECT id FROM users WHERE login = $1
        )", login);

        if (result.empty()) {
            return ""; // Пользователь не найден
        }

        result = work.exec_params(R"(
			SELECT products.name, products.quantity, products.price, images.link, user_cart.quantity
            FROM user_cart
			LEFT JOIN products ON user_cart.id_product = products.id
			LEFT JOIN users ON user_cart.id_user = users.id
			LEFT JOIN images ON products.image_id = images.id
            WHERE users.login = $1
        )", login);

		std::string result_str = "[";
		for (auto it = result.begin(); it != result.end(); ++it) {
			result_str += "{\"name\":\"";    
			result_str += (*it)[0].c_str();
			result_str += "\",\"product_quantity\":";    
			result_str += (*it)[1].c_str();
			result_str += ",\"price\":";    
			result_str += (*it)[2].c_str();
			result_str += ",\"image\":\"";    
			result_str += (*it)[3].c_str();
			result_str += "\",\"quantity\":\"";    
			result_str += (*it)[4].c_str();
			result_str += "\"}";
			
			if (std::next(it) != result.end()) {
				result_str += ",";
			}
		}
		result_str += "]";
		return result_str;
	} else{
		return "";
	}
}
int db::db_manager::remove_from_cart(std::string login, std::string product) {
    if (connection_ptr->is_open()) {
        pqxx::work work(*connection_ptr);

        // Получаем id пользователя
        pqxx::result result = work.exec_params(
            "SELECT id FROM users WHERE login = $1", login);

        if (result.empty()) {
            return 0; // Пользователь не найден
        }

        int user_id = result[0][0].as<int>();

        // Удаляем товар из корзины
        work.exec_params(R"(
            DELETE FROM user_cart 
            WHERE id_user = $1 AND id_product = (
                SELECT id FROM products WHERE name = $2
            )
        )", user_id, product);

        work.commit(); // Фиксируем изменения
        return 1;
    }

    return 0;
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
