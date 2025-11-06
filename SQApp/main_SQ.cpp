#include "TStack.h"
#include "TQueue.h"
#include "TMultiStack.h"

int main() {
    TMultiStack<char> d(3, 3);
    d.Push(0, 'a');
    d.Push(0, 'b');
    d.Push(0, 'c');
    d.Push(1, 'f');
    d.Push(1, 'g');
    d.Push(1, 'h');
    d.Push(2, 'j');
    std::cout << d << std::endl;
    d.Push(1, 'k');
    d.Push(0, 'm');
    std::cout << d;
    return 0;
}