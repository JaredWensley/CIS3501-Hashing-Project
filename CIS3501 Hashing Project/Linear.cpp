#include "Hashing.h"



linear::linear() {
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        hashTable[i] = hashNode();
    }
}

void linear::processMethod(string method, ofstream& output) {
    bool linearisFull = true;
    bool overflowisFull = true;
    
    srand(time(nullptr)); // Seed the random number generator 

    if (method == "file")
    {
        string file;
        
        cout << "Enter file name excluding the .txt: ";
        getline(cin, file);
        file = file + ".txt";
        fileprocess(file, output);
    }
    else if (method == "random")
    {
       
        while (linearisFull || overflowisFull) {

            int tempNum = rand() % 100;
            if (linearisFull) {
                LinearHashInsert(tempNum, output, linearisFull);
            }
            //else if (!linearisFull) continue;

            if (overflowisFull) {
                overflowinsert(tempNum, overflowisFull);
            }
           // else if (!overflowisFull) continue;


            SearchQueue.push(tempNum);
        }
          
        
    }
    else
    {
        cerr << "something is wrong in getNumberMethod";
        return;
    }
   
}

void linear::SearchItem() 
{
  
    int endindex;
    while (!SearchQueue.empty()) {
        int value = SearchQueue.front();
        SearchQueue.pop();
     
        searchLinear(value);
    }
}




// HELPER FUNCTIONS------------------------------------------

// Finds the Homebucket using a mod function
int linear::hashFunction(int value) {
	return value % HASH_TABLE_SIZE;
}

void linear::LinearHashInsert(int value, ofstream& outputfile, bool& isFull)
{
    int index = hashFunction(value);        // Finds the initial index in the table (Home bucket for this insert)
    int HomeBucket = index;                 // variable to keep track of the home bucket
    int distance = 0;                       // Keeps track of how far we are probed in the hash table from the home bucket
    bool direction = true;                  // true means probe to the right, false means probe to the left
    int probeDistance = 1;                  // Initializes the probing distance.


    // If the loop ends because a spot with the same value was found, increase the count of that key value by 1.
    if (hashTable[index].keyValue == value) {
        // This is a duplicate value found directly in its home bucket
        hashTable[index].keyCount++;
        count.duplicateValueCount++;
        count.directInsertCount++; 
        return;
    }
    // If the spot is empty -->( -1 )<-- insert the new value into the hash table :)
    else if (hashTable[index].keyValue == -1) {
        // Home bucket is empty, this is a unique direct insert
        
        // Insertion happens here
        hashTable[index] = hashNode(value, 1, -1);
        count.uniqueValueCount++;
        count.directInsertCount++;
        return;
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
            isFull = false;
            return;
        }
    }
    
    // If there was at least one collision then increment the nondirectinsert counter
    if (hashTable[index].keyValue == value) {
        hashTable[index].keyCount++;
        count.duplicateValueCount++;
        count.inDirectInsertCount++;
    }
    else {
        hashTable[index] = hashNode(value, 1, -1);
        count.uniqueValueCount++;
        count.inDirectInsertCount++;
    }

    // Keeps track of total distance probed. 
    count.totalProbingDistance = count.totalProbingDistance + distance;

  
    

    
    // If the probed distance is larger than the current largest, update the largest probing distance with the current distance
    count.updateLargestProbingdist(distance);
}

void linear::overflowinsert(int value, bool& isFull) 
{
    int index = hashFunction(value);

    // Check if the spot in the primary array is empty or a duplicate key
    if (ChainPrimary[index].keyValue == -1) {
        // Insert key directly
        count.OVuniqueValueCount++;
        count.OVdirectInsertCount++;
        ChainPrimary[index] = hashNode(value, 1, -1);
        return;
    }
    else if (ChainPrimary[index].keyValue == value) {
        // Handle duplicate key as needed (e.g., increment count)
        count.OVdirectInsertCount++;
        count.OVduplicateValueCount++;
        ChainPrimary[index].keyCount++;
        return;
    }
    else {
        // Collision: Look for an empty spot starting from the chain for this bucket
        int chainIndex = ChainPrimary[index].nextIndex;     // Start of the chain
        if (chainIndex == -1) {
            chainIndex = nextOpenIndex;
            count.OVcollisionCount++;
        }
        int lastChainIndex = nextOpenIndex;                 // Will store the last index in the chain

        // Find the next aviably spot in he 
        while (chainIndex != -1) 
        {   //Duplicate found
            if (OverFlow[chainIndex].keyValue == value) {
                OverFlow[chainIndex].keyCount++;
                count.OVduplicateValueCount++;
                count.OVinDirectInsertCount++;
                return;
            }

            // iterate through the overflow array, increament collision
            lastChainIndex = chainIndex;
            chainIndex = OverFlow[chainIndex].nextIndex;
            count.OVcollisionCount++;
        }

       
        // Find a free spot in the overflow array, should be instant because newoverflowindex is the next 
        int newOverflowIndex = findNextAvailableOverflowIndex(nextOpenIndex); 
        
        // triggers if the overflow array is full
        if (newOverflowIndex == -1) {
            isFull = false;
            return;
        }

        // Insert new key into the overflow array
        OverFlow[newOverflowIndex] = hashNode(value, 1, -1);
        count.OVinDirectInsertCount++;
        count.OVuniqueValueCount++;

        // Finds the next free space starting at nextopenindex so it should be instant
        nextOpenIndex = findNextAvailableOverflowIndex(newOverflowIndex); 

        if (ChainPrimary[index].nextIndex == -1) {
            // No previous chain entry; make the new node the start of the chain
            ChainPrimary[index].nextIndex = newOverflowIndex;
        }
        else {
            // Point the last chain entry's nextIndex to the new node
            OverFlow[lastChainIndex].nextIndex = newOverflowIndex;
        }
    }
}

