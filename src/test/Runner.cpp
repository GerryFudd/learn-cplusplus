#include <iostream>
#include <stdexcept>
#include "utilities/Assertions.cpp"

using namespace ASSERT;
using namespace std;

int main()
{
    try {
        cout << "\nSomething ran!";
        assertTrue(false);
    } catch(runtime_error e) {
        cout << "\nSomething is awry!";
        cout << "\nError: " << e.what();
    }
    return 0;
}
