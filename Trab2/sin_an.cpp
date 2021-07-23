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

class sin_an
{
public:
    queue<Token> tokens;

    sin_an(queue<Token> tkns) : tokens(tkns) {}

    void obter_simbolo(queue<Token> tokens)
    {
        cout << tokens.front().simb << endl;
        tokens.pop();
    }

    void numero(queue<Token> tokens)
    {
        if (tokens.front().simb == "simb_tipo_int" || tokens.front().simb == "simb_tipo_real")
            obter_simbolo(tokens);
        cout << "Erro sintatico na linha: " << tokens.front().linha << endl;
    }

    void tipo_var(queue<Token> tokens)
    {
        if (tokens.front().simb == "simb_tipo_int" || tokens.front().simb == "simb_tipo_real")
            obter_simbolo(tokens);
        cout << "Erro sintatico na linha: " << tokens.front().linha << endl;
    }

    void variaveis(queue<Token> tokens)
    {
        if (tokens.front().simb == "simb_ident")
            obter_simbolo(tokens);
        else
            cout << "Erro sintatico na linha: " << tokens.front().linha << endl;
        mais_var(tokens);
    }

    void mais_var(queue<Token> tokens)
    {
        if (tokens.front().simb == "simb_virgula")
        {
            obter_simbolo(tokens);
            variaveis(tokens);
        }
    }

    void lista_par(queue<Token> tokens)
    {
        variaveis(tokens);
        if (tokens.front().simb == "simb_dp")
            obter_simbolo(tokens);
        else
            cout << "Erro sintatico na linha: " << tokens.front().linha << endl;
        tipo_var(tokens);
        mais_var(tokens);
    }

    void dc_c(queue<Token> tokens)
    {
        if (tokens.front().simb == "simb_const")
        {
            obter_simbolo(tokens);
            if (tokens.front().simb == "simb_ident")
                obter_simbolo(tokens);
            else
                cout << "Erro sintatico na linha: " << tokens.front().linha << endl;
            if (tokens.front().simb == "simb_igual")
                obter_simbolo(tokens);
            else
                cout << "Erro sintatico na linha: " << tokens.front().linha << endl;
            numero(tokens);
            if (tokens.front().simb == "simb_pv")
                obter_simbolo(tokens);
            else
                cout << "Erro sintatico na linha: " << tokens.front().linha << endl;
        }
    }

    void dc_v(queue<Token> tokens)
    {
        if (tokens.front().simb == "simb_var")
        {
            obter_simbolo(tokens);
            variaveis(tokens);
            if (tokens.front().simb == "simb_dp")
                obter_simbolo(tokens);
            else
                cout << "Erro sintatico na linha: " << tokens.front().linha << endl;
            tipo_var(tokens);
            if (tokens.front().simb == "simb_pv")
                obter_simbolo(tokens);
            else
                cout << "Erro sintatico na linha: " << tokens.front().linha << endl;
        }
    }

    void parametros(queue<Token> tokens)
    {
        if (tokens.front().simb == "simb_abrir_parentese")
        {
            obter_simbolo(tokens);
            lista_par(tokens);
            if (tokens.front().simb == "simb_fechar_parentese")
                obter_simbolo(tokens);
            else
                cout << "Erro sintatico na linha: " << tokens.front().linha << endl;
        }
    }

    void dc_loc(queue<Token> tokens)
    {
        return dc_v(tokens);
    }

    void corpo_p(queue<Token> tokens)
    {
        dc_loc(tokens);
        if (tokens.front().simb == "simb_begin")
            obter_simbolo(tokens);
        else
            cout << "Erro sintatico na linha: " << tokens.front().linha << endl;
        comandos(tokens);
        if (tokens.front().simb == "simb_end")
            obter_simbolo(tokens);
        else
            cout << "Erro sintatico na linha: " << tokens.front().linha << endl;
        if (tokens.front().simb == "simb_pv")
            obter_simbolo(tokens);
        else
            cout << "Erro sintatico na linha: " << tokens.front().linha << endl;
    }

