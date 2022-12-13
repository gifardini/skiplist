#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<string.h>
#include"skiplist.h"
#include<time.h>

typedef struct no no_t;

struct no{
    char palavra[50], significado[140];
    no_t *prox, *abaixo;
    int nivel; // nivel do no
};

struct skiplist{
    no_t *inicio;
    int niv_total; //nivel da lista ate aquele momento
};

skipl_t *criar(){

    skipl_t *l;
    l = (skipl_t *)malloc(sizeof(skipl_t));
    assert(l != NULL);
    l->niv_total = 0;

    no_t *novo_ini = (no_t *)malloc(sizeof(no_t)); // cria o no cabecalho de inicio
        strcpy(novo_ini->palavra, "-1");
        strcpy(novo_ini->significado, "-1");
        novo_ini->nivel = 0;
        novo_ini->prox = NULL;
        novo_ini->abaixo = NULL;                        
        l->inicio = novo_ini;    
    
    return l;
}

int gerar_nivel(){
    
    int nivel = 0;
    int var = rand()%100;

    /* gera valores aleatorios e compara se
    sao pares ou impares (prob = 0.5 cada), 
    caso par e nivel menor que nivel max 
    de 15 continua */

    while (var%2 == 0 && nivel <= nivel_max){ 
        nivel++;
        var = rand()%100;
    }

    return nivel; // retorna o valor sorteado
}

char *busca(skipl_t *l, char palavra[50]){
    assert(l != NULL);
    no_t *p = l->inicio;
    
    /* a busca eh feita do maior pro menor
    nivel, enqto a palavra encontrada na lista 
    for menor que a procurada continua indo
    para a proxima, caso contrario desce um nivel */

    while(p != NULL && p->nivel != 0){ 
        while(p->prox != NULL && strcmp(p->prox->palavra, palavra) < 0){
            p = p->prox;
        }
        p = p->abaixo;
    }

    /*qdo chega no nivel zero (inicial) busca a palavra
    de forma analoga ate ser menor que a procurada ou
    ate chegar ao final da lista (p->prox == NULL) */

    while(p->prox != NULL && strcmp(p->prox->palavra, palavra) < 0){
        p = p->prox;
    }

    if(p->prox != NULL && !strcmp(p->prox->palavra, palavra)){  //encontrou e retorna
        return p->prox->significado;                            // a palavra
    }else{      
        return NULL;        // nao encontrou retorna nulo
    }
}

int inserir(skipl_t *l, char palavra[50], char sig[140]){
    assert(l != NULL);
    
    if(l->inicio != NULL && busca(l, palavra) != NULL){ // palavra ja esta na lista
        return 0;
    }

    no_t *p = l->inicio;
    no_t **update = (no_t **)malloc(sizeof(no_t *)*(nivel_max + 2)); //garante que tem mem suficiente p tds
                                                                    // os possiveis niveis da lista

    /* de forma semelhante a busca, comeca do nivel 
    mais alto e vai ate a palavra menor do que aquela 
    procurada, caso seja maior desce um nivel, alem 
    disso armazena a posicao em que parou no update 
    para conseguir inserir a palavra na ordem certa 
    de cada nivel */

    while(p != NULL && p->nivel != 0){
        while(p->prox != NULL && strcmp(p->prox->palavra, palavra) < 0){
            p = p->prox;
        }
        update[p->nivel] = p;
        p = p->abaixo;
    }
       
    while(p->prox != NULL && strcmp(p->prox->palavra, palavra) < 0){ // faz o mesmo que o while
        p = p->prox;                                                 // acima so que p nivel 0
    } 

    update[p->nivel] = p; // armazena a posicao para o nivel 0
    
    int nivel_atual = 0; 
    int nivel_no = gerar_nivel(); // sorteia o nivel da nova string

    /* insere debaixo para cima, aqui eh o caso em que o
    nivel gerado eh menor ou igual ao nivel presente da
    lista  */

    while(nivel_atual <= l->niv_total && nivel_atual <= nivel_no){    
       
        no_t *n = (no_t *)malloc(sizeof(no_t));  // cria novos nos p cada nivel
        strcpy(n->palavra, palavra);        // em que vai inserir a nova string
        strcpy(n->significado, sig);            
        n->nivel = nivel_atual;                  
        n->abaixo = NULL;
        n->prox = NULL;

        /* inicializa os ponteiros de prox e
        abaixo dos novos nos em cada nivel
        com as posicoes armazenadas no update */

        if(nivel_atual == 0){  
            n->prox = update[nivel_atual]->prox;
            n->abaixo = NULL;
        }else{
            n->prox = update[nivel_atual]->prox;
            n->abaixo = update[nivel_atual - 1]->prox;
        }
        update[nivel_atual]->prox = n;
        nivel_atual++; 
    }   

    /* o laco abaixo eh o caso em que o nivel_no sorteado
    eh maior que o nivel da lista em tem-se que criar
    um novo cabecalho e inserir o novo nivel na lista */
    
    for(int i = l->niv_total + 1; i <= nivel_no; i++){
        no_t *n = (no_t *)malloc(sizeof(no_t));
        strcpy(n->palavra, palavra);
        strcpy(n->significado, sig);
        n->nivel = i;
        n->prox = NULL;
        n->abaixo = update[i-1]->prox;

        no_t *novo_ini = (no_t *)malloc(sizeof(no_t));
        strcpy(novo_ini->palavra, "-1");
        strcpy(novo_ini->significado, "-1");
        novo_ini->nivel = i;
        novo_ini->prox = n; // insere a string no novo nivel
        novo_ini->abaixo = l->inicio; 
        l->inicio = novo_ini; // atualiza o cabecalho
        update[i] = novo_ini; 
        }
        if(l->niv_total < nivel_no){
            l->niv_total = nivel_no; // atualiza o nivel da lista
        }

    free(update); 
    return 1;
}

