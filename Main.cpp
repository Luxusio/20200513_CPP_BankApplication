#include <iostream>

using namespace std;

#define NAME_LEN 20
#define BANK_SIZE 100


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