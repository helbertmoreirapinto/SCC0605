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

typedef struct
{
    string erro;
    int linha;
} Erro;

class lex_an
{
public:
    map<string, string> words, symbols;
    queue<Token> tokens;
    queue<Erro> erros_queue;
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
        words["write"] = "simb_write";
        words["while"] = "simb_while";
        words["do"] = "simb_do";
        words["for"] = "simb_for";
        words["to"] = "simb_to";
        words["downto"] = "simb_downto";

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
            i++;
            while (isNumber(str[i]))
                number.push_back(str[i++]);
            if (isChar(str[i]))
            {
                Erro newErro;
                newErro.erro = "número real mal formado";
                newErro.linha = linha;
                erros_queue.push(newErro);
                Token newToken;
                newToken.simb = words["real"];
                newToken.linha = linha;
                tokens.push(newToken);
                errors++;
                while (isChar(str[i]) || isNumber(str[i]))
                    i++;
                i--;
                return i;
            }
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
                Erro newErro;
                newErro.erro = "ident mal formado";
                newErro.linha = linha;
                erros_queue.push(newErro);
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
            Erro newErro;
            newErro.erro = "comentário mal formado";
            newErro.linha = linha;
            erros_queue.push(newErro);
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
                Erro newErro;
                newErro.erro = "caractere não permitido";
                newErro.linha = linha;
                erros_queue.push(newErro);
                errors++;
            }
        }
    }

    void imprimeTokens()
    {
        cout << "Quantidade de tokens: " << tokens.size() << endl;
        Token token;
        while (!tokens.empty())
        {
            token = tokens.front();
            cout << "Linha: " << token.linha << " Token: " << token.simb << endl;
            tokens.pop();
        }
        cout << "Imprimiu todos" << endl;
    }
};

class sin_an
{
public:
    queue<Token> tokens;
    vector<string> simb_sincr{"simb_program", "simb_begin", "simb_end", "simb_const", "simb_procedure", "simb_read", "simb_write", "simb_while", "simb_if", "simb_else", "simb_do", "simb_to", "simb_igual"};
    stack<vector<string>> Seg;
    queue<Erro> erros_queue;
    int errors = 0;
    bool panico = false;

    sin_an(queue<Token> tkns) : tokens(tkns) {}

    void obter_simbolo()
    {
        //cout << tokens.front().linha << ": " << tokens.front().simb << endl;
        tokens.pop();
    }

    void ERRO(string msg)
    {
        if (tokens.empty())
            return;

        errors++;
        Erro newErro;
        newErro.erro = msg;
        newErro.linha = tokens.front().linha;
        erros_queue.push(newErro);
        // verifica se ainda não chegou ao fim do programa
        // verifica se token corrente nao pertence ao conjunto simb_sincr
        // verifica se tokens corrente nao pertence ao vetor de seguidores

        while (!tokens.empty() && find(simb_sincr.begin(), simb_sincr.end(), tokens.front().simb) == simb_sincr.end())
        {
            if (!Seg.empty() && find(Seg.top().begin(), Seg.top().end(), tokens.front().simb) != Seg.top().end())
                break;

            if (!Seg.empty())
                Seg.pop();

            obter_simbolo();
        }
    }

    void numero()
    {
        if (tokens.front().simb == "simb_tipo_int" || tokens.front().simb == "simb_tipo_real")
            obter_simbolo();
        else
        {
            vector<string> S;
            Seg.push(S);
            ERRO("numero esperado");
        }
    }

    void tipo_var()
    {
        if (tokens.front().simb == "simb_tipo_int" || tokens.front().simb == "simb_tipo_real")
            obter_simbolo();
        else
        {
            vector<string> S;
            Seg.push(S);
            ERRO("tipo de variável esperada");
        }
    }

