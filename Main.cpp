#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <Windows.h>

#define TEXT_LEN 20
#define BANK_BOOK_SIZE 100
#define BANK_SIZE 100

using namespace std;


class BankBook {
private:

	int id; // bank book id
	double balance;	// Balance

protected:

	virtual double getInterestPercent() = 0;

	BankBook() : id(-2147483648), balance(0.0) {

	}

	BankBook(int bankBookID, double balance) : id(bankBookID), balance(balance) {

	}

	BankBook(BankBook& bankBook) : id(bankBook.id), balance(bankBook.balance) {

	}

	virtual ~BankBook() {

	}

public:

	const bool deposit(double amount) {
		if (amount < 0) return false;
		this->balance += amount;
		this->balance += getInterestPercent() * this->balance;
		return true;
	}

	const bool withdraw(double amount) {
		if (amount < 0) return false;
		this->balance -= amount;
		return true;
	}

public:

	const int getID() const {
		return this->id;
	}

	const double getBalance() const {
		return this->balance;
	}

};

class NormalBankBook : public BankBook {
protected:
	// ���� 1%
	virtual double getInterestPercent() final {
		return 0.01;
	}

public:

	NormalBankBook() : BankBook() {

	}

	NormalBankBook(int id, int amount) : BankBook(id, amount) {

	}

	NormalBankBook(NormalBankBook& bankBook) : BankBook(bankBook) {

	}

};

class DepositBankBook : public BankBook {
protected:
	// ���� 2%
	virtual double getInterestPercent() final {
		return 0.02;
	}

public:

	DepositBankBook() : BankBook() {

	}

	DepositBankBook(int id, int amount) : BankBook(id, amount) {

	}

	DepositBankBook(DepositBankBook& bankBook) : BankBook(bankBook) {

	}

};

class Account {
private:

	char* cusName;	// Customer Name
	char* regNum;	// Registration Number

	NormalBankBook* normalBankBooks;
	DepositBankBook* depositBankBooks;

	int normalBankBookTop;
	int depositBankBookTop;

public:
	
	Account():
		cusName(nullptr), regNum(nullptr),
		normalBankBooks(nullptr), depositBankBooks(nullptr),
		normalBankBookTop(0), depositBankBookTop(0) {

	}

	Account(const char* cusName, const char* regNum) :
		cusName(new char[TEXT_LEN]), regNum(new char[TEXT_LEN]),
		normalBankBooks(new NormalBankBook[BANK_BOOK_SIZE]), depositBankBooks(new DepositBankBook[BANK_BOOK_SIZE]),
		normalBankBookTop(0), depositBankBookTop(0) {

		strcpy(this->cusName, cusName);
		strcpy(this->regNum, regNum);

	}
	
	Account(const Account& account): 
		cusName(new char[TEXT_LEN]), regNum(new char[TEXT_LEN]), 
		normalBankBooks(new NormalBankBook[BANK_BOOK_SIZE]), depositBankBooks(new DepositBankBook[BANK_BOOK_SIZE]),
		normalBankBookTop(account.normalBankBookTop), depositBankBookTop(account.depositBankBookTop) {

		for (int i = 0; i < account.normalBankBookTop; i++) {
			this->normalBankBooks[i] = NormalBankBook(account.normalBankBooks[i]);
		}
		for (int i = 0; i < account.depositBankBookTop; i++) {
			this->depositBankBooks[i] = DepositBankBook(account.depositBankBooks[i]);
		}

		strcpy(this->cusName, account.cusName);
		strcpy(this->regNum, account.regNum);
	}

	virtual ~Account() {
		if (this->cusName != nullptr) delete[] cusName;
		if (this->regNum != nullptr) delete[] regNum;
	}

public:

	bool createBankBook(int id, int amount, bool isNormalBankBook) {
		if (isNormalBankBook) {
			if (this->normalBankBookTop == BANK_BOOK_SIZE) {
				return false;
			}
			this->normalBankBooks[normalBankBookTop++] = NormalBankBook(id, amount);
			return true;
		}
		else {
			if (this->depositBankBookTop == BANK_BOOK_SIZE) {
				return false;
			}
			this->depositBankBooks[depositBankBookTop++] = DepositBankBook(id, amount);
			return true;
		}

	}

public: // getter
	
	char* getName() const {
		return this->cusName;
	}

	char* getRegNum() const {
		return this->regNum;
	}

	NormalBankBook* getNormalBankBook() const {
		return this->normalBankBooks;
	}

	DepositBankBook* getDepositBankBook() const {
		return this->depositBankBooks;
	}

	int getNormalBankBookTop() const {
		return this->normalBankBookTop;
	}

