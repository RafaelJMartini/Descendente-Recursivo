#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TKId 1
#define TKVoid 2
#define TKInt 3
#define TKFloat 4
#define TKVirgula 5
#define TKDoisPontos 6
#define TKAbrePar 7
#define TKFechaPar 8
#define TKAtrib 9
#define TKPontoEVirgula 10
#define TKAbreChaves 11
#define TKFechaChaves 12
#define TKMais 13
#define TKDuploMais 14
#define TKProd 15
#define TKChar 16
#define TKSub 17
#define TKAbreColchete 18
#define TKFechaColchete 19
#define TKAtribMais 20
#define TKDouble 21
#define TKCteInt 22
#define TKElse 23
#define TKIf 24
#define TKString 25
#define TKFimArquivo 26
#define TKPonto 27
#define TKNumDouble 28
#define TKNegate 29
#define TKUnequal 30
#define TKNumFloat 31
#define TKEComercial 32
#define TKAND 33
#define TKEIgual 34
#define TKMaior 35
#define TKShiftR 36
#define TKShiftL 37
#define TKMaiorIgual 38
#define TKShiftRIgual 39
#define TKMenorIgual 40
#define TKMenor 41
#define TKShiftLIgual 42
#define TKOR 43
#define TKORIgual 44
#define TKPipe 45
#define TKTypedef 46
#define TKStruct 47
#define TKTrue 48
#define TKFalse 49
#define TKSizeof 50
#define TKDuploMenos 51
#define TKArrow 52
#define TKDiferenca 53       // '%' (modulo)
#define TKDiferencaIgual 54  // '%='
#define TKDivisao 55
#define TKXOR 56
#define TKComplemento1 57
#define TKXORIgual 58
#define TKTernario 59
#define TKReturn 60
#define TKFor 61
#define TKWhile 62
#define TKDo 63
#define TKSwitch 64
#define TKCase 65
#define TKBreak 66
#define TKDefault 67
#define TKUnion 68
#define TKEnum 69
#define TKGoto 70
#define TKShort 71
#define TKLong 72
#define TKUnsigned 73
#define TKSigned 74
#define TKCompara 75  // '=='

typedef struct
{
    int tipo;
    char nome[50];
    char lexema[50];
    int linha;
    int coluna;

} Token;

Token lookahead;
FILE *arquivo;

// Protótipos de todas as funções
void Programa();
void ListaDeclaracoes();
void Declaracao();
void DeclaracaoFuncao();
void DeclaracaoVariavel();
void DeclaracaoVariavelLocal();
void InicializacaoOpt();
void Declarador();
void PointerOpt();
void ParametrosOpt();
void Parametros();
void Parametro();
void ArrayOpt();
void ArrayParametroOpt();
void Tipo();
void TipoBase();
void Bloco();
void ListaComandos();
void Comando();
void If();
void While();
void DoWhile();
void For();
void ExpressaoOpt();
void Expressao();
void ExpressaoLinha();
void Atribuicao();
void AtribuicaoLinha();
void LogicoOU();
void LogicoOULinha();
void LogicoE();
void LogicoELinha();
void BitOr();
void BitOrLinha();
void BitXor();
void BitXorLinha();
void BitAnd();
void BitAndLinha();
void Igualdade();
void IgualdadeLinha();
void Relacional();
void RelacionalLinha();
void Shift();
void ShiftLinha();
void Aditiva();
void AditivaLinha();
void Multiplicativa();
void MultiplicativaLinha();
void Unaria();
void Primaria();
void Sufixo();
void ArrayAcesso();
void ArgumentosOpt();
void Argumentos();
void ArgumentosLinha();

// -----------------------------------------------------------------------

Token nextToken()
{
    Token t;

    if (fread(&t, sizeof(Token), 1, arquivo) != 1)
    {
        t.tipo = TKFimArquivo;
        strcpy(t.nome, "TKFimArquivo");
        strcpy(t.lexema, "EOF");
        t.linha = -1;
        t.coluna = -1;
    }

    return t;
}

void erro(char *esperado)
{
    printf("\n==============================\n");
    printf("ERRO SINTATICO\n");
    printf("==============================\n");

    printf("Linha  : %d\n", lookahead.linha);
    printf("Coluna : %d\n\n", lookahead.coluna);

    printf("Esperado : %s\n", esperado);
    printf("Encontrado: %s (%d) | lexema: '%s'\n",
           lookahead.nome,
           lookahead.tipo,
           lookahead.lexema);

    printf("==============================\n");
    exit(1);
}

