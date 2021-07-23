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
            Token newToken;
            newToken.simb = words["integer"];
            newToken.linha = linha;
            tokens.push(newToken);
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

class sin_an
{
public:
    queue<Token> tokens;

    sin_an(queue<Token> tkns) : tokens(tkns) {}

    void obter_simbolo()
    {
        cout << tokens.front().linha << ": " << tokens.front().simb << endl;
        tokens.pop();
    }

    void numero()
    {
        if (tokens.front().simb == "simb_tipo_int" || tokens.front().simb == "simb_tipo_real")
            obter_simbolo();
        else
            cout << "Erro sintatico na linha: " << tokens.front().linha << endl;
    }

    void tipo_var()
    {
        if (tokens.front().simb == "simb_tipo_int" || tokens.front().simb == "simb_tipo_real")
            obter_simbolo();
        else
            cout << "Erro sintatico na linha: " << tokens.front().linha << endl;
    }

    void variaveis()
    {
        if (tokens.front().simb == "simb_ident")
            obter_simbolo();
        else
            cout << "Erro sintatico na linha: " << tokens.front().linha << endl;
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
            cout << "Erro sintatico na linha: " << tokens.front().linha << endl;
        tipo_var();
        mais_var();
    }

    void dc_c()
    {
        if (tokens.front().simb == "simb_const")
        {
            obter_simbolo();
            if (tokens.front().simb == "simb_ident")
                obter_simbolo();
            else
                cout << "Erro sintatico na linha: " << tokens.front().linha << endl;
            if (tokens.front().simb == "simb_igual")
                obter_simbolo();
            else
                cout << "Erro sintatico na linha: " << tokens.front().linha << endl;
            numero();
            if (tokens.front().simb == "simb_pv")
                obter_simbolo();
            else
                cout << "Erro sintatico na linha: " << tokens.front().linha << endl;
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
                cout << "Erro sintatico na linha: " << tokens.front().linha << endl;
            tipo_var();
            if (tokens.front().simb == "simb_pv")
                obter_simbolo();
            else
                cout << "Erro sintatico na linha: " << tokens.front().linha << endl;
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
                cout << "Erro sintatico na linha: " << tokens.front().linha << endl;
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
            cout << "Erro sintatico na linha: " << tokens.front().linha << endl;
        comandos();
        if (tokens.front().simb == "simb_end")
            obter_simbolo();
        else
            cout << "Erro sintatico na linha: " << tokens.front().linha << endl;
        if (tokens.front().simb == "simb_pv")
            obter_simbolo();
        else
            cout << "Erro sintatico na linha: " << tokens.front().linha << endl;
    }

    void dc_p()
    {
        if (tokens.front().simb == "simb_procedure")
        {
            obter_simbolo();
            if (tokens.front().simb == "simb_ident")
                obter_simbolo();
            else
                cout << "Erro sintatico na linha: " << tokens.front().linha << endl;
            parametros();
            if (tokens.front().simb == "simb_pv")
                obter_simbolo();
            else
                cout << "Erro sintatico na linha: " << tokens.front().linha << endl;
            corpo_p();
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
        cmd();
        if (tokens.front().simb == "simb_pv")
            obter_simbolo();
        else
            cout << "Erro sintatico na linha: " << tokens.front().linha << endl;
    }

    void cmd()
    {
        if (tokens.front().simb == "simb_read" || tokens.front().simb == "simb_write")
        {
            obter_simbolo();
            if (tokens.front().simb == "simb_abrir_parentese")
                obter_simbolo();
            else
                cout << "Erro sintatico na linha: " << tokens.front().linha << endl;
            variaveis();
            if (tokens.front().simb == "simb_fechar_parentese")
                obter_simbolo();
            else
                cout << "Erro sintatico na linha: " << tokens.front().linha << endl;
        }
        else if (tokens.front().simb == "simb_while")
        {
            obter_simbolo();
            if (tokens.front().simb == "simb_abrir_parentese")
                obter_simbolo();
            else
                cout << "Erro sintatico na linha: " << tokens.front().linha << endl;
            condicao();
            if (tokens.front().simb == "simb_fechar_parentese")
                obter_simbolo();
            else
                cout << "Erro sintatico na linha: " << tokens.front().linha << endl;
            if (tokens.front().simb == "simb_do")
                obter_simbolo();
            else
                cout << "Erro sintatico na linha: " << tokens.front().linha << endl;
            cmd();
        }
        else if (tokens.front().simb == "simb_if")
        {
            obter_simbolo();
            condicao();
            if (tokens.front().simb == "simb_then")
                obter_simbolo();
            else
                cout << "Erro sintatico na linha: " << tokens.front().linha << endl;
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
                cout << "Erro sintatico na linha: " << tokens.front().linha << endl;
        }
        else
        {
            cout << "Erro sintatico na linha: " << tokens.front().linha << endl;
        }
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
            cout << "Erro sintatico na linha: " << tokens.front().linha << endl;
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
            cout << "Erro sintatico na linha: " << tokens.front().linha << endl;
    }

    void op_mul()
    {
        if (tokens.front().simb == "simb_mul" || tokens.front().simb == "simb_div")
            obter_simbolo();
        else
            cout << "Erro sintatico na linha: " << tokens.front().linha << endl;
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
                cout << "Erro sintatico na linha: " << tokens.front().linha << endl;
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
        }
    }

    void outros_termos()
    {
        if (tokens.front().simb == "simb_soma" || tokens.front().simb == "simb_sub")
        {
            obter_simbolo();
            termo();
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
            cout << "Erro sintatico na linha: " << tokens.front().linha << endl;
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
                cout << "Erro sintatico na linha: " << tokens.front().linha << endl;
        }
    }

    void corpo()
    {
        dc();
        if (tokens.front().simb == "simb_begin")
            obter_simbolo();
        else
            cout << "Erro sintatico na linha: " << tokens.front().linha << endl;
        comandos();
        if (tokens.front().simb == "simb_end")
            obter_simbolo();
        else
            cout << "Erro sintatico na linha: " << tokens.front().linha << endl;
    }

    void programa()
    {
        if (tokens.front().simb == "simb_program")
            obter_simbolo();
        else
            cout << "Erro sintatico na linha: " << tokens.front().linha << endl;
        if (tokens.front().simb == "simb_ident")
            obter_simbolo();
        else
            cout << "Erro sintatico na linha: " << tokens.front().linha << endl;
        if (tokens.front().simb == "simb_pv")
            obter_simbolo();
        else
            cout << "Erro sintatico na linha: " << tokens.front().linha << endl;
        corpo();
        if (tokens.front().simb == "simb_dot")
            obter_simbolo();
        else
            cout << "Erro sintatico na linha: " << tokens.front().linha << endl;
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

    //{"simb_program", "simb_ident", "simb_pv", "simb_var", "simb_ident", "simb_dp", "simb_tipo_int", "simb_pv", "simb_begin", "simb_ident", "simb_atrib", "simb_tipo_int", "simb_pv", "simb_while", "simb_abrir_parentese", "simb_ident", "simb_menor", "simb_tipo_int", "simb_fechar_parentese", "simb_do", "simb_ident", "simb_atrib", "simb_tipo_int", "simb_pv", "simb_ident", "simb_while", "simb_abrir_parentese", "simb_ident", "simb_menor", "simb_tipo_int", "simb_fechar_parentese", "simb_do", "simb_ident", "simb_atrib", "simb_ident", "simb_soma", "simb_tipo_int", "simb_pv", "simb_end", "simb_dot"};

    sin_an SinAn(LexAn.tokens);
    //SinAn.imprimeTokens();
    SinAn.processaTokens();
    return 0;
}
