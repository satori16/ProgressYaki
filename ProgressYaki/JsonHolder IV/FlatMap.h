#pragma once

template<class Key, class T>
class FlatMap {
public:
	typedef std::pair<Key, T> Item;
	typedef std::vector<Item> Map;

	FlatMap() {}
	FlatMap(const std::initializer_list<Item>& IL) :M(IL) {}
	template<class it>
	FlatMap(it F, it E) : M(F, E) {}

	Item& at(const Key& k) {
		auto It = std::find_if(M.begin(), M.end(), [&](auto& o) {return k == o; });
		if (It == M.end()) throw std::out_of_range("FlatMap::at()");
		return *It;
	}
	typename Map::iterator find(const Key& k) {
		return std::find_if(M.begin(), M.end(), [&](auto& o) {return k == o; });
	}
	template<class I>
	typename Map::iterator find(const I& k) {
		return std::find_if(M.begin(), M.end(), [&](auto& o) {return k == o; });
	}
	typename Map::const_iterator find(const Key& k)const {
		return std::find_if(M.cbegin(), M.cend(), [&](auto& o) {return k == o; });
	}
	template<class I>
	typename Map::const_iterator find(const I& k)const {
		return std::find_if(M.cbegin(), M.cend(), [&](auto& o) {return k == o; });
	}
	typename Map::iterator  erase(typename Map::const_iterator it) {
		return M.erase(it);
	}
	typename Map::iterator erase(const Key& k) {
		return M.erase(std::find_if(M.begin(), M.end(), [&](auto& o) {return k == o; }));
	}
	bool insert(const Item& I) {
		if (find(I.first) == M.end()) return false;
		M.push_back(I);
	}
	typename Map::iterator  insert(typename Map::const_iterator Pos, const Item& I) {
		if (find(I.first) == M.end()) return false;
		return M.insert(Pos, I);
	}
	bool clear() {
		M.clear();
		return true;
	}
	std::size_t size() {
		return M.size(); 
	}
	std::size_t max_size() {
		return M.max_size();
	}
	bool empty() {
		return M.size() == 0;
	}
	Item& operator[](const std::size_t& Idx) {
		return M[Idx];
	}
	T& operator[](const Key& Idx) {
		auto It = find(Idx);
		return It != M.end() ? It.second : T();
	}
	typename Map::iterator begin() {
		return M.begin();
	}
	typename Map::iterator end() {
		return M.end();
	}
	typename Map::const_iterator cbegin() {
		return M.begin();
	}
	typename Map::const_iterator cend() {
		return M.end();
	}
	typename Map::reverse_iterator rbegin() {
		return M.rbegin();
	}
	typename Map::reverse_iterator rend() {
		return M.rend();
	}
	typename Map::const_reverse_iterator crbegin() {
		return M.rbegin();
	}
	typename Map::const_reverse_iterator crend() {
		return M.rend();
	}
	bool push_back(const Item& I) {
		M.push_back(I);
		return true;
	}
	bool pop_back() {
		M.pop_back();
		return true;
	}
protected:
	Map M;
};