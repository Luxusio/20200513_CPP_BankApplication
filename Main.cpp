#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <Windows.h>

using namespace std;

#define TEXT_LEN 20
#define BANK_BOOK_SIZE 100
#define BANK_SIZE 100

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
	// 이자 1%
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
	// 이자 2%
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
		cusName(NULL), regNum(NULL), 
		normalBankBooks(NULL), depositBankBooks(NULL), 
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

public:
	// getter
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

	virtual ~Bank() {
		if (this->accounts != NULL) delete[] accounts;
	}

public:

	const void createAccount(char* cusName, char* regNum) {
		if (isAccountExists(cusName, regNum)) {
			cout << "해당 이름과 주민번호로 된 계정이 이미 존재합니다." << endl;
			return;
		}

		if (top == BANK_SIZE - 1) {
			cout << "은행이 가득 찼습니다." << endl;
			return;
		}
		
		Account acc(cusName, regNum);

		accounts[top++] = acc;
		
		cout << "계정이 생성되었습니다" << endl;
	}

	const void createBankBook(const char* cusName, const char* regNum, const int accID, const int balance, const int type) {
		Account* account = this->getAccount(cusName, regNum);

		if (account == NULL) {
			cout << "존재하지 않는 계정입니다." << endl;
			return;
		}

		if (getBankBook(accID) != NULL) {
			cout << "해당 계좌번호는 이미 존재합니다." << endl;
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
			cout << "잘못된 계좌 종류입니다." << endl;
			return;
		}

		if (balance < 10) {
			cout << "계좌개설 시 10원이상의 입금액을 넣어야 합니다." << endl;
			return;
		}

		if (!isNormalBankBook && account->getNormalBankBookTop() == 0) {
			cout << "입금계좌를 개설하기 위해서는 보통계좌가 개설되어 있어야 합니다." << endl;
			return;
		}

		if (account->createBankBook(accID, balance, isNormalBankBook)) {
			if (isNormalBankBook) {
				cout << "보통계좌가 개설되었습니다." << endl;
			}
			else {
				cout << "입금계좌가 개설되었습니다." << endl;
			}
		}
		else {
			if (isNormalBankBook) {
				cout << "더이상 보통계좌를 생성할 수 없습니다." << endl;
			}
			else {
				cout << "더이상 입금계좌를 생성할 수 없습니다." << endl;
			}
		}

	}
	

	const bool bankDeposit(int accID, int value) {
		BankBook* bankBook = this->getBankBook(accID);

		if (bankBook == NULL) {
			cout << "계좌가 존재하지 않습니다." << endl;
			return false;
		}

		if (bankBook->deposit(value)) {
			cout << value << "원을 입금했습니다." << endl;
			return true;
		}
		else {
			cout << "값이 0보다 작을 수는 없습니다." << endl;
			return false;
		}
	}

	const bool bankWithdraw(int accID, int value) {
		BankBook* bankBook = this->getBankBook(accID);

		if (bankBook == NULL) {
			cout << "계좌가 존재하지 않습니다." << endl;
			return false;
		}

		if (bankBook->withdraw(value)) {
			cout << value << "원을 출금했습니다." << endl;
			return true;
		}
		else {
			cout << "값이 0보다 작을 수는 없습니다." << endl;
			return false;
		}

	}

	const bool printAccountData(const char* cusName, const char* regNum) const {
		const Account* account = this->getAccount(cusName, regNum);
		if (account == NULL) {
			cout << "존재하지 않는 ID입니다." << endl;
			return false;
		}

		cout << "소유주 : " << account->getName() << endl;
		cout << "주민번호 : " << account->getRegNum() << endl;

		BankBook* bb;
		int top;

		bb = account->getNormalBankBook();
		top = account->getNormalBankBookTop();
		for (int i = 0; i < top; i++) {
			cout << "보통계좌(" << bb[i].getID() << ") " << bb[i].getBalance() << "원" << endl;
		}

		bb = account->getDepositBankBook();
		top = account->getDepositBankBookTop();
		for (int i = 0; i < top; i++) {
			cout << "예금계좌(" << bb[i].getID() << ") " << bb[i].getBalance() << "원" << endl;
		}

		return true;
	}