int remover(skipl_t *l, char palavra[50]){
    assert(l != NULL);
    
    if(busca(l, palavra) == NULL){ // palavra nao esta na lista
        return 0;
    }

    no_t *p = l->inicio;
    no_t **update = (no_t **) malloc(sizeof(no_t *)*(nivel_max+2));
    
    /* como na insercao procura de cima para baixo
    e armazena no update as posicoes dos nos anteriores
    ao procurado para atualizar seu prox apos a remocao 
    da string */

    while(p != NULL && p->nivel != 0){
        while(p->prox != NULL && strcmp(p->prox->palavra, palavra) < 0){
            p = p->prox;
        }
        update[p->nivel] = p;
        p = p->abaixo;
    }
    
    while(p->prox != NULL && strcmp(p->prox->palavra, palavra) < 0){
            p = p->prox;
    }
    
    update[p->nivel] = p;
    int nivel_atual = 0;

    /*remove debaixo para cima */

    while(nivel_atual <= l->niv_total && update[nivel_atual]->prox != NULL){
        if(!strcmp(update[nivel_atual]->prox->palavra, palavra)){
            no_t *rem = update[nivel_atual]->prox;  // armazena em rem o no a ser removido
            update[nivel_atual]->prox = rem->prox; // atualiza o ponteiro de prox
            free(rem);                            //remove o no
        }
        nivel_atual++; 
    }

    /* se o prox do cabecalho for nulo
    quer dizer que nao tem strings armazenadas
    naquele nivel entao remove o inicio e atualiza
    o nivel da lista */

    while(l->niv_total > 0 && l->inicio->prox == NULL){
        no_t *rem = l->inicio;
        l->inicio = l->inicio->abaixo;
        free(rem);
        l->niv_total--;
    }
    
    free(update);
    return 1;
}


int altera(skipl_t *l, char palavra[50], char novo_sig[140]){
    
    if(busca(l, palavra) == NULL){ //palavra nao existe na lista
        return 0;
    }

    no_t *p = l->inicio;

    /*faz a busca do mais alto nivel ate o menor
    ate encontrar a palavra anterior a procurada */

    while(p != NULL && p->nivel != 0){
        while(p->prox != NULL && strcmp(p->prox->palavra, palavra) < 0){
            p = p->prox;
        }
        p = p->abaixo;
    }

    /* faz o mesmo para o nivel zero */

    while(p->prox != NULL && strcmp(p->prox->palavra, palavra) < 0){
        p = p->prox;
    }

    if(p->prox != NULL && !strcmp(p->prox->palavra, palavra)){ // encontrou
        strcpy(p->prox->significado, novo_sig); // e substitui pelo novo significado
        return 1;
    }else{ // nao encontrou
        return 0;
    }
}

int imprimir(skipl_t *l, char letra[]){
    assert(l != NULL);

    no_t *p = l->inicio;
    int cont = 0;
    
    /* busca por strings inciadas pela letra
    do maior pro menor nivel */

    while(p != NULL){ 
        while(p->nivel != 0){
            while(p->prox != NULL && strcmp(p->prox->palavra, letra) < 0){
                p = p->prox;
            }
            p = p->abaixo;
        }

        while(p->prox != NULL && strcmp(p->prox->palavra, letra) < 0){
            p = p->prox;
        }
    
        if(p->palavra[0] == letra[0]){  //imprime se for iniciada pela letra
            printf("%s %s", p->palavra, p->significado);
            printf("\n");
            cont++; 
        }
        p = p->prox;
    }

    if(cont == 0){   // se cont continua zero quer dizer que n existem
        return 0;   //palavras iniciadas pela letra
    }else{
        return 1;
    }
}

void libera(skipl_t *l){
    if(l == NULL)
        return;
    
    no_t *p, *aux, *abaixo;
    p = l->inicio;
    
 /*desaloca a lista de cima para baixo */   
    while(p != NULL && p->nivel != 0){
        abaixo = p->abaixo; //armazena o cabecalho do nivel abaixo
        while(p->prox != NULL){ //desaloca todos os nos daquele nivel
            aux = p->prox;
            free(p);
            p = aux;
        }
        free(p);
        p = abaixo; //vai pro cabecalho do nivel debaixo
    }

    while(p != NULL){ //desaloca o nivel inical 0
      aux = p->prox;
      free(p);
      p = aux;
    
    }
    free(l);  
}