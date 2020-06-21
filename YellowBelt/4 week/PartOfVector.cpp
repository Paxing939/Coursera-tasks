#include <iostream>
#include <vector>

using namespace std;

void PrintVectorPart(const vector<int>& numbers) {
	auto it = numbers.begin();
	for (; it != numbers.end(); ++it) {
		if (*it < 0) {
			break;
		}
	}
	if (it != numbers.begin()) {
		it--;
	} else {
		return;
	}
	for (; it != numbers.begin(); --it) {
		cout << *it << " ";
	}
	cout << *numbers.begin();
}

int main() {
	PrintVectorPart({6, 1, 8, -5, 4});
	cout << endl;
	PrintVectorPart({-6, 1, 8, -5, 4});  // ничего не выведется
	cout << endl;
	PrintVectorPart({6, 1, 8, 5, 4});
	cout << endl;
	return 0;
}
