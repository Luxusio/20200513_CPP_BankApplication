#include <iostream>
//#include <hash_map>

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
	
public:
	
	Account():accID(-2147483648), balance(0), cusName(NULL), regNum(NULL) {
	}
	
	Account(const Account& account):accID(account.accID), balance(account.balance), 
		cusName(new char[NAME_LEN]), regNum(new char[NAME_LEN]){
		strcpy(account.cusName, this->cusName);
		strcpy(account.regNum, this->regNum);
	}

	~Account() {
		if (this->cusName != NULL) delete[] cusName;
		if (this->regNum != NULL) delete[] regNum;
	}

	Account(int accID, int balance, char*& cusName, char*& regNum)
		:accID(accID), balance(balance), cusName(cusName), regNum(regNum){

	}

	const int getAccID() const {
		return this->accID;
	}

	const int getBalance() const {
		return this->balance;
	}

	const bool deposit(int amount) {
		if (amount < 0) return false;
		this->balance += amount;
		return true;
	}

	const bool withdraw(int amount) {
		if (amount < 0) return false;
		this->balance -= amount;
		return true;
	}

	const char* getName() const {
		return this->cusName;
	}

	const char* getRegNum() const {
		return this->regNum;
	}

};

class Bank {
private:
	Account* accounts;
	int top;

public:

	Bank(int maxSize) {
		accounts = new Account[maxSize]();
		top = 0;
	}

	~Bank() {
		if (this->accounts != NULL) delete[] accounts;
	}

	const bool hasAccID(const int accID) const {
		return getAccNum(accID) != -1;
	}

	const int getAccNum(const int accID) const {
		int i;
		for (i = 0; i < top; i++) {
			if (accounts[i].getAccID() == accID) return i;
		}
		return -1;
	}

	Account* getAccount(const int accID) const {
		int no = this->getAccNum(accID);
		if (no == -1) return NULL;
		return &accounts[no];
	}

	const void addAccount(const int accID, const int balance, char*& cusName, char*& regNum) {
		if (hasAccID(accID)) {
			cout << "이미 존재하는 ID입니다." << endl;
			return;
		}

		if (top == BANK_SIZE - 1) {
			cout << "은행이 가득 찼습니다." << endl;
			return;
		}

		if (balance < 10) {
			cout << "계좌개설 시 10원이상의 입금액을 넣어야 합니다." << endl;
			return;
		}

		Account acc(accID, balance, cusName, regNum);

		accounts[top] = acc;
		top++;

		cout << "계좌가 개설되었습니다" << endl;
	}
	

	const bool bankDeposit(int accID, int value) {
		Account* account = this->getAccount(accID);

		if (account == NULL) {
			cout << "존재하지 않는 ID입니다." << endl;
			return false;
		}

		if (!account->deposit(value)) {
			cout << "값이 0보다 작을 수는 없습니다." << endl;
			return false;
		}
		else {
			cout << value << "원을 입금했습니다." << endl;
			return true;
		}
	}

	const bool bankWithdraw(int accID, int value) {
		Account* account = this->getAccount(accID);

		if (account == NULL) {
			cout << "존재하지 않는 ID입니다." << endl;
			return false;
		}

		if (!account->withdraw(value)) {
			cout << "값이 0보다 작을 수는 없습니다." << endl;
			return false;
		}
		else {
			cout << value << "원을 출금했습니다." << endl;
			return true;
		}

	}

	const bool printAccountData(int accID) const {
		const Account* account = this->getAccount(accID);
		if (account == NULL) {
			cout << "존재하지 않는 ID입니다." << endl;
			return false;
		}

		cout << "계좌번호 : " << account->getAccID() << endl;
		cout << "금액 : " << account->getBalance() << endl;
		cout << "소유주 : " << account->getName() << endl;
		return true;
	}

};

class AccountManager {
private:
	int select;
	Bank* bank;
	bool running;


	const void printMenu() const {
		cout << "-----Menu-----" << endl;
		cout << "1. 계좌개설" << endl;
		cout << "2. 입   금" << endl;
		cout << "3. 출   금" << endl;
		cout << "4. 계좌정보 전체 출력" << endl;
		cout << "5. 프로그램 종료" << endl;
		cout << "선택: ";
	}

	const int inputInt(const char* text) const {
		int value;
		cout << text << ":";
		cin >> value;
		return value;
	}

	char* inputCharArray(const char* text) const {
		char* value = new char[NAME_LEN];
		cout << text << ":";
		cin >> value;
		return value;
	}

	void cycle() {
		cin >> select;
		int accID, balance;
		char* cusName;
		char* cusNum;

		switch (select) {
		case 1:
			cout << "[계좌개설]" << endl;
			accID = inputInt("계좌ID");
			cusName = inputCharArray("이  름");
			cusNum = inputCharArray("주민번호");
			balance = inputInt("입금액");

			bank->addAccount(accID, balance, cusName, cusNum);
			break;
		case 2:
			cout << "[입   금]" << endl;
			accID = inputInt("계좌ID");
			balance = inputInt("입금액");

			bank->bankDeposit(accID, balance);
			break;
		case 3:
			cout << "[출   금]" << endl;
			accID = inputInt("계좌ID");
			balance = inputInt("출금액");

			bank->bankWithdraw(accID, balance);
			break;
		case 4:
			cout << "[계좌정보]" << endl;
			accID = inputInt("계좌ID");

			bank->printAccountData(accID);
			break;
		case 5:
			cout << "프로그램이 종료됩니다." << endl;
			running = false;

			break;
		default:
			cout << "잘못된 값입니다." << endl;
		}

	}

public:

	AccountManager()
			:bank(new Bank(BANK_SIZE)), select(0), running(true) {

	}

	AccountManager(const AccountManager& bankApp)
			:bank(bankApp.bank), select(bankApp.select), running(bankApp.running) {
	}

	~AccountManager() {
		if (this->bank != NULL) delete bank;
	}

	const void run() {
		while (this->running) {
			this->printMenu();
			this->cycle();
		}
	}

};

int main() {
	AccountManager bankApplication;
	bankApplication.run();

	//if (bankApplication != NULL) delete bankApplication;
	return 0;
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