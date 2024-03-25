#ifndef HASHING_H
#define HASHING_H

#include <iomanip>
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

struct HashMetrics 
{
	// METRICS FOR INSERTS
	int uniqueValueCount = 0;				// DONE
	int duplicateValueCount = 0;			// DONE

	/*relating to collisions below*/
	int collisionCount = 0;					//DONE

	/*distance metrics below for collisions*/
	int totalProbingDistance = 0;			// NO
	int directInsertCount = 0;				// NO
	int nonDirectInsertCount = 0;			// NO
	int largestProbingDistance = 0;			// DONE
	// average probing distance is a function
	// average probing distance excluding direct is a function




	// METRICS FOR SEARCHES
	int searchCount = 0;
	int totalComparisons = 0;
	int directAccesses = 0;
	int indirectAccesses = 0;
	int largestComparisons = 0;
	// average number of comparisions is a funciton
	// total number of comparisions for indirect accesses


	// Methods to calculate derived metrics
	float averageProbingDistance() const {
		return (collisionCount == 0) ? 0 : static_cast<float>(totalProbingDistance) / collisionCount;
	}

	float averageProbingDistanceExcludingDirect() const {
		int distanceExcludingDirect = totalProbingDistance - directInsertCount;
		int collisionsExcludingDirect = collisionCount - directInsertCount;
		return (collisionsExcludingDirect == 0) ? 0 : static_cast<float>(distanceExcludingDirect) / collisionsExcludingDirect;
	}

	// Average number of comparisions for searching. 
	float averageComparisons() const {
		return (searchCount == 0) ? 0 : static_cast<float>(totalComparisons) / searchCount;
	}

};

struct hashNode 
{
	int keyValue;
	int keyCount;

	hashNode() : keyValue(-1), keyCount(0) {}
	hashNode(int key, int count) : keyValue(key), keyCount(count) {}
};

class linear 
{
	public:
		linear();

		void printHashTable();
		void processMethod(string method);				// get file or random number
		void SearchItem();
		void PrintOperations();

	private: 
		hashNode hashTable[HASH_TABLE_SIZE];  // Linear open addressing hash table
		HashMetrics count;
		queue<int> testNumbers;
		queue<int> SearchQueue;
		int hashFunction(int value);
		void fileprocess(string filename);
		void LinearHashInsert(int value);
		bool searchLinear(int value, int& endindex);
	
	
	//Counters ARE NOT DONE YET - STILL NEED TO DO THEM
	//Create an array for counters and create probably formatting for counters
};

#endif