#pragma once

#include <thread>
#include <queue>
#include <functional>
#include <condition_variable>

namespace Gin::Thread {

	class ThreadPool {
	public:
		ThreadPool();
		~ThreadPool();

		void Execute(const std::function<void()>& job);
		unsigned int GetThreadCount() const;

	private:
		void TheadLoop();

	private:
		bool terminate{ false };
		std::mutex mutex{};
		std::condition_variable cv{};
		std::vector<std::thread> workers{};
		std::queue<std::function<void()>> jobs{};
	};

}