    void variaveis()
    {
        if (tokens.front().simb == "simb_ident")
            obter_simbolo();
        else
        {
            // Push Primeiro mais_var
            vector<string> S{"simb_virgula"};

            Seg.push(S);
            ERRO("identificador esperado");
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
            // Push Primeiro tipo_var
            vector<string> S{"simb_tipo_int, simb_tipo_real"};
            Seg.push(S);
            ERRO("símbolo ':' esperado");
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
                vector<string> S{"simb_igual"};
                Seg.push(S);
                ERRO("identificador esperado");
            }
            if (tokens.front().simb == "simb_igual")
                obter_simbolo();
            else
            {
                // Push Primeiro numero
                vector<string> S{"simb_tipo_int, simb_tipo_real"};
                Seg.push(S);
                ERRO("símbolo de igual '=' esperado");
            }
            numero();
            if (tokens.front().simb == "simb_pv")
                obter_simbolo();
            else
            {
                // Push primeiro dc_c
                vector<string> S{"simb_const"};
                Seg.push(S);
                ERRO("símbolo ';' esperado");
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
                // Push Primeiro tipo_var
                vector<string> S{"simb_tipo_int, simb_tipo_real"};
                Seg.push(S);
                ERRO("símbolo ':' esperado");
            }
            tipo_var();
            if (tokens.front().simb == "simb_pv")
                obter_simbolo();
            else
            {
                // Push Primeiro dc_v
                vector<string> S{"simb_var"};
                Seg.push(S);
                ERRO("símbolo ';' esperado");
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
                vector<string> S;
                Seg.push(S);
                ERRO("símbolo de fechar parêntese ')' esperado");
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
            // Push Primeiro comandos
            // Todos estao nos simbolos de sincronizacao
            vector<string> S;
            Seg.push(S);
            ERRO("símbolo 'begin' esperado");
        }
        comandos();
        if (tokens.front().simb == "simb_end")
            obter_simbolo();
        else
        {
            vector<string> S{"simb_pv"};
            Seg.push(S);
            ERRO("símbolo 'end' esperado");
        }
        if (tokens.front().simb == "simb_pv")
            obter_simbolo();
        else
        {
            vector<string> S;
            Seg.push(S);
            ERRO("símbolo ';' esperado");
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
                // Push Primeiro parametros
                vector<string> S{"simb_abrir_parentese"};
                Seg.push(S);
                ERRO("identificador esperado");
            }
            parametros();
            if (tokens.front().simb == "simb_pv")
                obter_simbolo();
            else
            {
                // Push Primeiro corpo_p
                vector<string> S{"simb_var"};
                Seg.push(S);
                ERRO("símbolo ';' esperado");
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
                // Push Primeiro comandos
                // Todos estao nos simbolos de sincronizacao
                vector<string> S;
                Seg.push(S);
                ERRO("símbolo ';' esperado");
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
                // Push Primeiro variaveis
                vector<string> S{"simb_ident"};
                Seg.push(S);
                ERRO("símbolo abrir parêntese '(' esperado");
            }
            variaveis();
            if (tokens.front().simb == "simb_fechar_parentese")
                obter_simbolo();
            else
            {
                vector<string> S;
                Seg.push(S);
                ERRO("símbolo fechar parêntese ')' esperado");
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
                // Push Primeiro condicao
                vector<string> S{"simb_soma", "simb_sub"};
                Seg.push(S);
                ERRO("símbolo abrir parêntese '(' esperado");
            }
            condicao();
            if (tokens.front().simb == "simb_fechar_parentese")
                obter_simbolo();
            else
            {
                vector<string> S{"simb_do"};
                Seg.push(S);
                ERRO("símbolo fechar parêntese ')' esperado");
            }
            if (tokens.front().simb == "simb_do")
                obter_simbolo();
            else
            {
                // Push primeiro cmd
                vector<string> S{"simb_ident"};
                Seg.push(S);
                ERRO("simb_do");
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
                vector<string> S{"simb_atrib"};
                Seg.push(S);
                ERRO("identificador esperado");
            }
            if (tokens.front().simb == "simb_atrib")
                obter_simbolo();
            else
            {
                vector<string> S{"simb_tipo_int", "simb_ident"};
                Seg.push(S);
                ERRO("símbolo de atribuição ':=' esperado");
            }
            if (tokens.front().simb == "simb_tipo_int" || tokens.front().simb == "simb_ident")
                obter_simbolo();
            else
            {
                vector<string> S{"simb_to", "simb_downto"};
                Seg.push(S);
                ERRO("símbolo de inteiro 'integer' ou identificador esperado");
            }
            if (tokens.front().simb == "simb_to" || tokens.front().simb == "simb_downto")
                obter_simbolo();
            else
            {
                vector<string> S{"simb_tipo_int", "simb_ident"};
                Seg.push(S);
                ERRO("símbolo 'to' ou símbolo 'downto' esperado");
            }
            if (tokens.front().simb == "simb_tipo_int" || tokens.front().simb == "simb_ident")
                obter_simbolo();
            else
            {
                vector<string> S;
                Seg.push(S);
                ERRO("símbolo de inteiro 'integer' ou identificador esperado");
            }
            if (tokens.front().simb == "simb_do")
                obter_simbolo();
            else
            {
                // Push Primeiro cmd
                vector<string> S{"simb_ident"};
                Seg.push(S);
                ERRO("símbolo 'do' esperado");
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
                // Push Primeiro cmd
                vector<string> S{"simb_ident"};
                Seg.push(S);
                ERRO("símbolo 'then' esperado");
            }
            cmd();
            pfalsa();
        }
        else if (tokens.front().simb == "simb_ident")
        {
            obter_simbolo();
            if (tokens.front().simb == "simb_atrib" || tokens.front().simb == "simb_igual")
            {
                if (tokens.front().simb == "simb_igual")
                {
                    errors++;
                    Erro newErro;
                    newErro.erro = "comparacão não esperada";
                    newErro.linha = tokens.front().linha;
                    erros_queue.push(newErro);
                }
                obter_simbolo();
                expressao();
            }
            else if (tokens.front().simb == "simb_abrir_parentese")
            {
                obter_simbolo();
                argumentos();
                if (tokens.front().simb == "simb_fechar_parentese")
                    obter_simbolo();
                else
                {
                    vector<string> S;
                    Seg.push(S);
                    ERRO("símbolo fechar parêntese ')' esperado");
                }
            }
            else
            {
                vector<string> S{"simb_pv", "simb_ident"};
                Seg.push(S);
                ERRO("símbolo de atribuição ':=' ou símbolo abrir parêntese '(' esperado");
            }
        }
        else if (tokens.front().simb == "simb_begin")
        {
            obter_simbolo();
            comandos();
            if (tokens.front().simb == "simb_end")
                obter_simbolo();
            else
            {
                vector<string> S;
                Seg.push(S);
                ERRO("símbolo 'end' esperado");
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
            vector<string> S;
            Seg.push(S);
            ERRO("relação esperada");
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
            vector<string> S;
            Seg.push(S);
            ERRO("operação de aritmética (op_ad) esperada");
        }
    }

