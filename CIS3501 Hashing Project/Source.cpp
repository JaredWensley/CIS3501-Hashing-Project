#include "Hashing.h"


int main() {
   

    string Method;
    linear A;

    cout << "Do you want to use a file or the random number generator for hashing values?" << endl;
    cout << "type file or random: " << endl;
    cin >> Method;
    cout << endl;

    A.processMethod(Method);
    A.printHashTable();
    A.SearchItem();
    A.PrintOperations();
   

    // Metrics can be printed here as required by your assignment specification.
    return 0;
}