void match(int esperado)
{
    if (lookahead.tipo == esperado)
    {
        lookahead = nextToken();
        return;
    }
    printf("\n==============================\n");
    printf("ERRO SINTATICO\n");
    printf("==============================\n");

    printf("Linha  : %d\n", lookahead.linha);
    printf("Coluna : %d\n\n", lookahead.coluna);

    printf("Esperado: token (%d)\n", esperado);
    printf("Encontrado: %s (%d) | lexema: '%s'\n",
           lookahead.nome,
           lookahead.tipo,
           lookahead.lexema);

    printf("==============================\n");
    exit(1);
}

// Verifica se o token atual pode iniciar um Tipo
int ehTipo()
{
    return lookahead.tipo == TKVoid ||
           lookahead.tipo == TKInt    ||
           lookahead.tipo == TKLong   ||
           lookahead.tipo == TKUnsigned ||
           lookahead.tipo == TKFloat  ||
           lookahead.tipo == TKDouble ||
           lookahead.tipo == TKChar;
}

// Verifica se o token atual pode iniciar uma Expressao
int ehInicioExpressao()
{
    return lookahead.tipo == TKId         ||
           lookahead.tipo == TKCteInt     ||
           lookahead.tipo == TKNumFloat   ||
           lookahead.tipo == TKNumDouble  ||
           lookahead.tipo == TKString     ||
           lookahead.tipo == TKAbrePar    ||
           lookahead.tipo == TKSub        ||   // -expr
           lookahead.tipo == TKNegate     ||   // !expr
           lookahead.tipo == TKComplemento1 || // ~expr
           lookahead.tipo == TKDuploMais  ||   // ++expr
           lookahead.tipo == TKDuploMenos ||   // --expr
           lookahead.tipo == TKEComercial ||   // &var
           lookahead.tipo == TKProd;           // *ptr
}

// -----------------------------------------------------------------------
//  Produções
// -----------------------------------------------------------------------

void Programa()
{
    ListaDeclaracoes();
}

void ListaDeclaracoes()
{
    while (ehTipo())
    {
        Declaracao();
    }
}

// Declaração no escopo global: variável ou função
void Declaracao()
{
    Tipo();
    PointerOpt();
    match(TKId);

    if (lookahead.tipo == TKAbrePar)
        DeclaracaoFuncao();
    else
        DeclaracaoVariavel();
}

void DeclaracaoFuncao()
{
    match(TKAbrePar);
    ParametrosOpt();
    match(TKFechaPar);
    Bloco();
}

// Continuação de declaração de variável GLOBAL
// (Tipo, PointerOpt e id já foram consumidos em Declaracao)
void DeclaracaoVariavel()
{
    ArrayOpt();
    InicializacaoOpt(); // FIX: primeiro declarador também pode ter inicialização

    while (lookahead.tipo == TKVirgula)
    {
        match(TKVirgula);
        Declarador();
    }

    match(TKPontoEVirgula);
}

// Declaração de variável LOCAL (dentro de bloco)
// Consome Tipo, PointerOpt, id e depois o restante
void DeclaracaoVariavelLocal()
{
    Tipo();       // FIX: consumir Tipo dentro do bloco
    PointerOpt(); // FIX: consumir ponteiros opcionais
    match(TKId);  // FIX: consumir o identificador

    ArrayOpt();
    InicializacaoOpt();

    while (lookahead.tipo == TKVirgula)
    {
        match(TKVirgula);
        Declarador();
    }

    match(TKPontoEVirgula);
}

// FIX: aceita tanto "= expr" simples quanto "= { expr, ... }" para arrays
void InicializacaoOpt()
{
    if (lookahead.tipo == TKAtrib)
    {
        match(TKAtrib);

        if (lookahead.tipo == TKAbreChaves)
        {
            // inicialização de array/struct: = { v1, v2, ... }
            match(TKAbreChaves);

            if (lookahead.tipo != TKFechaChaves)
            {
                Expressao();
                while (lookahead.tipo == TKVirgula)
                {
                    match(TKVirgula);
                    Expressao();
                }
            }

            match(TKFechaChaves);
        }
        else
        {
            // inicialização simples: = expr
            Expressao();
        }
    }
}

