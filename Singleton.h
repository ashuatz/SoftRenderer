#pragma once
#include <mutex>
#include <memory>

template < typename T >
class Singleton
{
protected:
	Singleton() = default;
	Singleton(const Singleton<T>& src) = delete;
	Singleton<T>& operator=(const Singleton<T>& rhs) = delete;

	virtual void Initialize() {};

public:
	static T& GetInstance()
	{
		std::call_once(m_onceFlag, []
		{
			auto instance = new T;
			m_instance.reset(instance);
		});

		return *m_instance.get();
	};

private:
	static std::unique_ptr<T> m_instance;
	static std::once_flag m_onceFlag;
};

template <typename T> 
std::unique_ptr<T> Singleton<T>::m_instance;

template <typename T>
std::once_flag Singleton<T>::m_onceFlag;