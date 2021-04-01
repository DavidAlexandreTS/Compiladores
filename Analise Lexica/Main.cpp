#include <iostream>
#include "AnaliseLexica.cpp"

void print(Token t, std::map<int , std::string>enumToString)
{
	printf("        [%04d, %04d] (%04d, %20s) {%s}\n", t.line + 1, t.column + 1, t.type , t.getString(t.type, enumToString).c_str() , t.value.c_str());
}

int main(int argc, char *argv[]) 
{
	AnaliseLexica analise;
	
    std::map<int, std::string>mp;
	
    analise.init(std::string("Iziput.txt"));

    while(analise.hasToken())
    {
		Token t = analise.nextToken();
		print(t, mp);
    }
    
	return 0;
}