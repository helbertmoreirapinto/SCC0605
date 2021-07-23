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
    vector<Token> tokens;
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
            tokens.push_back(newToken);
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
            tokens.push_back(newToken);
        }
        else
        {
            Token newToken;
            newToken.simb = words["integer"];
            newToken.linha = linha;
            tokens.push_back(newToken);
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
        tokens.push_back(newToken);
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
        tokens.push_back(newToken);
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
        tokens.push_back(newToken);
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
        tokens.push_back(newToken);
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
        for (int i = 0; i < tokens.size(); i++)
        {
            token = tokens[i];
            cout << "Linha: " << token.linha << " Token: " << token.simb << endl;
        }
    }
};

class sin_an
{
public:
    vector<Token> tokens;

    sin_an(vector<Token> tkns) : tokens(tkns) {}

    int obter_simbolo(Token token, int i)
    {
        cout << token.simb << endl;
        i++;
        return i;
    }

    int numero(vector<Token> tokens, int i)
    {
        if (tokens[i].simb == "simb_tipo_int" || tokens[i].simb == "simb_tipo_real")
            return obter_simbolo(tokens[i], i);
        cout << "Erro sintatico na linha: " << tokens[i].linha << endl;
        return i;
    }

    int tipo_var(vector<Token> tokens, int i)
    {
        if (tokens[i].simb == "simb_tipo_int" || tokens[i].simb == "simb_tipo_real")
            return obter_simbolo(tokens[i], i);
        cout << "Erro sintatico na linha: " << tokens[i].linha << endl;
        return i;
    }

    int variaveis(vector<Token> tokens, int i)
    {
        if (tokens[i].simb == "simb_ident")
            return obter_simbolo(tokens[i], i);
        else
            cout << "Erro sintatico na linha: " << tokens[i].linha << endl;
        i = mais_var(tokens, i);
        return i;
    }

    int mais_var(vector<Token> tokens, int i)
    {
        if (tokens[i].simb == "simb_virgula")
        {
            i = obter_simbolo(tokens[i], i);
            i = variaveis(tokens, i);
        }
        return i;
    }

    int lista_par(vector<Token> tokens, int i)
    {
        i = variaveis(tokens, i);
        if (tokens[i].simb == "simb_dp")
            return obter_simbolo(tokens[i], i);
        else
            cout << "Erro sintatico na linha: " << tokens[i].linha << endl;
        i = tipo_var(tokens, i);
        i = mais_var(tokens, i);
        return i;
    }

    int dc_c(vector<Token> tokens, int i)
    {
        if (tokens[i].simb == "simb_const")
        {
            i = obter_simbolo(tokens[i], i);
            if (tokens[i].simb == "simb_ident")
                i = obter_simbolo(tokens[i], i);
            else
                cout << "Erro sintatico na linha: " << tokens[i].linha << endl;
            if (tokens[i].simb == "simb_igual")
                i = obter_simbolo(tokens[i], i);
            else
                cout << "Erro sintatico na linha: " << tokens[i].linha << endl;
            i = numero(tokens, i);
            if (tokens[i].simb == "simb_pv")
                i = obter_simbolo(tokens[i], i);
            else
                cout << "Erro sintatico na linha: " << tokens[i].linha << endl;
        }

        return i;
    }

    int dc_v(vector<Token> tokens, int i)
    {
        if (tokens[i].simb == "simb_var")
        {
            i = obter_simbolo(tokens[i], i);
            i = variaveis(tokens, i);
            if (tokens[i].simb == "simb_dp")
                i = obter_simbolo(tokens[i], i);
            else
                cout << "Erro sintatico na linha: " << tokens[i].linha << endl;
            i = tipo_var(tokens, i);
            if (tokens[i].simb == "simb_pv")
                i = obter_simbolo(tokens[i], i);
            else
                cout << "Erro sintatico na linha: " << tokens[i].linha << endl;
        }
        return i;
    }

