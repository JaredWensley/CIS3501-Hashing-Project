#include "Hashing.h"



TableFunctions::TableFunctions() {
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        hashTable[i] = hashNode();
    }
}

// performs all inserts calls for both hash tables based on the type of method, 
void TableFunctions::processMethod(string method, ofstream& output) {
    bool linearisFull = true;
    bool overflowisFull = true;
    
    srand(time(nullptr)); // Seed the random number generator 

    if (method == "file" || method == "f")
    {
        string file;
        
        cout << "Enter file name excluding the .txt: ";
        getline(cin, file);
        file = file + ".txt";
        fileprocess(file, output);
    }
    else if (method == "random" || method == "r")
    {
       
        while (linearisFull || overflowisFull) {

            int tempNum = rand() % 100;
            if (linearisFull) {
                LinearHashInsert(tempNum, output, linearisFull);
            }
        

            if (overflowisFull) {
                overflowinsert(tempNum, overflowisFull);
            }
         
            SearchQueue.push(tempNum);
        }
          
        
    }
    else
    {
        cerr << "something is wrong in getNumberMethod";
        return;
    }
   
}

//Searches for an item in both tables
void TableFunctions::SearchItem()
{
  
    
    while (!SearchQueue.empty()) {
        int value = SearchQueue.front();
       
        SearchQueue.pop();
     
        searchLinear(value);
        searchOverflow(value);
    }
}

// Prints the hashtables
void TableFunctions::printHashTables(ofstream& outputfile, string title) {

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
            cout << ChainPrimary[i].keyValue << "\t" << "\t" << ChainPrimary[i].keyCount << "\t" << ChainPrimary[i].nextIndex << endl;
            outputfile << ChainPrimary[i].keyValue << "\t" << "\t" << ChainPrimary[i].keyCount << "\t" << ChainPrimary[i].nextIndex << endl;
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
            cout << OverFlow[i].keyValue << "\t" << "\t" << OverFlow[i].keyCount << "\t" << OverFlow[i].nextIndex << endl;
            outputfile << OverFlow[i].keyValue << "\t" << "\t" << OverFlow[i].keyCount << "\t" << OverFlow[i].nextIndex << endl;
        }
        else {
            // This should never be used in an ideal run.
            cout << "NULL" << "\t" << "\t" << "0" << endl; // Indicate an empty slot
            outputfile << "NULL" << "\t" << "\t" << "0" << endl; // Indicate an empty slot
        }
    }
}

