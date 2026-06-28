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
#define TKDiferenca 53
#define TKDiferencaIgual 54
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
#define TKCompara 75

#define false 0
#define true 1

#include <string.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>

typedef struct
{
    int tipo;
    char nome[50];
    char lexema[50];
    int linha;
    int coluna;

} Token;

char nome[100][20]={"","TKId","TKvoid","TKInt","TKFloat",
"TKVirgula","TKDoisPontos","TKAbrePar"
,"TKFechaPar","TKAtrib","TKPontoEVirgula"
,"TKAbreChaves","TKFechaChaves"
,"TKMais","TKDuploMais","TKProd","TKChar"
,"TKSub","TKAbreColchete","TKFechaColchete"
,"TKAtribMais","TKDouble","TKCteInt","TKElse"
,"TKIf","TKString","TKFimArquivo","TKPonto"
,"TKNumDouble","TKNegate", "TKUnequal", "TKNumFloat"
,"TKEComercial","TKAND","TKEIgual","TKMaior"
,"TKShiftR","TKShiftL","TKMaiorIgual","TKShiftRIgual"
,"TKMenor","TKMenorIgual","TKShiftLIgual"
,"TKOR","TKORIgual","TKPipe","TKTypedef","TKStruct"
,"TKTrue","TKFalse","TKSizeof","TKDuploMenos","TKArrow"
,"TKDiferenca","TKDiferencaIgual","TKDivisao"
,"TKXOR","TKComplemento1","TKXORIgual","TKTernario"
,"TKReturn","TKFor","TKWhile","TKDo","TKSwitch","TKCase","TKBreak","TKDefault","TKUnion","TKEnum","TKGoto"
,"TKShort", "TKLong", "TKUnsigned","TKSigned","TKCompara"};

int pos = 0;

int tk;
char lex[20];
int lin=1;
int col=1;
FILE *arqin;
char c; // último caracter lido do arquivo

struct pal_res{char palavra[20]; int tk;};
struct pal_res lista_pal[]={{"void",TKVoid},
                  {"int",TKInt},
                  {"float",TKFloat},
                  {"char",TKChar},
                  {"double",TKDouble},
                  {"else",TKElse},
                  {"if",TKIf},
                  {"struct",TKStruct},
                  {"typedef",TKTypedef},
                  {"true",TKTrue},
                  {"false",TKFalse},
                  {"sizeof",TKSizeof},
                  {"return",TKReturn},
                  {"for",TKFor},
                  {"while",TKWhile},
                  {"do",TKDo},
                  {"switch",TKSwitch},
                  {"case",TKCase},
                  {"break",TKBreak},
                  {"default",TKDefault},
                  {"union",TKUnion},
                  {"enum",TKEnum},
                  {"goto",TKGoto},
                  {"signed",TKSigned},
                  {"unsigned",TKUnsigned},
                  {"short",TKShort},
                  {"long",TKLong},
                  {"fimtabela",TKId}
                  };

int palavra_reservada(char lex[])
{
int postab=0;
while (strcmp("fimtabela",lista_pal[postab].palavra)!=0)
   {
   if (strcmp(lex,lista_pal[postab].palavra)==0)
      return lista_pal[postab].tk;
   postab++;
   }
return TKId;
}


void getToken(); // protótipos
void proxC();

// variáveis globais para retrocesso

typedef struct contexto{long posglobal;
               int tkant;
               char cant;
               char lexant[20];} tcontexto;

tcontexto pilhacon[1000];
int topcontexto=0;

void marcaPosToken() {
pilhacon[topcontexto].posglobal=ftell(arqin);
pilhacon[topcontexto].tkant=tk;
pilhacon[topcontexto].cant=c;
    strcpy(pilhacon[topcontexto].lexant,lex);
    topcontexto++;
}

//Implemente aqui a sua funcao restauraPosToken()

void restauraPosToken() {
    topcontexto--;
fseek(arqin,pilhacon[topcontexto].posglobal,SEEK_SET);
    c=pilhacon[topcontexto].cant;
tk=pilhacon[topcontexto].tkant;
    strcpy(lex,pilhacon[topcontexto].lexant);
}

void proxC()
{
if (feof(arqin)) {
   c=-1;
//   printf("Chegou no fim de arquivo\n");
   return;
   }
fread(&c,1,1,arqin);
if (c=='\n') {
    lin++;
    col = 1;
}
else
{
    col++;
}
}

