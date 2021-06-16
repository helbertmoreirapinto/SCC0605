import sys

reserved_words = {
    'program': 'simb_program',
    'begin': 'simb_begin',
    'end': 'simb_end',
    'const': 'simb_const',
    'var': 'simb_var',
    'real': 'simb_real',
    'integer': 'simb_integer',
    'procedure': 'simb_procedure',
    'else': 'simb_else',
    'read': 'simb_read',
    'write': 'simb_write',
    'while': 'simb_while',
    'do': 'simb_do',
    'if': 'simb_if',
    'then': 'simb_then',
    'for': 'simb_for',
    'to': 'simb_to'
}

simbols = {'-': "simb_menos", '+': "simb_mais", '/': "simb_div", '*': "simb_mult", ',': "simb_virg",
           '.': "simb_point", '=': 'simb_igual', ';': 'simb_pv', '(': 'simb_apar', ')': 'simb_fpar'}


def main():

    try:
        file_name = sys.argv[1]
        file = open(file_name, "r")
    except IndexError:
        print("Please enter a file name.")
        exit()
    except FileNotFoundError:
        print("File not found.")
        exit()

    content = file.read()
    file.close()

    while(i < len(content)):
        print(lexicalAutomate(content))
    return


i = 0


def lexicalAutomate(stream):
    global i
    while(i < len(stream) and (stream[i] == ' ' or stream[i] == '\t' or stream[i] == '\n')):
        i += 1
    if(i == len(stream)):
        return

    if(isNumber(stream[i])):
        return numberAutomate(stream)
    if(isLetra(stream[i])):
        return cadeiaAutomate(stream)
    if(stream[i] == '>'):
        return greaterAutomate(stream)
    if(stream[i] == '<'):
        return lesserAutomate(stream)
    if(stream[i] == '{'):
        return commentAutomate(stream)
    if(stream[i] == ':'):
        return colonAutomate(stream)
    if(isSimbol(stream[i])):
        i += 1
        return isSimbol(stream[i - 1])
    i += 1
    return (stream[i-1], 'Caractere nao permitido')


def numberAutomate(stream):
    global i
    init = i
    while(isNumber(stream[i])):
        i += 1

    if(stream[i] == '.'):
        while(isNumber(stream[i])):
            i += 1
        return (stream[init:i], "real_number")  # PCL
    else:
        return (stream[init:i], "integer_number")


def cadeiaAutomate(stream):
    global i
    init = i
    while(isLetra(stream[i]) or isNumber(stream[i])):
        i += 1
    return checkCadeia(stream[init:i])


def checkCadeia(cadeia):
    global i
    if cadeia in reserved_words:
        return (cadeia, reserved_words[cadeia])  # PCL
    return (cadeia, "id")


def isNumber(char):
    return char >= '0' and char <= '9'


def isLetra(char):
    return char >= 'A' and char <= 'z'


def isSimbol(char):
    keys = simbols.keys()
    if char in keys:
        return (char, simbols[char])
    else:
        return False


def lesserAutomate(stream):
    global i
    i += 1
    if stream[i] == '=':
        i += 1
        return ('<=', 'simb_menor_igual')
    elif stream[i] == '>':
        i += 1
        return ('<>', 'simb_diff')
    else:
        return ('<', 'simb_menor')


def greaterAutomate(stream):
    global i
    i += 1
    if stream[i] == '=':
        i += 1
        return ('>=', 'simb_maior_igual')
    else:
        return ('>', 'simb_maior')


def commentAutomate(stream):
    global i
    init = i
    i += 1
    while(stream[i] != '\n' and stream[i] != '}'):
        i += 1
    if(stream[i] == '}'):
        i += 1
        return (stream[init:i], 'comentario')
    else:
        return (stream[init:i-1] + '\\n', 'Comentario mal formado')


def colonAutomate(stream):
    global i
    i += 1
    if stream[i] == '=':
        i += 1
        return (':=', 'simb_atrib')
    else:
        return (':', 'simb_dp')


if __name__ == '__main__':
    main()
