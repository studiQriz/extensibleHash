#include "Bucket.h"
#include "MyException.h"
#include <fstream>
#include <vector>
#include <tuple>
#include <iostream>
#include <math.h>
#include <sstream>

// Index of the loal depth in the tuple
#define LOCAL_DEPTH_INDEX 0

// Index of the bucket address in the tuple
#define BUCKET_INDEX 1

// Hash table starts with one entry (bucket)
#define HASH_TABLE_STARTVALUE 2

#define W_VALUE 4294967296.0
#define S_VALUE 2654435769.0



template<int datasetCount>
class HashDat {
private:
	string _fileName;
	fstream _fileStream;
	string _configFile;
	fstream _configStream;

	int _globalDepth;
	int _nextBucketIndex;

	int _currBucketIndex;
	Bucket<datasetCount> _currBucket;

	//Pointers should be NULL, at init.
	int* _localDepth;
	//Its not an address its an index.
	//address is calculated at read/write
	int* _bucketIndex;

	// Calculates the hash for a Customer number
	long hashCalc(int kunNum);
	// Writes the current Bucket to disk
	void write();
	// Writes a bucket to disk
	void insertWrite(Bucket<datasetCount>& bucket, int bucketIndex);
	// Retrieves a bucket and sets is as the _currentBucket
	void read(int index);

	int getTableEntry(int kunNum);
	//Returns Table-Index.
	int setCurrBucket(int kunNum);

	bool addData(Bucket<datasetCount> *bucket, int kunNum, char kunName[(NAME_LENGTH)], int skonto);

	void extendTable();

	void splitBucket(int index);

	void readConfig();
	void writeConfig();
public:
	HashDat(string fileName);
	HashDat(string fileName, string configFile);
	HashDat(HashDat<datasetCount>& hashDat);

	// Fuegt neuen Kundensatz in Hash-Datei ein
	void Insert(int kunNum, char kunName[(NAME_LENGTH)], int skonto);
	// Holt Kundensatz mit vorgegebener Nummer hervor
	Kunde& Retrieve(int kunNum);
	// Displays all datasets.
	void Show();
	// Displays table.
	void ShowTable();

	void SaveTable();

	class BadArticle : public MyException
	{
	public:
		BadArticle(string msg);
	};

	class NotFound : public MyException
	{
	private:
		int _kunNum;
	public:
		NotFound(string msg, int kunNum);
		int GetKunNum();
	};
};

template<int datasetCount>
HashDat<datasetCount>::HashDat(string fileName) {
	_fileName = fileName;

	_fileStream.open(_fileName, ios::binary | ios::in | ios::out | ios::trunc);
	insertWrite(_currBucket, 0);
	insertWrite(_currBucket, 1);

	_globalDepth = 1;
	_localDepth = new int[2];
	_localDepth[0] = 1;
	_localDepth[1] = 1;

	_bucketIndex = new int[2];
	_bucketIndex[0] = 0;
	_bucketIndex[1] = 1;

	_nextBucketIndex = 2;

	_currBucketIndex = 0;
}

template<int datasetCount>
HashDat<datasetCount>::HashDat(string fileName, string configFile) {
	_fileName = fileName;
	_configFile = configFile;

	_fileStream.open(_fileName, ios::binary | ios::in | ios::out);
	_configStream.open(_configFile, ios::binary | ios::in | ios::out);

	// If file or config doesn't exist, create it
	if(!_fileStream || !_configStream) {
		_fileStream.open(_fileName, ios::binary | ios::in | ios::out | ios::trunc);
		_configStream.open(_configFile, ios::binary | ios::in | ios::out | ios::trunc);

		insertWrite(_currBucket, 0);
		insertWrite(_currBucket, 1);

		_globalDepth = 1;
		_localDepth = new int[2];
		_localDepth[0] = 1;
		_localDepth[1] = 1;

		_bucketIndex = new int[2];
		_bucketIndex[0] = 0;
		_bucketIndex[1] = 1;

		_nextBucketIndex = 2;
		_currBucketIndex = 0;
	}
	else {
		readConfig();
	}
}

