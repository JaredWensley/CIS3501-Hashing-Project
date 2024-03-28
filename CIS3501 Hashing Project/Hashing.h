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


const int HASH_TABLE_SIZE = 10;  // Use 1000 for the final test run

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
	int inDirectInsertCount = 0;			
	int largestProbingDistance = 0;			
	// METRICS FOR SEARCHES
	int searchCount = 0;
	int totalComparisons = 0;
	int directAccesses = 0;
	int indirectAccesses = 0;
	int largestComparisons = 0;
	


	// Methods to calculate derived metrics
	float averageProbingDistance() const {
		return (collisionCount == 0) ? 0 : static_cast<float>(totalProbingDistance) / (directInsertCount + inDirectInsertCount);
	}

	float averageProbingDistanceExcludingDirect() const {
		return (collisionCount == 0) ? 0 : static_cast<float>(totalProbingDistance) / directInsertCount;
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


	// overflow metrics

	// METRICS FOR INSERTS
	int OVuniqueValueCount = 0;
	int OVduplicateValueCount = 0;
	/*relating to collisions below*/
	int OVcollisionCount = 0;
	/*distance metrics below for collisions*/
	int OVtotalProbingDistance = 0;
	int OVdirectInsertCount = 0;
	int OVinDirectInsertCount = 0;
	int OVlargestProbingDistance = 0;
	// METRICS FOR SEARCHES
	int OVsearchCount = 0;
	int OVtotalComparisons = 0;
	int OVdirectAccesses = 0;
	int OVindirectAccesses = 0;
	int OVlargestComparisons = 0;


	// Methods to calculate derived metrics
	float OVaverageProbingDistance() const {
		return (OVcollisionCount == 0) ? 0 : static_cast<float>(OVtotalProbingDistance) / (OVdirectInsertCount + OVinDirectInsertCount);
	}

	float OVaverageProbingDistanceExcludingDirect() const {
		return (OVcollisionCount == 0) ? 0 : static_cast<float>(OVtotalProbingDistance) / OVdirectInsertCount;
	}

	// Average number of comparisions for searching. 
	float OVaverageComparisons() const {
		return (OVsearchCount == 0) ? 0 : static_cast<float>(OVtotalComparisons) / OVsearchCount;
	}

	void OVupdateLargestProbingdist(int distance) {
		if (distance > OVlargestProbingDistance) {
			OVlargestProbingDistance = distance;
		}
	}

	//Updates the largest comparison
	void OVupdateLargestComparisons(int comparisons) {
		if (comparisons > OVlargestComparisons) {
			OVlargestComparisons = comparisons;
		}
	}



};

struct hashNode 
{
	int keyValue;
	int keyCount;
	int nextIndex; 
	hashNode() : keyValue(-1), keyCount(0), nextIndex(-1) {}
	hashNode(int key, int count, int chainindex) : keyValue(key), keyCount(count), nextIndex(chainindex) {}

};

class TableFunctions 
{
	public:
		TableFunctions();

		void printHashTables(ofstream&, string title);
		void processMethod(string method, ofstream&);				// get file or random number
		void SearchItem();
		void PrintOperations(ofstream& outputfile);

	private: 
		hashNode OverFlow[HASH_TABLE_SIZE];
		hashNode ChainPrimary[HASH_TABLE_SIZE];
		hashNode hashTable[HASH_TABLE_SIZE];		// Linear open addressing hash table
		HashMetrics count;
		queue<int> SearchQueue;
		int nextOpenIndex = 0;
		int hashFunction(int value);
		void fileprocess(string filename, ofstream&);
		void LinearHashInsert(int value, ofstream&, bool&);
		void overflowinsert(int value, bool& isFull);
		bool searchLinear(int value);
		bool searchOverflow(int value);

		int findNextAvailableOverflowIndex(int& nextOpenIndex);
		
	
};

#endif