void getToken()
{
int estado=0,
    fim=0,
    posl=0;
while (!fim)
   {
/*printf("%s\n",exp1);
printf("char=%c pos=%d\n",c,pos);*/
   lex[posl++]=c;
   switch(estado){
      case 0:if ((c>='a' && c<='z') || (c>='A' && c<='Z') || c=='_')
                {proxC();estado=1;break;}
             if (c>='0' && c<='9')
                {proxC();estado=2;tk=TKCteInt;break;}
             if (c=='/') {
                proxC();
                if (c=='/')
                {
                    while(c != '\n'){
                        proxC();
                    }
                    return;
                }

                if (c=='*')
                {
                    char anterior = 0;
                    proxC(); // começa a ler depois do *

                    while (c != EOF) {
                        if (anterior == '*' && c == '/') {
                            proxC(); // anda depois do /
                            return;
                        }

                        anterior = c;
                        proxC();
                     }
                }
                lex[posl]= '\0';
                tk=TKDivisao;
                return;
              }

             if (c == '"') {
                char simboloAnterior = '\0';
                char simboloAnteriorAnterior = '\0';
                proxC(); // pula a primeira "

                while (!(c == '"' && (simboloAnterior != '\\' || simboloAnteriorAnterior=='\\'))) {
                    lex[posl++] = c;
                    simboloAnteriorAnterior = simboloAnterior;
                    simboloAnterior = c;
                    proxC();
                }

                lex[posl++] = '"';
                proxC();

                lex[posl] = '\0';
                tk = TKString;
                return;
             }
             if (c=='\''){
                proxC();
                char charAnterior = '\0';
                char charAnteriorAnterior = '\0';
                    while (!(c == '\'' && (charAnterior != '\\' || charAnteriorAnterior == '\\')))
                    {
                        lex[posl++]=c;
                        charAnteriorAnterior = charAnterior;
                        charAnterior = c;
                        proxC();
                    }
                    lex[posl++]='\'';
                    proxC();
                    lex[posl]='\0';
                    tk=TKString;
                    return;
             }
             if (c=='='){
                proxC();
                if (c=='='){
                    lex[posl++]='=';
                    lex[posl]='\0';
                    proxC();
                    tk = TKCompara;
                    return;
                }
                else{
                    lex[posl]='\0';
                    tk=TKAtrib;
                }
                return;
             }

             if (c=='+'){
                   proxC();
                if (c=='+')
                      {
                  lex[posl++]='+';
                        lex[posl]='\0';
                  proxC();
                  tk=TKDuploMais;/*printf("Reconheceu token TKDuploMais\n");*/return;
                  }
                else if (c=='=')
                      {
                  lex[posl++]='=';
                        lex[posl]='\0';
                  proxC();
                  tk=TKAtribMais;/*printf("Reconheceu token TKDuploMais\n");*/return;
                  }
                else
                  {
                              lex[posl]='\0';
                  tk=TKMais;/*printf("Reconheceu token TKSoma\n");*/return;
                  }
             }

            if (c=='>'){
                proxC();
                if(c=='='){lex[posl++]='=';lex[posl]='\0';proxC();tk=TKMaiorIgual;return;}
                if(c=='>'){
                    lex[posl++]='>';
                    proxC();
                    if(c=='='){
                        tk=TKShiftRIgual;proxC();lex[posl]='\0';return;
                    }
                    else{
                        tk=TKShiftR;lex[posl]='\0';return;
                    }
                }
                tk=TKMaior;
                lex[posl]='\0';
                proxC();return;
            }

            if (c=='<'){
                proxC();
                if(c=='='){lex[posl++]='=';lex[posl]='\0';proxC();tk=TKMenorIgual;return;}
                if(c=='<'){
                    lex[posl++]='<';
                    proxC();
                    if(c=='='){
                        tk=TKShiftLIgual;proxC();lex[posl]='\0';return;
                    }
                    else{
                        tk=TKShiftL;lex[posl]='\0';return;
                    }
                }
                tk=TKMenor;
                lex[posl]='\0';
                proxC();return;
            }

            if (c=='|'){
                proxC();
                if (c=='|'){
                    lex[posl++]='|';
                    tk=TKOR;
                    proxC();
                    lex[posl]='\0';
                    return;
                }
                if (c=='='){
                    lex[posl++]='=';
                    tk=TKORIgual;
                    proxC();
                    lex[posl]='\0';
                    return;
                }
                tk=TKPipe;lex[posl]='\0';return;
            }


             if (c=='*'){lex[posl]='\0';proxC();tk=TKProd;/*printf("Reconheceu token TKProd\n");*/return;}
             if (c=='['){lex[posl]='\0';proxC();tk=TKAbreColchete;/*printf("Reconheceu token TKAbrePar\n");*/return;}
             if (c==']'){lex[posl]='\0';proxC();tk=TKFechaColchete;/*printf("Reconheceu token TKAbrePar\n");*/return;}
             if (c=='('){lex[posl]='\0';proxC();tk=TKAbrePar;/*printf("Reconheceu token TKAbrePar\n");*/return;}
             if (c==')'){lex[posl]='\0';proxC();tk=TKFechaPar;/*printf("Reconheceu token FechaPar\n");*/return;}
             if (c=='{'){lex[posl]='\0';proxC();tk=TKAbreChaves;/*printf("Reconheceu token TKAbreChaves\n");*/return;}
             if (c=='}'){lex[posl]='\0';proxC();tk=TKFechaChaves;/*printf("Reconheceu token TKFechaChaves\n");*/return;}
             if (c==','){lex[posl]='\0';proxC();tk=TKVirgula;/*printf("Reconheceu token TKVirgula\n");*/return;}
             if (c==';'){lex[posl]='\0';proxC();tk=TKPontoEVirgula;/*printf("Reconheceu token TKPontoEVirgula\n");*/return;}
             if (c==':'){lex[posl]='\0';proxC();tk=TKDoisPontos;/*printf("Reconheceu token TKDoisPontos\n");*/return;}

             if (c=='^'){
                proxC();
                if (c=='='){
                    lex[posl++] = '=';lex[posl]='\0';proxC();tk=TKXORIgual;return;
                }
                lex[posl]='\0';tk=TKXOR;return;
             }

             if(c=='~'){lex[posl]='\0';tk=TKComplemento1;proxC();return;}
             if(c=='?'){lex[posl]='\0';tk=TKTernario;proxC();return;}


             if (c=='%'){
                proxC();
                if(c=='='){
                    lex[posl++] = '=';lex[posl]='\0';proxC();tk=TKDiferencaIgual;return;
                }
                lex[posl]='\0';tk=TKDiferenca;return;
             }

             if (c=='-'){
                    proxC();
                    if (c=='>'){proxC();lex[posl++]='>';lex[posl]='\0';tk=TKArrow;return;}
                    if (c=='-'){proxC();lex[posl++]='-';lex[posl]='\0';tk=TKDuploMenos;return;}
                    lex[posl]='\0';tk=TKSub;return;
             }

             if (c==-1){lex[posl]='\0';proxC();tk=TKFimArquivo;/*printf("Reconheceu token TKFimArquivo\n");*/return;}
             if (c==' ' || c=='\n' || c=='\t' || c=='\r') {proxC();posl--;break;}


             if (c=='.') {
                proxC();
                lex[posl]='\0';
                tk=TKPonto;
                return;
             }

             if (c=='!') {
                proxC();
                if (c== '='){
                    lex[posl++]='=';lex[posl]='\0';tk=TKUnequal;return;
                }
                lex[posl]='\0';tk=TKNegate;return;
             }

             if (c=='&'){
                proxC();
                if (c=='='){
                    lex[posl++] = c;
                    lex[posl]='\0';
                    tk = TKEIgual;proxC();return;
                }
                if (c=='&'){
                    lex[posl++] = c;
                    lex[posl]='\0';
                    tk = TKAND;proxC();return;
                }
                lex[posl]='\0';
                tk = TKEComercial;proxC();return;
             }


             if (c=='\0') {tk=-1;return;}
             printf("Erro léxico: encontrou o caracter %c (%d) na linha %d\n",c,c,lin);
             while (c!='\n') proxC();
             break;
      case 1:if (c>='a' && c<='z' || c>='A' && c<='Z' || c=='_' || c>='0' && c<='9') {proxC();break;}
             lex[--posl]='\0';
             tk=palavra_reservada(lex);
             //printf("reconheceu token %s\n",lex);
             return;
      case 2:
            if (c>='0' && c<='9') {proxC();break;}
            if (c == '.' && tk!=TKNumDouble){
                proxC(); tk=TKNumDouble;break;
            }
            if (lex[posl-1] != '.' && (c == 'f' || c == 'F')){
                proxC();
                tk=TKNumFloat;
                lex[posl]='\0';
                return;
            }
            lex[--posl]='\0';
            return;
      } //switch
   }// while
}// função

int main()
{
arqin=fopen("../entrada.c","rb");
if (!arqin) {
printf("Erro na abertura do fonte.\n");
return 0;
}
FILE *arqToken = fopen("../tokens.dat","wb");




proxC(); // lê primeiro caracter do arquivo
getToken(); // lê primeiro token
while (tk!=TKFimArquivo)
   {
    Token token;

    token.tipo = tk;
    strcpy(token.lexema, lex);
    strcpy(token.nome, nome[tk]);
    token.linha = lin;
    token.coluna = col;
    fwrite(&token,sizeof(Token),1,arqToken);

    printf("linha: %d tk=%d-%s lexema=%s\n",lin,tk,nome[tk],lex);
    getToken();
   }
Token token;
token.tipo = TKFimArquivo;
strcpy(token.lexema,"EOF");
strcpy(token.nome, "TKFimArquivo");
fwrite(&token,sizeof(Token),1,arqToken);

fclose(arqToken);
fclose(arqin);
int a;
printf("pressione qualquer tecla para sair");
scanf("%d",&a);
}
