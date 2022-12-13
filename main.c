#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<string.h>
#include"skiplist.h"
#include<time.h>

int main(){
    skipl_t *l = criar();

    srand((unsigned int)time(NULL)); //gera numeros aleatorios
    char cmd[10] = {0};

    char palavra[50], sig[140], letra[2], novo_sig[140];

     while(scanf("%s ", cmd) != EOF){ // le a operacao
          
       if(!strcmp(cmd, "insercao")){
     
          scanf("%s ", palavra); // le ate o espaco
          scanf("%[^\n]", sig);  // le ate o \n

          if(!inserir(l, palavra, sig)){ // palavra ja esta no dicionario
            printf("OPERACAO INVALIDA\n");
          }
       }

       else if(!strcmp(cmd, "remocao")){
          scanf("%s", palavra);
          if(!remover(l, palavra)){
            printf("OPERACAO INVALIDA\n"); // palavra nao existe no dicionario
          }   
       }
       else if(!strcmp(cmd, "busca")){
          scanf("%s", palavra);
          if(busca(l, palavra) != NULL){
               printf("%s %s\n", palavra, busca(l, palavra)); 
          }else{
               printf("OPERACAO INVALIDA\n"); // palavra nao existe no dicionario
          } 
       }
       else if(!strcmp(cmd, "alteracao")){ 
          scanf("%s ", palavra);
          scanf("%[^\n]", novo_sig);
          if(!altera(l, palavra, novo_sig)){
              printf("OPERACAO INVALIDA\n"); // palavra nao existe no dicionario
          }
       }
       else if(!strcmp(cmd, "impressao")){
          scanf("%s", letra);

          if(!imprimir(l, letra)){
            printf("NAO HA PALAVRAS INICIADAS POR %s\n", letra);
          }
        }
     }
    libera(l);
    return 0;
}