    int parametros(vector<Token> tokens, int i)
    {
        if (tokens[i].simb == "simb_abrir_parentese")
        {
            i = obter_simbolo(tokens[i], i);
            i = lista_par(tokens, i);
            if (tokens[i].simb == "simb_fechar_parentese")
                i = obter_simbolo(tokens[i], i);
            else
                cout << "Erro sintatico na linha: " << tokens[i].linha << endl;
        }
        return i;
    }

    int dc_loc(vector<Token> tokens, int i)
    {
        return dc_v(tokens, i);
    }

    int corpo_p(vector<Token> tokens, int i)
    {
        i = dc_loc(tokens, i);
        if (tokens[i].simb == "simb_begin")
            i = obter_simbolo(tokens[i], i);
        else
            cout << "Erro sintatico na linha: " << tokens[i].linha << endl;
        i = comandos(tokens, i);
        if (tokens[i].simb == "simb_end")
            i = obter_simbolo(tokens[i], i);
        else
            cout << "Erro sintatico na linha: " << tokens[i].linha << endl;
        if (tokens[i].simb == "simb_pv")
            i = obter_simbolo(tokens[i], i);
        else
            cout << "Erro sintatico na linha: " << tokens[i].linha << endl;
        return i;
    }

    int dc_p(vector<Token> tokens, int i)
    {
        if (tokens[i].simb == "simb_procedure")
        {
            i = obter_simbolo(tokens[i], i);
            if (tokens[i].simb == "simb_ident")
                i = obter_simbolo(tokens[i], i);
            else
                cout << "Erro sintatico na linha: " << tokens[i].linha << endl;
            i = parametros(tokens, i);
            if (tokens[i].simb == "simb_pv")
                i = obter_simbolo(tokens[i], i);
            else
                cout << "Erro sintatico na linha: " << tokens[i].linha << endl;
            i = corpo_p(tokens, i);
        }
        return i;
    }

    int dc(vector<Token> tokens, int i)
    {
        i = dc_c(tokens, i);
        i = dc_v(tokens, i);
        i = dc_p(tokens, i);
        return i;
    }

    int comandos(vector<Token> tokens, int i)
    {
        i = cmd(tokens, i);
        if (tokens[i].simb == "simb_pv")
            i = obter_simbolo(tokens[i], i);
        else
            cout << "Erro sintatico na linha: " << tokens[i].linha << endl;
        return i;
    }

    int cmd(vector<Token> tokens, int i)
    {
        if (tokens[i].simb == "simb_read" || tokens[i].simb == "simb_write")
        {
            i = obter_simbolo(tokens[i], i);
            if (tokens[i].simb == "simb_abrir_parentese")
                i = obter_simbolo(tokens[i], i);
            else
                cout << "Erro sintatico na linha: " << tokens[i].linha << endl;
            i = variaveis(tokens, i);
            if (tokens[i].simb == "simb_fechar_parentese")
                i = obter_simbolo(tokens[i], i);
            else
                cout << "Erro sintatico na linha: " << tokens[i].linha << endl;
        }
        else if (tokens[i].simb == "simb_while")
        {
            i = obter_simbolo(tokens[i], i);
            if (tokens[i].simb == "simb_abrir_parentese")
                i = obter_simbolo(tokens[i], i);
            else
                cout << "Erro sintatico na linha: " << tokens[i].linha << endl;
            i = condicao(tokens, i);
            if (tokens[i].simb == "simb_fechar_parentese")
                i = obter_simbolo(tokens[i], i);
            else
                cout << "Erro sintatico na linha: " << tokens[i].linha << endl;
            if (tokens[i].simb == "simb_do")
                i = obter_simbolo(tokens[i], i);
            else
                cout << "Erro sintatico na linha: " << tokens[i].linha << endl;
            i = cmd(tokens, i);
        }
        else if (tokens[i].simb == "simb_if")
        {
            i = obter_simbolo(tokens[i], i);
            i = condicao(tokens, i);
            if (tokens[i].simb == "simb_then")
                i = obter_simbolo(tokens[i], i);
            else
                cout << "Erro sintatico na linha: " << tokens[i].linha << endl;
            i = cmd(tokens, i);
            i = pfalsa(tokens, i);
        }
        else if (tokens[i].simb == "simb_ident")
        {
            i = obter_simbolo(tokens[i], i);
            i = cmd_linha(tokens, i);
        }
        else if (tokens[i].simb == "simb_begin")
        {
            i = obter_simbolo(tokens[i], i);
            i = comandos(tokens, i);
            if (tokens[i].simb == "simb_end")
                i = obter_simbolo(tokens[i], i);
            else
                cout << "Erro sintatico na linha: " << tokens[i].linha << endl;
        }
        else
        {
            cout << "Erro sintatico na linha: " << tokens[i].linha << endl;
        }

        return i;
    }

