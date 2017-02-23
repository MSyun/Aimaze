// イベント関数
// 2016.12.30	プログラム作成
// author		SyunMizuno


#pragma once

#include	<list>
#include	"../Delegate/Delegate.h"
using namespace std;


/*
	// 引数無しのクラスメンバ
	AddListener(std::bind(&Class::method, this));
	AddListener(std::bind(&Class::method, this, 5));
	// 引数ありのクラスメンバ
	AddListener(std::bind(&Class::method, this, std::placeholders::_1));
	AddListener([=](int i) {this->method(i);});
	// グローバルな関数
	AddListener(&method);
*/


template<typename... Args>
class CPPEvent {
private:
#pragma region variable

	// 関数ポインタリスト
	list<IDelegate<Args...>*>	m_lstListener;

#pragma endregion

public:
#pragma region method

	CPPEvent() {};
	virtual ~CPPEvent() {
		RemoveAllListener();
	};

	// 関数を登録
	void AddListener(function<void(Args...)> func) {
		m_lstListener.push_back(Delegate<void, Args...>::Create(func));
	};

	// 関数を開放
	void RemoveListener(function<void(Args...)> func) {
		for (auto it = m_lstListener.begin(); it != m_lstListener.end(); ++it) {
			if (!(*it)->CheckSame(func))
				continue;

			delete (*it);
			(*it) = NULL;
			return;
		}
	}

	// 登録済みの関数をすべて開放
	void RemoveAllListener() {
		for (auto it = m_lstListener.begin(); it != m_lstListener.end(); ++it) {
			delete *it;
			*it = NULL;
		}
		m_lstListener.clear();
	};

	// 関数の個数を返す
	int GetListenerCount() {
		return m_lstListener.size();
	};

	// 登録済みの関数を実行
	void Invoke(Args... args) {
		for (auto it = m_lstListener.begin(); it != m_lstListener.end(); ++it) {
			(*it)->Invoke(args...);
		}
	};

#pragma endregion
};