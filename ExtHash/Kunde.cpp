#include "Kunde.h"

Kunde::Kunde() {
	Clear();
}

Kunde::Kunde(int kunNum, char kunName[(NAME_LENGTH)], int skonto) {
	_kunNum = kunNum;
	_skonto = skonto;

	for(int i = 0; i < (NAME_LENGTH); i++) {
		_kunName[i] = kunName[i];
	}
}

int Kunde::GetKunNum() const {
	return _kunNum;
}

char* Kunde::GetKunName() {
	return _kunName;
}

int Kunde::GetSkonto() const {
	return _skonto;
}

void Kunde::SetKunNum(int kunNum) {
	_kunNum = kunNum;
}

void Kunde::SetKunName(const char kunName[(NAME_LENGTH)]) {
	for(int i = 0; i < (NAME_LENGTH); i++) {
		_kunName[i] = kunName[i];
	}
}

void Kunde::SetSkonto(int skonto) {
	_skonto = skonto;
}

void Kunde::Clear() {
	char tmpName[20] = {'n','n','n','n','n', '\0'};
	_kunNum = (KUNDE_EMPTY);
	_skonto = 0;

	for(int i = 0; i < (NAME_LENGTH); i++) {
		_kunName[i] = tmpName[i];
	}
}