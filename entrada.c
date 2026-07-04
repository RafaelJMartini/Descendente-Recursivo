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
,"TKMenorIgual","TKMenor","TKShiftLIgual"
,"TKOR","TKORIgual","TKPipe","TKTypedef","TKStruct"
,"TKTrue","TKFalse","TKSizeof","TKDuploMenos","TKArrow"
,"TKDiferenca","TKDiferencaIgual","TKDivisao"
,"TKXOR","TKComplemento1","TKXORIgual","TKTernario"
,"TKReturn","TKFor","TKWhile","TKDo","TKSwitch","TKCase","TKBreak","TKDefault","TKUnion","TKEnum","TKGoto"
,"TKShort", "TKLong", "TKUnsigned","TKSigned","TKCompara"};

int pos = 0;

int tk;
char lex[50];
int lin=1;
int col=1;
char c; // último caracter lido do arquivo

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

// variáveis globais para retrocesso

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
      if ((c>='a' && c<='z') || (c>='A' && c<='Z') || c=='_')
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
                    getToken();  // <-- troca return por isso
                    return;
                }

                if (c=='*')
                {
                    char anterior = 0;
                    proxC();
                    while (c != EOF) {
                        if (anterior == '*' && c == '/') {
                            proxC();
                            getToken();  // <-- idem aqui
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
                proxC(); // pula o ' de abertura
                while (1)
                {
                    if (c == '\\') {       // encontrou escape
                        lex[posl++] = c;
                        proxC();           // consome o '\'
                        lex[posl++] = c;
                        proxC();           // consome o char escapado (n, t, ', \ etc.)
                        continue;
                    }
                    if (c == '\'') {       // fechamento real
                        lex[posl++] = '\'';
                        proxC();
                        break;
                    }
                    lex[posl++] = c;
                    proxC();
                }
                lex[posl] = '\0';
                tk = TKString;
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
                    lex[posl++]='=';lex[posl]='\0';tk=TKUnequal;proxC();return;
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
                tk = TKEComercial;return;
             }


             if (c=='\0') {tk=-1;return;}
             printf("Erro léxico: encontrou o caracter %c (%d) na linha %d\n",c,c,lin);
             while (c!='\n') proxC();
             break;
             if (c>='a' && c<='z' || c>='A' && c<='Z' || c=='_' || c>='0' && c<='9') {proxC();break;}
             lex[--posl]='\0';
             tk=palavra_reservada(lex);
             //printf("reconheceu token %s\n",lex);
             return;
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
    token.tipo = tk;
    strcpy(token.lexema, lex);
    strcpy(token.nome, nome[tk]);
    token.linha = lin;
    token.coluna = col;
    fwrite(&token,Token,1,arqToken);

    printf("linha: %d tk=%d-%s lexema=%s\n",lin,tk,nome[tk],lex);
    getToken();
   }
token.tipo = TKFimArquivo;
strcpy(token.lexema,"EOF");
strcpy(token.nome, "TKFimArquivo");
fwrite(&token,Token,1,arqToken);

fclose(arqToken);
fclose(arqin);
int a;
printf("pressione qualquer tecla para sair");
scanf("%d",&a);
}
