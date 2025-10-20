#include "TStack.h"
#include "TQueue.h"

int main() {
	TQueue<int> N(7);
	if (N.IsEmpty()) cout << 1 << endl;
	for (auto i = 0; i < 7; i++) N.Put(i); // 0, 1, 2 ,3, 4, 5, 6
	cout << N << endl;
	for (auto i = 0; i < 4; i++) N.Get();  // _, _, _, _, 4, 5, 6
	cout << N << endl;
	for (auto i = 0; i < 4; i++) N.Put(i); //  4, 5, 6, 0, 1, 2, 3
	cout << N << endl;
	if (N.IsEmpty()) cout << 1 << endl;
	if (N.IsFull()) cout << 1 << endl;
}