void Declarador()
{
    PointerOpt();
    match(TKId);
    ArrayOpt();
    InicializacaoOpt();
}

void PointerOpt()
{
    while (lookahead.tipo == TKProd)
    {
        match(TKProd);
    }
}

void ParametrosOpt()
{
    if (ehTipo())
    {
        Parametros();
    }
}

void Parametros()
{
    Parametro();

    while (lookahead.tipo == TKVirgula)
    {
        match(TKVirgula);
        Parametro();
    }
}

void Parametro()
{
    Tipo();
    PointerOpt();
    match(TKId);
    ArrayParametroOpt();
}

void ArrayOpt()
{
    while (lookahead.tipo == TKAbreColchete)
    {
        match(TKAbreColchete);

        if (lookahead.tipo != TKFechaColchete)
            Expressao();

        match(TKFechaColchete);
    }
}

void ArrayParametroOpt()
{
    if (lookahead.tipo == TKAbreColchete)
    {
        match(TKAbreColchete);
        match(TKFechaColchete);
    }
}

void Tipo()
{
    if (lookahead.tipo == TKUnsigned)
        match(TKUnsigned);

    TipoBase();
}

void TipoBase()
{
    switch (lookahead.tipo)
    {
        case TKVoid:
            match(TKVoid);
            break;

        case TKInt:
            match(TKInt);
            break;

        case TKLong:
            match(TKLong);

            if (lookahead.tipo == TKLong)
                match(TKLong);

            if (lookahead.tipo == TKInt)
                match(TKInt);

            break;

        case TKFloat:
            match(TKFloat);
            break;

        case TKDouble:
            match(TKDouble);
            break;

        case TKChar:
            match(TKChar);
            break;

        default:
            erro("tipo (int, long, float, double, char)");
    }
}

void Bloco()
{
    match(TKAbreChaves);
    ListaComandos();
    match(TKFechaChaves);
}

void ListaComandos()
{
    // FIX: expandido com TKReturn, TKBreak, TKString, TKDuploMais, TKDuploMenos, TKEComercial, TKProd
    while (lookahead.tipo == TKIf       ||
           lookahead.tipo == TKWhile    ||
           lookahead.tipo == TKDo       ||
           lookahead.tipo == TKFor      ||
           lookahead.tipo == TKReturn   ||
           lookahead.tipo == TKBreak    ||
           lookahead.tipo == TKAbreChaves ||

           ehTipo()                     ||

           lookahead.tipo == TKId           ||
           lookahead.tipo == TKCteInt       ||
           lookahead.tipo == TKNumFloat     ||
           lookahead.tipo == TKNumDouble    ||
           lookahead.tipo == TKString       ||
           lookahead.tipo == TKAbrePar      ||
           lookahead.tipo == TKSub          ||
           lookahead.tipo == TKNegate       ||
           lookahead.tipo == TKComplemento1 ||
           lookahead.tipo == TKDuploMais    ||
           lookahead.tipo == TKDuploMenos   ||
           lookahead.tipo == TKEComercial   ||
           lookahead.tipo == TKProd)
    {
        Comando();
    }
}

void Comando()
{
    switch (lookahead.tipo)
    {
        case TKIf:
            If();
            break;

        case TKWhile:
            While();
            break;

        case TKDo:
            DoWhile();
            break;

        case TKFor:
            For();
            break;

        case TKAbreChaves:
            Bloco();
            break;

        // FIX: return e break tratados explicitamente
        case TKReturn:
            match(TKReturn);
            if (lookahead.tipo != TKPontoEVirgula)
                Expressao();
            match(TKPontoEVirgula);
            break;

        case TKBreak:
            match(TKBreak);
            match(TKPontoEVirgula);
            break;

        // FIX: declarações locais passam por DeclaracaoVariavelLocal()
        case TKInt:
        case TKLong:
        case TKUnsigned:
        case TKFloat:
        case TKDouble:
        case TKChar:
            DeclaracaoVariavelLocal();
            break;

        default:
            // expressão seguida de ';'
            Expressao();
            match(TKPontoEVirgula);
            break;
    }
}

