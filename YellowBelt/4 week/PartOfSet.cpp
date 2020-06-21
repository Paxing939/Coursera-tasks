#include <iostream>
#include <set>
#include <vector>

using namespace std;

template<typename T>
vector<T> FindGreaterElements(const set<T> &elements, const T &border) {
	vector<T> result;
	auto it = elements.begin();
	for (; it != elements.end(); ++it) {
		if (*it > border) {
			result.push_back(*it);
		}
	}
	return result;
}

int main() {
	for (int x : FindGreaterElements(set<int>{1, 5, 7, 8}, 5)) {
		cout << x << " ";
	}
	cout << endl;

	string to_find = "Python";
	cout << FindGreaterElements(set<string>{"C", "C++"}, to_find).size() << endl;
	return 0;
}
