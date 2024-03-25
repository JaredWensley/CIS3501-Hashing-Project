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
            int tempNum = rand() % 10;
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
     
        if (searchLinear(value, endindex)) {
            cout << "Search for: " << value << "\t" << " Found at index " << endindex << endl;
        }
        else {
            cout << "Search for: " << value << "\t" << " Not Found" << endl;
        }
    }
}




// HELPER FUNCTIONS------------------------------------------

// Finds the Homebucket using a mod function
int linear::hashFunction(int value) {
	return value % HASH_TABLE_SIZE;
}

void linear::LinearHashInsert(int value)
{
    int index = hashFunction(value);        // Finds the initial index in the table (Home bucket for this insert)
    int HomeBucket = index;                 // variable to keep track of the home bucket
    int distance = 0;                       // Keeps track of how far we are probed in the hash table from the home bucket
    bool direction = true;                  // true means probe to the right, false means probe to the left
    int probeDistance = 1;                  // Initializes the probing distance.

    // If first index occuplied increment the collision counter
    if (hashTable[index].keyValue != -1) {
        count.collisionCount++;
    }
    // If first index is -1 then the number is directly inserted. Increment the dirctinsert counter
    else {
        count.directInsertCount++;
    }


    // Loop through hash table until an empty spot is found or same value is found
    while (hashTable[index].keyValue != -1 && hashTable[index].keyValue != value) {
        
        // Calcuates the index by adding or subtracting the probedistance based on current direction(true or false)
        index = HomeBucket + (direction ? probeDistance : -probeDistance);

    

        // if the index is negative, go to the end of the hash table
        if (index < 0)
        {
            index = index + HASH_TABLE_SIZE;
        }

        // If index goes beyond table size, go to the begining of the hash table
        if (index >= HASH_TABLE_SIZE)
        {
            index = index - HASH_TABLE_SIZE;
        }

        // Increase counters
        count.collisionCount++;
        distance++;

        // If left was last searched (false) then the probe distance is increased by 1.
        if (!direction) {
            probeDistance++;
        }
        // Change direction to the opposite side. Left to right OR right to left.
        direction = !direction;

        // if probe distance is greater than half of table size then there will be no empty spaces, the table is full
        if (probeDistance > HASH_TABLE_SIZE / 2) {
            cerr << "Unable to find an empty spot in the hash table." << endl;
            return;
        }
    }
    
    // If there was at least one collision then increment the nondirectinsert counter
    if (distance > 0) {
        count.nonDirectInsertCount++;
   }

    // Keeps track of total distance probed. 
    count.totalProbingDistance = count.totalProbingDistance + distance;

    // If the loop ends because a spot with the same value was found, increase the count of that key value by 1.
    if (hashTable[index].keyValue == value) {
        hashTable[index].keyCount++;
        count.duplicateValueCount++;
    }
    // If the spot is empty -->( -1 )<-- insert the new value into the hash table :)
    else {
        hashTable[index] = hashNode{ value, 1 };
        count.uniqueValueCount++;
    }

    // If the probed distance is larger than the current largest, update the largest probing distance with the current distance
    if (distance > count.largestProbingDistance) count.largestProbingDistance = distance;
}

bool linear::searchLinear(int value, int &endindex) 
{
    int index = hashFunction(value);
    int originalIndex = index;
    int comparisons = 0;
    bool direction = true;
    int probeDistance = 1;

        while (hashTable[index].keyCount != 0) {
            comparisons++;
            if (hashTable[index].keyValue == value) {
                count.searchCount++;
                count.totalComparisons = count.totalComparisons + comparisons;
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
                count.totalComparisons = count.totalComparisons + comparisons;
                return false;
            }
        }
    

        count.totalComparisons = count.totalComparisons + comparisons;
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

void linear::PrintOperations() 
{
    int totalInserts = count.uniqueValueCount + count.duplicateValueCount;

    float percentDirectInserts = 0.0f;
    float percentNonDirectInserts = 0.0f;

    if (totalInserts > 0) {
        // Calculate the percentage of direct inserts
        percentDirectInserts = (100.0f * count.directInsertCount) / totalInserts;

        // Calculate the percentage of non-direct inserts
        percentNonDirectInserts = (100.0f * count.nonDirectInsertCount) / totalInserts;
    }

    float averageDistanceIncludingDirect = count.averageProbingDistance();
    float averageDistanceExcludingDirect = count.averageProbingDistanceExcludingDirect();

    // Print formatted metrics
    cout << endl << endl;
    cout << "Operation Counts" << endl;
    cout << "-----------------------------------" << endl;
    cout << setw(45) << "Linear" << endl;

    cout << left << setw(40) << "Number of key values inserted" << setw(15) << totalInserts << endl;
    cout << left << setw(40) << "Unique values" << setw(15) << count.uniqueValueCount << endl;
    cout << left << setw(40) << "Duplicate values" << setw(15) << count.duplicateValueCount << endl;

    cout << endl;
    cout << left << setw(40) << "Collisions" << endl;
    cout << left << setw(40) << "Number of collisions" << setw(15) << count.collisionCount << endl << endl;

    cout << left << setw(40) << "Distance from \"home bucket\" below"  << endl;
    cout << left << setw(40) << "Number of direct inserts" << setw(10) << count.directInsertCount << setw(5) << static_cast<int>(percentDirectInserts) << "%" << endl;
    cout << left << setw(40) << "Number of non-direct inserts" << setw(10) << count.nonDirectInsertCount << setw(5) << static_cast<int>(percentNonDirectInserts) << "%" << endl << endl;

    cout << left << setw(40) << "Average distance from home" << endl;
    cout << left << setw(40) << "including direct inserts" << setw(15) << averageDistanceIncludingDirect << endl;
    cout << left << setw(40) << "not-including direct inserts" << setw(15) << averageDistanceExcludingDirect << endl;

    cout << left << setw(40) << "Largest distance" << setw(15) << count.largestProbingDistance << endl;

    cout << endl;

    // Searches metrics will go here... 
    // For now, let's print placeholders
    cout << left << setw(40) << "Searches" << endl;
    cout << left << setw(40) << "Number of searches" << setw(15) << "vvv" << endl;
    cout << left << setw(40) << "Number of comparisons" << setw(15) << "xxx" << endl;
    cout << left << setw(40) << "Total number of comparisons" << setw(15) << "zzz" << endl;
    cout << left << setw(40) << "Number of direct accesses" << setw(15) << "bcd" << endl;
    cout << left << setw(40) << "Total number of accesses" << setw(15) << "def" << endl;
    cout << left << setw(40) << "Total number of comparisons" << setw(15) << "ghi" << endl;
    cout << left << setw(40) << "Average number of comparisons" << setw(15) << "ijk" << endl;
    cout << left << setw(40) << "Largest number of comparisons" << setw(15) << "klm" << endl;
}