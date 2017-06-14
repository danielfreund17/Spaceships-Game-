#ifndef SCREEN_H
#define SCREEN_H

#include <iostream>
#include "TheShipsGame.h"
#include <string>
#include <list>
enum{ROWS=24,COLS=80};
class Screens{
	const string fileName;
	const string screen_id;
	int *ID = nullptr;
	string math;
	string mathTemp="";
	int par1=-2, par2=-2, answer=-2,num_of_par=1;
	int par1Pos = -1, par2Pos = -1, answerPos=-1;
	char ch='0',ch2='0';
	char** board=nullptr;
	list<string> errors;
	int countPar = 0;
	bool played = false, finished = false, checked = false, printedErrors=false;
public:
	bool isValid = true;

public:
	//~Screens(){	delete board; if (ID != nullptr) delete ID; } //destractor
	Screens(const Screens &s) = delete;//delete copy ctor
	Screens(string fname, string screenID, string mathEX, char ** &b);
	Screens(){};
	void FreeID();
	void FreeBoard();
	int compareFileNames(const string &fname){ return fileName.compare(fname); }
	bool checkScreen();
	void copyMathEquition() { mathTemp.insert(0,math); }
	void eraseMathEquition() { math.clear(), math.insert(0,mathTemp); }
	void convertParameters();
	bool numbers(int &index);
	int calculate(int num1, int num2, char ch);
	bool checkScreenID();
	bool checkMathExp();
	bool calc();
	bool getcountPar() { return countPar == 0; }
	int insertNum(int num);
	void pushError(string er);
	void printErrors();

	int getPar1()const { return par1; }
	int getPar2()const { return par2; }
	int getAnswer()const { return answer; }
	int getNumOfPar()const { return num_of_par; }
	int getPar1Pos()const { return par1Pos; }
	int getPar2Pos()const { return par2Pos; }
	int getAnswerPos()const { return answerPos; }
	int getcountPar1()const { return countPar; };


	void addParameters() { countPar++; };
	void printMath();
	char** getBoard(){ return board; };
	int getID() { if (ID != nullptr)return *ID; else return 0; }
	bool checkID(){ if (ID != nullptr) return true; else return false; }
	void Finished(){ finished = true; }
	void Played(){ played = true; }
	void Checked(){ checked = true; }
	void PrintedErrors(){ printedErrors = true; }
	bool isPrintedErrors(){ return printedErrors; }
	bool isFinished(){ return finished; }
	bool isPlayed(){ return played; }
	bool isChecked(){ return checked; }
	bool errorExist( string str);
	string getFileName(){ return fileName; }
	string getScreenId() { return screen_id; }
	string getMath()const { return math; }
	void pushNumber(string num);
	void initParameters();
	void operator=(const Screens& screen);
};
#endif