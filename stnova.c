/*****************************************************************/
/*    Estrutura nova a implementar | PROG2 | MIEEC | 2020/21     */
/*****************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stnova.h"

estrutura *st_nova()
{
    estrutura *st;
    st=(estrutura*)malloc(sizeof(estrutura));
    if(st==NULL) return NULL;
    st->capacidade=100000;
    st->tamanho=0;
    st->hfunc=hash_krm;
    st->sfunc=sond_rh;
    st->v=(aresta_grafo***)calloc(100000, sizeof(aresta_grafo**));
    if(st->v ==NULL){
        free(st);
        return NULL;
    }
    st->est_celulas=(int*)calloc(100000, sizeof(int));
    if(st->est_celulas ==NULL){
        free(st->v);
        free(st);
        return NULL;
    }
    st->n_voo=(int*)calloc(100000, sizeof(int));
    if(st->n_voo == NULL){
        free(st->v);
        free(st->est_celulas);
        free(st);
        return NULL;
    }
    st->c_origem=(char**)malloc(100000 * sizeof(char*));
    if(st->c_origem == NULL){
        free(st->v);
        free(st->est_celulas);
        free(st->n_voo);
        free(st);
        return NULL;
    }
    return st;
}
int st_insere(estrutura *st, aresta_grafo *entrada, char *cidade_o)
{
    if(st==NULL || entrada ==NULL) return -1;
    int indice, fixo, tentativas=0;
    aresta_grafo *aux;
    indice=(hash_krm(cidade_o, st->capacidade)+hash_krm(entrada->destino->cidade, st->capacidade))/2;
    fixo=indice;
    while(st->est_celulas[indice]==1){
        if(strcmp(st->c_origem[indice], cidade_o)==0 && strcmp(st->v[indice][0]->destino->cidade, entrada->destino->cidade)==0){
            st->v[indice]=(aresta_grafo**)realloc(st->v[indice], (st->n_voo[indice]+1) * sizeof(aresta_grafo*));
            if(st->v == NULL) return -1;
            if(entrada->preco<st->v[indice][0]->preco){
                aux=st->v[indice][0];
                st->v[indice][0]=entrada;
                st->v[indice][st->n_voo[indice]]=aux;
                st->n_voo[indice]++;
                return 1;
            }
            if(entrada->preco>=st->v[indice][0]->preco){
                st->v[indice][st->n_voo[indice]]=entrada;
                st->n_voo[indice]++;
                return 1;
            }
        }
        indice=sond_rh(fixo, tentativas, st->capacidade);
        tentativas++;
    }
    st->v[indice]=(aresta_grafo**)malloc(sizeof(aresta_grafo*));
    if(st->v==NULL) return -1;
    st->v[indice][0]=entrada;
    st->c_origem[indice]=(char*)malloc((strlen(cidade_o)+1)*sizeof(char));
    if(st->c_origem==NULL){
        free(st->v);
        return -1;
    }
    strcpy(st->c_origem[indice], cidade_o);
    st->est_celulas[indice]=1;
    st->n_voo[indice]=1; 
    return 1;
}

int st_importa_grafo(estrutura *st, grafo *g)
{  
    if(st==NULL || g==NULL) return 0;
    int valido;
    for (int i = 0; i < g->tamanho; i++){
        for (int j = 0; j < g->nos[i]->tamanho; j++){
            valido=st_insere(st, g->nos[i]->arestas[j], g->nos[i]->cidade);
            if(valido==-1){
                return -1;
            }
        }
    }
    return 0;
}

char *st_pesquisa(estrutura *st, char *origem, char *destino)
{
    if(st==NULL) return NULL;
    int indice, fixo, tentativas=0;
    indice=(hash_krm(origem, st->capacidade)+hash_krm(destino, st->capacidade))/2;
    fixo=indice;
    while(1){
        if(st->est_celulas[indice]==0){
            return NULL;
        }
        if(strcmp(st->v[indice][0]->destino->cidade, destino)==0 && strcmp(st->c_origem[indice], origem)==0){
            break;
        }
        if(strcmp(st->v[indice][0]->destino->cidade, destino)!=0 || strcmp(st->c_origem[indice], origem)!=0){
            indice = sond_rh(fixo, tentativas, st->capacidade);
            tentativas++;
        }
    }
    return st->v[indice][0]->codigo;
}

int st_apaga(estrutura *st)
{
    if(st==NULL) return -1;
    for (int i = 0; i < st->capacidade; i++){
        if(st->est_celulas[i]==1){
        free(st->c_origem[i]);
        free(st->v[i]);
        }
    }
    free(st->v);
    free(st->c_origem);
    free(st->est_celulas);
    free(st->n_voo);
    free(st);
    return 0;
}
