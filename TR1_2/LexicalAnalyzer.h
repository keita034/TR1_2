#pragma once
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <vector>
#include <string>

using namespace std;

bool isPunctuator(char ch);

bool validIdentifier(char* str);

bool isOpe(char ch);

bool isOperator(char ch);

bool isKeyword(char* str);

bool isNumber(char* str);

bool isType(char* str);

char* subString(char* realStr, int l, int r);

vector<string> parse(char* str);