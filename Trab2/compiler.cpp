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
    int linha = 1;
    int errors = 0;

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
                errors++;
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
                Token newToken;
                newToken.simb = "simb_ident";
                newToken.linha = linha;
                tokens.push(newToken);
                errors++;
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
        {
            cout << "Erro léxico na linha " << linha << ": comentário mal formado" << endl;
            errors++;
        }
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
            {
                cout << "Erro léxico na linha " << linha << ": caractere não permitido" << endl;
                errors++;
            }
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

class sin_an
{
public:
    queue<Token> tokens;
    queue<vector<string>> seguidores;
    vector<string> simb_sincr{"simb_program", "simb_begin", "simb_end", "simb_const", "simb_procedure", "simb_read", "simb_write", "simb_while", "simb_if", "simb_else", "simb_do", "simb_to"};
    int errors = 0;

    sin_an(queue<Token> tkns) : tokens(tkns) {}

    void obter_simbolo()
    {
        //cout << tokens.front().linha << ": " << tokens.front().simb << endl;
        tokens.pop();
    }

    void ERRO()
    {
        errors++;
        // verifica se ainda não chegou ao fim do programa
        // verifica se token corrente nao pertence ao conjunto simb_sincr
        /*cout << "Token de ERRO: " << tokens.front().simb << endl;
        cout << "Tamanho seguidor: " << seguidores.size() << endl;
        for (int i = 0; i < seguidores.front().size(); i++)
        {
            cout << "Seguidor de ERRO: " << seguidores.front()[i] << endl;
        }*/
        // tratar caso do seguidor vazio!
        while (!tokens.empty() && *find(simb_sincr.begin(), simb_sincr.end(), tokens.front().simb) != tokens.front().simb)
        {
            if (!seguidores.empty() && *find(seguidores.front().begin(), seguidores.front().end(), tokens.front().simb) != tokens.front().simb)
                seguidores.pop();
            obter_simbolo();
            /*cout << "Token de ERRO: " << tokens.front().simb << endl;
            cout << "Tamanho seguidor: " << seguidores.size() << endl;
            for (int i = 0; i < seguidores.front().size(); i++)
            {
                cout << "Seguidor de ERRO: " << seguidores.front()[i] << endl;
            }*/
        }
    }

    void numero()
    {
        if (tokens.front().simb == "simb_tipo_int" || tokens.front().simb == "simb_tipo_real")
            obter_simbolo();
        else
        {
            cout << "Erro sintatico na linha: " << tokens.front().linha << ", numero esperado" << endl;
            ERRO();
        }
    }

    void tipo_var()
    {
        if (tokens.front().simb == "simb_tipo_int" || tokens.front().simb == "simb_tipo_real")
            obter_simbolo();
        else
        {
            cout << "Erro sintatico na linha: " << tokens.front().linha << ", tipo_var esperado" << endl;
            ERRO();
        }
    }

    void variaveis()
    {
        if (tokens.front().simb == "simb_ident")
            obter_simbolo();
        else
        {
            cout << "Erro sintatico na linha: " << tokens.front().linha << ", simb_ident esperado" << endl;
            // Push Primeiro mais_var
            vector<string> Primeiro{"simb_virgula"};
            seguidores.push(Primeiro);
            ERRO();
        }
        mais_var();
    }

    void mais_var()
    {
        if (tokens.front().simb == "simb_virgula")
        {
            obter_simbolo();
            variaveis();
        }
    }

    void lista_par()
    {
        variaveis();
        if (tokens.front().simb == "simb_dp")
            obter_simbolo();
        else
        {
            cout << "Erro sintatico na linha: " << tokens.front().linha << ", simb_dp esperado" << endl;
            // Push Primeiro tipo_var
            vector<string> Primeiro{"simb_tipo_int, simb_tipo_real"};
            seguidores.push(Primeiro);
            ERRO();
        }
        tipo_var();
        mais_par();
    }

