#pragma once

#include <gin/thread/threadpool.hpp>

namespace Gin::Thread {

	/*
	*	Really unity inspired huh
	*/

	struct JobParallelFor {
		virtual void Execute(unsigned int threadIdx) = 0;
		void Run(ThreadPool& pool, unsigned int count);
		void WaitForCompletion();

	private:
		unsigned int running{ 0 };
		std::mutex mutex{};
		std::condition_variable cv{};
	};

}