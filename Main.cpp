#include <iostream>
#include <hash_map>

using namespace std;
using namespace stdext;

#define NAME_LEN 20
#define BANK_SIZE 100

class Account {
private:
	int accID;		// Account ID
	int balance;	// Balance
	char* cusName;	// Customer Name
	char* regNum;	// Registration Number

	void addBalance(int value) {
		this->balance += value;
	}

public:

	Account() {}

	~Account() {
		if (this->cusName != NULL) delete[] cusName;
		if (this->regNum != NULL) delete[] regNum;
	}

	void initAccount(int accID, int balance, char*& cusName, char*& regNum) {
		this->accID = accID;
		this->balance = balance;
		this->cusName = cusName;
		//cusName = new char[BANK_SIZE];
		this->regNum = regNum;
	}

	int getAccID() {
		return this->accID;
	}

	int getBalance() {
		return this->balance;
	}

	bool deposit(int amount) {
		if (amount < 0) return false;
		this->addBalance(amount);
		return true;
	}

	bool withdraw(int amount) {
		if (amount < 0) return false;
		this->addBalance(-amount);
		return true;
	}

	char* getName() {
		return this->cusName;
	}

};

class Bank {
private:
	Account* accounts;
	int top;

public:

	Bank(int maxSize) {
		accounts = new Account[maxSize];
		top = 0;
	}

	~Bank() {
		if (this->accounts != NULL) delete[] accounts;
	}

	bool hasAccID(int accID) {
		return getAccNum(accID) != -1;
	}

	int getAccNum(int accID) {
		int i;
		for (i = 0; i < top; i++) {
			if (accounts[i].getAccID() == accID) return i;
		}
		return -1;
	}

	Account* getAccount(int accID) {
		int no = this->getAccNum(accID);
		if (no == -1) return NULL;
		return &(accounts[no]);
	}

	void addAccount(int accID, int balance, char*& cusName, char*& regNum) {
		if (hasAccID(accID)) {
			cout << "�̹� �����ϴ� ID�Դϴ�." << endl;
			return;
		}

		if (top == BANK_SIZE - 1) {
			cout << "������ ���� á���ϴ�." << endl;
			return;
		}

		if (balance < 10) {
			cout << "���°��� �� 10���̻��� �Աݾ��� �־�� �մϴ�." << endl;
			return;
		}

		Account* acc = new Account();
		acc->initAccount(accID, balance, cusName, regNum);

		//acc->accID = accID;
		//acc->balance = balance;
		//strcpy_s(acc->cusName, cusName);

		accounts[top] = *acc;
		top++;

		cout << "���°� �����Ǿ����ϴ�" << endl;
	}
	

	bool bankDeposit(int accID, int value) {
		Account* account = this->getAccount(accID);

		if (account == NULL) {
			cout << "�������� �ʴ� ID�Դϴ�." << endl;
			return false;
		}

		if (account->deposit(value)) {
			cout << "���� 0���� ���� ���� �����ϴ�." << endl;
			return false;
		}
		else {
			cout << value << "���� �Ա��߽��ϴ�." << endl;
			return true;
		}
	}

	bool bankWithdraw(int accID, int value) {
		Account* account = this->getAccount(accID);

		if (account == NULL) {
			cout << "�������� �ʴ� ID�Դϴ�." << endl;
			return false;
		}

		if (account->withdraw(value)) {
			cout << "���� 0���� ���� ���� �����ϴ�." << endl;
			return false;
		}
		else {
			cout << value << "���� ����߽��ϴ�." << endl;
			return true;
		}

	}

	bool printAccountData(int accID) {
		Account* account = this->getAccount(accID);
		if (account == NULL) {
			cout << "�������� �ʴ� ID�Դϴ�." << endl;
			return false;
		}

		cout << "���¹�ȣ : " << account->getAccID() << endl;
		cout << "�ݾ� : " << account->getBalance() << endl;
		cout << "������ : " << account->getName() << endl;
		return true;
	}

};

class BankApplication {
private:
	int select;
	Bank* bank;
	bool running;


	void printMenu() {
		cout << "-----Menu-----" << endl;
		cout << "1. ���°���" << endl;
		cout << "2. ��   ��" << endl;
		cout << "3. ��   ��" << endl;
		cout << "4. �������� ��ü ���" << endl;
		cout << "5. ���α׷� ����" << endl;
		cout << "����: ";
	}

	int inputInt(const char* text) {
		int value;
		cout << text;
		cin >> value;
		return value;
	}

	char* inputCharArray(const char* text) {
		char* value;
		cout << text << ":";
		cin >> value;
		return value;
	}

	void cycle() {
		cin >> select;

		switch (select) {
		case 1:
			int accID, balance;
			char* cusName;
			char* cusNum;
			//= new char[NAME_LEN];

			cout << "[���°���]" << endl;
			accID = inputInt("����ID");
			cusName = inputCharArray("��  ��");
			cusNum = inputCharArray("�ֹι�ȣ");
			balance = inputInt("�Աݾ�");

			bank->addAccount(accID, balance, cusName, cusNum);
			break;
		case 2:
			cout << "[��   ��]" << endl;
			accID = inputInt("����ID");
			balance = inputInt("�Աݾ�");

			bank->bankDeposit(accID, balance);
			break;
		case 3:
			cout << "[��   ��]" << endl;
			accID = inputInt("����ID");
			balance = inputInt("��ݾ�");

			bank->bankWithdraw(accID, balance);
			break;
		case 4:
			cout << "[��������]" << endl;
			accID = inputInt("����ID");

			bank->printAccountData(accID);
			break;
		case 5:
			cout << "���α׷��� ����˴ϴ�." << endl;
			running = false;

			break;
		default:

		}

	}

public:

