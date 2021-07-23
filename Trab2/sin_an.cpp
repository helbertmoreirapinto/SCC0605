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

class sin_an
{
public:
    vector<string> tokens;

    sin_an(vector<string> tkns) : tokens(tkns) {}

    int obter_simbolo(string token, int i)
    {
        cout << token << endl;
        i++;
        return i;
    }

    int numero(vector<string> tokens, int i)
    {
        if (tokens[i] == "simb_tipo_int" || tokens[i] == "simb_tipo_real")
            return obter_simbolo(tokens[i++], i);
        cout << "Erro sintatico!" << endl;
        return i;
    }

    int tipo_var(vector<string> tokens, int i)
    {
        if (tokens[i] == "simb_tipo_int" || tokens[i] == "simb_tipo_real")
            return obter_simbolo(tokens[i++], i);
        cout << "Erro sintatico!" << endl;
        return i;
    }

    int variaveis(vector<string> tokens, int i)
    {
        if (tokens[i] == "simb_ident")
            return obter_simbolo(tokens[i++], i);
        else
            cout << "Erro sintatico!" << endl;
        i = mais_var(tokens, i);
        return i;
    }

    int mais_var(vector<string> tokens, int i)
    {
        if (tokens[i] == "simb_virgula")
        {
            i = obter_simbolo(tokens[i++], i);
            i = variaveis(tokens, i);
        }
        return i;
    }

    int lista_par(vector<string> tokens, int i)
    {
        i = variaveis(tokens, i);
        if (tokens[i] == "simb_dp")
            return obter_simbolo(tokens[i++], i);
        else
            cout << "Erro sintatico!" << endl;
        i = tipo_var(tokens, i);
        i = mais_var(tokens, i);
        return i;
    }

    int dc_c(vector<string> tokens, int i)
    {
        if (tokens[i] == "simb_const")
        {
            i = obter_simbolo(tokens[i], i);
            if (tokens[i] == "simb_ident")
                i = obter_simbolo(tokens[i], i);
            else
                cout << "Erro sintatico!" << endl;
            if (tokens[i] == "simb_igual")
                i = obter_simbolo(tokens[i], i);
            else
                cout << "Erro sintatico!" << endl;
            i = numero(tokens, i);
            if (tokens[i] == "simb_pv")
                i = obter_simbolo(tokens[i], i);
            else
                cout << "Erro sintatico!" << endl;
        }

        return i;
    }

    int dc_v(vector<string> tokens, int i)
    {
        if (tokens[i] == "simb_var")
        {
            i = obter_simbolo(tokens[i], i);
            i = variaveis(tokens, i);
            if (tokens[i] == "simb_dp")
                i = obter_simbolo(tokens[i], i);
            else
                cout << "Erro sintatico!" << endl;
            i = tipo_var(tokens, i);
            if (tokens[i] == "simb_pv")
                i = obter_simbolo(tokens[i], i);
            else
                cout << "Erro sintatico!" << endl;
        }
        return i;
    }

    int parametros(vector<string> tokens, int i)
    {
        if (tokens[i] == "simb_abrir_parentese")
        {
            i = obter_simbolo(tokens[i], i);
            i = lista_par(tokens, i);
            if (tokens[i] == "simb_fechar_parentese")
                i = obter_simbolo(tokens[i], i);
            else
                cout << "Erro sintatico!" << endl;
        }
        return i;
    }

    int dc_loc(vector<string> tokens, int i)
    {
        return dc_v(tokens, i);
    }

    int corpo_p(vector<string> tokens, int i)
    {
        i = dc_loc(tokens, i);
        if (tokens[i] == "simb_begin")
            i = obter_simbolo(tokens[i], i);
        else
            cout << "Erro sintatico!" << endl;
        i = comandos(tokens, i);
        if (tokens[i] == "simb_end")
            i = obter_simbolo(tokens[i], i);
        else
            cout << "Erro sintatico!" << endl;
        if (tokens[i] == "simb_pv")
            i = obter_simbolo(tokens[i], i);
        else
            cout << "Erro sintatico!" << endl;
        return i;
    }

