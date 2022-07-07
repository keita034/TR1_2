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

//�ϐ��̒u��������̕����@1:a 27:Ab
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

//�������ǂ���
bool isNumber(const std::string& str)
{
	for (char const& c : str)
	{
		if (std::isdigit(c) == 0) return false;
	}
	return true;
}

//10�i����2�i����
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

//�ϐ��u������
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

//�s�̍Ō�ɈӖ��̂Ȃ�������ǉ�
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
	//�G�����R�}���h
	std::stringstream enemyPopCommands;

	//�t�@�C�����J��
	file.open("cpp/main.cpp");
	assert(file.is_open());

	//�t�@�C���̓��e�𕶎���X�g���[���ɃR�s�[
	enemyPopCommands << file.rdbuf();

	//�t�@�C�������
	file.close();

	// 1�s���̕����������ϐ�
	std::string line;

	//����
	std::string code;

	//�ϐ��i�[
	std::vector<std::string> variable;

	//�u��������̕ϐ��i�[
	std::vector<std::string> variableTrns;

	//�R�}���h���s���[�v
	while (std::getline(enemyPopCommands, line))
	{
		// 1�s���̕�������X�g���[���ɕϊ����ĉ�͂��₷���Ȃ�
		std::istringstream line_stream(line);

		std::string word;

		std::getline(line_stream, line, '\n');//���s�����܂Ŏ擾

		line.erase(std::remove(line.begin(), line.end(), '\t'), line.end());//�^�u�����폜

		std::vector<std::string> ary = StringTrans(line, variable, variableTrns);//�ǂݎ�����s�𕪉�

		//�󔒂��폜
		for (size_t i = 0; i < ary.size(); i++)
		{
			if (ary[i] == " " || ary[i].size() == 0)
			{
				ary.erase(ary.begin() + i);
			}
		}

		if (ary.size() == 0)//�f�[�^�����݂��Ȃ�
		{

		}
		else if (ary.front() == "")
		{
			word = line;
		}
		else if (ary.front() == "#include")//�C���N���[�h
		{
			for (std::string out : ary)
			{
				word += out;
			}
			word += "\n";
		}
		else if (ary.front() == "int")//�ϐ�
		{
			//�^
			std::string type = ary[0];
			type += " ";
			word += type;

			//�ʏ�̕ϐ�
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
			else if (ary.back() == ")")//�֐�
			{
				if (ary[1] == "main")//main�֐�
				{
					for (size_t i = 1; i < ary.size(); i++)
					{
						word += ary[i];
					}

				}
			}
			else//�z��
			{
				if (ary[2] == "[" && ary[5] == "[")//2�����z��
				{
					//�ϐ�
					variable.push_back(ary[1]);
					variableTrns.push_back(ASCII());
					ary[1] = variableTrns.back();

					for (size_t i = 1; i < ary.size(); i++)
					{
						if (isNumber(ary[i]))//�������ǂ���
						{
							ary[i] = StrToNum(ary[i]);
						}

						word += ary[i];
					}
				}

			}
		}
		else if (ary.front() == "for")//for��
		{
			if (isType(const_cast<char*>(ary[2].c_str())))
			{
				//�ϐ�
				variable.push_back(ary[3]);
				variableTrns.push_back(ASCII());
				ary[3] = variableTrns.back();

				ary = StringTrans(line, variable, variableTrns);

				//�^
				std::string type = ary[2];
				type += " ";
				word += type;
			}


			for (size_t i = 0; i < ary.size(); i++)
			{
				if (isNumber(ary[i]))//�������ǂ���
				{
					ary[i] = StrToNum(ary[i]);
				}

				word += ary[i];
			}
		}
		else if (ary.front() == "{" && ary.size() == 1 || ary.front() == "}" && ary.size() == 1)//�����ʂ̂�
		{
			word += ary.front();
		}
		else if (ary.front() == "}" && ary.size() == 2)//};�̎�
		{
			for (size_t i = 0; i < ary.size(); i++)
			{
				word += ary[i];
			}
		}
		else if (ary[0].front() == '{' && ary[ary.size() - 2] == "}" && ary.back() == ",")//�z��̗v�f
		{
			for (size_t i = 0; i < ary.size(); i++)
			{
				if (isNumber(ary[i]))//�������ǂ���
				{
					ary[i] = StrToNum(ary[i]);
				}

				word += ary[i];
			}

		}
		else if (ary.front() == "return")//���^�[��
		{
			ary[0] += " ";
			for (size_t i = 0; i < ary.size(); i++)
			{
				word += ary[i];
			}
		}
		else
		{
			for (size_t i = 0; i < ary.size(); i++)//����ȊO
			{
				if (isNumber(ary[i]))//�������ǂ���
				{
					ary[i] = StrToNum(ary[i]);
				}

				word += ary[i];
			}
		}


		if (word.size() != 0 && word.back() == ';'&& ary.front() != "return ")//�s�̍Ō�ɈӖ��̖������ǉ�
		{
			word += rndIf();
		}

		code += word;

	}

	//�f�o��
	ofstream outputfile("output/main.cpp");
	outputfile << code;
	outputfile.close();

	return 0;
}