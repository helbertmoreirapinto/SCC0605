/*
SCC0605 - Teoria da Computação e Compiladores
Antônio Sebastian - 10797781
Bruno Lanzoni Rossi - 4309596
Gabriell Tavares Luna - 10716400
Helbert Moreira Pinto - 10716504
*/

#include <string.h>

#include <fstream>
#include <iostream>
#include <map>
#include <bits/stdc++.h>

#define MAX_LINE 100
using namespace std;

typedef struct
{
    string simb;
    int linha;
} Token;

class lex_an
{
public:
    map<string, string> words, symbols;
    queue<Token> tokens;
    int linha;

    lex_an()
    {
        words["program"] = "simb_program";
        words["begin"] = "simb_begin";
        words["end"] = "simb_end";
        words["var"] = "simb_var";
        words["procedure"] = "simb_procedure";
        words["const"] = "simb_const";
        words["integer"] = "simb_tipo_int";
        words["real"] = "simb_tipo_real";
        words["if"] = "simb_if";
        words["else"] = "simb_else";
        words["then"] = "simb_then";
        words["read"] = "simb_read";
        words["write"] = "write";
        words["while"] = "simb_while";
        words["do"] = "simb_do";
        words["for"] = "simb_for";
        words["to"] = "simb_to";

        symbols[":="] = "simb_atrib";
        symbols["<>"] = "simb_diff";
        symbols[">="] = "simb_maior_igual";
        symbols["<="] = "simb_menor_igual";
        symbols["<"] = "simb_menor";
        symbols[">"] = "simb_maior";
        symbols["="] = "simb_igual";
        symbols[","] = "simb_virgula";
        symbols[":"] = "simb_dp";
        symbols[";"] = "simb_pv";
        symbols["."] = "simb_dot";
        symbols["+"] = "simb_soma";
        symbols["-"] = "simb_sub";
        symbols["/"] = "simb_div";
        symbols["*"] = "simb_mul";
        symbols["("] = "simb_abrir_parentese";
        symbols[")"] = "simb_fechar_parentese";

        linha = 1;
    }

    bool isNumber(char ch)
    {
        return ch >= '0' && ch <= '9';
    }

    bool isChar(char ch)
    {
        return (ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z') || ch == '_';
    }

    bool isSymbol(char ch)
    {
        string chain = {ch};
        string x = symbols[chain];
        if (!x.empty())
        {
            Token newToken;
            newToken.simb = symbols[chain];
            newToken.linha = linha;
            tokens.push(newToken);
            return true;
        }
        return false;
    }

    int autoNumber(char *str, int i)
    {
        string number;
        while (isNumber(str[i]))
            number.push_back(str[i++]);

        // str[i] n eh mais um number

        if (str[i] == '.')
        {
            number.push_back(str[i++]);
            if (!isNumber(str[i]))
            {
                number.push_back(str[i]);
                cout << "Erro léxico na linha " << linha << ": número real mal formado" << endl;
                return i;
            }
            while (isNumber(str[i]))
                number.push_back(str[i++]);
            // str[i] n eh mais um number
            Token newToken;
            newToken.simb = words["real"];
            newToken.linha = linha;
            tokens.push(newToken);
        }
        else
        {
            if (!isChar(str[i]))
            {
                Token newToken;
                newToken.simb = words["integer"];
                newToken.linha = linha;
                tokens.push(newToken);
            }
            else
            {
                while (isChar(str[i]) || isNumber(str[i]))
                    i++;
                cout << "Erro léxico na linha " << linha << ": ident mal formado" << endl;
            }
        }
        i--;
        return i;
    }

    int autoString(char *str, int i)
    {
        string chain;
        while (isChar(str[i]) || isNumber(str[i]))
            chain.push_back(str[i++]);
        Token newToken;
        newToken.linha = linha;
        if (!words[chain].empty())
            newToken.simb = words[chain];
        else
            newToken.simb = "simb_ident";
        tokens.push(newToken);
        i--;
        return i;
    }

    int autoGreater(char *str, int i)
    {
        Token newToken;
        newToken.linha = linha;
        if (str[i + 1] == '=')
        {
            newToken.simb = symbols[">="];
            i++;
        }
        else
            newToken.simb = symbols[">"];
        tokens.push(newToken);
        return i;
    }

    int autoLesser(char *str, int i)
    {
        Token newToken;
        newToken.linha = linha;
        if (str[i + 1] == '=')
        {
            newToken.simb = symbols["<="];
            i++;
        }
        else if (str[i + 1] == '>')
        {
            newToken.simb = symbols["<>"];
            i++;
        }
        else
            newToken.simb = symbols["<"];
        tokens.push(newToken);
        return i;
    }

    int autoColon(char *str, int i)
    {
        Token newToken;
        newToken.linha = linha;
        if (str[i + 1] == '=')
        {
            newToken.simb = symbols[":="];
            i++;
        }
        else
            newToken.simb = symbols[":"];
        tokens.push(newToken);
        return i;
    }

    int autoComment(char *str, int i)
    {
        string chain;
        while (str[i] != '\n' && str[i] != '}' && str[i] != '\0')
            chain.push_back(str[i++]);

        if (str[i] == '}')
            chain.push_back(str[i++]);
        else
            cout << "Erro léxico na linha " << linha << ": comentário mal formado" << endl;
        return i;
    }

    void processaLinha(char *str)
    {
        for (int i = 0; i < (int)strlen(str); i++)
        {
            if (str[i] == ' ' || str[i] == '\t' || str[i] == '\n' || str[i] == 13 || !str[i])
                continue;

            if (isNumber(str[i]))
                i = autoNumber(str, i);
            else if (isChar(str[i]))
                i = autoString(str, i);
            else if (str[i] == '>')
                i = autoGreater(str, i);
            else if (str[i] == '<')
                i = autoLesser(str, i);
            else if (str[i] == '{')
                i = autoComment(str, i);
            else if (str[i] == ':')
                i = autoColon(str, i);
            else if (isSymbol(str[i]))
                continue;
            else
                cout << "Erro léxico na linha " << linha << ": caractere não permitido" << endl;
        }
    }

    void imprimeTokens()
    {
        cout << "Quantidade de tokens: " << tokens.size() << endl;
        Token token;
        for (int i = 0; i < tokens.size(); i++, tokens.pop())
        {
            token = tokens.front();
            cout << "Linha: " << token.linha << " Token: " << token.simb << endl;
        }
    }
};

int main()
{

    string fileName;
    cin >> fileName;

    string fileNameIn = fileName;
    //string fileNameOut = fileName;

    fileNameIn.append(".txt");
    //fileNameOut.append("_out.txt");

    // open files

    ifstream fileIn(fileNameIn);
    //ofstream fileOut(fileNameOut);

    if (!fileIn) // || !fileOut)
    {
        cout << "file not found!\n";
    }

    lex_an LexAn;

    char str[MAX_LINE];
    while (fileIn.getline(str, MAX_LINE))
    {
        LexAn.processaLinha(str);
        LexAn.linha++;
    }

    LexAn.imprimeTokens();

    //cout << "Arquivo " << fileNameOut << " gerado com sucesso!" << endl;

    fileIn.close();
    //fileOut.close();

    return 0;
}
