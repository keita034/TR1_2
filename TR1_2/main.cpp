#include <stdio.h>
#include<cassert>
#include <fstream>
#include<sstream>
#include<string>
#include<iostream>
#include<vector>
#include"LexicalAnalyzer.h"
#include<random>



std::vector<std::string> split(std::string str, std::string separator)
{
	if (separator == "") return { str };
	std::vector<std::string> result;
	std::string tstr = str + separator;
	long l = tstr.length(), sl = separator.length();
	std::string::size_type pos = 0, prev = 0;

	for (; pos < l && (pos = tstr.find(separator, pos)) != std::string::npos; prev = (pos += sl))
	{
		result.emplace_back(tstr, prev, pos - prev);
	}
	return result;
}

char ascii = 0;

std::string ASCII()
{
	std::string result;

	ascii++;

	if (ascii / 25 != 0)
	{
		result += 96 + (ascii / 25);
	}

	if (ascii % 25 != 0)
	{
		result += 64 + (ascii % 25);
	}
	else
	{
		result += 65;
	}

	return result;
}

bool isNumber(const std::string& str)
{
	for (char const& c : str)
	{
		if (std::isdigit(c) == 0) return false;
	}
	return true;
}

std::string StrToNum(const std::string& str)
{
	std::string result = "0b";
	int num = std::stoi(str.c_str(), NULL);

	int binary = 0;
	int base = 1;

	if (num == 0)
	{
		return result + "0";
	}


	while (num > 0)
	{
		binary = binary + (num % 2) * base;
		num = num / 2;
		base = base * 10;
	}

	result += std::to_string(binary);

	return result;
}

std::string ReplaceString(std::string String1, std::string String2, std::string String3)
{
	std::string::size_type  Pos(String1.find(String2));

	while (Pos != std::string::npos)
	{
		String1.replace(Pos, String2.length(), String3);
		Pos = String1.find(String2, Pos + String3.length());
	}

	return String1;
}

std::vector<std::string> StringTrans(const std::string& string, const std::vector<std::string>& variable, const std::vector<std::string>& variableTrns)
{
	std::vector<std::string> var;

	var = parse(const_cast<char*>(string.c_str()));

	for (size_t i = 0; i < var.size(); i++)
	{
		for (size_t j = 0; j < variable.size(); j++)
		{
			if (var[i] == variable[j])
			{
				var[i] = variableTrns[j];
			}
		}
	}
	return var;
}


std::string rndIf()
{
	std::random_device seedGen;
	std::mt19937_64 engine(seedGen());
	std::uniform_int_distribution<int> rand(0, 4);

	std::string str[] = {
		"if(0==0){int a = 1;a+=2;}",
		"if(1<=3){int a = 0;a-=2;}",
		"{int i=1;if(i>=3){i++;}else{i--;}}",
		"for(int f=0;f<5;f++){int g=5;g++;if(g>=8){break;}}",
		"{int s=7;while(true){if(s>=8){break;}s+=2;}}" };

	return str[rand(engine)];
};





int main()
{
	std::ifstream file;
	//敵発生コマンド
	std::stringstream enemyPopCommands;

	//ファイルを開く
	file.open("cpp/main.cpp");
	assert(file.is_open());

	//ファイルの内容を文字列ストリームにコピー
	enemyPopCommands << file.rdbuf();

	//ファイルを閉じる
	file.close();

	// 1行分の文字列を入れる変数
	std::string line;

	std::string code;

	std::vector<std::string> variable;

	std::vector<std::string> variableTrns;

	//コマンド実行ループ
	while (std::getline(enemyPopCommands, line))
	{
		// 1行分の文字列をストリームに変換して解析しやすくなる
		std::istringstream line_stream(line);

		std::string word;

		std::getline(line_stream, line, '\n');
		line.erase(std::remove(line.begin(), line.end(), '\t'), line.end());

		std::vector<std::string> ary = StringTrans(line, variable, variableTrns);


		for (size_t i = 0; i < ary.size(); i++)
		{
			if (ary[i] == " " || ary[i].size() == 0)
			{
				ary.erase(ary.begin() + i);
			}
		}

		if (ary.size() == 0)
		{

		}
		else if (ary.front() == "")
		{
			word = line;
		}
		else if (ary.front() == "#include")//インクルード
		{
			for (std::string out : ary)
			{
				word += out;
			}
			word += "\n";
		}
		else if (ary.front() == "int")
		{
			//型
			std::string type = ary[0];
			type += " ";
			word += type;

			if (ary.back() != ")" && ary[2] != "[")//変数
			{
				variable.push_back(ary[1]);
				variableTrns.push_back(ASCII());
				word += variableTrns.back();

				for (size_t i = 2; i < ary.size(); i++)
				{
					if (isNumber(ary[i]))
					{
						ary[i] = StrToNum(ary[i]);
					}

					word += ary[i];
				}

			}
			else if (ary.back() == ")")//関数
			{
				if (ary[1] == "main")//main関数
				{
					for (size_t i = 1; i < ary.size(); i++)
					{
						word += ary[i];
					}

				}
				else//それ以外
				{

				}
			}
			else//配列
			{
				if (ary[2] == "[" && ary[5] == "[")//2次元配列
				{
					//変数
					variable.push_back(ary[1]);
					variableTrns.push_back(ASCII());
					ary[1] = variableTrns.back();

					for (size_t i = 1; i < ary.size(); i++)
					{
						if (isNumber(ary[i]))//数字かどうか
						{
							ary[i] = StrToNum(ary[i]);
						}

						word += ary[i];
					}
				}

			}
		}
		else if (ary.front() == "for")
		{
			if (isType(const_cast<char*>(ary[2].c_str())))
			{
				//変数
				variable.push_back(ary[3]);
				variableTrns.push_back(ASCII());
				ary[3] = variableTrns.back();

				ary = StringTrans(line, variable, variableTrns);

				//型
				std::string type = ary[2];
				type += " ";
				word += type;
			}


			for (size_t i = 0; i < ary.size(); i++)
			{
				if (isNumber(ary[i]))//数字かどうか
				{
					ary[i] = StrToNum(ary[i]);
				}

				word += ary[i];
			}
		}
		else if (ary.front() == "{" && ary.size() == 1 || ary.front() == "}" && ary.size() == 1)
		{
			word += ary.front();
		}
		else if (ary.front() == "}" && ary.size() == 2)
		{
			for (size_t i = 0; i < ary.size(); i++)
			{
				word += ary[i];
			}
		}
		else if (ary[0].front() == '{' && ary[ary.size() - 2] == "}" && ary.back() == ",")//配列の要素
		{
			for (size_t i = 0; i < ary.size(); i++)
			{
				if (isNumber(ary[i]))//数字かどうか
				{
					ary[i] = StrToNum(ary[i]);
				}

				word += ary[i];
			}

		}
		else if (ary.front() == "return")
		{
			ary[0] += " ";
			for (size_t i = 0; i < ary.size(); i++)
			{
				word += ary[i];
			}
		}
		else
		{
			for (size_t i = 0; i < ary.size(); i++)
			{
				if (isNumber(ary[i]))//数字かどうか
				{
					ary[i] = StrToNum(ary[i]);
				}

				word += ary[i];
			}
		}


		if (word.size() != 0 && word.back() == ';'&& ary.front() != "return ")
		{
			word += rndIf();
		}

		code += word;

	}

	ofstream outputfile("output/main.cpp");
	outputfile << code;
	outputfile.close();

	return 0;
}