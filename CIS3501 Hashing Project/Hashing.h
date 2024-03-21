#ifndef HASHING_H
#define HASHING_H

#include <iostream>
#include <string>
#include <array>
#include <sstream>
#include <queue>
#include <ctime>    // So i can use time()
#include <cstdlib>  // So i can use rand() and srand()
#include <fstream>

using namespace std;


const int HASH_TABLE_SIZE = 100;  // Use 1000 for the final test run

struct hashNode {
	int keyValue;
	int keyCount;

	hashNode() : keyValue(-1), keyCount(0) {}
	hashNode(int key, int count) : keyValue(key), keyCount(count) {}
};

class linear {
public:
	linear();

	void printHashTable();
	void processMethod(string method);				// get file or random number
	void SearchItem();
	void PrintOperations();
private: 
	hashNode hashTable[HASH_TABLE_SIZE];  // Linear open addressing hash table
	queue<int> testNumbers;
	queue<int> SearchQueue;
	int hashFunction(int value);
	void fileprocess(string filename);
	void LinearHashInsert(int value);
	bool searchLinear(int value, int& endindex, int& searchCount, int& totalComparisons);
	
	
	//Counters ARE NOT DONE YET - STILL NEED TO DO THEM
	//Create an array for counters and create probably formatting for counters
	

	//NUMBER OF KEY VALUES INSERTED
	int uniqueInserts = 0;
	int duplicateInserts = 0;

	//COLLISIONS
	int collisionCount = 0;
	int directInserts = 0;
	int in_directInserts = 0;
	//Average distance from  home?
	//inclduing direct inserts?
	//not including direct inserts?
	int largestDistance = 0;
	
	//SEARCHES
	int searchCount = 0;
	int totalComparisons = 0;
	int directAcesses = 0;
	int indirectAccesses = 0;
	int averagecomparisons = 0;
	int largestcomparisons = 0; 
};



#endif