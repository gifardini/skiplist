#define nivel_max 15 // nivel maximo que a lista pode ter
typedef struct skiplist skipl_t;


skipl_t *criar();
int inserir(skipl_t *l, char palavra[50], char sig[140]);
int gerar_nivel();
char *busca(skipl_t *l, char palavra[50]);
void libera(skipl_t *l);
int imprimir(skipl_t *l, char letra[]);
int altera(skipl_t *l, char palavra[50], char novo_sig[140]);
int remover(skipl_t *l, char palavra[50]);