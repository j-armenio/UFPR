#include <stdio.h>

struct nodo_f {
    int chave;
    struct nodo_f *prox;
};
typedef struct nodo_f nodo_f_t;

struct fila {
    nodo_f_t* cabeca;
    nodo_f_t* cauda;
    int tamanho;
};
typedef struct fila fila_t;

fila_t* fila_cria ();
int queue (fila_t* f, int elemento);
int dequeue (fila_t* f, int* elemento);
int fila_tamanho (fila_t* f);
int fila_vazia (fila_t* f);
fila_t* fila_destroi (fila_t* f);

nodo_f_t *cria_nodo(int elemento)
{
    fila_t f;
    if (!(f = malloc(sizeof(fila_t))))
        return NULL;
}
    
fila_t* fila_cria ()
{

}

int queue (fila_t* f, int elemento)
{
    nodo_f_t *novo = cria_nodo(elemento);
    if (fila_vazia(f)){
        f->cabeca = novo;
    }
    f->cauda->prox = novo;
    f->cauda = novo;
    f->tamanho++;
    return 1;
}

int dequeue (fila_t* f, int* elemento)
{
    if (!fila_vazia){
        elemento = f->cabeca->chave;
        struct node_f_t *aux = f->cabeca; /*pq struct aq?*/
        f->cabeca = f->cabeca->prox;
        free(aux);
        return 1;
    }
    return 0;
}

int fila_tamanho (fila_t* f)
{
    return f->tamanho;
}

int fila_vazia (fila_t* f)
{
    if (f->cabeca == f->cauda == NULL)
        return 1;
    return 0;
}

fila_t* fila_destroi (fila_t* f)
{
    int *elemento = 0;
    while (dequeue(f, elemento));
    return NULL;
}

int main(){

}