template<int datasetCount>
HashDat<datasetCount>::HashDat(HashDat<datasetCount>& hashDat) {
	_fileName = hashDat._fileName + "_copy";
	ifstream  src(hashDat._fileName);
	ofstream  dst(_fileName);
	dst << src.rdbuf();

	_configFile = hashDat._configFile + "_copy";
	src.open(hashDat._configFile);
	dst.open(_configFile);
	dst << src.rdbuf();

	_fileStream.open(_fileName, ios::binary | ios::in | ios::out);
	_configStream.open(_configFile, ios::binary | ios::in | ios::out);

	_globalDepth = hashDat._globalDepth;
	_nextBucketIndex = hashDat._globalDepth;

	_currBucketIndex = hashDat._currBucketIndex;

	tableSize = (int)pow((double)2, _globalDepth);
	_localDepth = new int[tableSize];
	_bucketIndex = new int[tableSize];

	for(int i = 0; i < tableSize; i++) {
		_localDepth[i] = hashDat._localDepth[i];
		_bucketIndex[i] = hashDat._bucketIndex[i];
	}
}

template<int datasetCount>
void HashDat<datasetCount>::readConfig() {
	int tmp = 0;
	int tableSize = 0;

	_configStream.clear();
	_configStream.seekg(0);
	_configStream.read((char*)&tmp, sizeof(int));

	_globalDepth = tmp;
	tableSize = (int)pow((double)2, _globalDepth);
	_localDepth = new int[tableSize];
	_bucketIndex = new int[tableSize];

	for(int i = 0; i < tableSize; i++) {
		_configStream.read((char*)&tmp, sizeof(int));
		_localDepth[i] = tmp;

		_configStream.read((char*)&tmp, sizeof(int));
		_bucketIndex[i] = tmp;
	}

	_nextBucketIndex = 0;
	for(int i = 0; i < tableSize; i++) {
		if(_bucketIndex[i] > _nextBucketIndex) {
			_nextBucketIndex = _bucketIndex[i];
		}
	}
	_nextBucketIndex++;
}

template<int datasetCount>
void HashDat<datasetCount>::writeConfig() {
	_configStream.clear();
	_configStream.seekp(0);

	int tmp = 0;
	tmp = _globalDepth;

	_configStream.write((char*)&tmp, sizeof(int));

	for(int i = 0; i < (int)pow((double)2, _globalDepth); i++) {
		tmp = _localDepth[i];
		_configStream.write((char*)&tmp, sizeof(int));

		tmp = _bucketIndex[i];
		_configStream.write((char*)&tmp, sizeof(int));
	}

	_configStream.flush();
}

template<int datasetCount>
long HashDat<datasetCount>::hashCalc(int kunNum) {
	//unsigned int m = pow((double)2, _globalDepth);
	//unsigned int ret = (unsigned int) floor( ( (double) m * fmod( S_VALUE * (double) kunNum, W_VALUE) ) / W_VALUE );
	//return ret;

	//float r = (sqrt(5.0f) - 1)/2;
	//float tmp = kunNum * r;
	//return (tmp - ((int)tmp))*LONG_MAX;

	return kunNum;
}

/*
* Writes the current bucket to disk
*/
template<int datasetCount>
void HashDat<datasetCount>::write() {
	_currBucket.Write(_currBucketIndex*sizeof(Bucket<datasetCount>), _fileStream);
}

/*
* Writes a bucket to disk
*/
template<int datasetCount>
void HashDat<datasetCount>::insertWrite(Bucket<datasetCount>& bucket, int bucketIndex) {
	bucket.Write(bucketIndex*sizeof(Bucket<datasetCount>), _fileStream);
}

