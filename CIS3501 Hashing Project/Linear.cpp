#include "Hashing.h"



linear::linear() {
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        hashTable[i] = hashNode();
    }
}

void linear::processMethod(string method) {

    srand(time(nullptr)); // Seed the random number generator 

    if (method == "file")
    {
        string file;
        cout << "Enter file name excluding the .txt: ";
        file = file + ".txt";
        getline(cin, file);
        fileprocess(file);
    }
    else if (method == "random")
    {
        for (int i = 0; i < HASH_TABLE_SIZE; ++i) {
            int tempNum = rand() % 100;
            testNumbers.push(tempNum);
            SearchQueue.push(tempNum);
        }
    }
    else
    {
        cerr << "something is wrong in getNumberMethod";
        return;
    }

    // insert into actual hashtable
    while (!testNumbers.empty()) {
        int value = testNumbers.front();
        testNumbers.pop();
        LinearHashInsert(value);
    }
}

void linear::SearchItem() 
{
    cout << "SEARCHE FOR VALUES" << endl << "-----------------------------------" << endl;


    int endindex;
    while (!SearchQueue.empty()) {
        int value = SearchQueue.front();
        SearchQueue.pop();
     
        if (searchLinear(value, endindex, searchCount, totalComparisons)) {
            cout << "Search for: " << value << "\t" << " Found at index " << endindex << endl;
        }
        else {
            cout << "Search for: " << value << "\t" << " Not Found" << endl;
        }
    }
}




// HELPER FUNCTIONS------------------------------------------

int linear::hashFunction(int value) {
	return value % HASH_TABLE_SIZE;
}

void linear::LinearHashInsert(int value)
{
    int index = hashFunction(value);
    int originalIndex = index;
    int distance = 0;
    bool direction = true;  // true means probe to the right, false means probe to the left
    int probeDistance = 1;

    while (hashTable[index].keyValue != -1 && hashTable[index].keyValue != value) {

        index = originalIndex + (direction ? probeDistance : -probeDistance);

        if (index < 0)
        {
            index = index + HASH_TABLE_SIZE;
        }

        if (index >= HASH_TABLE_SIZE)
        {
            index = index - HASH_TABLE_SIZE;
        }

        //collisionCount++;
        distance++;

        if (!direction) {
            probeDistance++;
        }
        direction = !direction;

        if (probeDistance > HASH_TABLE_SIZE / 2) {
            cerr << "Unable to find an empty spot in the hash table." << endl;
            return;
        }
    }

    if (hashTable[index].keyValue == value) {
        hashTable[index].keyCount++;
        duplicateInserts++;
    }
    else {
        hashTable[index] = hashNode{ value, 1 };
        uniqueInserts++;
    }

    if (distance > largestDistance) largestDistance = distance;
}

bool linear::searchLinear(int value, int &endindex, int& searchCount, int& totalComparisons) 
{
    int index = hashFunction(value);
    int originalIndex = index;
    int comparisons = 0;
    bool direction = true;
    int probeDistance = 1;

        while (hashTable[index].keyCount != 0) {
            comparisons++;
            if (hashTable[index].keyValue == value) {
                searchCount++;
                totalComparisons += comparisons;
                endindex = index;
                return true;
            }

            index = originalIndex + (direction ? probeDistance : -probeDistance);
            if (index < 0) index += HASH_TABLE_SIZE;
            if (index >= HASH_TABLE_SIZE) index -= HASH_TABLE_SIZE;

            if (!direction) {
                probeDistance++;
            }
            direction = !direction;

            if (probeDistance > HASH_TABLE_SIZE / 2) {
                totalComparisons += comparisons;
                return false;
            }
        }
    

    totalComparisons = totalComparisons + comparisons;
    return false;
}

void linear::printHashTable() {
   
    cout << "INSERTS" << endl << "-----------------------------------" << endl;

    for (int i = 0; i < HASH_TABLE_SIZE; ++i) {
        cout << "Bucket " << i << "\t";

        if (hashTable[i].keyCount > 0) { // Check if the slot in the hash table is occupied
            cout << hashTable[i].keyValue << "\t"  << "Count: " << hashTable[i].keyCount << endl;
        }
        else {
            cout << "NULL" << "\t" << "Count: 0" << endl; // Indicate an empty slot
        }
    }
}

// Helper function to getNumberMethod
void linear::fileprocess(string filename) {

    ifstream insertfile(filename);	//Input file stream
    string line;					// String to hold each line of input
    int lineNumber = 0;				// Track line numbers for error messages

    // Check if the file can open
    if (!insertfile.is_open()) {
        cerr << "Error opening file: " + filename << endl;
        return; // Stop the operation
    }

    // Check if the file is empty
    if (insertfile.peek() == ifstream::traits_type::eof()) {
        cerr << "Error: The file " + filename + " is empty, please enter a different file" << endl;
        return; // Stop the operation
    }

    // Read from the file line by line
    while (getline(insertfile, line)) {
        lineNumber++; // Increment line number

        // Use a stringstream to read from the line
        stringstream ss(line);

        // Attempt to read a number from the line
        int num; //temp storage
        if (!(ss >> num)) {
            // If extraction fails, report an error and continue to the next line
            cerr << "Error: Non-integer data found on line " << lineNumber << " in the InsertFile " << filename << ". Skipping line." << endl;
            continue;
        }

        // Check if there's anything else on the line after the integer
        string extra;
        if (ss >> extra) {
            // If extra data is found after the integer, report an error and continue to the next line
            cerr << "Error: More than one token found on line " << lineNumber << " in the Insertfile " << filename << ". Skipping line." << endl;
            continue;
        }

        // Put 
        testNumbers.push(num); 
        SearchQueue.push(num);

    }
}

void linear::PrintOperations() {

}