    int dc_p(vector<string> tokens, int i)
    {
        if (tokens[i] == "simb_procedure")
        {
            i = obter_simbolo(tokens[i], i);
            if (tokens[i] == "simb_ident")
                i = obter_simbolo(tokens[i], i);
            else
                cout << "Erro sintatico!" << endl;
            i = parametros(tokens, i);
            if (tokens[i] == "simb_pv")
                i = obter_simbolo(tokens[i], i);
            else
                cout << "Erro sintatico!" << endl;
            i = corpo_p(tokens, i);
        }
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
        i = cmd(tokens, i);
        if (tokens[i] == "simb_pv")
            i = obter_simbolo(tokens[i], i);
        else
            cout << "Erro sintatico!" << endl;
        return i;
    }

    int cmd(vector<string> tokens, int i)
    {
        if (tokens[i] == "simb_read" || tokens[i] == "simb_write")
        {
            i = obter_simbolo(tokens[i], i);
            if (tokens[i] == "simb_abrir_parentese")
                i = obter_simbolo(tokens[i], i);
            else
                cout << "Erro sintatico!" << endl;
            i = variaveis(tokens, i);
            if (tokens[i] == "simb_fechar_parentese")
                i = obter_simbolo(tokens[i], i);
            else
                cout << "Erro sintatico!" << endl;
        }
        else if (tokens[i] == "simb_while")
        {
            i = obter_simbolo(tokens[i], i);
            if (tokens[i] == "simb_abrir_parentese")
                i = obter_simbolo(tokens[i], i);
            else
                cout << "Erro sintatico!" << endl;
            i = condicao(tokens, i);
            if (tokens[i] == "simb_fechar_parentese")
                i = obter_simbolo(tokens[i], i);
            else
                cout << "Erro sintatico!" << endl;
            if (tokens[i] == "simb_do")
                i = obter_simbolo(tokens[i], i);
            else
                cout << "Erro sintatico!" << endl;
            i = cmd(tokens, i);
        }
        else if (tokens[i] == "simb_if")
        {
            i = obter_simbolo(tokens[i], i);
            i = condicao(tokens, i);
            if (tokens[i] == "simb_then")
                i = obter_simbolo(tokens[i], i);
            else
                cout << "Erro sintatico!" << endl;
            i = cmd(tokens, i);
            i = pfalsa(tokens, i);
        }
        else if (tokens[i] == "simb_ident")
        {
            i = obter_simbolo(tokens[i], i);
            i = cmd_linha(tokens, i);
        }
        else if (tokens[i] == "simb_begin")
        {
            i = obter_simbolo(tokens[i], i);
            i = comandos(tokens, i);
            if (tokens[i] == "simb_end")
                i = obter_simbolo(tokens[i], i);
            else
                cout << "Erro sintatico!" << endl;
        }
        else
        {
            cout << "Erro sintatico!" << endl;
        }

        return i;
    }

    int pfalsa(vector<string> tokens, int i)
    {
        if (tokens[i] == "else")
        {
            i = obter_simbolo(tokens[i], i);
            i = cmd(tokens, i);
        }
        return i;
    }

    int relacao(vector<string> tokens, int i)
    {
        if (tokens[i] == "simb_igual" || tokens[i] == "simb_diff" || tokens[i] == "simb_maior_igual" || tokens[i] == "simb_menor_igual" || tokens[i] == "simb_maior" || tokens[i] == "simb_menor")
            i = obter_simbolo(tokens[i], i);
        else
            cout << "Erro sintatico!" << endl;
        return i;
    }

    int expressao(vector<string> tokens, int i)
    {
        i = termo(tokens, i);
        i = outros_termos(tokens, i);
        return i;
    }

    int op_un(vector<string> tokens, int i)
    {
        if (tokens[i] == "simb_soma" || tokens[i] == "simb_sub")
            i = obter_simbolo(tokens[i], i);
        return i;
    }

    int op_ad(vector<string> tokens, int i)
    {
        if (tokens[i] == "simb_soma" || tokens[i] == "simb_sub")
            i = obter_simbolo(tokens[i], i);
        else
            cout << "Erro sintatico!" << endl;
        return i;
    }

    int op_mul(vector<string> tokens, int i)
    {
        if (tokens[i] == "simb_mul" || tokens[i] == "simb_div")
            i = obter_simbolo(tokens[i], i);
        else
            cout << "Erro sintatico!" << endl;
        return i;
    }

    int termo(vector<string> tokens, int i)
    {
        i = op_un(tokens, i);
        i = fator(tokens, i);
        i = mais_fatores(tokens, i);
        return i;
    }