public:

	const bool isBankBookIDExists(const int accID) const {
		return getBankBook(accID) != NULL;
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
		return NULL;
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
		if (no == -1) return NULL;
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
		:bank(bankApp.bank), select(bankApp.select), running(bankApp.running) {
	}

	virtual ~AccountManager() {
		if (this->bank != NULL) delete bank;
	}

public:

	const void printMenu() const {
		cout << "-----Menu-----" << endl;
		cout << "1. 계정생성" << endl;
		cout << "2. 계좌개설" << endl;
		cout << "3. 입    금" << endl;
		cout << "4. 출    금" << endl;
		cout << "5. 계정정보 전체 출력" << endl;
		cout << "6. 프로그램 종료" << endl;
		cout << "선택: ";
	}

	void cycle() {
		cin >> select;
		int accID, balance, select2;
		char* cusName = NULL;
		char* regNum = NULL;

		cout << endl;

		switch (select) {
		case 1:
			cout << "[계정생성]" << endl;
			cusName = inputCharArray("이  름");
			regNum = inputCharArray("주민번호");

			bank->createAccount(cusName, regNum);
			break;
		case 2:
			cout << "[계좌개설]" << endl;
			cusName = inputCharArray("이  름");
			regNum = inputCharArray("주민번호");
			accID = inputInt("계좌번호");
			select2 = inputInt("계좌종류(1:보통, 2:입금)");
			balance = inputInt("입금액");

			bank->createBankBook(cusName, regNum, accID, balance, select2);
			break;
		case 3:
			cout << "[입   금]" << endl;
			accID = inputInt("계좌번호");
			balance = inputInt("입금액");

			bank->bankDeposit(accID, balance);
			break;
		case 4:
			cout << "[출   금]" << endl;
			accID = inputInt("계좌번호");
			balance = inputInt("출금액");

			bank->bankWithdraw(accID, balance);
			break;
		case 5:
			cout << "[계좌정보]" << endl;
			cusName = inputCharArray("이  름");
			regNum = inputCharArray("주민번호");

			bank->printAccountData(cusName, regNum);
			break;
		case 6:
			cout << "프로그램이 종료됩니다." << endl;
			running = false;

			break;
		default:
			cout << "잘못된 값입니다." << endl;
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
		cout << "이미 존재하는 ID입니다." << endl;
		return;
	}

	if (*top == BANK_SIZE - 1) {
		cout << "은행이 가득 찼습니다." << endl;
		return;
	}

	if (balance < 10) {
		cout << "계좌개설 시 10원이상의 입금액을 넣어야 합니다." << endl;
		return;

	}

	Account* acc = new Account;
	acc->accID = accID;
	acc->balance = balance;
	strcpy_s(acc->cusName, cusName);

	(*bank)[*top] = *acc;
	*top++;

	cout << "계좌가 개설되었습니다" << endl;

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
		cout << "존재하지 않는 ID입니다." << endl;
		return;
	}

	if (value <= 0) {
		cout << "값이 0보다 작을 수는 없습니다." << endl;
		return;
	}

	(*bank)[accNum].balance += value;

	cout << value << "원을 입금했습니다." << endl;

}

void bankWithdraw(Account** bank, int top, int accID, int value) {
	int accNum = getAccNum(*bank, top, accID);

	if (accNum == -1) {
		cout << "존재하지 않는 ID입니다." << endl;
		return;
	}

	if (value <= 0) {
		cout << "값이 0보다 작을 수는 없습니다." << endl;
		return;
	}

	(*bank)[accNum].balance -= value;

	cout << value << "원을 출금했습니다." << endl;
}

void printAccountData(Account** bank, int top, int accID) {
	int accNum = getAccNum(*bank, top, accID);
	if (accNum == -1) {
		cout << "존재하지 않는 ID입니다." << endl;
		return;
	}
	Account ac = (*bank)[accNum];
	
	cout << "계좌번호 : " << ac.accID << endl;
	cout << "금액 : " << ac.balance << endl;
	cout << "소유주 : " << ac.cusName << endl;

}

void main() {
	Account* bank = (Account*)calloc(BANK_SIZE, sizeof(Account));
	int top = 0;
	int select = 0;

	bool run = true;

	while (run) {
		cout << "-----Menu-----" << endl;
		cout << "1. 계좌개설" << endl;
		cout << "2. 입   금" << endl;
		cout << "3. 출   금" << endl;
		cout << "4. 계좌정보 전체 출력" << endl;
		cout << "5. 프로그램 종료" << endl;
		cout << "선택: ";
		cin >> select;

		switch (select) {
		case 1:
			int accID, balance;
			char* cusName = new char[NAME_LEN];
			
			cout << "[계좌개설]" << endl;
			cout << "계좌ID: ";
			cin >> accID;
			cout << "이  름: ";
			cin >> cusName;
			cout << "입금액: ";
			cin >> balance;

			addAccount(&bank, &top, accID, cusName, balance);

			break;
		case 2:
			cout << "[입   금]" << endl;
			cout << "계좌ID: ";
			cin >> accID;
			cout << "입금액: ";
			cin >> balance;

			bankDeposit(&bank, top, accID, balance);
			break;
		case 3:
			cout << "[출   금]" << endl;
			cout << "계좌ID: ";
			cin >> accID;
			cout << "출금액: ";
			cin >> balance;

			bankWithdraw(&bank, top, accID, balance);
			break;
		case 4:
			cout << "[계좌정보]" << endl;
			cout << "계좌ID: ";
			cin >> accID;

			printAccountData(&bank, top, accID);
			break;
		case 5:

			cout << "프로그램이 종료됩니다." << endl;
			run = false;

			if (bank != NULL) delete[] bank;

			break;
		default:

		}


	}


	free(bank);
}
*/