#include <stdio.h>
#include<cassert>
#include <fstream>
#include<sstream>
#include<string>
#include<iostream>
#include<vector>
#include"LexicalAnalyzer.h"
#include<random>

char ascii = 0;

//変数の置き換え後の文字　1:a 27:Ab
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

//数字かどうか
bool isNumber(const std::string& str)
{
	for (char const& c : str)
	{
		if (std::isdigit(c) == 0) return false;
	}
	return true;
}

//10進数を2進数に
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

//変数置き換え
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

//行の最後に意味のない処理を追加
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

	//結果
	std::string code;

	//変数格納
	std::vector<std::string> variable;

	//置き換え後の変数格納
	std::vector<std::string> variableTrns;

	//コマンド実行ループ
	while (std::getline(enemyPopCommands, line))
	{
		// 1行分の文字列をストリームに変換して解析しやすくなる
		std::istringstream line_stream(line);

		std::string word;

		std::getline(line_stream, line, '\n');//改行文字まで取得

		line.erase(std::remove(line.begin(), line.end(), '\t'), line.end());//タブ文字削除

		std::vector<std::string> ary = StringTrans(line, variable, variableTrns);//読み取った行を分解

		//空白を削除
		for (size_t i = 0; i < ary.size(); i++)
		{
			if (ary[i] == " " || ary[i].size() == 0)
			{
				ary.erase(ary.begin() + i);
			}
		}

		if (ary.size() == 0)//データが存在しない
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
		else if (ary.front() == "int")//変数
		{
			//型
			std::string type = ary[0];
			type += " ";
			word += type;

			//通常の変数
			if (ary.back() != ")" && ary[2] != "[")
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
		else if (ary.front() == "for")//for文
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
		else if (ary.front() == "{" && ary.size() == 1 || ary.front() == "}" && ary.size() == 1)//中括弧のみ
		{
			word += ary.front();
		}
		else if (ary.front() == "}" && ary.size() == 2)//};の時
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
		else if (ary.front() == "return")//リターン
		{
			ary[0] += " ";
			for (size_t i = 0; i < ary.size(); i++)
			{
				word += ary[i];
			}
		}
		else
		{
			for (size_t i = 0; i < ary.size(); i++)//それ以外
			{
				if (isNumber(ary[i]))//数字かどうか
				{
					ary[i] = StrToNum(ary[i]);
				}

				word += ary[i];
			}
		}


		if (word.size() != 0 && word.back() == ';'&& ary.front() != "return ")//行の最後に意味の無い文追加
		{
			word += rndIf();
		}

		code += word;

	}

	//吐出し
	ofstream outputfile("output/main.cpp");
	outputfile << code;
	outputfile.close();

	return 0;
}