    int pfalsa(vector<Token> tokens, int i)
    {
        if (tokens[i].simb == "else")
        {
            i = obter_simbolo(tokens[i], i);
            i = cmd(tokens, i);
        }
        return i;
    }

    int relacao(vector<Token> tokens, int i)
    {
        if (tokens[i].simb == "simb_igual" || tokens[i].simb == "simb_diff" || tokens[i].simb == "simb_maior_igual" || tokens[i].simb == "simb_menor_igual" || tokens[i].simb == "simb_maior" || tokens[i].simb == "simb_menor")
            i = obter_simbolo(tokens[i], i);
        else
            cout << "Erro sintatico na linha: " << tokens[i].linha << endl;
        return i;
    }

    int expressao(vector<Token> tokens, int i)
    {
        i = termo(tokens, i);
        i = outros_termos(tokens, i);
        return i;
    }

    int op_un(vector<Token> tokens, int i)
    {
        if (tokens[i].simb == "simb_soma" || tokens[i].simb == "simb_sub")
            i = obter_simbolo(tokens[i], i);
        return i;
    }

    int op_ad(vector<Token> tokens, int i)
    {
        if (tokens[i].simb == "simb_soma" || tokens[i].simb == "simb_sub")
            i = obter_simbolo(tokens[i], i);
        else
            cout << "Erro sintatico na linha: " << tokens[i].linha << endl;
        return i;
    }

    int op_mul(vector<Token> tokens, int i)
    {
        if (tokens[i].simb == "simb_mul" || tokens[i].simb == "simb_div")
            i = obter_simbolo(tokens[i], i);
        else
            cout << "Erro sintatico na linha: " << tokens[i].linha << endl;
        return i;
    }

    int termo(vector<Token> tokens, int i)
    {
        i = op_un(tokens, i);
        i = fator(tokens, i);
        i = mais_fatores(tokens, i);
        return i;
    }

    int fator(vector<Token> tokens, int i)
    {
        if (tokens[i].simb == "simb_ident")
        {
            i = obter_simbolo(tokens[i], i);
        }
        else if (tokens[i].simb == "simb_abrir_parentese")
        {
            i = obter_simbolo(tokens[i], i);
            i = expressao(tokens, i);
            if (tokens[i].simb == "simb_fechar_parentese")
                i = obter_simbolo(tokens[i], i);
            else
                cout << "Erro sintatico na linha: " << tokens[i].linha << endl;
        }
        else
        {
            i = numero(tokens, i);
        }
        return i;
    }

    int mais_fatores(vector<Token> tokens, int i)
    {
        if (tokens[i].simb == "simb_mul" || tokens[i].simb == "simb_div")
        {
            i = obter_simbolo(tokens[i], i);
            i = fator(tokens, i);
        }
        return i;
    }