int linear::findNextAvailableOverflowIndex(int& nextOpenIndex) 
{
    for (int i = nextOpenIndex; i < HASH_TABLE_SIZE; ++i) {
            if (OverFlow[i].keyValue == -1) {
                return i;
            }
            count.OVcollisionCount++;
    }
    return -1; // Indicates overflow space is full
}

bool linear::searchLinear(int value) 
{
    int index = hashFunction(value);
    int originalIndex = index;
    int comparisons = 1;        // increment at first for initinal comparision
    bool direction = true;
    int probeDistance = 1;

    if (hashTable[index].keyValue == value) {
        count.directAccesses++;
        count.searchCount++;
        count.totalComparisons = count.totalComparisons + comparisons;
        count.updateLargestComparisons(comparisons);
        return true;
    }


        while (hashTable[index].keyCount != 0) {
            comparisons++;
            if (hashTable[index].keyValue == value) {
                count.searchCount++;
                count.totalComparisons = count.totalComparisons + comparisons;
                count.updateLargestComparisons(comparisons);
                count.indirectAccesses++;
                return true;
            }

            index = originalIndex + (direction ? probeDistance : -probeDistance);
            if (index < 0) {
                index = index + HASH_TABLE_SIZE;
            }
            if (index >= HASH_TABLE_SIZE) {
                index = index - HASH_TABLE_SIZE;
            }
            if (!direction) {
                probeDistance++;
            }
            direction = !direction;

            if (probeDistance > HASH_TABLE_SIZE / 2) {
                count.searchCount++;
                count.totalComparisons = count.totalComparisons + comparisons;
                count.updateLargestComparisons(comparisons);
                return false;
            }
        }
    
        count.searchCount++;
        count.totalComparisons = count.totalComparisons + comparisons;
        count.updateLargestComparisons(comparisons);
    return false;
}

bool linear::searchOverflow(int value) 
{
    int comparisons = 1;
    int index = hashFunction(value);

    if (ChainPrimary[index].keyValue == value) {
        count.OVdirectAccesses++;
        count.OVtotalComparisons = count.OVtotalComparisons + comparisons;
        return true;
    }
    int searchindex = ChainPrimary[index].nextIndex;
}

