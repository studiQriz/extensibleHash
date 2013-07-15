#include <vector>
#include <fstream>
#include "Kunde.h"

template<int n>
class Bucket {
private:
	Kunde _kunVector[n];

public:
	Bucket();
	void Write(int position, fstream &stream);
	void Read(int position, fstream &stream);
	Kunde& GetKunde(int index);
};

template<int n>
Bucket<n>::Bucket() {
	for(int i = 0; i < n; i++) {
		_kunVector[i] = Kunde();
	}
}

template<int n>
void Bucket<n>::Write(int position, fstream &stream) {
	int tmp = 0;
	stream.clear();
	stream.seekp(position);

	for(int i = 0; i < n; i++) {
		tmp = _kunVector[i].GetKunNum();
		stream.write((char*)&tmp, sizeof(int));

		tmp = _kunVector[i].GetSkonto();
		stream.write((char*)&tmp, sizeof(int));

		stream.write(_kunVector[i].GetKunName(), (NAME_LENGTH) * sizeof(char));
	}

	stream.flush();
}

template<int n>
void Bucket<n>::Read(int position, fstream &stream) {
	int intTmp[2];
	char charTmp[NAME_LENGTH];
	stream.clear();
	stream.seekg(position);

	for(int i = 0; i < n; i++) {
		stream.read((char*)&intTmp, 2 * sizeof(int));
		_kunVector[i].SetKunNum(intTmp[0]);
		_kunVector[i].SetSkonto(intTmp[1]);
		stream.read(charTmp, (NAME_LENGTH) * sizeof(char));
		_kunVector[i].SetKunName(charTmp);
	}
}

template<int n>
Kunde& Bucket<n>::GetKunde(int index) {
	return _kunVector[index];
}
