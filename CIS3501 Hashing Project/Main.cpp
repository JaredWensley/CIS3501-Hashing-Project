#include "Hashing.h"


int main() {
   
    string title;
    string Method;
    linear A;

    cout << "Enter name of this test: ";
    getline(cin, title);
    cout << endl;

    while (Method != "file" && Method != "f" && Method != "random" && Method != "r") {
        cout << "Do you want to use a file or the random number generator for hashing values?" << endl;
        cout << "type file or random: " << endl;
        getline(cin, Method);
        cout << endl;
        if (Method != "file" && Method != "f" && Method != "random" && Method != "r") {
            cout << "Error input, try again. 'r' for random and 'f' for file will work too.";
        }
    }

    string outputfilename;
    cout << "Note, file can not be made if the file name contains: / \\ : ? * \" < > |" << endl;
    cout << "Enter output file name excluding the .txt: ";
    getline(cin, outputfilename);
    outputfilename = outputfilename + ".txt";
    cout << endl;
    ofstream output(outputfilename);

    A.processMethod(Method, output);
    A.printHashTables(output, title);
    A.SearchItem();
    A.PrintOperations(output);
   

    // Metrics can be printed here as required by your assignment specification.
    return 0;
}