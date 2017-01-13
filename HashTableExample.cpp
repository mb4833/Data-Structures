#include <iostream>
#include <string>
using namespace std;



class HashEntryNode {
private:
	string key;
	int value;
	HashEntryNode* next;
public:
	// Constructor to initialize a HashEntryNode
	HashEntryNode(string k, int v) : key(k), value(v), next(NULL) {}

	// Helper methods to get/set member variables
	string getKey() const { return key; }
	int getValue() const { return value;  }
	void setValue(int value) { this->value = value; }
	HashEntryNode* getNext() const { return next; }
	void setNext(HashEntryNode* next) { this->next = next; }
};

const int DEFAULT_TABLE_SIZE = 128;
class HashTable {
private:
	HashEntryNode** table;
	int tableSize;
	float threshold;
	int maxSize;
	int size;

	// Resize method to make the hash table as effiecient as possible
	void resize() {
		int oldTableSize = tableSize;
		tableSize *= 2;
		maxSize = (int)(tableSize * threshold);
		HashEntryNode** oldTable = table;
		table = new HashEntryNode*[tableSize];
		for (int i = 0; i < tableSize; i++) {
			table[i] = NULL;
		}
		size = 0;
		for (int index = 0; index < oldTableSize; index++)
			if (oldTable[index] != NULL) {
				HashEntryNode* oldEntry;
				HashEntryNode* entry = oldTable[index];
				while (entry != NULL) {
					put(entry->getKey(), entry->getValue());
					oldEntry = entry;
					entry = entry->getNext();
					delete oldEntry;
				}
			}
		delete[] oldTable;
	}

	
public:
	// Constructor for Hash Table
	// Initialize the size of the table and set each entries value as NULL
	HashTable() {
		threshold = 0.75;
		maxSize = 96;
		size = 0;
		tableSize = DEFAULT_TABLE_SIZE;

		table = new HashEntryNode* [tableSize];
		for (int i = 0; i < tableSize; i++) {
			table[i] = NULL;
		}
	}

	// Change the percantge of what the hash table can be filled until resizing is peformed
	void setThreshold(float threshold) {
		this->threshold = threshold;
		maxSize = (int)(tableSize * threshold);
	}



	// Simple hash function to generate an index for demonstrating purpose 
	// Note, in practical cases, you will not have to create your own hash function as there will be in built ones
	int hash(string key) {
		int index = 0;
		for (int i = 0; i < key.length(); i++) {
			index += (int)key[i];
		}
		return index * 17 % tableSize;
	}

	// Method to add a key and value to an entry in the hash table
	void put(string key, int value) {
		int index = hash(key);
		// Case where the index is available to store a key and value
		if (table[index] == NULL) {
			table[index] = new HashEntryNode(key, value);
			size++;
		}

		// Case where the index is taken and we need to resolve the collision through chaining
		else {
			// Create a temporary HashEntryNode* to iterate through the linked list until we reach the tail
			HashEntryNode* entry = table[index];
			while (entry->getNext() != NULL || entry->getKey() != key) {
				entry = entry->getNext();
			}
			// Since we need to make sure all keys are distinct, if the same key is given, we just changed the value that was already stored
			if (entry->getKey() == key) {
				entry->setValue(value);
			} 
			else {
				entry->setNext(new HashEntryNode(key, value));
				size++;
			}
		}
		if (size >= maxSize) {
			resize();
		}
	}

	// Method to return the value that is associated with the key in the hash table
	// Returns -1 if the given key does not exist in the hash table
	int get(string key) {
		int index = hash(key);

		// Case where the index in the hash table is empty
		if (table[index] == NULL) {
			return -1;
		}
		// Case where the index in the hash table is not empty
		// Iterate through the chain (linked list) until key is found
		else {
			HashEntryNode* entry = table[index];
			while (entry != NULL && entry->getKey() != key) {
				entry = entry->getNext();
			}
			// If the key does not exist in the chain
			if (entry == NULL) {
				return -1;
			}
			else {
				return entry->getValue();
			}
		}
	}

	// Remove an entry from the hash table given a key
	void remove(string key) {
		int index = hash(key);

		// Check if the key passed in exists
		if (table[index] != NULL) {
			HashEntryNode* prevEntry = NULL;
			HashEntryNode* entry = table[index];
			while (entry->getNext() != NULL && entry->getKey() != key) {
				prevEntry = entry;
				entry = entry->getNext();
			}

			if (entry->getKey() == key) {
				// Case where there was no chain and the key was found
				if (prevEntry == NULL) {
					//HashEntryNode* nextEntry = entry->getNext();
					delete entry;
					//table[index] = nextEntry;
					table[index] = NULL;
				}
				// Case where HashEntryNode needs to point to the entry->next link
				else {
					HashEntryNode* next = entry->getNext();
					delete entry;
					prevEntry->setNext(next);
				}
				size--;
			}
		}
	}

	// Destructor to clean up our hash table and free up memory that was used in the heap
	~HashTable() {
		for (int i = 0; i < tableSize; i++)
			if (table[i] != NULL) {
				HashEntryNode* prevEntry = NULL;
				HashEntryNode* entry = table[i];
				while (entry != NULL) {
					prevEntry = entry;
					entry = entry->getNext();
					delete prevEntry;
				}
			}
		delete[] table;
	}

};


int main() {
	HashTable myTable;
	myTable.put("a", 1);
	myTable.put("b", 2);
	myTable.put("c", 3);
	cout << myTable.get("a");
	cout << myTable.get("b");
	cout << myTable.get("c");
	return 0;
}