    void mais_par()
    {
        if (tokens.front().simb == "simb_pv")
        {
            lista_par();
        }
    }

    void dc_c()
    {
        if (tokens.front().simb == "simb_const")
        {
            obter_simbolo();
            if (tokens.front().simb == "simb_ident")
                obter_simbolo();
            else
            {
                cout << "Erro sintatico na linha: " << tokens.front().linha << ", simb_ident esperado" << endl;
                vector<string> Primeiro{"simb_igual"};
                seguidores.push(Primeiro);
                ERRO();
            }
            if (tokens.front().simb == "simb_igual")
                obter_simbolo();
            else
            {
                cout << "Erro sintatico na linha: " << tokens.front().linha << ", simb_igual esperado" << endl;
                // Push Primeiro numero
                vector<string> Primeiro{"simb_tipo_int, simb_tipo_real"};
                seguidores.push(Primeiro);
                ERRO();
            }
            numero();
            if (tokens.front().simb == "simb_pv")
                obter_simbolo();
            else
            {
                cout << "Erro sintatico na linha: " << tokens.front().linha << ", simb_pv esperado" << endl;
                // Push primeiro dc_c
                vector<string> Primeiro{"simb_const"};
                seguidores.push(Primeiro);
                ERRO();
            }
            dc_c();
        }
    }

    void dc_v()
    {
        if (tokens.front().simb == "simb_var")
        {
            obter_simbolo();
            variaveis();
            if (tokens.front().simb == "simb_dp")
                obter_simbolo();
            else
            {
                cout << "Erro sintatico na linha: " << tokens.front().linha << ", simb_dp esperado" << endl;
                // Push Primeiro tipo_var
                vector<string> Primeiro{"simb_tipo_int, simb_tipo_real"};
                seguidores.push(Primeiro);
                ERRO();
            }
            tipo_var();
            if (tokens.front().simb == "simb_pv")
                obter_simbolo();
            else
            {
                cout << "Erro sintatico na linha: " << tokens.front().linha << ", simb_pv esperado" << endl;
                // Push Primeiro dc_v
                vector<string> Primeiro{"simb_var"};
                seguidores.push(Primeiro);
                ERRO();
            }
            dc_v();
        }
    }

    void parametros()
    {
        if (tokens.front().simb == "simb_abrir_parentese")
        {
            obter_simbolo();
            lista_par();
            if (tokens.front().simb == "simb_fechar_parentese")
                obter_simbolo();
            else
            {
                cout << "Erro sintatico na linha: " << tokens.front().linha << ", simb_fechar_parentese esperado" << endl;
                ERRO();
            }
        }
    }

    void dc_loc()
    {
        return dc_v();
    }

    void corpo_p()
    {
        dc_loc();
        if (tokens.front().simb == "simb_begin")
            obter_simbolo();
        else
        {
            cout << "Erro sintatico na linha: " << tokens.front().linha << ", simb_begin esperado" << endl;
            // Push Primeiro comandos
            // Todos estao nos simbolos de sincronizacao
            ERRO();
        }
        comandos();
        if (tokens.front().simb == "simb_end")
            obter_simbolo();
        else
        {
            cout << "Erro sintatico aqui linha: " << tokens.front().linha << ", simb_end esperado" << endl;
            vector<string> Primeiro{"simb_pv"};
            seguidores.push(Primeiro);
            ERRO();
        }
        if (tokens.front().simb == "simb_pv")
            obter_simbolo();
        else
        {
            cout << "Erro sintatico na linha: " << tokens.front().linha << ", simb_pv esperado" << endl;
            ERRO();
        }
    }

