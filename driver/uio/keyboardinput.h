#ifndef KEYBOARDINPUT_H
#define KEYBOARDINPUT_H

#include <termios.h>
#include <stdio.h>

static struct termios old, new;

void initTermios(int);

void resetTermios(void);

char getch_(int);

char getch(void);

#endif
