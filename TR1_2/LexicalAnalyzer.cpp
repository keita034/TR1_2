#include "LexicalAnalyzer.h"

bool isPunctuator(char ch)
{
	if (ch == ' ' || ch == '+' || ch == '-' || ch == '*' ||
		ch == '/' || ch == ',' || ch == ';' || ch == '>' ||
		ch == '<' || ch == '=' || ch == '(' || ch == ')' ||
		ch == '[' || ch == ']' || ch == '{' || ch == '}' ||
		ch == '&' || ch == '|')
	{
		return true;
	}
	return false;
}

bool validIdentifier(char* str)
{
	if (str[0] == '0' || str[0] == '1' || str[0] == '2' ||
		str[0] == '3' || str[0] == '4' || str[0] == '5' ||
		str[0] == '6' || str[0] == '7' || str[0] == '8' ||
		str[0] == '9' || isPunctuator(str[0]) == true)
	{
		return false;
	}
	int i, len = strlen(str);
	if (len == 1)
	{
		return true;
	}
	else
	{
		for (i = 1; i < len; i++) 
		{
			if (isPunctuator(str[i]) == true)
			{
				return false;
			}
		}
	}
	return true;
}

bool isOperator2(char ch)
{
	if (ch == ' ' || ch == ',' || ch == ';' ||
		ch == '(' || ch == ')' || ch == '[' ||
		ch == ']' || ch == '{' || ch == '}')
	{
		return true;
	}
	return false;
}

bool isOperator(char ch)
{
	if (ch == '+' || ch == '-' || ch == '*' ||
		ch == '/' || ch == '>' || ch == '<' ||
		ch == '=' || ch == '|' || ch == '&')
	{
		return true;
	}
	return false;
}

bool isKeyword(char* str)
{
	if (!strcmp(str, "if") || !strcmp(str, "else") ||
		!strcmp(str, "while") || !strcmp(str, "do") ||
		!strcmp(str, "break") || !strcmp(str, "continue")
		||  !strcmp(str, "return") ||  !strcmp(str, "case")
		||  !strcmp(str, "typedef") || !strcmp(str, "switch")
		||  !strcmp(str, "static") || !strcmp(str, "struct")
		|| !strcmp(str, "sizeof") || !strcmp(str, "volatile")
		|| !strcmp(str, "typedef")|| !strcmp(str, "enum")
		|| !strcmp(str, "const")|| !strcmp(str, "union")
		|| !strcmp(str, "extern")|| !strcmp(str, "main")
		|| !strcmp(str, "unsigned"))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool isType(char* str)
{
	if (!strcmp(str, "int") || !strcmp(str, "double")
		|| !strcmp(str, "float") || !strcmp(str, "long")
		|| !strcmp(str, "short") || !strcmp(str, "long")
		|| !strcmp(str, "void") ||!strcmp(str, "bool")
		|| !strcmp(str, "char") )
	{
		return true;
	}
	else
	{
		return false;
	}
}


bool isNumber(char* str)
{
	int i, len = strlen(str), numOfDecimal = 0;
	if (len == 0)
	{
		return false;
	}
	for (i = 0; i < len; i++)
	{
		if (numOfDecimal > 1 && str[i] == '.')
		{
			return false;
		}
		else if (numOfDecimal <= 1)
		{
			numOfDecimal++;
		}
		if (str[i] != '0' && str[i] != '1' && str[i] != '2'
			&& str[i] != '3' && str[i] != '4' && str[i] != '5'
			&& str[i] != '6' && str[i] != '7' && str[i] != '8'
			&& str[i] != '9' || (str[i] == '-' && i > 0))
		{
			return false;
		}
	}
	return true;
}

char* subString(char* realStr, int l, int r)
{
	int i;

	char* str = (char*)malloc(sizeof(char) * (r - l + 2));

	for (i = l; i <= r; i++)
	{
		str[i - l] = realStr[i];
		str[r - l + 1] = '\0';
	}
	return str;
}

vector<string> parse(char* str)
{
	int left = 0, right = 0;
	int len = strlen(str);
	vector<string> strin;
	string buff;

	while (right <= len && left <= right)
	{
		if (isPunctuator(str[right]) == false)
		{
			right++;
		}

		if (isPunctuator(str[right]) == true && left == right)
		{
			if (isOperator(str[right]) == true)
			{
				buff = str[right];
				strin.push_back(buff);
			}
			else if (isOperator2(str[right]) == true)
			{
				buff = str[right];
				strin.push_back(buff);
			}

			right++;
			left = right;
		}
		else if (isPunctuator(str[right]) == true && left != right
			|| (right == len && left != right))
		{
			char* sub = subString(str, left, right - 1);

			if (isKeyword(sub) == true)
			{
				buff = sub;
				strin.push_back(buff);
			}
			else if (isType(sub) == true)
			{
				buff = sub;
				strin.push_back(buff);
			}
			else if (isNumber(sub) == true)
			{
				buff = sub;
				strin.push_back(buff);
			}
			else if (validIdentifier(sub) == true
				&& isPunctuator(str[right - 1]) == false)
			{
				buff = sub;
				strin.push_back(buff);
			}
			else if (validIdentifier(sub) == false
				&& isPunctuator(str[right - 1]) == false)
			{
				buff = sub;
				strin.push_back(buff);
			}

			left = right;
		}
	}
	return strin;
}
