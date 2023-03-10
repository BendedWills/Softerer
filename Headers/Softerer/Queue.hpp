#pragma once

#include <Softerer/Common/Ref.hpp>

#include <thread>
#include <vector>
#include <semaphore>
#include <atomic>
#include <functional>
#include <shared_mutex>

namespace Softerer
{
	class Queue
	{
	public:
		class Work
		{
			friend Queue;
		public:
			Work() = default;
			Work(Work& other);
			Work(Work&& other) noexcept;
			Work& operator=(Work& other);
			Work& operator=(Work&& other) noexcept;
			virtual ~Work() = 0;
		protected:
			virtual void Execute() = 0;

			std::atomic_bool m_IsCompleted = false;
		};

		Queue();
		~Queue();

		void AddWork(Scope<Work>&& work);

		bool IsInUse();
		void WaitIdle();
	private:
		void AddWorkToVec(std::vector<Scope<Work>>& workVec, Scope<Work>&& work);

		void RunQueueThread();
		void ProcessWork(std::vector<Scope<Work>>& workVec, std::shared_mutex& mutex);

		std::jthread m_QueueThread;
		
		std::binary_semaphore m_WorkSmph;
		
		std::shared_mutex m_WorkMutex;
		std::vector<Scope<Work>> m_Work;
		std::shared_mutex m_WorkMutex2;
		std::vector<Scope<Work>> m_Work2;

		std::atomic_bool m_Running = true;
	};
}