//Prints metrics and operations for both hashtables
void TableFunctions::PrintOperations(ofstream& outputfile)
{
    int totalInserts = count.uniqueValueCount + count.duplicateValueCount;
    int OVtotalInserts = count.OVuniqueValueCount + count.duplicateValueCount;
    int totalaccesses = count.directAccesses + count.indirectAccesses;
    int OVtotalaccesses = count.OVdirectAccesses + count.OVindirectAccesses;

    float percentDirectInserts = 0.0f;
    float percentNonDirectInserts = 0.0f;
    float OVpercentDirectInserts = 0.0f;
    float OVpercentNonDirectInserts = 0.0f;

    if (totalInserts > 0) {
        // Calculate the percentage of direct inserts
        percentDirectInserts = (100.0f * count.directInsertCount) / totalInserts;

        // Calculate the percentage of non-direct inserts
        percentNonDirectInserts = (100.0f * count.inDirectInsertCount) / totalInserts;
    }

    if (OVtotalInserts > 0) {
        // Calculate the percentage of direct inserts
        OVpercentDirectInserts = (100.0f * count.OVdirectInsertCount) / OVtotalInserts;

        // Calculate the percentage of non-direct inserts
        OVpercentNonDirectInserts = (100.0f * count.OVinDirectInsertCount) / OVtotalInserts;
    }

    // THESE ARENT WORKING FOR SOME REASON
    float averageDistanceIncludingDirect = count.averageProbingDistance();
    float averageDistanceExcludingDirect = count.averageProbingDistanceExcludingDirect();
    float OVaverageDistanceIncludingDirect = count.OVaverageProbingDistance();
    float OVaverageDistanceExcludingDirect = count.OVaverageProbingDistanceExcludingDirect();


    // Print formatted metrics
    cout << endl << endl;
    cout << "Operation Counts" << endl;
    cout << "-----------------------------------" << endl;
    cout << setw(45) << "Linear" << setw(15) << "OverFlow" << endl;

    cout << left << setw(40) << "Number of key values inserted" << setw(15) << totalInserts << setw(10) << OVtotalInserts << endl;
    cout << left << setw(40) << "Unique values" << setw(15) << count.uniqueValueCount << setw(10) << count.OVuniqueValueCount << endl;
    cout << left << setw(40) << "Duplicate values" << setw(15) << count.duplicateValueCount << setw(10) << count.OVduplicateValueCount << endl;

    cout << endl;
    cout << left << setw(40) << "Collisions" << endl;
    cout << left << setw(40) << "Number of collisions" << setw(15) << count.collisionCount << setw(10) << count.OVcollisionCount << endl << endl;

    cout << left << setw(40) << "Number of direct inserts" << count.directInsertCount << " - " << setprecision(3) << (percentDirectInserts) << setw(8) << "%" << count.OVdirectInsertCount << " - " << OVpercentDirectInserts << "%" << endl;
    cout << left << setw(40) << "Number of non-direct inserts" << count.inDirectInsertCount << " - " << setprecision(3) << (percentNonDirectInserts) << setw(8) << "%" << count.OVinDirectInsertCount << " - " << OVpercentNonDirectInserts << "%" << endl << endl;

    cout << left << setw(40) << "Average distance from home" << endl;
    cout << left << setw(40) << "including direct inserts" << setw(15) << averageDistanceIncludingDirect << setw(10) << OVaverageDistanceIncludingDirect << endl;
    cout << left << setw(40) << "not-including direct inserts" << setw(15) << averageDistanceExcludingDirect << setw(10) << OVaverageDistanceExcludingDirect << endl;

    cout << left << setw(40) << "Largest distance" << setw(15) << count.largestProbingDistance << setw(10) << count.OVlargestProbingDistance << endl;

    cout << endl;


    cout << left << setw(40) << "Searches" << endl;
    cout << left << setw(40) << "Number of searches" << setw(15) << count.searchCount << setw(10) << count.OVsearchCount << endl;
    cout << left << setw(40) << "Number of comparisons" << setw(15) << count.totalComparisons << setw(10) << count.OVtotalComparisons << endl;
    cout << left << setw(40) << "Number of direct accesses" << setw(15) << count.directAccesses << setw(10) << count.OVdirectAccesses << endl;
    cout << left << setw(40) << "Number of indirect accesses" << setw(15) << count.indirectAccesses << setw(10) << count.OVindirectAccesses << endl;
    cout << left << setw(40) << "Total number of accesses" << setw(15) << totalaccesses << setw(10) << OVtotalaccesses << endl;
    cout << left << setw(40) << "Average number of comparisons" << setw(15) << count.averageComparisons() << setw(10) << count.OVaverageComparisons() << endl;
    cout << left << setw(40) << "Largest number of comparisons" << setw(15) << count.largestComparisons << setw(10) << count.OVlargestComparisons << endl;



    outputfile << endl << endl;
    outputfile << "Operation Counts" << endl;
    outputfile << "-----------------------------------" << endl;
    outputfile << setw(45) << "Linear" << setw(15) << "OverFlow" << endl;

    outputfile << left << setw(40) << "Number of key values inserted" << setw(15) << totalInserts << setw(10) << OVtotalInserts << endl;
    outputfile << left << setw(40) << "Unique values" << setw(15) << count.uniqueValueCount << setw(10) << count.OVuniqueValueCount << endl;
    outputfile << left << setw(40) << "Duplicate values" << setw(15) << count.duplicateValueCount << setw(10) << count.OVduplicateValueCount << endl;

    outputfile << endl;
    outputfile << left << setw(40) << "Collisions" << endl;
    outputfile << left << setw(40) << "Number of collisions" << setw(15) << count.collisionCount << setw(10) << count.OVcollisionCount << endl << endl;

    outputfile << left << setw(40) << "Number of direct inserts" << count.directInsertCount << " - " << setprecision(3) << (percentDirectInserts) << setw(8) << "%" << count.OVdirectInsertCount << " - " << OVpercentDirectInserts << "%" << endl;
    outputfile << left << setw(40) << "Number of non-direct inserts" << count.inDirectInsertCount << " - " << setprecision(3) << (percentNonDirectInserts) << setw(8) << "%" << count.OVinDirectInsertCount << " - " << OVpercentNonDirectInserts << "%" << endl << endl;

    outputfile << left << setw(40) << "Average distance from home" << endl;
    outputfile << left << setw(40) << "including direct inserts" << setw(15) << averageDistanceIncludingDirect << setw(10) << OVaverageDistanceIncludingDirect << endl;
    outputfile << left << setw(40) << "not-including direct inserts" << setw(15) << averageDistanceExcludingDirect << setw(10) << OVaverageDistanceExcludingDirect << endl;

    outputfile << left << setw(40) << "Largest distance" << setw(15) << count.largestProbingDistance << setw(10) << count.OVlargestProbingDistance << endl;

    outputfile << endl;


    outputfile << left << setw(40) << "Searches" << endl;
    outputfile << left << setw(40) << "Number of searches" << setw(15) << count.searchCount << setw(10) << count.OVsearchCount << endl;
    outputfile << left << setw(40) << "Number of comparisons" << setw(15) << count.totalComparisons << setw(10) << count.OVtotalComparisons << endl;
    outputfile << left << setw(40) << "Number of direct accesses" << setw(15) << count.directAccesses << setw(10) << count.OVdirectAccesses << endl;
    outputfile << left << setw(40) << "Number of indirect accesses" << setw(15) << count.indirectAccesses << setw(10) << count.OVindirectAccesses << endl;
    outputfile << left << setw(40) << "Total number of accesses" << setw(15) << totalaccesses << setw(10) << OVtotalaccesses << endl;
    outputfile << left << setw(40) << "Average number of comparisons" << setw(15) << count.averageComparisons() << setw(10) << count.OVaverageComparisons() << endl;
    outputfile << left << setw(40) << "Largest number of comparisons" << setw(15) << count.largestComparisons << setw(10) << count.OVlargestComparisons << endl;
}