void linear::printHashTables(ofstream& outputfile, string title) {
   
    cout << "\t" << "\t" << title << endl;
    cout << "   Quadratic Open Addressing Hash Table" << endl;
    cout << "-------------------------------------" << endl;
    cout << "Index " << "\t" << "\t" << "Key " << "\t" << "\t" << "count " << endl;
   


    for (int i = 0; i < HASH_TABLE_SIZE; ++i) {
        cout << i << "\t" << "\t";
        outputfile << i << "\t" << "\t";

        if (hashTable[i].keyCount > 0) { // Check if the slot in the hash table is occupied
            cout << hashTable[i].keyValue << "\t" << "\t" << hashTable[i].keyCount << endl;
            outputfile << hashTable[i].keyValue << "\t" << "\t" << hashTable[i].keyCount << endl;
        }
        else {
            // This should never be used in an ideal run.
            cout << "NULL" << "\t" << "\t" << "0" << endl; // Indicate an empty slot
            outputfile << "NULL" << "\t" << "\t" << "0" << endl; // Indicate an empty slot
        }
    }

    cout << endl; 
    
    // START OF OVERFLOW HASHING 
    cout << "\t" << "\t" << title << endl;
    cout << "   Overflow chain Hashing" << endl;
    cout << "-----------------------------------" << endl;
    cout << "Index " << "\t" << "\t" << "Key " << "\t" << "\t" << "count " << endl;



    for (int i = 0; i < HASH_TABLE_SIZE; ++i) {
        cout << i << "\t" << "\t";
        outputfile << i << "\t" << "\t";

        if (ChainPrimary[i].keyCount > 0) { // Check if the slot in the hash table is occupied
            cout << ChainPrimary[i].keyValue << "\t" << "\t" << ChainPrimary[i].keyCount << endl;
            outputfile << ChainPrimary[i].keyValue << "\t" << "\t" << ChainPrimary[i].keyCount << endl;
        }
        else {
            // This should never be used in an ideal run.
            cout << "NULL" << "\t" << "\t" << "0" << endl; // Indicate an empty slot
            outputfile << "NULL" << "\t" << "\t" << "0" << endl; // Indicate an empty slot
        }
    }

    cout << "\t" << "\t" << title << endl;
    cout << "   Overflow Array" << endl;
    cout << "-----------------------------------" << endl;
    cout << "Index " << "\t" << "\t" << "Key " << "\t" << "\t" << "count " << endl;



    for (int i = 0; i < HASH_TABLE_SIZE; ++i) {
        cout << i << "\t" << "\t";
        outputfile << i << "\t" << "\t";

        if (OverFlow[i].keyCount > 0) { // Check if the slot in the hash table is occupied
            cout << OverFlow[i].keyValue << "\t" << "\t" << OverFlow[i].keyCount << endl;
            outputfile << OverFlow[i].keyValue << "\t" << "\t" << OverFlow[i].keyCount << endl;
        }
        else {
            // This should never be used in an ideal run.
            cout << "NULL" << "\t" << "\t" << "0" << endl; // Indicate an empty slot
            outputfile << "NULL" << "\t" << "\t" << "0" << endl; // Indicate an empty slot
        }
    }
}


// Helper function to getNumberMethod
void linear::fileprocess(string filename, ofstream& outputfile) {

    ifstream insertfile(filename);	//Input file stream
    string line;					// String to hold each line of input
    int lineNumber = 0;				// Track line numbers for error messages

    // Check if the file can open
    if (!insertfile.is_open()) {
        cerr << "Error opening file: " + filename << endl;
        outputfile << "Error opening file: " + filename << endl;
        return; // Stop the operation
    }

    // Check if the file is empty
    if (insertfile.peek() == ifstream::traits_type::eof()) {
        cerr << "Error: The file " + filename + " is empty, please enter a different file" << endl;
        outputfile << "Error: The file " + filename + " is empty, please enter a different file" << endl;
        return; // Stop the operation
    }

    bool isfull = true;
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
            outputfile << "Error: Non-integer data found on line " << lineNumber << " in the InsertFile " << filename << ". Skipping line." << endl;
            continue;
        }

        // Check if there's anything else on the line after the integer
        string extra;
        if (ss >> extra) {
            // If extra data is found after the integer, report an error and continue to the next line
            cerr << "Error: More than one token found on line " << lineNumber << " in the Insertfile " << filename << ". Skipping line." << endl;
            outputfile << "Error: More than one token found on line " << lineNumber << " in the Insertfile " << filename << ". Skipping line." << endl;
            continue;
        }

        // Put 

        LinearHashInsert(num, outputfile, isfull);
       // overflowHashInsert();
        if(isfull){
            SearchQueue.push(num);
        }
        else {
            break;
        }
     

        

    }
}

