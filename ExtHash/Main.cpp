#include "HashDat.h"

#define SIZE 1

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


	/*cout << "Create a new database or use an existing one." << endl;
	cout << "Specify location for data file: ";
	cin >> file;
	cout << endl;
	cout << "Specify location for the configuration file: ";
	cin >> conf;
	cout << endl;
	HashDat<(SIZE)> hashdat(file, conf);*/

	HashDat<(SIZE)> hashdat("killme");

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