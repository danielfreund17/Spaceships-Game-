#include "Screens.h"
#include <iostream>
using namespace std;

Screens::Screens(string fname, string screenID, string mathEX, char ** &b):fileName(fname), screen_id(screenID), math(mathEX) 
{
	unsigned int i, j;
	board = new char*[ROWS];
	for (i = 0; i < ROWS; i++)
	{
		board[i] = b[i];
	}
}

void Screens::operator=(const Screens& screen)
{
	if (this != &screen)
	{
		math = screen.getMath();
		par1 = screen.getPar1();
		par2 = screen.getPar2();
		answer = screen.getAnswer();
		num_of_par = screen.getNumOfPar();
		par1Pos = screen.getPar1Pos();
		par2Pos = screen.getPar2Pos();
		answerPos = screen.getAnswerPos();
		countPar = screen.getcountPar1();
	}

}
void Screens::FreeBoard()
{
	if (board != nullptr)
	{
		for (int i = 0; i < ROWS; i++)
		{
			delete[]board[i];
		}
		delete[] board;
	}
}

void Screens::FreeID()
{
	if (ID != nullptr)
		delete ID;
}

void Screens::pushError(string er)
{
	errors.push_back(er);
}
void Screens::printMath()
{
	bool flag = true;
	for (unsigned int i = 9; i < math.size(); ++i)
	{
		cout << math[i];
	}
}
bool Screens::checkMathExp()
{
	string sCheck("Exercise=");
	string notValid = ("Problem in the math expression");
	char *str;
	int mathSize = math.size();
	if (math.compare(0, 9, sCheck) != 0 || mathSize <= 9)
	{
		if (!errorExist(notValid))
			errors.push_back(notValid);
		isValid = false;
		return false;
	}
	for (unsigned int i = 9; i < mathSize; i++)
		if ((math[i] == '_' || math[i] == '+' || math[i] == '-' || math[i] == '*' || math[i] == '/' || math[i] == '=') || (math[i] <= '9' && math[i] >= '0'))
		{
		}
		else{
			if (!errorExist(notValid))
				errors.push_back(notValid);
			isValid = false;
			return false;
		}
	return true;
}
void Screens::initParameters()
{
	par1 = -2;
	par2 = -2;
	answer = -2;
	num_of_par = 1;
	par1Pos = -1;
	par2Pos = -1;
	answerPos = -1;
	countPar = 0;
}
void Screens::convertParameters()
{
	initParameters();
	for (int i = 9; i < math.size(); i++)
	{
		if (numbers(i)){}
		else
		{
			if (ch == '0' && math[i]!='=')
				ch = math[i];
			else if (math[i] != '=')
				ch2 = math[i];
			else if (math[i] == '=')
			{
				num_of_par = 3;
				numbers(i);
			}
		}
	}
}
bool Screens::numbers(int& index)
{
	int num = 1;
	bool flag = false;

	switch (num_of_par)
	{
	case 1:
		if (math[index] == '_'){
			countPar++;
			par1++;
			par1Pos = index;
			num_of_par++;
			return true;
		}
		else if (math[index]<'0' || math[index]>'9') 
			return false;
		par1 = 0;
		while (math[index] >= '0' && math[index] <= '9')
		{
			flag = true;
			par1 *= num;
			par1 += math[index] - '0';
			num *= 10;
			index++;
		}
		index--;
		num_of_par++;
		return flag;
		break;
	case 2:
		if (math[index] == '_'){
			countPar++;
			par2++;
			par2Pos = index;
			num_of_par++;
			return true;
		}
		else if (math[index]<'0' || math[index]>'9')
			return false;
		par2 = 0;
		while (math[index] >= '0' && math[index] <= '9')
		{
			flag = true;
			par2 *= num;
			par2 += math[index] - '0';
			num *= 10;
			index++;
		}
		index--;
		num_of_par++;
		return flag;
		break;
	case 3:
		index++;
		if (math[index] == '_') {
			countPar++;
			answerPos = index;
			answer++;
			return true;
		}
		else if (math[index]<'0' || math[index]>'9')
			return false;
		answer = 0;
		while (math[index] >= '0' && math[index] <= '9')
		{
			flag = true;
			answer *= num;
			answer += math[index] - '0';
			num *= 10;
			index++;
		}
		index--;
		return flag;
		break;
	}
}