void linear::PrintOperations(ofstream& outputfile)
{
    int totalInserts = count.uniqueValueCount + count.duplicateValueCount;
    int OVtotalInserts = count.OVuniqueValueCount + count.duplicateValueCount;
    int totalaccesses = count.directAccesses + count.indirectAccesses;
    int OVtotalaccesses = count.OVdirectAccesses + count.OVindirectAccesses;

    float percentDirectInserts = 0.0f;
    float percentNonDirectInserts = 0.0f;

    if (totalInserts > 0) {
        // Calculate the percentage of direct inserts
        percentDirectInserts = (100.0f * count.directInsertCount) / totalInserts;

        // Calculate the percentage of non-direct inserts
        percentNonDirectInserts = (100.0f * count.inDirectInsertCount) / totalInserts;
    }

    // THESE ARENT WORKING FOR SOME REASON
    float averageDistanceIncludingDirect = count.averageProbingDistance();
    float averageDistanceExcludingDirect = count.averageProbingDistanceExcludingDirect();
    

    // Print formatted metrics
    cout << endl << endl;
    cout << "Operation Counts" << endl;
    cout << "-----------------------------------" << endl;
    cout << setw(45) << "Linear" << setw(15) << "OverFlow" << endl;

    cout << left << setw(40) << "Number of key values inserted" << setw(15) << totalInserts << setw(10) << "12" << endl;
    cout << left << setw(40) << "Unique values" << setw(15) << count.uniqueValueCount << setw(10) << "12" << endl;
    cout << left << setw(40) << "Duplicate values" << setw(15) << count.duplicateValueCount << setw(10) << "12" << endl;

    cout << endl;
    cout << left << setw(40) << "Collisions" << endl;
    cout << left << setw(40) << "Number of collisions" << setw(15) << count.collisionCount << setw(10) << "12" << endl << endl;

    cout << left << setw(40) << "Number of direct inserts" << count.directInsertCount << " - " << setprecision(4) << (percentDirectInserts) << "%    " << setw(13) << "12" << endl;
    cout << left << setw(40) << "Number of non-direct inserts"  << count.inDirectInsertCount << " - " << setprecision(4) << (percentNonDirectInserts) << "%    " << setw(13) << "12" << endl << endl;

    cout << left << setw(40) << "Average distance from home" << endl;
    cout << left << setw(40) << "including direct inserts" << setw(15) << averageDistanceIncludingDirect << setw(10) << "12" << endl;
    cout << left << setw(40) << "not-including direct inserts" << setw(15) << averageDistanceExcludingDirect << setw(10) << "12" << endl;

    cout << left << setw(40) << "Largest distance" << setw(15) << count.largestProbingDistance << setw(10) << "12" << endl;

    cout << endl;

   
    cout << left << setw(40) << "Searches" << endl;
    cout << left << setw(40) << "Number of searches" << setw(15) << count.searchCount << setw(10) << "12" << endl;
    cout << left << setw(40) << "Number of comparisons" << setw(15) << count.totalComparisons << setw(10) << "12" << endl;
    cout << left << setw(40) << "Number of direct accesses" << setw(15) << count.directAccesses << setw(10) << "12" << endl;
    cout << left << setw(40) << "Number of indirect accesses" << setw(15) << count.indirectAccesses << setw(10) << "12" << endl;
    cout << left << setw(40) << "Total number of accesses" << setw(15) << totalaccesses << setw(10) << "12" << endl;
    cout << left << setw(40) << "Average number of comparisons" << setw(15) << count.averageComparisons() << setw(10) << "12" << endl;
    cout << left << setw(40) << "Largest number of comparisons" << setw(15) << count.largestComparisons << setw(10) << "12" << endl;
    



    outputfile << endl << endl;
    outputfile << "Operation Counts" << endl;
    outputfile << "-----------------------------------" << endl;
    outputfile << setw(45) << "Linear" << endl;

    outputfile << left << setw(40) << "Number of key values inserted" << setw(15) << totalInserts << endl;
    outputfile << left << setw(40) << "Unique values" << setw(15) << count.uniqueValueCount << endl;
    outputfile << left << setw(40) << "Duplicate values" << setw(15) << count.duplicateValueCount << endl;

    outputfile << endl;
    outputfile << left << setw(40) << "Collisions" << endl;
    outputfile << left << setw(40) << "Number of collisions" << setw(15) << count.collisionCount << endl << endl;

    outputfile << left << setw(40) << "Distance from \"home bucket\" below" << endl;
    outputfile << left << setw(40) << "Number of direct inserts" << setw(10) << count.directInsertCount << setw(5) << static_cast<int>(percentDirectInserts) << "%" << endl;
    outputfile << left << setw(40) << "Number of non-direct inserts" << setw(10) << count.inDirectInsertCount << setw(5) << static_cast<int>(percentNonDirectInserts) << "%" << endl << endl;

    outputfile << left << setw(40) << "Average distance from home" << endl;
    outputfile << left << setw(40) << "including direct inserts" << setw(15) << averageDistanceIncludingDirect << endl;
    outputfile << left << setw(40) << "not-including direct inserts" << setw(15) << averageDistanceExcludingDirect << endl;

    outputfile << left << setw(40) << "Largest distance" << setw(15) << count.largestProbingDistance << endl;

    outputfile << endl;

    // Search metrics
    outputfile << left << setw(40) << "Searches" << endl;
    outputfile << left << setw(40) << "Number of searches" << setw(15) << count.searchCount << endl;
    outputfile << left << setw(40) << "Number of comparisons" << setw(15) << count.totalComparisons << endl;
    outputfile << left << setw(40) << "Number of direct accesses" << setw(15) << count.directAccesses << endl;
    outputfile << left << setw(40) << "Number of indirect accesses" << setw(15) << count.indirectAccesses << endl;
    outputfile << left << setw(40) << "Total number of accesses" << setw(15) << totalaccesses << endl;
    outputfile << left << setw(40) << "Average number of comparisons" << setw(15) << count.averageComparisons() << endl;
    outputfile << left << setw(40) << "Largest number of comparisons" << setw(15) << count.largestComparisons << endl;
}