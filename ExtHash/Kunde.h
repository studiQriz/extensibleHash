#include <string>
using namespace std;

#ifndef KUNDE_H
#define KUNDE_H
#define NAME_LENGTH 6
#define KUNDE_EMPTY -1

class Kunde {
private:
	int _kunNum;
	int _skonto;
	char _kunName[(NAME_LENGTH)];


public:
	Kunde();
	Kunde(int kunNum, char kunName[(NAME_LENGTH)], int skonto);

	int GetKunNum() const;
	//Not very good practice to give the pointer since the caller is able to change the field.
	//Should be const but didn't figure out how to manage, or a reference with const marker.
	char* GetKunName();

	int GetSkonto() const;

	void SetKunNum(int kunNum);

	void SetKunName(const char kunName[(NAME_LENGTH)]);

	void SetSkonto(int skonto);

	void Clear();
};
#endif