    int outros_termos(vector<Token> tokens, int i)
    {
        if (tokens[i].simb == "simb_soma" || tokens[i].simb == "simb_sub")
        {
            i = obter_simbolo(tokens[i], i);
            i = termo(tokens, i);
        }
        return i;
    }

    int condicao(vector<Token> tokens, int i)
    {
        i = expressao(tokens, i);
        i = relacao(tokens, i);
        i = expressao(tokens, i);
        return i;
    }

    int cmd_linha(vector<Token> tokens, int i)
    {
        if (tokens[i].simb == "simb_atrib")
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

    int argumentos(vector<Token> tokens, int i)
    {
        if (tokens[i].simb == "simb_ident")
            i = obter_simbolo(tokens[i], i);
        else
            cout << "Erro sintatico na linha: " << tokens[i].linha << endl;
        i = mais_ident(tokens, i);
        return i;
    }

    int mais_ident(vector<Token> tokens, int i)
    {
        if (tokens[i].simb == "simb_pv")
        {
            i = obter_simbolo(tokens[i], i);
            i = argumentos(tokens, i);
        }
        return i;
    }

    int lista_arg(vector<Token> tokens, int i)
    {
        if (tokens[i].simb == "simb_abrir_parentese")
        {
            i = obter_simbolo(tokens[i], i);
            i = argumentos(tokens, i);
            if (tokens[i].simb == "simb_fechar_parentese")
                i = obter_simbolo(tokens[i], i);
            else
                cout << "Erro sintatico na linha: " << tokens[i].linha << endl;
        }
        return i;
    }

    int corpo(vector<Token> tokens, int i)
    {
        i = dc(tokens, i);
        if (tokens[i].simb == "simb_begin")
            i = obter_simbolo(tokens[i], i);
        else
            cout << "Erro sintatico na linha: " << tokens[i].linha << endl;
        i = comandos(tokens, i);
        if (tokens[i].simb == "simb_end")
            i = obter_simbolo(tokens[i], i);
        else
            cout << "Erro sintatico na linha: " << tokens[i].linha << endl;
        return i;
    }

    bool programa(vector<Token> tokens, int i)
    {
        if (tokens[i].simb == "simb_program")
            i = obter_simbolo(tokens[i], i);
        else
            cout << "Erro sintatico na linha: " << tokens[i].linha << endl;
        if (tokens[i].simb == "simb_ident")
            i = obter_simbolo(tokens[i], i);
        else
            cout << "Erro sintatico na linha: " << tokens[i].linha << endl;
        if (tokens[i].simb == "simb_pv")
            i = obter_simbolo(tokens[i], i);
        else
            cout << "Erro sintatico na linha: " << tokens[i].linha << endl;
        i = corpo(tokens, i);
        if (tokens[i].simb == "simb_dot")
            i = obter_simbolo(tokens[i], i);
        else
            cout << "Erro sintatico na linha: " << tokens[i].linha << endl;
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
            cout << tokens[i].simb << endl;
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

    //vector<Token> tokens{"simb_program", "simb_ident", "simb_pv", "simb_var", "simb_ident", "simb_dp", "simb_tipo_int", "simb_pv", "simb_begin", "simb_ident", "simb_atrib", "simb_tipo_int", "simb_pv", "simb_while", "simb_abrir_parentese", "simb_ident", "simb_menor", "simb_tipo_int", "simb_fechar_parentese", "simb_do", "simb_ident", "simb_atrib", "simb_tipo_int", "simb_pv", "simb_ident", "simb_while", "simb_abrir_parentese", "simb_ident", "simb_menor", "simb_tipo_int", "simb_fechar_parentese", "simb_do", "simb_ident", "simb_atrib", "simb_ident", "simb_soma", "simb_tipo_int", "simb_pv", "simb_end", "simb_dot"};

    sin_an SinAn(LexAn.tokens);
    //SinAn.imprimeTokens();
    SinAn.processaTokens();
    return 0;
}
