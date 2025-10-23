#include "TStack.h"
#include "TQueue.h"
#include "TMultiStack.h"

int main() {
	TMultiStack<int> M(30, 3);
	M.PutM(0, 1);
	M.PutM(0, 8);
	M.PutM(2, 1);
	M.PutM(2, 5);
	M.PutM(2, 18);
	cout << M;
}