/*
* Gets a new Bucket from disk and sets it as the current bucket
*/
template<int datasetCount>
void HashDat<datasetCount>::read(int index) {
	_currBucketIndex = index;
	_currBucket.Read(index*sizeof(Bucket<datasetCount>), _fileStream);
}

template<int datasetCount>
int HashDat<datasetCount>::getTableEntry(int kunNum){
	// Compute hash
	long hash = hashCalc(kunNum);

	//Computes for given hash, numbers in interval [0, 2^globalDepth-1]
	int tableIndex = (hash % (int)pow((double)2, _globalDepth));

	return tableIndex;
}
/*
* Sets the current bucket to one that contains the dataset with kunNum
* and returns the hash table index
*/
template<int datasetCount>
int HashDat<datasetCount>::setCurrBucket(int kunNum){
	int tableIndex = getTableEntry(kunNum);

	read(_bucketIndex[tableIndex]);

	return tableIndex;
}

template<int datasetCount>
bool HashDat<datasetCount>::addData(Bucket<datasetCount> *bucket, int kunNum, char kunName[(NAME_LENGTH)], int skonto) {
	for(int i=0; i < datasetCount; i++) {
		Kunde& kun = bucket->GetKunde(i);

		if(kun.GetKunNum() == kunNum) {
			throw BadArticle("Data is already stored.");
		}
		else if(kun.GetKunNum() == (KUNDE_EMPTY)) {
			kun.SetKunNum(kunNum);
			kun.SetKunName(kunName);
			kun.SetSkonto(skonto);

			return true;
		}
	}

	return false;
}

template<int datasetCount>
void HashDat<datasetCount>::splitBucket(int tableIndex) {
	int oldLocalDepth = _localDepth[tableIndex];
	int newBucketIndex = _nextBucketIndex++;

	int oldBucket = _bucketIndex[tableIndex];
	_localDepth[tableIndex] = oldLocalDepth + 1;
	_bucketIndex[tableIndex] = newBucketIndex;

	Bucket<datasetCount> tmpBucket;
	Bucket<datasetCount> newBucket;

	cout << "-SplitBucket-" << endl;
	cout << "Changed Table-Index[" << tableIndex << "] Bucket from " << oldBucket << " to " << _bucketIndex[tableIndex] << endl;

	for(int i= 0; i < datasetCount; i++) {
		Kunde& kun = _currBucket.GetKunde(i);
		int newTable = getTableEntry(kun.GetKunNum());

		// Keep in first bucket
		if(_bucketIndex[newTable] == _currBucketIndex) {
			cout << "Keep Kunde " << kun.GetKunNum() << " in current Bucket " << _currBucketIndex << endl;
			addData(&tmpBucket, kun.GetKunNum(), kun.GetKunName(), kun.GetSkonto());
			kun.Clear();
		}
		// Put in second bucket
		else {
			cout << "Store Kunde " << kun.GetKunNum() << " in Bucket-Index " << newBucketIndex << endl;

			addData(&newBucket, kun.GetKunNum(), kun.GetKunName(), kun.GetSkonto());
			kun.Clear();
		}	
	}

	for(int i = 0; i < datasetCount; i++) {
		Kunde& tmpKun = tmpBucket.GetKunde(i);
		if(tmpKun.GetKunNum() == (KUNDE_EMPTY)) {
			break;
		}
		else {
			Kunde& kun = _currBucket.GetKunde(i);
			kun.SetKunNum(tmpKun.GetKunNum());
			kun.SetKunName(tmpKun.GetKunName());
			kun.SetSkonto(tmpKun.GetSkonto());
		}
	}

	write();
	insertWrite(newBucket, newBucketIndex);
}

