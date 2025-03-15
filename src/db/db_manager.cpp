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
