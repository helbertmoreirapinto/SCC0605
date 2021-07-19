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

#define ERRO 0;
class sin_an
{
public:
    vector<string> tokens;

    sin_an(vector<string> tkns) : tokens(tkns) {}

    bool obter_simbolo(string token)
    {
        cout << token << endl;
        return true;
    }

    int numero(vector<string> tokens, int i)
    {
        obter_simbolo(tokens[i++]);
        return i;
    }

    int dc_c(vector<string> tokens, int i)
    {
        if (tokens[i] == "simb_const")
            obter_simbolo(tokens[i]);
        else
            cout << "Erro sintatico!" << endl;
        i++;
        if (tokens[i] == "simb_ident")
            obter_simbolo(tokens[i]);
        else
            cout << "Erro sintatico!" << endl;
        i++;
        if (tokens[i] == "simb_igual")
            obter_simbolo(tokens[i]);
        else
            cout << "Erro sintatico!" << endl;
        i++;
        i = numero(tokens, i);
        if (tokens[i] == "simb_dp")
            obter_simbolo(tokens[i]);
        else
            cout << "Erro sintatico!" << endl;
        i++;
        return i;
    }

    int dc_v(vector<string> tokens, int i)
    {
        obter_simbolo(tokens[i++]);
        return i;
    }

    int dc_p(vector<string> tokens, int i)
    {
        obter_simbolo(tokens[i++]);
        return i;
    }

    int dc(vector<string> tokens, int i)
    {
        i = dc_c(tokens, i);
        i = dc_v(tokens, i);
        i = dc_p(tokens, i);
        return i;
    }

    int comandos(vector<string> tokens, int i)
    {
        obter_simbolo(tokens[i++]);
        return i;
    }

    int corpo(vector<string> tokens, int i)
    {
        i = dc(tokens, i);
        if (tokens[i] == "simb_begin")
            obter_simbolo(tokens[i]);
        else
            cout << "Erro sintatico!" << endl;
        i++;
        i = comandos(tokens, i);
        if (tokens[i] == "simb_end")
            obter_simbolo(tokens[i]);
        else
            cout << "Erro sintatico!" << endl;
        i++;
        return i;
    }

    bool programa(vector<string> tokens, int i)
    {
        if (tokens[i] == "simb_program")
            obter_simbolo(tokens[i]);
        else
            cout << "Erro sintatico!" << endl;
        i++;
        if (tokens[i] == "simb_ident")
            obter_simbolo(tokens[i]);
        else
            cout << "Erro sintatico!" << endl;
        i++;
        if (tokens[i] == "simb_pv")
            obter_simbolo(tokens[i]);
        else
            cout << "Erro sintatico!" << endl;
        i++;
        i = corpo(tokens, i);
        if (tokens[i] == "simb_dot")
            obter_simbolo(tokens[i]);
        else
            cout << "Erro sintatico!" << endl;
        i++;
        return true;
    }

    void processaTokens()
    {
        programa(tokens, 0);
    }

    void imprimeTokens()
    {
        cout << "Quantidade de tokens: " << tokens.size() << endl;
        for (int i = 0; i < tokens.size(); i++)
        {
            cout << tokens[i] << endl;
        }
    }
};

int main()
{
    vector<string> tokens{"simb_program", "simb_ident", "simb_pv", "dc", "simb_begin", "comandos", "simb_end", "simb_dot"};
    sin_an SinAn(tokens);
    SinAn.imprimeTokens();
    SinAn.processaTokens();
    return 0;
}
