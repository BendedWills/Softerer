#include <Softerer/Queue.hpp>

namespace Softerer
{
	Queue::Work::Work(Work& other)
		:
		m_IsCompleted(other.m_IsCompleted.load())
	{}

	Queue::Work::Work(Work&& other) noexcept
		:
		m_IsCompleted(other.m_IsCompleted.load())
	{}

	Queue::Work& Queue::Work::operator=(Work& other)
	{
		m_IsCompleted = other.m_IsCompleted.load();
		return *this;
	}

	Queue::Work& Queue::Work::operator=(Work&& other) noexcept
	{
		m_IsCompleted = other.m_IsCompleted.load();
		return *this;
	}

	Queue::Queue()
		:
		m_QueueThread(&Queue::RunQueueThread, this),
		m_WorkSmph(0)
	{}

	Queue::~Queue()
	{
		m_WorkSmph.release();
		m_Running = false;
	}

	void Queue::AddWork(Scope<Work>&& work)
	{
		if (m_WorkMutex.try_lock())
		{
			AddWorkToVec(m_Work, std::move(work));
			m_WorkMutex.unlock();
		}
		else
		{
			m_WorkMutex2.lock();
			AddWorkToVec(m_Work2, std::move(work));
			m_WorkMutex2.unlock();
		}

		m_WorkSmph.release();
	}

	bool Queue::IsInUse()
	{
		if (!m_WorkMutex.try_lock())
			return true;
		else
			m_WorkMutex.unlock();

		if (!m_WorkMutex2.try_lock())
			return true;
		else
			m_WorkMutex2.unlock();

		return false;
	}

	void Queue::WaitIdle()
	{
		m_WorkMutex.lock();
		m_WorkMutex2.lock();
		m_WorkMutex.unlock();
		m_WorkMutex2.unlock();
	}

	void Queue::AddWorkToVec(std::vector<Scope<Work>>& workVec,
		Scope<Work>&& work)
	{
		workVec.push_back(std::move(work));

		for (size_t i = 0; i < workVec.size(); )
		{
			if (workVec[i]->m_IsCompleted)
			{
				workVec.erase(workVec.begin() + i);
				continue;
			}

			i++;
		}
	}

	void Queue::RunQueueThread()
	{
		while (m_Running)
		{
			m_WorkSmph.acquire();

			ProcessWork(m_Work, m_WorkMutex);
			ProcessWork(m_Work2, m_WorkMutex2);
		}
	}

	void Queue::ProcessWork(std::vector<Scope<Work>>& workVec, std::shared_mutex& mutex)
	{
		std::shared_lock lock(mutex);

		for (size_t i = 0; i < workVec.size(); i++)
		{
			Work& work = *workVec.at(i);
			if (work.m_IsCompleted)
				continue;

			work.Execute();
			work.m_IsCompleted = true;
		}
	}
}