template<int datasetCount>
void HashDat<datasetCount>::extendTable() {
	int oldTableSize = (int)std::pow((double)2, _globalDepth);
	_globalDepth++;
	int newTableSize = (int)std::pow((double)2, _globalDepth);

	cout << "--EXTEND TABLE--" << endl;
	cout << "Old-Size: " << oldTableSize << " New-Size: " << newTableSize << endl;

	int* newLocalDepth = new int[newTableSize];
	int* newBucketIndex = new int[newTableSize];

	for(int i=0; i < oldTableSize; i++) {
		newLocalDepth[i] = _localDepth[i];
		newBucketIndex[i] = _bucketIndex[i];

		newLocalDepth[i + oldTableSize] = _localDepth[i];
		newBucketIndex[i + oldTableSize] = _bucketIndex[i];
	}

	// Free memory
	delete[] _localDepth;
	delete[] _bucketIndex;

	// set new pointer
	_localDepth = newLocalDepth;
	_bucketIndex = newBucketIndex;
}

/*
* Inserts a dataset
*/
template<int datasetCount>
void HashDat<datasetCount>::Insert(int kunNum, char kunName[(NAME_LENGTH)], int skonto) {
	int tableIndex = setCurrBucket(kunNum);

	cout << endl <<  "--Add Kunde--" << endl;
	cout << "KunNum: " << kunName << " Bucket-Index: " << _currBucketIndex << " Table-Index: " << tableIndex << endl;

	if(addData(&_currBucket, kunNum, kunName, skonto)) {
		cout << "Bucket empty" << endl;
		write();
	}
	else {
		//Extend Table?
		if(_localDepth[tableIndex] == _globalDepth) {
			extendTable();
			Insert(kunNum, kunName, skonto);
		}
		//Split Bucket?
		else if(_localDepth[tableIndex] < _globalDepth) {
			splitBucket(tableIndex);
			Insert(kunNum, kunName, skonto);
		}
	}
}

/*
* Retrieves a Customer from disk
*/
template<int datasetCount>
Kunde& HashDat<datasetCount>::Retrieve(int kunNum) {
	setCurrBucket(kunNum);

	for(int i = 0; i < datasetCount; i++) {
		Kunde& kun = _currBucket.GetKunde(i);

		if(kun.GetKunNum() == kunNum) {
			return kun;
		}
	}

	stringstream ss;
	ss << "Data ";
	ss << kunNum;
	ss << " is not stored.";
	throw NotFound(ss.str() , kunNum);
}

template<int datasetCount>
void HashDat<datasetCount>::ShowTable() {
	cout << "Index\tLDepth\tBIndex" << endl;
	for(int i = 0; i < (int)pow((double)2, _globalDepth); i++) {
		cout << i << "\t" << _localDepth[i] << "\t" << _bucketIndex[i] << endl;
	}
}

/*
* Displays all datasets from all buckets
*/
template<int datasetCount>
void HashDat<datasetCount>::Show() {
	for(int indexBucket = 0; indexBucket < _nextBucketIndex; indexBucket++) {
		read(indexBucket);
		cout << "BucketIndex: " << indexBucket << endl;

		for(int indexDataset = 0; indexDataset < datasetCount; indexDataset++) {
			Kunde& kun = _currBucket.GetKunde(indexDataset);
			if(kun.GetKunNum() == (KUNDE_EMPTY))
				cout << "EMPTY-SLOT" << endl;
			else
				cout << "KunNum: " << kun.GetKunNum() << " KunName: " << kun.GetKunName() << " Skonto: " << kun.GetSkonto() << endl;
		}
	}
}

template<int datasetCount>
void HashDat<datasetCount>::SaveTable() {
	writeConfig();
}

template<int datasetCount>
HashDat<datasetCount>::BadArticle::BadArticle(string reason) : MyException(reason) {

}

template<int datasetCount>
HashDat<datasetCount>::NotFound::NotFound(string reason, int kunNum) : MyException(reason) {
	_kunNum = kunNum;
}

template<int datasetCount>
int HashDat<datasetCount>::NotFound::GetKunNum() {
	return _kunNum;
}