	BankApplication() {
		this->bank = new Bank(BANK_SIZE);
		this->select = 0;
		this->running = true;
	}

	~BankApplication() {
		if (this->bank != NULL) delete[] bank;
	}

	void run() {
		while (this->running) {
			this->printMenu();
			this->cycle();
		}
	}

};

void main() {
	BankApplication* bankApplication = new BankApplication();
	bankApplication->run();

	if (bankApplication != NULL) delete[] bankApplication;
}

// OLD CODE ========================
/*
typedef struct {
	int accID;
	int balance;
	char cusName[NAME_LEN];
} Account;

void addAccount(Account** bank, int* top, int accID, char cusName[NAME_LEN], int balance) {

	if (hasAccID(*bank, *top, accID)) {
		cout << "�̹� �����ϴ� ID�Դϴ�." << endl;
		return;
	}

	if (*top == BANK_SIZE - 1) {
		cout << "������ ���� á���ϴ�." << endl;
		return;
	}

	if (balance < 10) {
		cout << "���°��� �� 10���̻��� �Աݾ��� �־�� �մϴ�." << endl;
		return;

	}

	Account* acc = new Account;
	acc->accID = accID;
	acc->balance = balance;
	strcpy_s(acc->cusName, cusName);

	(*bank)[*top] = *acc;
	*top++;

	cout << "���°� �����Ǿ����ϴ�" << endl;

}

bool hasAccID(Account* bank, int top, int accID) {
	return getAccNum(bank, top, accID) != -1;
}

int getAccNum(Account* bank, int top, int accID) {
	int i;
	for (i = 0; i < top; i++) {
		if (bank[i].accID == accID) return i;
	}
	return -1;
}

void bankDeposit(Account** bank, int top, int accID, int value) {
	int accNum = getAccNum(*bank, top, accID);

	if (accNum == -1) {
		cout << "�������� �ʴ� ID�Դϴ�." << endl;
		return;
	}

	if (value <= 0) {
		cout << "���� 0���� ���� ���� �����ϴ�." << endl;
		return;
	}

	(*bank)[accNum].balance += value;

	cout << value << "���� �Ա��߽��ϴ�." << endl;

}

void bankWithdraw(Account** bank, int top, int accID, int value) {
	int accNum = getAccNum(*bank, top, accID);

	if (accNum == -1) {
		cout << "�������� �ʴ� ID�Դϴ�." << endl;
		return;
	}

	if (value <= 0) {
		cout << "���� 0���� ���� ���� �����ϴ�." << endl;
		return;
	}

	(*bank)[accNum].balance -= value;

	cout << value << "���� ����߽��ϴ�." << endl;
}

void printAccountData(Account** bank, int top, int accID) {
	int accNum = getAccNum(*bank, top, accID);
	if (accNum == -1) {
		cout << "�������� �ʴ� ID�Դϴ�." << endl;
		return;
	}
	Account ac = (*bank)[accNum];
	
	cout << "���¹�ȣ : " << ac.accID << endl;
	cout << "�ݾ� : " << ac.balance << endl;
	cout << "������ : " << ac.cusName << endl;

}

void main() {
	Account* bank = (Account*)calloc(BANK_SIZE, sizeof(Account));
	int top = 0;
	int select = 0;

	bool run = true;

	while (run) {
		cout << "-----Menu-----" << endl;
		cout << "1. ���°���" << endl;
		cout << "2. ��   ��" << endl;
		cout << "3. ��   ��" << endl;
		cout << "4. �������� ��ü ���" << endl;
		cout << "5. ���α׷� ����" << endl;
		cout << "����: ";
		cin >> select;

		switch (select) {
		case 1:
			int accID, balance;
			char* cusName = new char[NAME_LEN];
			
			cout << "[���°���]" << endl;
			cout << "����ID: ";
			cin >> accID;
			cout << "��  ��: ";
			cin >> cusName;
			cout << "�Աݾ�: ";
			cin >> balance;

			addAccount(&bank, &top, accID, cusName, balance);

			break;
		case 2:
			cout << "[��   ��]" << endl;
			cout << "����ID: ";
			cin >> accID;
			cout << "�Աݾ�: ";
			cin >> balance;

			bankDeposit(&bank, top, accID, balance);
			break;
		case 3:
			cout << "[��   ��]" << endl;
			cout << "����ID: ";
			cin >> accID;
			cout << "��ݾ�: ";
			cin >> balance;

			bankWithdraw(&bank, top, accID, balance);
			break;
		case 4:
			cout << "[��������]" << endl;
			cout << "����ID: ";
			cin >> accID;

			printAccountData(&bank, top, accID);
			break;
		case 5:

			cout << "���α׷��� ����˴ϴ�." << endl;
			run = false;

			if (bank != NULL) delete[] bank;

			break;
		default:

		}


	}


	free(bank);
}
*/