    void dc_p()
    {
        if (tokens.front().simb == "simb_procedure")
        {
            obter_simbolo();
            if (tokens.front().simb == "simb_ident")
                obter_simbolo();
            else
            {
                cout << "Erro sintatico na linha: " << tokens.front().linha << ", simb_ident esperado" << endl;
                // Push Primeiro parametros
                vector<string> Primeiro{"simb_abrir_parentese"};
                seguidores.push(Primeiro);
                ERRO();
            }
            parametros();
            if (tokens.front().simb == "simb_pv")
                obter_simbolo();
            else
            {
                cout << "Erro sintatico na linha: " << tokens.front().linha << ", simb_pv esperado" << endl;
                // Push Primeiro corpo_p
                vector<string> Primeiro{"simb_var"};
                seguidores.push(Primeiro);
                ERRO();
            }
            corpo_p();
            dc_p();
        }
    }

    void dc()
    {
        dc_c();
        dc_v();
        dc_p();
    }

    void comandos()
    {
        if (cmd())
        {
            if (tokens.front().simb == "simb_pv")
                obter_simbolo();
            else
            {
                cout << "Erro sintatico na linha: " << tokens.front().linha << ", simb_pv esperado" << endl;
                // Push Primeiro comandos
                // Todos estao nos simbolos de sincronizacao
                ERRO();
            }
            comandos();
        }
    }

    bool cmd()
    {
        // read || write
        if (tokens.front().simb == "simb_read" || tokens.front().simb == "simb_write")
        {
            obter_simbolo();
            if (tokens.front().simb == "simb_abrir_parentese")
                obter_simbolo();
            else
            {
                cout << "Erro sintatico na linha: " << tokens.front().linha << ", simb_abrir_parentese esperado" << endl;
                // Push Primeiro variaveis
                vector<string> Primeiro{"simb_ident"};
                seguidores.push(Primeiro);
                ERRO();
            }
            variaveis();
            if (tokens.front().simb == "simb_fechar_parentese")
                obter_simbolo();
            else
            {
                cout << "Erro sintatico na linha: " << tokens.front().linha << ", simb_fechar_parentese esperado" << endl;
                ERRO();
            }
        }
        // while
        else if (tokens.front().simb == "simb_while")
        {
            obter_simbolo();
            if (tokens.front().simb == "simb_abrir_parentese")
                obter_simbolo();
            else
            {
                cout << "Erro sintatico na linha: " << tokens.front().linha << ", simb_abrir_parentese esperado" << endl;
                // Push Primeiro condicao
                vector<string> Primeiro{"simb_soma", "simb_sub"};
                seguidores.push(Primeiro);
                ERRO();
            }
            condicao();
            if (tokens.front().simb == "simb_fechar_parentese")
                obter_simbolo();
            else
            {
                cout << "Erro sintatico na linha: " << tokens.front().linha << ", simb_fechar_parentese esperado" << endl;
                vector<string> Primeiro{"simb_do"};
                seguidores.push(Primeiro);
                ERRO();
            }
            if (tokens.front().simb == "simb_do")
                obter_simbolo();
            else
            {
                cout << "Erro sintatico na linha: " << tokens.front().linha << ", simb_do esperado" << endl;
                // Push primeiro cmd
                //seguidores.push("ident");
                ERRO();
            }
            cmd();
        }
        else if (tokens.front().simb == "simb_for")
        {
            obter_simbolo();
            if (tokens.front().simb == "simb_ident")
                obter_simbolo();
            else
            {
                cout << "Erro sintatico na linha: " << tokens.front().linha << ", simb_ident esperado" << endl;
                vector<string> Primeiro{"simb_atrib"};
                seguidores.push(Primeiro);
                ERRO();
            }
            if (tokens.front().simb == "simb_atrib")
                obter_simbolo();
            else
            {
                cout << "Erro sintatico na linha: " << tokens.front().linha << ", simb_atrib esperado" << endl;
                vector<string> Primeiro{"simb_tipo_int"};
                seguidores.push(Primeiro);
                ERRO();
            }
            if (tokens.front().simb == "simb_tipo_int")
                obter_simbolo();
            else
            {
                cout << "Erro sintatico na linha: " << tokens.front().linha << ", simb_tipo_int esperado" << endl;
                vector<string> Primeiro{"simb_to"};
                seguidores.push(Primeiro);
                ERRO();
            }
            if (tokens.front().simb == "simb_to")
                obter_simbolo();
            else
            {
                cout << "Erro sintatico na linha: " << tokens.front().linha << ", simb_to esperado" << endl;
                vector<string> Primeiro{"simb_ident"};
                seguidores.push(Primeiro);
                ERRO();
            }
            if (tokens.front().simb == "simb_ident")
                obter_simbolo();
            else
            {
                cout << "Erro sintatico na linha: " << tokens.front().linha << ", simb_ident esperado" << endl;
                ERRO();
            }
            if (tokens.front().simb == "simb_do")
                obter_simbolo();
            else
            {
                cout << "Erro sintatico na linha: " << tokens.front().linha << ", simb_do esperado" << endl;
                // Push Primeiro cmd
                ERRO();
            }
            cmd();
        } // if
        else if (tokens.front().simb == "simb_if")
        {
            obter_simbolo();
            condicao();
            if (tokens.front().simb == "simb_then")
                obter_simbolo();
            else
            {
                cout << "Erro sintatico na linha: " << tokens.front().linha << ", simb_then esperado" << endl;
                // Push Primeiro cmd
                ERRO();
            }
            cmd();
            pfalsa();
        }
        else if (tokens.front().simb == "simb_ident")
        {
            obter_simbolo();
            cmd_linha();
        }
        else if (tokens.front().simb == "simb_begin")
        {
            obter_simbolo();
            comandos();
            if (tokens.front().simb == "simb_end")
                obter_simbolo();
            else
            {
                cout << "Erro sintatico na linha: " << tokens.front().linha << ", simb_end esperado" << endl;
                ERRO();
            }
        }
        else
        {
            return false;
        }
        return true;
    }