void If()
{
    match(TKIf);
    match(TKAbrePar);
    Expressao();
    match(TKFechaPar);

    Comando();

    if (lookahead.tipo == TKElse)
    {
        match(TKElse);
        Comando();
    }
}

void While()
{
    match(TKWhile);
    match(TKAbrePar);
    Expressao();
    match(TKFechaPar);
    Comando();
}

void DoWhile()
{
    match(TKDo);
    Comando();
    match(TKWhile);
    match(TKAbrePar);
    Expressao();
    match(TKFechaPar);
    match(TKPontoEVirgula);
}

void ExpressaoOpt()
{
    if (lookahead.tipo != TKPontoEVirgula &&
        lookahead.tipo != TKFechaPar)
        Expressao();
}

void For()
{
    match(TKFor);
    match(TKAbrePar);

    // Inicialização: pode ser declaração ou expressão
    if (ehTipo())
    {
        DeclaracaoVariavelLocal();  // já consome o ';'
    }
    else
    {
        ExpressaoOpt();
        match(TKPontoEVirgula);
    }

    ExpressaoOpt();
    match(TKPontoEVirgula);

    ExpressaoOpt();

    match(TKFechaPar);
    Comando();
}

// -----------------------------------------------------------------------
//  Expressões
// -----------------------------------------------------------------------

void Expressao()
{
    Atribuicao();
    ExpressaoLinha();
}

void ExpressaoLinha()
{
    while (lookahead.tipo == TKVirgula)
    {
        match(TKVirgula);
        Atribuicao();
    }
}

void Atribuicao()
{
    LogicoOU();
    AtribuicaoLinha();
}

void AtribuicaoLinha()
{
    switch (lookahead.tipo)
    {
        case TKAtrib:          // =
        case TKAtribMais:      // +=
        case TKDiferencaIgual: // %=
        case TKShiftRIgual:    // >>=
        case TKShiftLIgual:    // <<=
        case TKORIgual:        // |=   FIX: adicionados
        case TKEIgual:         // &=   FIX
        case TKXORIgual:       // ^=   FIX
            match(lookahead.tipo);
            Atribuicao();
            break;

        default:
            break; // ε
    }
}

void LogicoOU()
{
    LogicoE();
    LogicoOULinha();
}

void LogicoOULinha()
{
    while (lookahead.tipo == TKOR)
    {
        match(TKOR);
        LogicoE();
    }
}

void LogicoE()
{
    BitOr();
    LogicoELinha();
}

void LogicoELinha()
{
    while (lookahead.tipo == TKAND)
    {
        match(TKAND);
        BitOr();
    }
}

void BitOr()
{
    BitXor();
    BitOrLinha();
}

void BitOrLinha()
{
    while (lookahead.tipo == TKPipe)
    {
        match(TKPipe);
        BitXor();
    }
}

void BitXor()
{
    BitAnd();
    BitXorLinha();
}

void BitXorLinha()
{
    while (lookahead.tipo == TKXOR)
    {
        match(TKXOR);
        BitAnd();
    }
}

void BitAnd()
{
    Igualdade();
    BitAndLinha();
}

void BitAndLinha()
{
    while (lookahead.tipo == TKEComercial)
    {
        match(TKEComercial);
        Igualdade();
    }
}

void Igualdade()
{
    Relacional();
    IgualdadeLinha();
}

// FIX: era TKDiferenca (%), agora usa TKUnequal (!=) e TKCompara (==)
void IgualdadeLinha()
{
    while (lookahead.tipo == TKCompara ||
           lookahead.tipo == TKUnequal)
    {
        match(lookahead.tipo);
        Relacional();
    }
}

void Relacional()
{
    Shift();
    RelacionalLinha();
}

void RelacionalLinha()
{
    while (lookahead.tipo == TKMenor     ||
           lookahead.tipo == TKMaior     ||
           lookahead.tipo == TKMenorIgual ||
           lookahead.tipo == TKMaiorIgual)
    {
        match(lookahead.tipo);
        Shift();
    }
}

void Shift()
{
    Aditiva();
    ShiftLinha();
}

void ShiftLinha()
{
    while (lookahead.tipo == TKShiftL ||
           lookahead.tipo == TKShiftR)
    {
        match(lookahead.tipo);
        Aditiva();
    }
}

void Aditiva()
{
    Multiplicativa();
    AditivaLinha();
}

