#include "HashDat.h"
#include <stdlib.h>

#define SIZE 5

int main()
{
	int input = 1;
	string file;
	string conf;
	string custName;
	int skonto;
	int custId;
	char inputName[NAME_LENGTH];
	Kunde tmp;


	cout << "Create a new database or use an existing one." << endl;
	cout << "Specify location for data file: ";
	cin >> file;
	cout << endl;
	cout << "Specify location for the configuration file: ";
	cin >> conf;
	cout << endl;
	HashDat<(SIZE)> hashdat(file, conf);

	//Test Software like a baus!
	//Not generated.
	//int stuff[] = { 0, 1, 2, 3, 4, 5, 8, 9, 16, 17, 32, 33,
	//	64, 65, 128, 129, 256, 257, 59,60,107,173,191,223,101,23,41,157,151};
	//int size = sizeof(stuff) / sizeof(stuff[0]);

	/*//Generated.
	const int size = 1000;
	int stuff[size];
	//Set elements
	for(int i = 0; i < size; i++) {
		stuff[i] = i;
	}
	//Mix it.
	for(int mix = 0; mix < size*2; mix++) {
		int index = rand() % size;
		int index2 = rand() % size;

		while(index == index2)
			index2 = rand() % size;
		int tmp = stuff[index];
		stuff[index] = stuff[index2];
		stuff[index2] = tmp;
	}
	char tmpName[20] = {'n','n','n','n','n', '\0'};
	HashDat<(SIZE)> hashdat("killme");
	//Test it
	for(int i = 0; i < size; i++) {
		hashdat.Insert(stuff[i], tmpName, 0);
	}

	for(int i = 0; i < size; i++) {
		hashdat.Retrieve(stuff[i]);
	}

	for(int i = 0; i < size; i++) {
		bool flew = false;
		try{
			hashdat.Insert(stuff[i], tmpName, 0);	
		}catch (HashDat<(SIZE)>::BadArticle& e) {
			flew = true;
		}
		if(!flew)
			throw 9;
	}*/

	while(input != 0) {
		cout << endl << " --- Menu --- " << endl;
		cout << "(1) Insert a new customer." << endl;
		cout << "(2) Retrieve a customer." << endl;
		cout << "(3) Show all entries." << endl;
		cout << "(4) Show hash-table." << endl;
		cout << "(5) Save hash table." << endl;
		cout << "(0) Exit." << endl;

		cin >> input;
			switch(input) {
				case 1:
					cout << "Specify the customer id: ";
					cin >> custId;
					cout << endl;
					cout << "Specify the customer name: ";
					cin >> inputName;
					cout << endl;
					cout << "Specify the skonto: ";
					cin >> skonto;
					cout << endl;
					try {
						hashdat.Insert(custId, inputName, skonto);
					}
					catch (HashDat<(SIZE)>::BadArticle& e) {
						cout << e.GetReason() << endl;
					}
					break;
				case 2:
					cout << "Specify the customer id: ";
					cin >> custId;
					cout << endl;
					try {
						tmp = hashdat.Retrieve(custId);
						cout << "Customer id: " << tmp.GetKunNum() << endl;
						cout << "Name: " << tmp.GetKunName() << endl;
						cout << "Skonto: " << tmp.GetSkonto() << endl;
						cout << endl;
					}
					catch (HashDat<(SIZE)>::NotFound& e) {
						cout << e.GetReason() << endl;
					}
					break;
				case 3:
					hashdat.Show();
					break;
				case 4:
					hashdat.ShowTable();
					break;
				case 5:
					hashdat.SaveTable();
					break;
				default: 
					cout << "Unsupported input parameter. Please repeat." << endl;
			}
	}

	hashdat.SaveTable();
	return 0;
}