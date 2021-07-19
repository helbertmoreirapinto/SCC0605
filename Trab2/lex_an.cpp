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
#include <vector>

#define MAX_LINE 100

using namespace std;

bool isNumber(char ch)
{
    return ch >= '0' && ch <= '9';
}

bool isChar(char ch)
{
    return (ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z');
}

bool isSymbol(char ch, map<string, string> symbols, ofstream &fileOut)
{
    string chain = {ch};
    string x = symbols[chain];
    if (!x.empty())
    {
        fileOut << chain << ", " << symbols[chain] << endl;
        return true;
    }
    return false;
}

int autoNumber(char *str, int i, ofstream &fileOut, int linha)
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
            fileOut << number << ", ERRO! (\"Numero real mal formado\")" << endl;
            cout << "Erro léxico na linha " << linha << ": número real mal formado" << endl;
            i++;
            return i;
        }
        while (isNumber(str[i]))
            number.push_back(str[i++]);
        // str[i] n eh mais um number
        fileOut << number << ", real_number\n";
    }
    else
    {
        fileOut << number << ", integer_number\n";
    }
    i--;
    return i;
}

int autoString(char *str, int i, map<string, string> words, ofstream &fileOut)
{
    string chain;
    while (isChar(str[i]) || isNumber(str[i]))
        chain.push_back(str[i++]);
    if (!words[chain].empty())
        fileOut << chain << ", " << words[chain] << endl;
    else
        fileOut << chain << ", ident" << endl;

    i--;
    return i;
}

int autoGreater(char *str, int i, ofstream &fileOut)
{
    if (str[i + 1] == '=')
    {
        fileOut << ">=, simb_maior_igual" << endl;
        i++;
    }
    else
        fileOut << ">, simb_maior" << endl;
    return i;
}

int autoLesser(char *str, int i, ofstream &fileOut)
{
    if (str[i + 1] == '=')
    {
        fileOut << "<=, simb_menor_igual" << endl;
        i++;
    }
    else if (str[i + 1] == '>')
    {
        fileOut << "<>, simb_diff" << endl;
        i++;
    }
    else
        fileOut << "<, simb_menor" << endl;
    return i;
}

int autoColon(char *str, int i, ofstream &fileOut)
{
    if (str[i + 1] == '=')
    {
        fileOut << ":=, simb_atrib" << endl;
        i++;
    }
    else
        fileOut << ":, simb_dp" << endl;
    return i;
}

int autoComment(char *str, int i, ofstream &fileOut, int linha)
{
    string chain;
    while (str[i] != '\n' && str[i] != '}' && str[i] != '\0')
        chain.push_back(str[i++]);

    if (str[i] == '}')
    {
        chain.push_back(str[i]);
        // poderia ser impresso, mas uma função do léxico é ignorar comentários fileOut << chain << ", comment" << endl;
        i++;
    }
    else
    {
        fileOut << chain << " << ERRO!(\"comentario mal formado\")" << endl; //caso o comentário não seja encerrado na mesma linha
        cout << "Erro léxico na linha " << linha << ": comentário mal formado" << endl;
    }
    return i;
}

int main()
{
    map<string, string> words;
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

    map<string, string> symbols;
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

    string fileName;
    cin >> fileName;

    string fileNameIn = fileName;
    string fileNameOut = fileName;

    fileNameIn.append(".txt");
    fileNameOut.append("_out.txt");

    // open files

    ifstream fileIn(fileNameIn);
    ofstream fileOut(fileNameOut);

    if (!fileIn || !fileOut)
    {
        cout << "file not found!\n";
        return 0;
    }

    int linha = 1;
    char str[MAX_LINE];
    while (fileIn.getline(str, MAX_LINE))
    {
        for (int i = 0; i < (int)strlen(str); i++)
        {
            if (str[i] == ' ' || str[i] == '\t' || str[i] == '\n' || str[i] == 13 || !str[i])
                continue;

            if (isNumber(str[i]))
                i = autoNumber(str, i, fileOut, linha);
            else if (isChar(str[i]))
                i = autoString(str, i, words, fileOut);
            else if (str[i] == '>')
                i = autoGreater(str, i, fileOut);
            else if (str[i] == '<')
                i = autoLesser(str, i, fileOut);
            else if (str[i] == '{')
                i = autoComment(str, i, fileOut, linha);
            else if (str[i] == ':')
                i = autoColon(str, i++, fileOut);
            else if (isSymbol(str[i], symbols, fileOut))
                continue;
            else
            {
                fileOut << str[i] << ", ERRO! (\"Caractere não permitido\")" << endl;
                cout << "Erro léxico na linha " << linha << ": caractere não permitido" << endl;
            }
        }
        linha++;
    }

    cout << "Arquivo " << fileNameOut << " gerado com sucesso!" << endl;

    fileIn.close();
    fileOut.close();

    return 0;
}