    void pfalsa()
    {
        if (tokens.front().simb == "else")
        {
            obter_simbolo();
            cmd();
        }
    }

    void relacao()
    {
        if (tokens.front().simb == "simb_igual" || tokens.front().simb == "simb_diff" || tokens.front().simb == "simb_maior_igual" || tokens.front().simb == "simb_menor_igual" || tokens.front().simb == "simb_maior" || tokens.front().simb == "simb_menor")
            obter_simbolo();
        else
        {
            cout << "Erro sintatico na linha: " << tokens.front().linha << ", relacao esperado" << endl;
            ERRO();
        }
    }

    void expressao()
    {
        termo();
        outros_termos();
    }

    void op_un()
    {
        if (tokens.front().simb == "simb_soma" || tokens.front().simb == "simb_sub")
            obter_simbolo();
    }

    void op_ad()
    {
        if (tokens.front().simb == "simb_soma" || tokens.front().simb == "simb_sub")
            obter_simbolo();
        else
        {
            cout << "Erro sintatico na linha: " << tokens.front().linha << ", op_ad esperado" << endl;
            ERRO();
        }
    }

    void op_mul()
    {
        if (tokens.front().simb == "simb_mul" || tokens.front().simb == "simb_div")
            obter_simbolo();
        else
        {
            cout << "Erro sintatico na linha: " << tokens.front().linha << ", op_mul esperado" << endl;
            ERRO();
        }
    }

    void termo()
    {
        op_un();
        fator();
        mais_fatores();
    }

    void fator()
    {
        if (tokens.front().simb == "simb_ident")
        {
            obter_simbolo();
        }
        else if (tokens.front().simb == "simb_abrir_parentese")
        {
            obter_simbolo();
            expressao();
            if (tokens.front().simb == "simb_fechar_parentese")
                obter_simbolo();
            else
            {
                cout << "Erro sintatico na linha: " << tokens.front().linha << ", simb_fechar_parentese esperado" << endl;
                ERRO();
            }
        }
        else
        {
            numero();
        }
    }

    void mais_fatores()
    {
        if (tokens.front().simb == "simb_mul" || tokens.front().simb == "simb_div")
        {
            obter_simbolo();
            fator();
            mais_fatores();
        }
    }

