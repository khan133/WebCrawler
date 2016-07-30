
//
// CS251 Data Structures
// Hash Table
//

#include <assert.h>
#include <stdlib.h>
#include <string.h>

// Each hash entry stores a key, object pair
template <typename Data>
struct HashTableTemplateEntry {
	const char * _key;
	Data _data;
	HashTableTemplateEntry * _next;
};

// This is a Hash table that maps string keys to objects of type Data
template <typename Data>
class HashTableTemplate {
	public:
		// Number of buckets
		enum { TableSize = 2039};

		// Array of the hash buckets.
		HashTableTemplateEntry<Data> **_buckets;

		// Obtain the hash code of a key
		int hash(const char * key);

	public:
		HashTableTemplate();

		// Add a record to the hash table. Returns true if key already exists.
		// Substitute content if key already exists.
		bool insertItem( const char * key, Data data);

		// Find a key in the dictionary and place in "data" the corresponding record
		// Returns false if key is does not exist
		bool find( const char * key, Data * data);

		// Removes an element in the hash table. Return false if key does not exist.
		bool removeElement(const char * key);

		// Returns the data that corresponds to this index.
		// Data is initialized to 0s if the entry does not exist
		Data operator[] (const char * &key);
};

	template <typename Data>
int HashTableTemplate<Data>::hash(const char * key)
{
	int h = 0;
	const char * p = key;
	while (*p) {
		h += *p;
		p++;
	}
	return h % TableSize;
}

	template <typename Data>
HashTableTemplate<Data>::HashTableTemplate()
{
	// Add implementation here
	_buckets = new HashTableTemplateEntry<Data> * [TableSize];
	int i = 0; 
	while (i < TableSize) {
		_buckets[i] = NULL; 
		i++;
	}
	return; 
}

	template <typename Data>
bool HashTableTemplate<Data>::insertItem( const char * key, Data data)
{
	// Add implementation here
	int num = hash(key); 
	HashTableTemplateEntry<Data> * curr = _buckets[num];
	HashTableTemplateEntry<Data> * temp = new HashTableTemplateEntry<Data>(); 
	while (curr != NULL) {
		if (strcmp(curr -> _key, key) == 0) {
			curr -> _data = data;
			delete[] temp; 
			return true; 
		}
		curr = curr -> _next; 
	}
	temp -> _key = strdup(key);
	temp -> _data = data;
	temp -> _next = _buckets[num];
	_buckets[num] = temp; 
	return false;
}

	template <typename Data>
bool HashTableTemplate<Data>::find( const char * key, Data * data)
{
	// Add implementation here
	int num = hash(key); 
	HashTableTemplateEntry<Data> * curr = _buckets[num]; 
	int count = 0;
	while (curr != NULL) {
		if (strcmp(curr -> _key, key) == 0) {
			*data = curr -> _data;
			return true; 
		}
		else
			count++;
		curr = curr -> _next; 
	}
	if (count != 0)
		return false;
}

template <typename Data>
Data HashTableTemplate<Data>::operator[] (const char * &key) {
	Data d; 
	if (find(key,&d))
		return d;
	return 0;
}

	template <typename Data>
bool HashTableTemplate<Data>::removeElement(const char * key)
{
	// Add implementation here
	int num = hash(key);
	int count = 0; 
	HashTableTemplateEntry<Data> * curr = _buckets[num];
	HashTableTemplateEntry<Data> * temp = curr; 
	while (curr != NULL) {
		if (strcmp(key, curr -> _key) == 0) {
			if (count == 0) {
				_buckets[num] = NULL; 
				return true;
			}
			if (count != 0) {
				temp -> _next = curr -> _next;
				return true; 
			}
		}
		temp = curr; 
		curr = curr -> _next; 
		count++; 
	}
	return false;
}

template <typename Data>
class HashTableTemplateIterator {
	int _currentBucket;
	HashTableTemplateEntry<Data> *_currentEntry;
	HashTableTemplate<Data> * _hashTable;
	public:
	HashTableTemplateIterator(HashTableTemplate<Data> * hashTable);
	bool next(const char * & key, Data & data);
};

	template <typename Data>
HashTableTemplateIterator<Data>::HashTableTemplateIterator(HashTableTemplate<Data> * hashTable)
{
	// Add implementation here
	_currentBucket = 0;
	_hashTable = hashTable;
	_currentEntry = _hashTable -> _buckets[_currentBucket]; 
}

	template <typename Data>
bool HashTableTemplateIterator<Data>::next(const char * & key, Data & data)
{
	// Add implementation here
	int count = 0; 
	int i = _currentBucket + 1;  
	while (_hashTable -> _buckets[i]  == NULL) {
		i++; 
		if (i >= 2039)
			return false;
	} 
	if (count == 0) {
		if (_currentEntry != NULL) { 
			if (_currentEntry->_next != NULL) {

				_currentEntry = _currentEntry->_next;
				key = _currentEntry->_key;
				data = _currentEntry->_data;
				return true;
			}   
		}   


	}
	if (_hashTable -> _buckets[i] != NULL) {
		count++; 
		_currentBucket = i;
		_currentEntry = _hashTable -> _buckets[i];
		key = _hashTable -> _buckets[i] -> _key;
		data = _hashTable -> _buckets[i] -> _data; 
		return true; 
	}   
	return false;  

}