	int getDepositBankBookTop() const {
		return this->depositBankBookTop;
	}

	BankBook* getBankBook(bool isNormalBankBook) {
		if (isNormalBankBook) {
			return this->normalBankBooks;
		}
		else {
			return this->depositBankBooks;
		}
	}

};

class Bank {
private:
	Account* accounts;
	int top;

public:

	Bank(): accounts(new Account[BANK_SIZE]()), top(0) {
		
	}

	Bank(const Bank& bank): accounts(new Account[BANK_SIZE]()), top(bank.top) {

		for (int i = 0; i < bank.top; i++) {
			this->accounts[i] = Account(bank.accounts[i]);
		}

	}

	virtual ~Bank() {
		if (this->accounts != nullptr) delete[] accounts;
	}

public:

	const void createAccount(char* cusName, char* regNum) {
		if (isAccountExists(cusName, regNum)) {
			cout << "�ش� �̸��� �ֹι�ȣ�� �� ������ �̹� �����մϴ�." << endl;
			return;
		}

		if (top == BANK_SIZE - 1) {
			cout << "������ ���� á���ϴ�." << endl;
			return;
		}
		
		Account acc(cusName, regNum);

		accounts[top++] = acc;
		
		cout << "������ �����Ǿ����ϴ�" << endl;
	}

	const void createBankBook(const char* cusName, const char* regNum, const int accID, const int balance, const int type) {
		Account* account = this->getAccount(cusName, regNum);

		if (account == nullptr) {
			cout << "�������� �ʴ� �����Դϴ�." << endl;
			return;
		}

		if (getBankBook(accID) != nullptr) {
			cout << "�ش� ���¹�ȣ�� �̹� �����մϴ�." << endl;
			return;
		}

		bool isNormalBankBook;
		switch (type) {
		case 1:
			isNormalBankBook = true;
			break;
		case 2:
			isNormalBankBook = false;
			break;
		default:
			cout << "�߸��� ���� �����Դϴ�." << endl;
			return;
		}

		if (balance < 10) {
			cout << "���°��� �� 10���̻��� �Աݾ��� �־�� �մϴ�." << endl;
			return;
		}

		if (!isNormalBankBook && account->getNormalBankBookTop() == 0) {
			cout << "�Աݰ��¸� �����ϱ� ���ؼ��� ������°� �����Ǿ� �־�� �մϴ�." << endl;
			return;
		}

		if (account->createBankBook(accID, balance, isNormalBankBook)) {
			if (isNormalBankBook) {
				cout << "������°� �����Ǿ����ϴ�." << endl;
			}
			else {
				cout << "�Աݰ��°� �����Ǿ����ϴ�." << endl;
			}
		}
		else {
			if (isNormalBankBook) {
				cout << "���̻� ������¸� ������ �� �����ϴ�." << endl;
			}
			else {
				cout << "���̻� �Աݰ��¸� ������ �� �����ϴ�." << endl;
			}
		}

	}
	

	const bool bankDeposit(int accID, int value) {
		BankBook* bankBook = this->getBankBook(accID);

		if (bankBook == nullptr) {
			cout << "���°� �������� �ʽ��ϴ�." << endl;
			return false;
		}

		if (bankBook->deposit(value)) {
			cout << value << "���� �Ա��߽��ϴ�." << endl;
			return true;
		}
		else {
			cout << "���� 0���� ���� ���� �����ϴ�." << endl;
			return false;
		}
	}

	const bool bankWithdraw(int accID, int value) {
		BankBook* bankBook = this->getBankBook(accID);

		if (bankBook == nullptr) {
			cout << "���°� �������� �ʽ��ϴ�." << endl;
			return false;
		}

		if (bankBook->withdraw(value)) {
			cout << value << "���� ����߽��ϴ�." << endl;
			return true;
		}
		else {
			cout << "���� 0���� ���� ���� �����ϴ�." << endl;
			return false;
		}

	}

	const bool printAccountData(const char* cusName, const char* regNum) const {
		const Account* account = this->getAccount(cusName, regNum);
		if (account == nullptr) {
			cout << "�������� �ʴ� ID�Դϴ�." << endl;
			return false;
		}

		cout << "������ : " << account->getName() << endl;
		cout << "�ֹι�ȣ : " << account->getRegNum() << endl;

		BankBook* bb;
		int top;

		bb = account->getNormalBankBook();
		top = account->getNormalBankBookTop();
		for (int i = 0; i < top; i++) {
			cout << "�������(" << bb[i].getID() << ") " << bb[i].getBalance() << "��" << endl;
		}

		bb = account->getDepositBankBook();
		top = account->getDepositBankBookTop();
		for (int i = 0; i < top; i++) {
			cout << "���ݰ���(" << bb[i].getID() << ") " << bb[i].getBalance() << "��" << endl;
		}

		return true;
	}

public:

	const bool isBankBookIDExists(const int bankBookID) const {
		return getBankBook(bankBookID) != nullptr;
	}

	const bool isAccountExists(const char* cusName, const char* regNum) const {
		return getAccNum(cusName, regNum) != -1;
	}

	BankBook* getBankBook(const int accID) const {
		for (int i = 0; i < top; i++) {
			Account acc = this->accounts[i];

			BankBook* bb;
			int bankBookTop;

			bb = acc.getNormalBankBook();
			bankBookTop = acc.getNormalBankBookTop();
			for (int j = 0; j < bankBookTop; j++) {
				if (bb[j].getID() == accID) return &bb[j];
			}

			bb = acc.getDepositBankBook();
			bankBookTop = acc.getDepositBankBookTop();
			for (int j = 0; j < bankBookTop; j++) {
				if (bb[j].getID() == accID) return &bb[j];
			}
		}
		return nullptr;
	}

	const int getAccNum(const char* cusName, const char* regNum) const {
		for (int i = 0; i < top; i++) {
			Account acc = this->accounts[i];

			if (strcmp(acc.getName(), cusName) == 0 &&
				strcmp(acc.getRegNum(), regNum) == 0) {
				return i;
			}
		}
		return -1;
	}

	Account* getAccount(const char* cusName, const char* regNum) const {
		int no = this->getAccNum(cusName, regNum);
		if (no == -1) return nullptr;
		return &accounts[no];
	}

};

class AccountManager {
private:

	int select;
	Bank* bank;
	bool running;

	int inputInt(const char* text) const {
		int value;
		cout << text << ":";
		cin >> value;
		return value;
	}

	char* inputCharArray(const char* text) const {
		char* value = new char[TEXT_LEN];
		cout << text << ":";
		cin >> value;
		return value;
	}

public:
	
	AccountManager()
		:bank(new Bank()), select(0), running(true) {

	}

	AccountManager(const AccountManager& bankApp)
		:bank(Bank(bankApp.bank)), select(bankApp.select), running(bankApp.running) {
	}

	virtual ~AccountManager() {
		if (this->bank != nullptr) delete bank;
	}

public:

	const void printMenu() const {
		cout << "-----Menu-----" << endl;
		cout << "1. ��������" << endl;
		cout << "2. ���°���" << endl;
		cout << "3. ��    ��" << endl;
		cout << "4. ��    ��" << endl;
		cout << "5. �������� ��ü ���" << endl;
		cout << "6. ���α׷� ����" << endl;
		cout << "����: ";
	}

	void cycle() {
		cin >> select;
		int accID, balance, select2;
		char* cusName = nullptr;
		char* regNum = nullptr;

		cout << endl;

		switch (select) {
		case 1:
			cout << "[��������]" << endl;
			cusName = inputCharArray("��  ��");
			regNum = inputCharArray("�ֹι�ȣ");

			bank->createAccount(cusName, regNum);
			break;
		case 2:
			cout << "[���°���]" << endl;
			cusName = inputCharArray("��  ��");
			regNum = inputCharArray("�ֹι�ȣ");
			accID = inputInt("���¹�ȣ");
			select2 = inputInt("��������(1:����, 2:�Ա�)");
			balance = inputInt("�Աݾ�");

			bank->createBankBook(cusName, regNum, accID, balance, select2);
			break;
		case 3:
			cout << "[��   ��]" << endl;
			accID = inputInt("���¹�ȣ");
			balance = inputInt("�Աݾ�");

			bank->bankDeposit(accID, balance);
			break;
		case 4:
			cout << "[��   ��]" << endl;
			accID = inputInt("���¹�ȣ");
			balance = inputInt("��ݾ�");

			bank->bankWithdraw(accID, balance);
			break;
		case 5:
			cout << "[��������]" << endl;
			cusName = inputCharArray("��  ��");
			regNum = inputCharArray("�ֹι�ȣ");

			bank->printAccountData(cusName, regNum);
			break;
		case 6:
			cout << "���α׷��� ����˴ϴ�." << endl;
			running = false;

			break;
		default:
			cout << "�߸��� ���Դϴ�." << endl;
		}

		if (cusName != nullptr) delete[] cusName;
		if (regNum != nullptr) delete[] regNum;
	}

	const bool isRunning() {
		return this->running;
	}

};

int main() {
	AccountManager manager;

	while (manager.isRunning()) {
		manager.printMenu();
		manager.cycle();

		Sleep(2000);
		system("cls");
	}

	return 0;
}

// OLD C Style CODE ==========================================
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