    void op_mul()
    {
        if (tokens.front().simb == "simb_mul" || tokens.front().simb == "simb_div")
            obter_simbolo();
        else
        {
            vector<string> S;
            Seg.push(S);
            ERRO("operação de aritmética (op_mul) esperado");
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
                vector<string> S;
                Seg.push(S);
                ERRO("símbolo de fechar parêntese ')' esperado");
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
            // Push Primeiro mais_ident
            vector<string> S{"simb_pv"};
            Seg.push(S);
            ERRO("identificador esperado");
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
                vector<string> S;
                Seg.push(S);
                ERRO("símbolo de fechar parêntese ')' esperado");
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
            // Push Primeiro comandos
            // Igual aos simbolos de sincronizacao
            vector<string> S;
            Seg.push(S);
            ERRO("símbolo 'begin' esperado");
        }
        comandos();
        if (tokens.front().simb == "simb_end")
            obter_simbolo();
        else
        {
            vector<string> S;
            Seg.push(S);
            ERRO("símbolo 'end' esperado");
        }
    }

    void programa()
    {
        if (tokens.front().simb == "simb_program")
            obter_simbolo();
        else
        {
            vector<string> S{"simb_ident"};
            Seg.push(S);
            ERRO("símbolo 'program' esperado");
        }
        if (tokens.front().simb == "simb_ident")
            obter_simbolo();
        else
        {
            vector<string> S{"simb_pv"};
            Seg.push(S);
            ERRO("identificador esperado");
        }
        if (tokens.front().simb == "simb_pv")
            obter_simbolo();
        else
        {
            // Push primeiro corpo
            vector<string> S{"simb_const", "simb_var", "simb_procedure"};
            Seg.push(S);
            ERRO("símbolo ';' esperado");
        }
        corpo();
        if (tokens.front().simb == "simb_dot")
            obter_simbolo();
        else
        {
            vector<string> S;
            Seg.push(S);
            ERRO("símbolo '.' esperado");
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

void imprimirErros(lex_an LexAn, sin_an SinAn)
{
    Erro msg_erro;
    bool lex;
    int total_errors = SinAn.errors + LexAn.errors;
    if (total_errors > 0)
    {
        while (!LexAn.erros_queue.empty() || !SinAn.erros_queue.empty())
        {
            if (!LexAn.erros_queue.empty())
            {
                if (!SinAn.erros_queue.empty())
                {
                    if (LexAn.erros_queue.front().linha <= SinAn.erros_queue.front().linha)
                    {
                        msg_erro = LexAn.erros_queue.front();
                        lex = true;
                        LexAn.erros_queue.pop();
                    }
                    else
                    {
                        msg_erro = SinAn.erros_queue.front();
                        lex = false;
                        SinAn.erros_queue.pop();
                    }
                }
                else
                {
                    msg_erro = LexAn.erros_queue.front();
                    lex = true;
                    LexAn.erros_queue.pop();
                }
            }
            else
            {
                msg_erro = SinAn.erros_queue.front();
                lex = false;
                SinAn.erros_queue.pop();
            }
            if (lex)
                cout << "Erro léxico";
            else
                cout << "Erro sintático";
            cout << " na linha " << msg_erro.linha << ": " << msg_erro.erro << endl;
        }
    }

    cout << "Quantidade total de erros encontrados: " << total_errors << endl;
    if (total_errors == 0)
        cout << "Compilação foi um sucesso!" << endl;
    else
        cout << "Falha na compilação!" << endl;
}

int main()
{
    string fileName;
    cin >> fileName;

    string fileNameIn = fileName;

    fileNameIn.append(".txt");

    // open file

    ifstream fileIn(fileNameIn);

    if (!fileIn)
    {
        cout << "file not found!\n";
        return -1;
    }

    // Criacao do analisador lexico

    lex_an LexAn;

    char str[MAX_LINE];
    while (fileIn.getline(str, MAX_LINE))
    {
        LexAn.processaLinha(str);
        LexAn.linha++;
    }

    fileIn.close();

    // Comunicacao com o analisador Sintatico

    sin_an SinAn(LexAn.tokens);

    SinAn.processaTokens();

    imprimirErros(LexAn, SinAn);

    return 0;
}
