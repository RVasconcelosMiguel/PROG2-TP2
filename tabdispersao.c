/*****************************************************************/
/*         Tabela de Dispersao | PROG2 | MIEEC | 2020/21         */
/*****************************************************************/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "tabdispersao.h"
#include "grafo.h"

tabela_dispersao *tabela_nova(int capacidade, hash_func *hfunc, sond_func *sfunc)
{
    tabela_dispersao *t;
    t=(tabela_dispersao*)malloc(sizeof(tabela_dispersao));
    if(t==NULL) return NULL;
    t->capacidade=capacidade;
    t->hfunc=hfunc;
    t->sfunc=sfunc;
    t->tamanho=0;
    t->nos=(no_grafo**)calloc(capacidade, sizeof(no_grafo*));
    if(t->nos==NULL){
        free(t);
        return NULL;
    }
    t->estado_celulas=(int*)calloc(capacidade, sizeof(int));
    if(t->estado_celulas==NULL){
        free(t->nos);
        free(t);
        return NULL;
    }
    return t;
}

int tabela_adiciona(tabela_dispersao *td, no_grafo *entrada)
{
    if(td==NULL || entrada==NULL) return -1;
    int indice, tentativa=0, fixo;
    indice = td->hfunc(entrada->cidade, td->capacidade);
    fixo=indice;
    while (td->estado_celulas[indice]==1){
        if(strcmp(td->nos[indice]->cidade, entrada->cidade)==0){
            return -1;
        }
        indice=td->sfunc(fixo, tentativa, td->capacidade);
        tentativa++;
    }
    td->tamanho++;
    td->nos[indice]=entrada;
    td->estado_celulas[indice]=1;
    
    return indice;
}

int tabela_remove(tabela_dispersao *td, no_grafo *saida)
{
    if (td==NULL || saida==NULL) return -1;
    if(tabela_existe(td, saida->cidade)==-1) return -1;
    int indice, tentativa=0, fixo;
    indice=td->hfunc(saida->cidade, td->capacidade);
    fixo=indice;
    while (1){
        if (td->nos[indice]!=NULL){
            if (strcmp(td->nos[indice]->cidade, saida->cidade)==0){
                break;
            }
            if (strcmp(td->nos[indice]->cidade, saida->cidade)!=0){
            indice=td->sfunc(fixo, tentativa, td->capacidade);
            tentativa++;
            }
        }
        if (td->nos[indice]==NULL){
            indice=td->sfunc(fixo, tentativa, td->capacidade);
            tentativa++;
        }
    }
    td->nos[indice]=NULL;
    td->estado_celulas[indice]=-1;
    td->tamanho--;
    
    return 0;
}

void tabela_apaga(tabela_dispersao *td)
{
    if(td==NULL) return;
    free(td->nos);
    free(td->estado_celulas);
    free(td);
}

int tabela_existe(tabela_dispersao *td, const char *cidade)
{
    if(td==NULL) return -1;
    for (int i = 0; i < td->capacidade; i++){
        if(td->nos[i]!=NULL && strcmp(td->nos[i]->cidade, cidade)==0){
            return i;
        }
    }
    return -1;
}

tabela_dispersao *tabela_carrega(grafo *g, int tamanho)
{
    if(g==NULL) return NULL;
    int valido;
    tabela_dispersao *t=tabela_nova(tamanho, hash_krm, sond_rh);
    if(t==NULL){
        return NULL;
    }
    for (int i = 0; i < g->tamanho; i++){
        valido=tabela_adiciona(t, g->nos[i]);
        if(valido==-1){
            tabela_apaga(t);
            return NULL;
        }
    }
    return t;
}

/*================================================================================*/
unsigned long sond_rh(int pos, int tentativas, int tamanho)
{

    return (pos + tentativas * tentativas) % tamanho;
}

unsigned long hash_krm(const char *chave, int tamanho)
{
    int c, t = strlen(chave);
    unsigned long hash = 7;

    for (c = 0; c < t; c++)
    {
        hash += (int)chave[c];
    }

    return hash % tamanho;

    return 0;
}