    int fator(vector<string> tokens, int i)
    {
        if (tokens[i] == "simb_ident")
        {
            i = obter_simbolo(tokens[i], i);
        }
        else if (tokens[i] == "simb_abrir_parentese")
        {
            i = obter_simbolo(tokens[i], i);
            i = expressao(tokens, i);
            if (tokens[i] == "simb_fechar_parentese")
                i = obter_simbolo(tokens[i], i);
            else
                cout << "Erro sintatico!" << endl;
        }
        else
        {
            i = numero(tokens, i);
        }
        return i;
    }

    int mais_fatores(vector<string> tokens, int i)
    {
        if (tokens[i] == "simb_mul" || tokens[i] == "simb_div")
        {
            i = obter_simbolo(tokens[i], i);
            i = fator(tokens, i);
        }
        return i;
    }

    int outros_termos(vector<string> tokens, int i)
    {
        if (tokens[i] == "simb_soma" || tokens[i] == "simb_sub")
        {
            i = obter_simbolo(tokens[i], i);
            i = termo(tokens, i);
        }
        return i;
    }

    int condicao(vector<string> tokens, int i)
    {
        i = expressao(tokens, i);
        i = relacao(tokens, i);
        i = expressao(tokens, i);
        return i;
    }

    int cmd_linha(vector<string> tokens, int i)
    {
        if (tokens[i] == "simb_atrib")
        {
            i = obter_simbolo(tokens[i], i);
            i = expressao(tokens, i);
        }
        else
        {
            i = lista_arg(tokens, i);
        }
        return i;
    }

    int argumentos(vector<string> tokens, int i)
    {
        if (tokens[i] == "simb_ident")
            i = obter_simbolo(tokens[i], i);
        else
            cout << "Erro sintatico!" << endl;
        i = mais_ident(tokens, i);
        return i;
    }

    int mais_ident(vector<string> tokens, int i)
    {
        if (tokens[i] == "simb_pv")
        {
            i = obter_simbolo(tokens[i], i);
            i = argumentos(tokens, i);
        }
        return i;
    }

    int lista_arg(vector<string> tokens, int i)
    {
        if (tokens[i] == "simb_abrir_parentese")
        {
            i = obter_simbolo(tokens[i], i);
            i = argumentos(tokens, i);
            if (tokens[i] == "simb_fechar_parentese")
                i = obter_simbolo(tokens[i], i);
            else
                cout << "Erro sintatico!" << endl;
        }
        return i;
    }

    int corpo(vector<string> tokens, int i)
    {
        i = dc(tokens, i);
        if (tokens[i] == "simb_begin")
            i = obter_simbolo(tokens[i], i);
        else
            cout << "Erro sintatico!" << endl;
        i = comandos(tokens, i);
        if (tokens[i] == "simb_end")
            i = obter_simbolo(tokens[i], i);
        else
            cout << "Erro sintatico!" << endl;
        return i;
    }

    bool programa(vector<string> tokens, int i)
    {
        if (tokens[i] == "simb_program")
            i = obter_simbolo(tokens[i], i);
        else
            cout << "Erro sintatico!" << endl;
        if (tokens[i] == "simb_ident")
            i = obter_simbolo(tokens[i], i);
        else
            cout << "Erro sintatico!" << endl;
        if (tokens[i] == "simb_pv")
            i = obter_simbolo(tokens[i], i);
        else
            cout << "Erro sintatico!" << endl;
        i = corpo(tokens, i);
        if (tokens[i] == "simb_dot")
            i = obter_simbolo(tokens[i], i);
        else
            cout << "Erro sintatico!" << endl;
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
    vector<string> tokens{"simb_program", "simb_ident", "simb_pv", "simb_var", "simb_ident", "simb_dp", "simb_tipo_int", "simb_pv", "simb_begin", "simb_ident", "simb_atrib", "simb_tipo_int", "simb_pv", "simb_while", "simb_abrir_parentese", "simb_ident", "simb_menor", "simb_tipo_int", "simb_fechar_parentese", "simb_do", "simb_ident", "simb_atrib", "simb_tipo_int", "simb_pv", "simb_ident", "simb_while", "simb_abrir_parentese", "simb_ident", "simb_menor", "simb_tipo_int", "simb_fechar_parentese", "simb_do", "simb_ident", "simb_atrib", "simb_ident", "simb_soma", "simb_tipo_int", "simb_pv", "simb_end", "simb_dot"};
    sin_an SinAn(tokens);
    //SinAn.imprimeTokens();
    SinAn.processaTokens();
    return 0;
}
