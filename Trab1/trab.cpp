#include <string.h>

#include <fstream>
#include <iostream>
#include <map>
#include <vector>

#define MAX_LINE 100

using namespace std;

bool isNumber(char ch) {
    return ch >= '0' && ch <= '9';
}

bool isChar(char ch) {
    return (ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z');
}

bool isSymbol(char ch, map<string, string> words) {
    string chain = {ch};
    string x = words[chain];
    if (!x.empty()) {
        cout << chain << ", " << words[chain] << endl;
        return true;
    }
    return false;
}

int autoNumber(char* str, int i) {
    string number;
    while (isNumber(str[i]))
        number.push_back(str[i++]);

    if (str[i] == '.') {
        number.push_back(str[i++]);
        while (isNumber(str[i])) number.push_back(str[i++]);
        cout << number << ", real_number\n";
    } else
        cout << number << ", integer_number\n";

    i--;

    return i;
}

int autoString(char* str, int i, map<string, string> words) {
    string chain;
    while (isChar(str[i]) || isNumber(str[i])) chain.push_back(str[i++]);
    if (!words[chain].empty())
        cout << chain << ", " << words[chain] << endl;
    else
        cout << chain << ", ident" << endl;
    i--;

    return i;
}

int autoGreater(char* str, int i) {
    if (str[i + 1] == '=') {
        cout << ">=, simb_maior_igual" << endl;
        i++;
    } else
        cout << ">, simb_maior" << endl;
    return i;
}

int autoLesser(char* str, int i) {
    if (str[i + 1] == '=') {
        cout << "<=, simb_menor_igual" << endl;
        i++;
    } else if (str[i + 1] == '>') {
        cout << "<>, simb_diff" << endl;
        i++;
    } else
        cout << "<, simb_menor" << endl;
    return i;
}

int autoColon(char* str, int i) {
    if (str[i + 1] == '=') {
        cout << ":=, simb_atrib" << endl;
        i++;
    } else
        cout << ":, simb_dp" << endl;
    return i;
}

int autoComment(char* str, int i) {
    string chain;
    while (str[i] != '\n' && str[i] != '}') chain.push_back(str[i++]);

    if (str[i] == '}') {
        chain.push_back(str[i]);
        // poderia ser impresso, mas uma função do léxico é ignorar comentários cout << chain << ", comment" << endl;
        i++;
    } else
        cout << "ERRO! Comentario mal formado" << endl; //caso o comentário não seja encerrado na mesma linha
    return i;
}

int main() {
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
    words["{"] = "simb_inicio_chave";
    words["}"] = "simb_fim_chave";
    words[":="] = "simb_atrib";
    words["<>"] = "simb_diff";
    words[">="] = "simb_maior_igual";
    words["<="] = "simb_menor_igual";
    words["<"] = "simb_menor";
    words[">"] = "simb_maior";
    words["="] = "simb_igual";
    words[":"] = "simb_dp";
    words[";"] = "simb_pv";
    words["."] = "simb_dot";
    words["+"] = "simb_soma";
    words["-"] = "simb_sub";
    words["/"] = "simb_div";
    words["*"] = "simb_mul";
    words["("] = "simb_abrir_parentese";
    words[")"] = "simb_fechar_parentese";

    map<string, string> identifier;

    string fileName;
    cin >> fileName;

    string fileNameIn = fileName;
    string fileNameOut = fileName;

    fileNameIn.append(".in");
    fileNameOut.append(".out");

    // open files
    FILE* fileIn = fopen(fileNameIn.c_str(), "r");
    FILE* fileOut = fopen(fileNameOut.c_str(), "w");

    if (!fileIn || !fileOut) {
        cout << "file not found!\n";
        return 0;
    }

    char str[MAX_LINE];
    while (fgets(str, MAX_LINE, fileIn) != NULL) {
        for (int i = 0; i < (int)strlen(str); i++) {
            if (str[i] == ' ' || str[i] == '\t' || str[i] == '\n' || str[i] == 13 || !str[i])
                continue;

            if (isNumber(str[i]))
                i = autoNumber(str, i);
            else if (isChar(str[i]))
                i = autoString(str, i, words);
            else if (str[i] == '>')
                i = autoGreater(str, i);
            else if (str[i] == '<')
                i = autoLesser(str, i);
            else if (str[i] == '{')
                i = autoComment(str, i);
            else if (str[i] == ':')
                i = autoColon(str, i++);
            else if (isSymbol(str[i], words))
                i++;
            else
                cout << str[i] << ", ERRO! Caractere não permitido" << endl;
        }
    }

    fclose(fileIn);
    fclose(fileOut);

    return 0;
}
