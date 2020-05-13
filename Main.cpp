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