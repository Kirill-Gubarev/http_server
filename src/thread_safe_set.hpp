#ifndef THREAD_SAFE_SET_HPP
#define THREAD_SAFE_SET_HPP

#include <unordered_set>
#include <mutex>

namespace utils{
	template<typename T>
	class thread_safe_set{
	private:
		std::unordered_set<T> u_set;
		std::mutex mutex_;

	public:
		thread_safe_set(){}
		~thread_safe_set(){}

		void insert(const T& data){
			mutex_.lock();
			u_set.insert(data);
			mutex_.unlock();
		}
		void erase(const T& data){
			mutex_.lock();
			u_set.erase(data);
			mutex_.unlock();
		}
	};
}

#endif//THREAD_SAFE_SET_HPP