    void outros_termos()
    {
        if (tokens.front().simb == "simb_soma" || tokens.front().simb == "simb_sub")
        {
            obter_simbolo();
            termo();
            outros_termos();
        }
    }

    void condicao()
    {
        expressao();
        relacao();
        expressao();
    }

    void cmd_linha()
    {
        if (tokens.front().simb == "simb_atrib")
        {
            obter_simbolo();
            expressao();
        }
        else
        {
            lista_arg();
        }
    }

    void argumentos()
    {
        if (tokens.front().simb == "simb_ident")
            obter_simbolo();
        else
        {
            cout << "Erro sintatico na linha: " << tokens.front().linha << ", simb_ident esperado" << endl;
            // Push Primeiro mais_ident
            vector<string> Primeiro{"simb_pv"};
            seguidores.push(Primeiro);
            ERRO();
        }
        mais_ident();
    }

    void mais_ident()
    {
        if (tokens.front().simb == "simb_pv")
        {
            obter_simbolo();
            argumentos();
        }
    }

    void lista_arg()
    {
        if (tokens.front().simb == "simb_abrir_parentese")
        {
            obter_simbolo();
            argumentos();
            if (tokens.front().simb == "simb_fechar_parentese")
                obter_simbolo();
            else
            {
                cout << "Erro sintatico na linha: " << tokens.front().linha << ", simb_fechar_parentese esperado" << endl;
                ERRO();
            }
        }
    }

    void corpo()
    {
        dc();
        if (tokens.front().simb == "simb_begin")
            obter_simbolo();
        else
        {
            cout << "Erro sintatico na linha: " << tokens.front().linha << ", simb_begin esperado" << endl;
            // Push Primeiro comandos
            // Igual aos simbolos de sincronizacao
            ERRO();
        }
        comandos();
        if (tokens.front().simb == "simb_end")
            obter_simbolo();
        else
        {
            cout << "Erro sintatico na linha: " << tokens.front().linha << ", simb_end esperado" << endl;
            ERRO();
        }
    }

    void programa()
    {
        if (tokens.front().simb == "simb_program")
            obter_simbolo();
        else
        {
            cout << "Erro sintatico na linha: " << tokens.front().linha << ", simb_program esperado" << endl;
            vector<string> Primeiro{"simb_ident"};
            seguidores.push(Primeiro);
            ERRO();
        }
        if (tokens.front().simb == "simb_ident")
            obter_simbolo();
        else
        {
            cout << "Erro sintatico na linha: " << tokens.front().linha << ", simb_ident esperado" << endl;
            vector<string> Primeiro{"simb_pv"};
            seguidores.push(Primeiro);
            ERRO();
        }
        if (tokens.front().simb == "simb_pv")
            obter_simbolo();
        else
        {
            cout << "Erro sintatico na linha: " << tokens.front().linha << ", simb_pv esperado" << endl;
            // Push primeiro corpo
            vector<string> Primeiro{"simb_const", "simb_var", "simb_procedure"};
            seguidores.push(Primeiro);
            ERRO();
        }
        corpo();
        if (tokens.front().simb == "simb_dot")
            obter_simbolo();
        else
        {
            cout << "Erro sintatico na linha: " << tokens.front().linha << ", simb_dot esperado" << endl;
            //ERRO();
        }
    }

    void processaTokens()
    {
        programa();
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

    //LexAn.imprimeTokens();

    //cout << "Arquivo " << fileNameOut << " gerado com sucesso!" << endl;

    fileIn.close();
    //fileOut.close();

    sin_an SinAn(LexAn.tokens);

    SinAn.processaTokens();

    int total_errors = SinAn.errors + LexAn.errors;
    cout << "Quantidade total de erros encontrados: " << total_errors << endl;
    if (total_errors == 0)
        cout << "Compilação foi um sucesso!" << endl;
    else
        cout << "Falha na compilação!" << endl;

    return 0;
}
