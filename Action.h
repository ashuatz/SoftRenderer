#pragma once

#include "stdafx.h"

template<typename ... T>
struct Delegate
{
public:
	void(*Func)(T ...);

	void Invoke(T ... t) 
	{
		Func(t...);
	}

	bool operator== (const Delegate<T...>& rhs) const
	{
		return rhs.Func == this->Func;
	}
};

template<typename ... T>
class Action
{
public:
	void Invoke(T... obj)
	{
		for (auto iter : actionList) {
			(*iter).Invoke(obj);
		}
	}

	Action& operator += (const void(*Func)(T ...)& rhs)
	{
		this->actionList.emplace_back(Delegate<T...>{rhs});
	}

	Action& operator += (const Delegate<T>& rhs) {
		this->actionList.push_back(rhs);
		return *this;
	}


	Action& operator -= (const void(*Func)(T ...)& rhs)
	{
		this->actionList.erase(std::remove_if(actionList.begin(), actionList.end(),
			[=](const Delegate<T...>& t) {
			return t.Func == rhs;
		}), actionList.end());
		return *this;
	}

	Action& operator -= (const Delegate<T>& rhs) {
		this->actionList.erase(std::remove_if(actionList.begin(), actionList.end(),
			[=](const Delegate<T...>& t) {
			return t == rhs;
		}), actionList.end());
		return *this;
	}

private:
	std::vector<Action<T>> actionList;
};