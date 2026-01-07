#ifndef OBSERVABLE_TEMPLATE_HPP
#define OBSERVABLE_TEMPLATE_HPP

#include <functional>
#include <unordered_map>

template<typename... Args>
class Observable {
public:
	using Callback = std::function<void(Args...)>;

	Observable() = default;
	~Observable() = default;

	void Register(const void* owner, Callback cb);
	void Unregister(const void* owner);
	void Notify(Args... args);

private:
	std::unordered_map<const void*, Callback> m_observers;
};

//
// Template definitions (kept in header so templates are available at instantiation)
//
template<typename... Args>
void Observable<Args...>::Register(const void* i_owner, Callback i_cb) {
	if (i_owner == nullptr)
		return;

	m_observers[i_owner] = std::move(i_cb);
}

template<typename... Args>
void Observable<Args...>::Unregister(const void* i_owner) {
	if (i_owner == nullptr)
		return;

	m_observers.erase(i_owner);
}

template<typename... Args>
void Observable<Args...>::Notify(Args... i_args) {
	for (const auto& observer : m_observers) {
		if (observer.second)
			observer.second(i_args...);
	}
}

#endif // OBSERVABLE_TEMPLATE_HPP

