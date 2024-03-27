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
	int uniqueValueCount = 0;			
	int duplicateValueCount = 0;			

	/*relating to collisions below*/
	int collisionCount = 0;					

	/*distance metrics below for collisions*/
	int totalProbingDistance = 0;		
	int directInsertCount = 0;				
	int nonDirectInsertCount = 0;			
	int largestProbingDistance = 0;			
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

	void updateLargestProbingdist(int distance) {
		if (distance > largestProbingDistance) {
			largestProbingDistance = distance;
		}
	}

	//Updates the largest comparison
	void updateLargestComparisons(int comparisons) {
		if (comparisons > largestComparisons) {
			largestComparisons = comparisons;
		}
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

		void printHashTable(ofstream&);
		void processMethod(string method, ofstream&);				// get file or random number
		void SearchItem();
		void PrintOperations(ofstream& outputfile);

	private: 
		hashNode hashTable[HASH_TABLE_SIZE];  // Linear open addressing hash table
		HashMetrics count;
		queue<int> testNumbers;
		queue<int> SearchQueue;
		int hashFunction(int value);
		void fileprocess(string filename, ofstream&);
		void LinearHashInsert(int value, ofstream&);
		bool searchLinear(int value, int& endindex);
	
};

#endif