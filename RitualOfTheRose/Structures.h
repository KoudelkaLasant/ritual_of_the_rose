#pragma once
#include "framework.h"
#include "Strings.h"

template <typename T>
struct List {
	List<T>(list<T> list) {
		internalList = list;
	}
	List<T>(T item) {
		push_back(item);
	}
	List<T>() {}
	T& at(int index) {
		int x = 0;
		typename list<T>::iterator it = internalList.begin();
		while (x < index) {
			it++;
			x++;
		}
		return *it;
	}
	bool contains(T item) {
		for (T current : internalList) {
			if (current == item) {
				return true;
			}
		}
		return false;
	}
	bool doTheseListsHaveAnythingInCommon(List<T>& list2) {
	}
	bool sameContents(List<T>& list2) {
		if (size() != list2.size()) {
			return false;
		}
		for (int x = 0; x < internalList.size(); x++) {
			if (at(x) != list2.at(x)) {
				return false;
			}
		}
		return true;
	}
	void push_back(T item) {
		internalList.push_back(item);
	}
	void push_front(T item) {
		internalList.push_front(item);
	}
	void clear() {
		internalList.clear();
	}
	T pop_front() {
		T toReturn = internalList.front();
		internalList.pop_front();
		return toReturn;
	}
	void moveFrontToBack() {
		if (empty()) { return; }
		push_back(front());
		pop_front();
	}
	List<T>& operator=(const List<T>& RHS) {
		internalList = RHS.internalList;
		return *this;
	}
	List<T>& operator=(const list<T>& RHS) {
		internalList = RHS;
		return *this;
	}
	List<T>& operator -=(const List<T>& RHS) {
		for (auto x : RHS.internalList) {
			forcibleRemove(x);
		}
		return *this;
	}
	List<T>& operator +=(const List<T>& RHS) {
		for (auto x : RHS.internalList) {
			addToBackIfNotAlreadyInList(x);
		}
		return *this;
	}
	List<T> operator+(const List<T>& RHS) {
		for (auto x : RHS.internalList) {
			internalList.push_back(x);
		}
		return *this;
	}
	bool operator==(List<T>& RHS) {
		return sameContents(RHS);
		return true;
	}
	int size() {
		return internalList.size();
	}
	bool empty() {
		return internalList.empty();
	}
	bool even() {
		return internalList.size() % 2 == 0;
	}
	void addToBackIfNotAlreadyInList(T item) {
		if (!contains(item)) {
			push_back(item);
		}
	}
	void addToFrontIfNotAlreadyInList(T item) {
		if (!contains(item)) {
			push_front(item);
		}
	}
	T& front() {
		try {
			return internalList.front();
		}
		catch (exception ex) {
			throw runtime_error("This list has nothing in it, can't get anything out of it!");
		}
		return internalList.front();
	}
	T& back() {
		try {
			return internalList.back();
		}
		catch (exception ex) {
			throw runtime_error({ "This list has nothing in it, can't get anything out of it!" });
		}
		return internalList.front();
	}
	// add it if it's not there, remove it if it is there
	void toggle(T item) {
		if (!contains(item)) {
			push_back(item);
		}
		else {
			forcibleRemove(item);
		}
	}
	void forcibleRemove(T item) {
		list<T> result;
		for (auto x : internalList) {
			if (x != item) {
				result.push_back(x);
			}
		}
		internalList = result;
	}
	T begin() {
		return internalList.begin();
	}
	T end() {
		return internalList.end();
	}
	void remove_at(int index) {
		typename list<T>::iterator it = internalList.begin();
		advance(it, index);
		internalList.erase(it);
	}

	// returns list that is x -> the end, empty if the end is bigger than the whole list
	List<T> sublistXToEnd(int number) {
		List<T> result;
		for (int x = number; x < internalList.size(); x++) {
			result.push_back(at(x));
		}
		return result;
	}
	List<T> sublistStartToX(int number) {
		List<T> result;
		for (int x = 0; x < number && x < size(); x++) {
			result.push_back(at(x));
		}
		return result;
	}

	static List<string> filter(List<string> input, string tag) {
		List<string> result;
		for (auto x : input.internalList) {
			if (x.contains(tag)) {
				result.push_back(x);
			}
		}
		return result;
	}
	static List<string> quickMake(list<string> input) {
		List<string> result;
		for (auto x : input) {
			result.internalList.push_back(string{ "",x });
		}
		return result;
	}


	list<T> internalList;
};

template <typename A, typename B>
struct Map {
	B& operator[](A RHS) {
		return internalMap[RHS];
	}
	Map() {}
	Map (list<pair<A, B>> pairs){
		for (auto const& x : pairs) {
			internalMap[x.first] = x.second;
		}
	}
	Map(List<pair<A, B>> pairs) {
		for (auto x : pairs.internalList) {
			internalMap[x.first] = x.second;
		}
	}
	Map(pair<A, B> aPair) {
		internalMap[aPair.first] = aPair.second;
	}
	Map<A, B>& operator=(const Map<A, B>& RHS) {
		internalMap = RHS.internalMap;
		return *this;
	}
	void clear() {
		internalMap.clear();
	}
	bool empty() {
		return internalMap.empty();
	}
	List<B> getValues() {
		List<B> result;
		for (auto x : internalMap) {
			result.push_back(x.second);
		}
		return result;
	}
	List<A> getKeys() {
		List<A> result;
		for (auto x : internalMap) {
			result.push_back(x.first);
		}
		result.internalList.sort();
		return result;
	}
	A getKeyAssociatedWithThisValue(B RHS) {
		for (auto x : internalMap) {
			if (x.second == RHS) {
				return x.first;
			}
		}
		throw runtime_error("getKeyAssociatedWithThisValue found no key. Please check.");
		return A();
	}
	B getAnything() {
		for (auto x : internalMap) {
			return x.second;
		}
		return B();
	}
	bool hasValue(B x) {
		return (getValues().contains(x));
	}
	bool hasKey(A x) {
		return (getKeys().contains(x));
	}
	Map<A, B> operator +(const Map<A, B>& RHS) {
		Map<A, B> result = *this;
		for (auto x : RHS.internalMap) {
			result[x.first] = x.second;
		}
		return result;
	}
	int size() {
		return internalMap.size();
	}
	void add(pair<A, B> pair) {
		internalMap[pair.first] = pair.second;
	}
	map<A, B> internalMap;
};
