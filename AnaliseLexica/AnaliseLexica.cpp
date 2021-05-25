#include <map>
#include <iostream>
#include <cstdio>
#include <vector>
#include <deque>
#include <regex>
#include<fstream>
#include "Tokens.cpp"
#include "Token.cpp"

class AnaliseLexica
{

private:
	std::map<std::string, TokenType>tokensMap;
	std::ifstream inptfile;
	std::string auxArq;
	
	int line;
	int column;
	int index;
	void nextChar();
public:
	AnaliseLexica();
	~AnaliseLexica();
	void init(std::string ascArquivo);
	std::vector<Token> analyze();
	Token nextToken();
	bool hasToken();
};


AnaliseLexica::AnaliseLexica()
{
	line = -1;
}

AnaliseLexica::~AnaliseLexica()
{
	
}

void AnaliseLexica::init(std::string ascArquivo)
{
	inptfile.open(ascArquivo);
	index = 0;
	tokensMap["fun"] = TokenType::funDec;
	tokensMap[","] = TokenType::comma;
	tokensMap[";"] = TokenType::semiColon;
	tokensMap["main"] = TokenType::reMain;
	tokensMap["int"] = TokenType::intType;
	tokensMap["float"] = TokenType::floatType;
	tokensMap["bool"] = TokenType::boolType;
	tokensMap["false"] = TokenType::boolConst;
	tokensMap["true"] = TokenType::boolConst;
	tokensMap["string"] = TokenType::stringType;
	tokensMap["stringCons"] = TokenType::stringCons;
	tokensMap["+"] = TokenType::addOp;
	tokensMap["-"] = TokenType::subOp;
	tokensMap["*"] = TokenType::multOp;
	tokensMap["/"] = TokenType::divOp;
	tokensMap["&&"] = TokenType::andOpLog;
	tokensMap["&"] = TokenType::andOpBin;
	tokensMap["||"] = TokenType::orOpLog;
	tokensMap["|"] = TokenType::orOpBin;
	tokensMap["^"] = TokenType::xorOpBin;
	tokensMap["!"] = TokenType::negOp;
	tokensMap["=="] = TokenType::eqRel;
	tokensMap[">="] = TokenType::greEqRel;
	tokensMap["<="] = TokenType::lowEqRel;
	tokensMap["<"] = TokenType::lowRel;
	tokensMap["!="] = TokenType::notEqRel;
	tokensMap["+="] = TokenType::concaten;
	tokensMap["++"] = TokenType::increOp;
	tokensMap["--"] = TokenType::decreOp;
	tokensMap["["] = TokenType::openBrack;
	tokensMap["]"] = TokenType::closeBrack;
	tokensMap["("] = TokenType::openPar;
	tokensMap[")"] = TokenType::closePar;
	tokensMap["{"] = TokenType::openBrace;
	tokensMap["}"] = TokenType::closeBrace;
	tokensMap["\n"] = TokenType::endLine;
	tokensMap["if"] = TokenType::reIf;
	tokensMap["else"] = TokenType::reElse;
	tokensMap["for"] = TokenType::reFor;
	tokensMap["while"] = TokenType::reWhile;
	tokensMap["?"] = TokenType::ternOp1;
	tokensMap[":"] = TokenType::ternOp2;
	tokensMap["elseif"] = TokenType::reElseIf;
	tokensMap[">"] = TokenType::greRel;
	tokensMap["void"] = TokenType::reVoid;
	tokensMap["="] = TokenType::atrib;
	tokensMap["return"] = TokenType::reReturn;
}

bool AnaliseLexica::hasToken()
{
	if(index == (int)auxArq.size() || auxArq.empty())
	{
		++line;
		int pos = 0;
		column = 0;
		index = 0;

		if(getline(inptfile, auxArq))
		{
			while(auxArq[pos] == ' ' || auxArq[pos] == '\t')
			{
				++pos;
			}

			if(pos < (int)auxArq.size())
				printf("%04d  ", line + 1);

			while(pos < (int)auxArq.size())
			{
				std::cout << auxArq[pos];
				++pos;
			}
			std:: cout << "\n";
			
			if(!auxArq.size())
			{
				return hasToken();
			}

			return true;
		}
		else 
			return false;
	}
	
	return true;
}

Token AnaliseLexica::nextToken() 
{
	char c = auxArq[index];
	int col = column;
	std::string value = "";
	if(c == '\"')
	{
		nextChar();
		while(index != (int)auxArq.size())
		{
			c = auxArq[index];
			if(c == '\"')
			{
				Token retToken(TokenType::stringCons, line, col, value);
				nextChar();
				return retToken;
			}
			else
			{
				value += auxArq[index];
				nextChar();
			}
		}
		return Token(TokenType::unknown, line, col,"\"" + value);
	}else if(c >= '0' && c <= '9')
	{
		value += c;
		nextChar();
		bool point  = false;

		while(index != (int)auxArq.size())
		{
			c = auxArq[index];
			
			if(c >= '0' && c <= '9')
			{
				value += c;
				nextChar();
			}else if(c == '.' && !point)
			{
				point = true;
				value += c;
				nextChar();
			}else break;
		}

		if(point)
			return Token(TokenType::floatConst, line, col, value);
		else 
			return Token(TokenType::intConst, line, col, value);

	}else if((c >= 'a' && c <= 'z') || (c >='A' && c <= 'Z'))
	{
		value += c;
		nextChar();

		while(index != (int)auxArq.size())
		{
			c = auxArq[index];
			if((c >= 'a' && c <= 'z') || (c >='A' && c <= 'Z')|| (c >= '0' && c <= '9') || (c == '_'))
			{
				value += c;
				nextChar();
			}else break; 
		}

		if(tokensMap.count(value))
			return Token(tokensMap[value], line, col, value);

		return Token(TokenType::id, line, col, value);
	}
	else if(c == ' ' || c == '\t')
	{
		nextChar();
		if(hasToken())
			return nextToken();
		else 
			return Token(TokenType::unknown, line, col, value);
	}
	else
	{
		value += c;
		nextChar();

		if(tokensMap.count(value))
		{
			std::string aux = value;
			if(index != (int)auxArq.size())
			{
				c = auxArq[index];
				aux += c;
				if(tokensMap.count(aux))
				{
					nextChar();
					return Token(tokensMap[aux], line, col, aux);
				}
			}
			return Token(tokensMap[value], line, col, value);
		}
		else 
		{
			if(value == " ")
				return nextToken();

			return Token(TokenType::unknown, line, col, value);
		}
	}
}

void AnaliseLexica::nextChar()
{
	++index;
	++column;
}