void AditivaLinha()
{
    while (lookahead.tipo == TKMais ||
           lookahead.tipo == TKSub)
    {
        match(lookahead.tipo);
        Multiplicativa();
    }
}

void Multiplicativa()
{
    Unaria();
    MultiplicativaLinha();
}

// FIX: era TKDiferenca no lugar de TKDivisao/módulo — agora correto
void MultiplicativaLinha()
{
    while (lookahead.tipo == TKProd    ||  // *
           lookahead.tipo == TKDivisao ||  // /
           lookahead.tipo == TKDiferenca)  // % (modulo)
    {
        match(lookahead.tipo);
        Unaria();
    }
}

// FIX: adicionados pré-incremento (++x) e pré-decremento (--x)
void Unaria()
{
    switch (lookahead.tipo)
    {
        case TKSub:
            match(TKSub);
            Unaria();
            break;

        case TKNegate:
            match(TKNegate);
            Unaria();
            break;

        case TKComplemento1:
            match(TKComplemento1);
            Unaria();
            break;

        case TKDuploMais:
            match(TKDuploMais);
            Unaria();
            break;

        case TKDuploMenos:
            match(TKDuploMenos);
            Unaria();
            break;

        case TKEComercial:  // endereço: &var
            match(TKEComercial);
            Unaria();
            break;

        case TKProd:        // deref: *ptr
            match(TKProd);
            Unaria();
            break;

        default:
            Primaria();
            break;
    }
}

// FIX: adicionado TKString como literal primário
void Primaria()
{
    if (lookahead.tipo == TKId)
    {
        match(TKId);
        Sufixo();
    }
    else if (lookahead.tipo == TKCteInt  ||
             lookahead.tipo == TKNumFloat ||
             lookahead.tipo == TKNumDouble)
    {
        match(lookahead.tipo);
    }
    else if (lookahead.tipo == TKString) // FIX: literal string (ex.: printf("..."))
    {
        match(TKString);
    }
    else if (lookahead.tipo == TKAbrePar)
    {
        match(TKAbrePar);
        Expressao();
        match(TKFechaPar);
    }
    else
    {
        erro("expressao primaria (id, numero, string ou '(')");
    }
}

void Sufixo()
{
    if (lookahead.tipo == TKAbrePar)
    {
        // chamada de função: id(args)
        match(TKAbrePar);
        ArgumentosOpt();
        match(TKFechaPar);
        // FIX: após chamada ainda pode haver sufixo (ex.: f()[], f()())
        Sufixo();
    }
    else if (lookahead.tipo == TKAbreColchete)
    {
        ArrayAcesso();
    }
    else if (lookahead.tipo == TKPonto)
    {
        // acesso a membro: obj.campo
        match(TKPonto);
        match(TKId);
        Sufixo();
    }
    else if (lookahead.tipo == TKArrow)
    {
        // acesso via ponteiro: ptr->campo
        match(TKArrow);
        match(TKId);
        Sufixo();
    }
    else if (lookahead.tipo == TKDuploMais)
    {
        // pós-incremento: x++
        match(TKDuploMais);
    }
    else if (lookahead.tipo == TKDuploMenos)
    {
        // pós-decremento: x--
        match(TKDuploMenos);
    }
    // ε — sem sufixo
}

void ArrayAcesso()
{
    while (lookahead.tipo == TKAbreColchete)
    {
        match(TKAbreColchete);
        Expressao();
        match(TKFechaColchete);
    }
    Sufixo();
}

void ArgumentosOpt()
{
    if (lookahead.tipo != TKFechaPar)
    {
        Argumentos();
    }
}

void Argumentos()
{
    Expressao();
    ArgumentosLinha();
}

void ArgumentosLinha()
{
    while (lookahead.tipo == TKVirgula)
    {
        match(TKVirgula);
        Expressao();
    }
}

// -----------------------------------------------------------------------

int main()
{
    arquivo = fopen("../tokens.dat", "rb");

    if (arquivo == NULL)
    {
        printf("Erro ao abrir arquivo tokens.dat\n");
        return 1;
    }

    lookahead = nextToken();

    Programa();

    if (lookahead.tipo != TKFimArquivo)
        erro("fim de arquivo");

    printf("Programa correto!\n");

    fclose(arquivo);
    return 0;
}