void Screens:: pushNumber(string num)
{
	if (par1Pos != -1)
	{
		math.erase(par1Pos, 1);
		math.insert(par1Pos, num);
		par1Pos = -1;
	}
	else if (par2Pos != -1)
	{
		math.erase(par2Pos, 1);
		math.insert(par2Pos, num);		
		par2Pos = -1;
	}
	else if (answerPos != -1)
	{
		math.erase(answerPos, 1);
		math.insert(answerPos, num);
		answerPos = -1;
	}
}
int Screens::insertNum(int num)
{
	int temp;
	bool res;
	if (par1 == -1)
	{
		par1 = num;
		if (countPar == 1)
		{
			if (calc())
				return 1;
			else
			{
				par1 = -1;
				return 0;
			}
		}
		else
		{
			countPar--;
			return 2;
		}
	}
	else if (par2 == -1) {
		par2 = num;
		if (countPar == 1)
		{
			if (calc())
				return 1;
			else{
				par2 = -1;
				return 0;
			}
		}
		else
		{
			countPar--;
			return 2;
		}
	}
	else if (answer == -1)
	{
		answer = num;
		if (countPar == 1)
		{
			if (calc())
				return 1;
			else
			{
				answer = -1;
				return 0;
			}
		}
		else
		{
			countPar--;
			return 2;
		}
	}
}

bool Screens::calc()
{
	int num1 = calculate(par1, par2, ch);
	return num1 == answer;
}

int Screens::calculate(int num1, int num2, char ch)
{
	switch (ch) {
	case '+':
		return num1 + (num2);
		break;
	case '-':
		return num1 - (num2);
		break;
	case '*':
		return (num1)*(num2);
		break;
	case '/':
		return num1 / num2;
		break;
	}
	return num1;

}
void Screens::printErrors()
{
	int numofErrors = errors.size();
	string error;
	for (unsigned int i = 0; i < numofErrors; ++i)
	{
		error = errors.back();
		cout << error << endl;
		errors.pop_back();
	}
}

bool Screens::checkScreenID()
{
	char *str;
	int strSize = screen_id.size();
	unsigned int i;
	string notValid("Problem in the Screen ID");
	string sCheck("ScreenID=");
	if (screen_id.compare(0, 9, sCheck) != 0 || strSize <= 9)//bad screen id.
	{
		if (!errorExist(notValid))
			errors.push_back(notValid);
		isValid = false;
		return false;
	}
	else
	{
		for (unsigned int j = 9; j < strSize; j++)//going to first digit
		{
			if (screen_id[j] <'0' || screen_id[j] >'9')
			{
				if (!errorExist(notValid))
					errors.push_back(notValid);
				isValid = false;
				return false;
			}
		}
		//number id is valid.
		str = new char[strSize + 1];
		for (i = 0; i < strSize; i++)
			str[i] = screen_id[i];
		str[i] = '\0';
		ID = new int;
		*ID = atoi(str + 9);//getting id converting to int
		delete str;
		return true;
	}
}

bool Screens::errorExist(string str)
{
	list<string>::iterator itr;
	for (itr = errors.begin(); itr != errors.end(); itr++)
	{
		if (str.compare(*itr) == 0)
			return true;
	}
	return false;
}
bool Screens::checkScreen()
{
	bool S = false, X = false;
	char ch;
	bool IDcheck, MathCheck;
	string notvalid1 = "There's no place in this screen for the information window";
	string notvalid2 = "There's no place in this screen for the math expression";
//	if (!(this->checkScreenID()) || (!this->checkMathExp()))//screen id is illegal
	IDcheck = checkScreenID();
	MathCheck = checkMathExp();
	if (!IDcheck || !MathCheck)
		return false;
	else
	{
		for (unsigned int i = 0; i < ROWS; i++)
			for (unsigned int j = 0; j < COLS; j++)
			{
				ch = board[i][j];
				if (ch == 'S')
					S = true;
				if (ch == 'X')
					X = true;
			}
	}
	if (!S)
	{
		if (!errorExist(notvalid1))
			pushError("There's no place in this screen for the information window");
		this->isValid = false;
		return false;
	}
	if (!X)
	{
		if (!errorExist(notvalid1))
			pushError(notvalid2);
		this->isValid = false;
		return false;
	}
	return true;
}