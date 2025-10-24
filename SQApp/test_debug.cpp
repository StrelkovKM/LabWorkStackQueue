#include "TStack.h"
#include "TQueue.h"
#include "TMultiStack.h"

int main() {
	TMultiStack<int> M(9, 3);
	M.Put_M(0, 1);
	M.Put_M(0, 8);
	M.Put_M(2, 1);
	M.Put_M(2, 5);
	M.Put_M(2, 18);
	M.Put_M(2, 9); //Repack
	cout << M << " " << M.GetCapacity_M() << endl;
	M.SayStatusStack();
	M.PushStack_M(1, 2);
	M.Put_M(1, 2);
	M.Put_M(1, 1);
	cout << M << " " << M.GetCapacity_M() << endl;
	M.SayStatusStack();
	M.PopStack_M(1);
	cout << M << " " << M.GetCapacity_M() << endl;
	M.SayStatusStack();


}