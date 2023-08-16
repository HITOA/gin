#include <gin/thread/parallel.hpp>

void Gin::Thread::JobParallelFor::Run(ThreadPool& pool, unsigned int count)
{
	unsigned int batchSize = count / pool.GetThreadCount();
	unsigned int rest = count - batchSize * pool.GetThreadCount();

	for (unsigned int i = 0; i < pool.GetThreadCount(); ++i) {
		unsigned int currentIdx = batchSize * i;
		unsigned int currentSize = i == pool.GetThreadCount() - 1 ? batchSize + rest : batchSize;

		++running;

		pool.Execute([this, currentIdx, currentSize] {
			for (unsigned int i = currentIdx; i < currentIdx + currentSize; ++i) {
				this->Execute(i);
			}
			{
				std::unique_lock<std::mutex> lock{ mutex };
				--running;
				cv.notify_all();
			}
			});
	}
}

void Gin::Thread::JobParallelFor::WaitForCompletion()
{
	while (running) {
		std::unique_lock<std::mutex> lock{ mutex };
		cv.wait(lock, [&] {
			return running <= 0;
			});
	}
}