    void dc_p(queue<Token> tokens)
    {
        if (tokens.front().simb == "simb_procedure")
        {
            obter_simbolo(tokens);
            if (tokens.front().simb == "simb_ident")
                obter_simbolo(tokens);
            else
                cout << "Erro sintatico na linha: " << tokens.front().linha << endl;
            parametros(tokens);
            if (tokens.front().simb == "simb_pv")
                obter_simbolo(tokens);
            else
                cout << "Erro sintatico na linha: " << tokens.front().linha << endl;
            corpo_p(tokens);
        }
    }

    void dc(queue<Token> tokens)
    {
        dc_c(tokens);
        dc_v(tokens);
        dc_p(tokens);
    }

    void comandos(queue<Token> tokens)
    {
        cmd(tokens);
        if (tokens.front().simb == "simb_pv")
            obter_simbolo(tokens);
        else
            cout << "Erro sintatico na linha: " << tokens.front().linha << endl;
    }

    void cmd(queue<Token> tokens)
    {
        if (tokens.front().simb == "simb_read" || tokens.front().simb == "simb_write")
        {
            obter_simbolo(tokens);
            if (tokens.front().simb == "simb_abrir_parentese")
                obter_simbolo(tokens);
            else
                cout << "Erro sintatico na linha: " << tokens.front().linha << endl;
            variaveis(tokens);
            if (tokens.front().simb == "simb_fechar_parentese")
                obter_simbolo(tokens);
            else
                cout << "Erro sintatico na linha: " << tokens.front().linha << endl;
        }
        else if (tokens.front().simb == "simb_while")
        {
            obter_simbolo(tokens);
            if (tokens.front().simb == "simb_abrir_parentese")
                obter_simbolo(tokens);
            else
                cout << "Erro sintatico na linha: " << tokens.front().linha << endl;
            condicao(tokens);
            if (tokens.front().simb == "simb_fechar_parentese")
                obter_simbolo(tokens);
            else
                cout << "Erro sintatico na linha: " << tokens.front().linha << endl;
            if (tokens.front().simb == "simb_do")
                obter_simbolo(tokens);
            else
                cout << "Erro sintatico na linha: " << tokens.front().linha << endl;
            cmd(tokens);
        }
        else if (tokens.front().simb == "simb_if")
        {
            obter_simbolo(tokens);
            condicao(tokens);
            if (tokens.front().simb == "simb_then")
                obter_simbolo(tokens);
            else
                cout << "Erro sintatico na linha: " << tokens.front().linha << endl;
            cmd(tokens);
            pfalsa(tokens);
        }
        else if (tokens.front().simb == "simb_ident")
        {
            obter_simbolo(tokens);
            cmd_linha(tokens);
        }
        else if (tokens.front().simb == "simb_begin")
        {
            obter_simbolo(tokens);
            comandos(tokens);
            if (tokens.front().simb == "simb_end")
                obter_simbolo(tokens);
            else
                cout << "Erro sintatico na linha: " << tokens.front().linha << endl;
        }
        else
        {
            cout << "Erro sintatico na linha: " << tokens.front().linha << endl;
        }
    }

    void pfalsa(queue<Token> tokens)
    {
        if (tokens.front().simb == "else")
        {
            obter_simbolo(tokens);
            cmd(tokens);
        }
    }

    void relacao(queue<Token> tokens)
    {
        if (tokens.front().simb == "simb_igual" || tokens.front().simb == "simb_diff" || tokens.front().simb == "simb_maior_igual" || tokens.front().simb == "simb_menor_igual" || tokens.front().simb == "simb_maior" || tokens.front().simb == "simb_menor")
            obter_simbolo(tokens);
        else
            cout << "Erro sintatico na linha: " << tokens.front().linha << endl;
    }

    void expressao(queue<Token> tokens)
    {
        termo(tokens);
        outros_termos(tokens);
    }

    void op_un(queue<Token> tokens)
    {
        if (tokens.front().simb == "simb_soma" || tokens.front().simb == "simb_sub")
            obter_simbolo(tokens);
    }

