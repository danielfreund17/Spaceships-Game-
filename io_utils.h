#ifndef _IO_UTILS_H_
#define _IO_UTILS_H_
#include "config.h"
#include "Color.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

void gotoxy(int x, int y);
void setTextColor(Color);
void hideCursor();
void clear_screen();
void MakeBeep(int dec, int langth);
static int getIndex(const char* s, int lookFor);
void setTextColor(Color colorToSet);

	#ifndef WINDOWS	
		int _getch(void);
		int _kbhit(void);
		void Sleep(unsigned long);
		void _itoa_s(unsigned int, char s[], unsigned int, int);
	#else
		#include <stdlib.h>
		#include <conio.h>
		#include <windows.h>
	#endif

#endif