// HELPER FUNCTIONS------------------------------------------

// Finds the Homebucket using a mod function
int TableFunctions::hashFunction(int value) {
	return value % HASH_TABLE_SIZE;
}

// Inserts items into the linear open addressing hashtable techincally a quadratic open addresssing hashtable 
void TableFunctions::LinearHashInsert(int value, ofstream& outputfile, bool& isFull)
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

// inserts items into the overflow chain hashtable.
void TableFunctions::overflowinsert(int value, bool& isFull)
{
    int index = hashFunction(value);
    int distance = 0;

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
            distance++;

        }
        int lastChainIndex = nextOpenIndex;                 // Will store the last index in the chain

        // Find the next aviably spot in he 
        while (chainIndex != -1) 
        {   //Duplicate found
            if (OverFlow[chainIndex].keyValue == value) {
                OverFlow[chainIndex].keyCount++;
                count.OVduplicateValueCount++;
                count.OVinDirectInsertCount++;
                count.OVtotalProbingDistance = count.OVtotalProbingDistance + distance;
                return;
            }

            // iterate through the overflow array, increament collision
            lastChainIndex = chainIndex;
            chainIndex = OverFlow[chainIndex].nextIndex;
            count.OVcollisionCount++;
            distance++;
        }

       
        // Find a free spot in the overflow array, should be instant because newoverflowindex is the next 
        int newOverflowIndex = findNextAvailableOverflowIndex(nextOpenIndex); 
        
        // triggers if the overflow array is full
        if (newOverflowIndex == -1) {
            isFull = false;
            count.OVtotalProbingDistance = count.OVtotalProbingDistance + distance;
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

        count.OVtotalProbingDistance = count.OVtotalProbingDistance + distance;
        count.OVupdateLargestProbingdist(distance);
    }
}

//finds the next empty index in the overflow array
int TableFunctions::findNextAvailableOverflowIndex(int& nextOpenIndex)
{
    for (int i = nextOpenIndex; i < HASH_TABLE_SIZE; ++i) {
            if (OverFlow[i].keyValue == -1) {
                return i;
            }
            count.OVcollisionCount++;
    }
    return -1; // Indicates overflow space is full
}

// Searches through the linear hashtable for a number
bool TableFunctions::searchLinear(int value)
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

// Searches through the primary/overflow hashtables for a number
bool TableFunctions::searchOverflow(int value)
{
    int searchindex = 0;
    int comparisons = 1;
    int index = hashFunction(value);

    if (ChainPrimary[index].keyValue == value) {
        count.OVdirectAccesses++;
        count.OVsearchCount++;
        count.OVtotalComparisons = count.OVtotalComparisons + comparisons;
        count.OVupdateLargestComparisons(comparisons);
        return true;
    }
    searchindex = ChainPrimary[index].nextIndex;
    while (searchindex != -1) {
        comparisons++;
        if (OverFlow[searchindex].keyValue == value) {
            count.OVindirectAccesses;
            count.OVsearchCount++;
            count.OVtotalComparisons = count.OVtotalComparisons + comparisons;
            count.OVupdateLargestComparisons(comparisons);
            return true;
        }
        searchindex = OverFlow[searchindex].nextIndex;
    }

    // if item was not found in array, 
    count.OVsearchCount++;
    count.OVtotalComparisons = count.OVtotalComparisons + comparisons;
    count.OVupdateLargestComparisons(comparisons);
    return false;
}

// gets the numbers from a file and inserts into hashtables
void TableFunctions::fileprocess(string filename, ofstream& outputfile) {

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
        overflowinsert(num, isfull);
       // overflowHashInsert();
        if(isfull){
            SearchQueue.push(num);
        }
        else {
            break;
        }
     

        

    }
}