    void op_ad(queue<Token> tokens)
    {
        if (tokens.front().simb == "simb_soma" || tokens.front().simb == "simb_sub")
            obter_simbolo(tokens);
        else
            cout << "Erro sintatico na linha: " << tokens.front().linha << endl;
    }

    void op_mul(queue<Token> tokens)
    {
        if (tokens.front().simb == "simb_mul" || tokens.front().simb == "simb_div")
            obter_simbolo(tokens);
        else
            cout << "Erro sintatico na linha: " << tokens.front().linha << endl;
    }

    void termo(queue<Token> tokens)
    {
        op_un(tokens);
        fator(tokens);
        mais_fatores(tokens);
    }

    void fator(queue<Token> tokens)
    {
        if (tokens.front().simb == "simb_ident")
        {
            obter_simbolo(tokens);
        }
        else if (tokens.front().simb == "simb_abrir_parentese")
        {
            obter_simbolo(tokens);
            expressao(tokens);
            if (tokens.front().simb == "simb_fechar_parentese")
                obter_simbolo(tokens);
            else
                cout << "Erro sintatico na linha: " << tokens.front().linha << endl;
        }
        else
        {
            numero(tokens);
        }
    }

    void mais_fatores(queue<Token> tokens)
    {
        if (tokens.front().simb == "simb_mul" || tokens.front().simb == "simb_div")
        {
            obter_simbolo(tokens);
            fator(tokens);
        }
    }

    void outros_termos(queue<Token> tokens)
    {
        if (tokens.front().simb == "simb_soma" || tokens.front().simb == "simb_sub")
        {
            obter_simbolo(tokens);
            termo(tokens);
        }
    }

    void condicao(queue<Token> tokens)
    {
        expressao(tokens);
        relacao(tokens);
        expressao(tokens);
    }

    void cmd_linha(queue<Token> tokens)
    {
        if (tokens.front().simb == "simb_atrib")
        {
            obter_simbolo(tokens);
            expressao(tokens);
        }
        else
        {
            lista_arg(tokens);
        }
    }

    void argumentos(queue<Token> tokens)
    {
        if (tokens.front().simb == "simb_ident")
            obter_simbolo(tokens);
        else
            cout << "Erro sintatico na linha: " << tokens.front().linha << endl;
        mais_ident(tokens);
    }

    void mais_ident(queue<Token> tokens)
    {
        if (tokens.front().simb == "simb_pv")
        {
            obter_simbolo(tokens);
            argumentos(tokens);
        }
    }

    void lista_arg(queue<Token> tokens)
    {
        if (tokens.front().simb == "simb_abrir_parentese")
        {
            obter_simbolo(tokens);
            argumentos(tokens);
            if (tokens.front().simb == "simb_fechar_parentese")
                obter_simbolo(tokens);
            else
                cout << "Erro sintatico na linha: " << tokens.front().linha << endl;
        }
    }

    void corpo(queue<Token> tokens)
    {
        dc(tokens);
        if (tokens.front().simb == "simb_begin")
            obter_simbolo(tokens);
        else
            cout << "Erro sintatico na linha: " << tokens.front().linha << endl;
        comandos(tokens);
        if (tokens.front().simb == "simb_end")
            obter_simbolo(tokens);
        else
            cout << "Erro sintatico na linha: " << tokens.front().linha << endl;
    }

    void programa(queue<Token> tokens)
    {
        if (tokens.front().simb == "simb_program")
            obter_simbolo(tokens);
        else
            cout << "Erro sintatico na linha: " << tokens.front().linha << endl;
        if (tokens.front().simb == "simb_ident")
            obter_simbolo(tokens);
        else
            cout << "Erro sintatico na linha: " << tokens.front().linha << endl;
        if (tokens.front().simb == "simb_pv")
            obter_simbolo(tokens);
        else
            cout << "Erro sintatico na linha: " << tokens.front().linha << endl;
        corpo(tokens);
        if (tokens.front().simb == "simb_dot")
            obter_simbolo(tokens);
        else
            cout << "Erro sintatico na linha: " << tokens.front().linha << endl;
    }

    void processaTokens()
    {
        programa(tokens);
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
    return 0;
}
