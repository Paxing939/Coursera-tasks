#include <string>
#include <vector>
using namespace std;

// Реализуйте этот макрос так, чтобы функция main компилировалась
#define ONE_MORE_MACROS(a, line) a##line
#define UNIQ_ID_REALIZATION(x, y) ONE_MORE_MACROS(x, y)
#define UNIQ_ID UNIQ_ID_REALIZATION(a, __LINE__)

int main() {
  int UNIQ_ID = 0;
  string UNIQ_ID = "hello";
  vector<string> UNIQ_ID = {"hello", "world"};
  vector<int> UNIQ_ID = {1, 2, 3, 4};
}