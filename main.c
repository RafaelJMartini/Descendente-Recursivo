Programa
    -> ListaDeclaracoes

ListaDeclaracoes
    -> Declaracao ListaDeclaracoes
    | ε

Declaracao
    -> DeclaracaoVariavel
    | DeclaracaoFuncao

Tipo
    -> UnsignedOpt TipoBase

UnsignedOpt
    -> unsigned
    | ε

TipoBase
    -> int
    | long LongOpt IntOpt
    | float
    | double
    | char

LongOpt
    -> long
    | ε

IntOpt
    -> int
    | ε

PointerOpt
    -> * PointerOpt
    | ε

DeclaracaoVariavel
    -> Tipo ListaDeclaradores ;

ListaDeclaradores
    -> Declarador ListaDeclaradores'

ListaDeclaradores'
    -> , Declarador ListaDeclaradores'
    | ε

Declarador
    -> PointerOpt id ArrayOpt

ArrayOpt
    -> [ Numero ] ArrayOpt
    | ε

DeclaracaoFuncao
    -> Tipo PointerOpt id ( ParametrosOpt ) Bloco

ParametrosOpt
    -> Parametros
    | ε

Parametros
    -> Parametro Parametros'

Parametros'
    -> , Parametro Parametros'
    | ε

Parametro
    -> Tipo PointerOpt id ArrayParametroOpt

ArrayParametroOpt
    -> []
    | ε

Bloco
    -> { ListaComandos }

ListaComandos
    -> Comando ListaComandos
    | ε

Comando
    -> If
    | While
    | DoWhile
    | For
    | Bloco
    | DeclaracaoVariavel
    | Expressao ;

If
    -> if ( Expressao ) Comando ElseOpt

ElseOpt
    -> else Comando
    | ε

While
    -> while ( Expressao ) Comando

DoWhile
    -> do Comando while ( Expressao ) ;

For
    -> for ( ExpressaoOpt ; ExpressaoOpt ; ExpressaoOpt ) Comando

ExpressaoOpt
    -> Expressao
    | ε

Expressao
    -> Atribuicao Expressao'

Expressao'
    -> , Atribuicao Expressao'
    | ε

Atribuicao
    -> LogicoOU Atribuicao'

Atribuicao'
    -> OperadorAtribuicao Atribuicao
    | ε

OperadorAtribuicao
    -> =
    | +=
    | -=
    | *=
    | /=
    | %=
    | >>=
    | <<=

LogicoOU
    -> LogicoE LogicoOU'

LogicoOU'
    -> || LogicoE LogicoOU'
    | ε

LogicoE
    -> BitOr LogicoE'

LogicoE'
    -> && BitOr LogicoE'
    | ε

BitOr
    -> BitXor BitOr'

BitOr'
    -> | BitXor BitOr'
    | ε

BitXor
    -> BitAnd BitXor'

BitXor'
    -> ^ BitAnd BitXor'
    | ε

BitAnd
    -> Igualdade BitAnd'

BitAnd'
    -> & Igualdade BitAnd'
    | ε

Igualdade
    -> Relacional Igualdade'

Igualdade'
    -> == Relacional Igualdade'
    | != Relacional Igualdade'
    | ε

Relacional
    -> Shift Relacional'

Relacional'
    -> < Shift Relacional'
    | > Shift Relacional'
    | <= Shift Relacional'
    | >= Shift Relacional'
    | ε

Shift
    -> Aditiva Shift'

Shift'
    -> << Aditiva Shift'
    | >> Aditiva Shift'
    | ε

Aditiva
    -> Multiplicativa Aditiva'

Aditiva'
    -> + Multiplicativa Aditiva'
    | - Multiplicativa Aditiva'
    | ε

Multiplicativa
    -> Unaria Multiplicativa'

Multiplicativa'
    -> * Unaria Multiplicativa'
    | / Unaria Multiplicativa'
    | % Unaria Multiplicativa'
    | ε

Unaria
    -> - Unaria
    | ! Unaria
    | ~ Unaria
    | Primaria

Primaria
    -> id Sufixo
    | Numero
    | ( Expressao )

Sufixo
    -> ( ArgumentosOpt )
    | ArrayAcesso
    | ε

ArrayAcesso
    -> [ Expressao ] ArrayAcesso
    | ε

ArgumentosOpt
    -> Argumentos
    | ε

Argumentos
    -> Expressao Argumentos'

Argumentos'
    -> , Expressao Argumentos'
    | ε

Numero
    -> Digito NumeroResto

NumeroResto
    -> Digito NumeroResto
    | ε

Digito
    -> 0
    | 1
    | 2
    | 3
    | 4
    | 5
    | 6
    | 7
    | 8
    | 9
