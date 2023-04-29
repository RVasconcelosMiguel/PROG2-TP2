/*****************************************************************/
/*          Grafo direcionado | PROG2 | MIEEC | 2020/21          */
/*****************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "grafo.h"
#include "heap.h"

grafo *grafo_novo()
{
    grafo *g = (grafo *)malloc(sizeof(grafo));
    g->tamanho = 0;
    g->nos = NULL;

    return g;
}

no_grafo *encontra_no(grafo *g, char *cidade)
{
    if (!g || !cidade)
        return NULL;

    // pesquisa por cidade no vetor de nós
    for (int i = 0; i < g->tamanho; i++)
    {
        if (strcmp(g->nos[i]->cidade, cidade) == 0)
            return g->nos[i];
    }
    return NULL;
}

no_grafo *no_insere(grafo *g, char *cidade)
{
    if (!g || !cidade)
        return NULL;

    // verifica se o nó já existe
    no_grafo *no = encontra_no(g, cidade);
    if (no)
        return NULL;

    // cria o novo nó para o user
    no = (no_grafo *)malloc(sizeof(no_grafo));
    if (!no)
        return NULL;

    // aloca espaço para o campo cidade
    no->cidade = (char *)malloc((strlen(cidade) + 1) * sizeof(char));
    if (!no->cidade)
    {
        free(no);
        return NULL;
    }
    // inicializa todos os campos
    strcpy(no->cidade, cidade);
    no->tamanho = 0;
    no->arestas = NULL;
    no->p_acumulado = 0;
    no->anterior = NULL;
    no->dataatualizada = NULL;

    // insere o nó criado no final do vetor de nós
    g->nos = (no_grafo **)realloc(g->nos, (g->tamanho + 1) * sizeof(no_grafo *));
    if (!g->nos)
    {
        free(no->cidade);
        free(no);
        return NULL;
    }
    // incrementa o tamanho do numero de nós e insere no vetor de apontadores de nós
    g->tamanho++;
    g->nos[g->tamanho - 1] = no;

    return no;
}

// função auxiliar que retorna 1 se existir a aresta para destino ou 0,
// se a aresta não existir, -1 se der erro
int existe_aresta(no_grafo *origem, no_grafo *destino, char *codigo)
{

    if (!origem || !destino)
        return -1;

    //pesquisa em todas as arestas se existe
    for (int i = 0; i < origem->tamanho; i++)
    {

        if ((origem->arestas[i]->destino == destino) && ((strcmp(origem->arestas[i]->codigo, codigo) == 0)))
            return 1;
    }

    return 0;
}

int cria_aresta(no_grafo *origem, no_grafo *destino, char *codigo, char *companhia, data partida, data chegada, float preco, int lugares)
{
    if (!origem || !destino || !codigo || !companhia)
        return -1;

    if (preco < 0 || lugares < 0)
        return -1;

    // verifica se a ligação já existe
    if (existe_aresta(origem, destino, codigo) > 0)
        return -1;

    // cria a nova ligação
    aresta_grafo *ag = (aresta_grafo *)malloc(sizeof(aresta_grafo));
    if (!ag)
        return -1;

    ag->destino = destino;
    ag->preco = preco;
    ag->lugares = lugares;
    // aloca espaço para o código
    ag->codigo = (char *)malloc((strlen(codigo) + 1) * sizeof(char));
    if (!ag->codigo)
    {
        free(ag);
        return -1;
    }
    strcpy(ag->codigo, codigo);
    // aloca espaço para o companhia
    ag->companhia = (char *)malloc((strlen(companhia) + 1) * sizeof(char));
    if (!ag->companhia)
    {
        free(ag->codigo);
        free(ag);
        return -1;
    }
    strcpy(ag->companhia, companhia);

    // inicializa todos os campos
    ag->partida.tm_year = partida.tm_year;
    ag->partida.tm_mon = partida.tm_mon;
    ag->partida.tm_mday = partida.tm_mday;
    ag->partida.tm_hour = partida.tm_hour;
    ag->partida.tm_min = partida.tm_min;
    ag->partida.tm_sec = 0;
    ag->partida.tm_isdst = 0;

    // inicializa todos os campos
    ag->chegada.tm_year = chegada.tm_year;
    ag->chegada.tm_mon = chegada.tm_mon;
    ag->chegada.tm_mday = chegada.tm_mday;
    ag->chegada.tm_hour = chegada.tm_hour;
    ag->chegada.tm_min = chegada.tm_min;
    ag->chegada.tm_sec = 0;
    ag->chegada.tm_isdst = 0;

    // insere a nova ligação no vetor de ligações
    origem->arestas = (aresta_grafo **)realloc(origem->arestas, (origem->tamanho + 1) * sizeof(aresta_grafo *));
    if (!origem->arestas)
    {
        free(ag->codigo);
        free(ag->companhia);
        free(ag);
        return -1;
    }
    origem->tamanho++;
    origem->arestas[origem->tamanho - 1] = ag;

    return 0;
}

no_grafo *no_remove(grafo *g, char *cidade)
{
    if(g==NULL) return NULL;
    no_grafo *no = encontra_no(g, cidade);
    if (no==NULL) return NULL;
    int indice=0;
    for (int i = 0; i < g->tamanho; i++){
        if (strcmp(g->nos[i]->cidade, cidade) == 0){
            indice=i;
            break;
        }
    }
    //no_apaga(g->nos[indice]);
    for (int i = indice; i < g->tamanho-1; i++){
        g->nos[i]=g->nos[i+1];
    } 
    g->nos=(no_grafo**)realloc(g->nos, (g->tamanho-1)*sizeof(no_grafo*));
    g->tamanho--;  
    for (int i = 0; i < g->tamanho; i++){
        for (int j = 0; j < g->nos[i]->tamanho; j++){
            if(strcmp(g->nos[i]->arestas[j]->destino->cidade, cidade)==0){
                aresta_apaga(g->nos[i]->arestas[j]);
                g->nos[i]->arestas[j]=g->nos[i]->arestas[g->nos[i]->tamanho-1];
                g->nos[i]->arestas=(aresta_grafo**)realloc(g->nos[i]->arestas, (g->nos[i]->tamanho-1)*sizeof(aresta_grafo*));
                g->nos[i]->tamanho--;
            }
        }
    }
   
    return no;
}

int aresta_apaga(aresta_grafo *aresta)
{
    if (aresta==NULL){
        return -1;
    }
    free(aresta->codigo);
    free(aresta->companhia);
    free(aresta);
    return 0; 
}

int no_apaga(no_grafo *no)
{
    if (no==NULL) return -1;
    int t=no->tamanho;
    for(int i=0;i<t;i++){
        aresta_apaga(no->arestas[i]);
    } 
    free(no->arestas);
    free(no->cidade);
    free(no);
    return 0;
}

void grafo_apaga(grafo *g)
{
    if(g==NULL) return;
    
    int t=g->tamanho;
    for (int j= 0; j < t; j++){
        no_apaga(g->nos[j]);
    }
    free(g->nos);
    free(g); 
}

no_grafo *encontra_voo(grafo *g, char *codigo, int *aresta_pos)
{
   if(g==NULL) return NULL;
    for (int i = 0; i < g->tamanho; i++){
        for (int j = 0; j < g->nos[i]->tamanho; j++){
            if(strcmp(g->nos[i]->arestas[j]->codigo, codigo)==0){
            *aresta_pos=j;
            return g->nos[i];
            }
        }
    }
    return NULL;
}

no_grafo **pesquisa_avancada(grafo *g, char *destino, data chegada, double preco_max, int *n)
{
    if(g==NULL || preco_max<0){
        *n=0;
        return NULL;
    }
    no_grafo **aux;
    int cont=1, flag=0;
    aux=(no_grafo**)malloc(sizeof(no_grafo*));
    if(aux==NULL){
        *n=0;
        return NULL;
    }
    for (int i = 0; i < g->tamanho; i++){
        for (int j = 0; j < g->nos[i]->tamanho; j++){
            if(strcmp(g->nos[i]->arestas[j]->destino->cidade, destino)==0 && g->nos[i]->arestas[j]->chegada.tm_year == chegada.tm_year && g->nos[i]->arestas[j]->chegada.tm_mon == chegada.tm_mon && g->nos[i]->arestas[j]->chegada.tm_mday == chegada.tm_mday && g->nos[i]->arestas[j]->preco<=preco_max){
                flag=1;
                aux=(no_grafo**)realloc(aux, cont*sizeof(no_grafo*));
                if(aux==NULL){
                    *n=0;
                    return NULL;
                }
                aux[cont-1]=g->nos[i];
                cont++;
            }
        }
    }
    if(flag==0){
        free(aux);
        *n=0;
        return NULL;
    }
    *n=cont-1;
    return aux;
}

no_grafo **trajeto_mais_rapido(grafo *g, char *origem, char *destino, data partida, int *n)
{
    if(g==NULL){
        *n=0;
        return NULL;
    }
    int cont=1, valido;
    no_grafo **resultado, **resultadoinv, *val;
    heap *h=heap_nova(g->tamanho), *h1=heap_nova(g->tamanho);
    if(h==NULL || h1==NULL){
        *n=0;
        return NULL;
    }
    no_grafo *noorigem=encontra_no(g, origem), *aux, *proximo, *dest;
    if(noorigem==NULL){
        heap_apaga(h);
        heap_apaga(h1);
        *n=0;
        return NULL;
    }
    time_t atual, part, cheg;
    for (int i = 0; i < g->tamanho; i++){
        g->nos[i]->anterior=NULL;
        g->nos[i]->p_acumulado=0;
    }
    valido=heap_insere(h, noorigem, 0);
    if(valido==0){
        heap_apaga(h);
        heap_apaga(h1);
        *n=0;
        return NULL;
    }
    for (int i = 0; i < g->tamanho; i++){
        if(g->nos[i]!=noorigem){
            valido=heap_insere(h, g->nos[i], (__INT_MAX__-1000));
            if(valido==0){
                heap_apaga(h);
                heap_apaga(h1);
                *n=0;
                return NULL;
            }
        }
    }
    h->elementos[1]->no->p_acumulado=0;
    h->elementos[1]->no->anterior=NULL;
    h->elementos[1]->no->dataatualizada=&partida;
    while (h->tamanho>0){
        aux=heap_remove(h);
        if(aux==NULL){
            heap_apaga(h);
            heap_apaga(h1);
            *n=0;
            return NULL;
        }
        if(aux->dataatualizada==NULL) break;
        atual=mktime(aux->dataatualizada);
        for (int i = 0; i < aux->tamanho; i++){
            proximo=aux->arestas[i]->destino;
            part=mktime(&aux->arestas[i]->partida);
            cheg=mktime(&aux->arestas[i]->chegada);
            if(difftime(part, atual)<0) continue;
            while (h->tamanho>0 && strcmp(h->elementos[1]->no->cidade, proximo->cidade)!=0){
                valido=heap_insere(h1, h->elementos[1]->no, h->elementos[1]->prioridade);
                if(valido==0){
                    heap_apaga(h);
                    heap_apaga(h1);
                    *n=0;
                    return NULL;
                }
                val=heap_remove(h);
                if(val==NULL){
                    heap_apaga(h);
                    heap_apaga(h1);
                    *n=0;
                    return NULL;
                }
            }
            if (h->tamanho>0 && h->elementos[1]->prioridade>(difftime(cheg, part)+aux->p_acumulado+difftime(part, atual))){
                valido=heap_insere(h1, h->elementos[1]->no, difftime(cheg, part)+aux->p_acumulado+difftime(part, atual));
                if(valido==0){
                    heap_apaga(h);
                    heap_apaga(h1);
                    *n=0;
                    return NULL;
                }
                val=heap_remove(h);
                if(val==NULL){
                    heap_apaga(h);
                    heap_apaga(h1);
                    *n=0;
                    return NULL;
                }
                valido=heap_insere(h, h1->elementos[1]->no, h1->elementos[1]->prioridade);
                if(valido==0){
                    heap_apaga(h);
                    heap_apaga(h1);
                    *n=0;
                    return NULL;
                }
                h->elementos[1]->no->p_acumulado=h->elementos[1]->prioridade;
                h->elementos[1]->no->anterior=aux;
                h->elementos[1]->no->dataatualizada=&aux->arestas[i]->chegada;
                val=heap_remove(h1);
                if(val==NULL){
                    heap_apaga(h);
                    heap_apaga(h1);
                    *n=0;
                    return NULL;
                }
            }
            while (h1->tamanho>0){
                valido=heap_insere(h, h1->elementos[1]->no, h1->elementos[1]->prioridade);
                if(valido==0){
                    heap_apaga(h);
                    heap_apaga(h1);
                    *n=0;
                    return NULL;
                }
                val=heap_remove(h1);
                if(val==NULL){
                    heap_apaga(h);
                    heap_apaga(h1);
                    *n=0;
                    return NULL;
                }
            } 
        }
    }
    heap_apaga(h);
    heap_apaga(h1);
    dest=encontra_no(g, destino);
    if(dest==NULL || dest->anterior==NULL){
        *n=0;
        return NULL;
    }
    resultado=(no_grafo**)malloc(cont*sizeof(no_grafo*));
    if(resultado==NULL){
        *n=0;
        return NULL;
    }
    while (dest!=NULL){
        resultado=(no_grafo**)realloc(resultado, cont* sizeof(no_grafo*));
        if(resultado==NULL){
            *n=0;
            return NULL;
        }
        resultado[cont-1]=dest;
        cont++;
        dest=dest->anterior;
    }
    resultadoinv=(no_grafo**)malloc((cont-1)*sizeof(no_grafo*));
    if(resultadoinv==NULL){
        free(resultado);
        *n=0;
        return NULL;
    }
    for (int i = 0; i < cont-1; i++){
        resultadoinv[i]=resultado[cont-1-1-i];
    }
    free(resultado);
    *n=cont-1;
    return resultadoinv;
}

no_grafo **menos_transbordos(grafo *g, char *origem, char *destino, data partida, int *n)
{
    if(g==NULL){
        *n=0;
        return NULL;
    }
    heap *h=heap_nova(g->tamanho);
    if(h==NULL){
        *n=0;
        return NULL;
    }
    int prio, existe=0, valido;
    time_t atual, p, c;
    no_grafo *noorigem=encontra_no(g, origem), *aux, *nodestino=encontra_no(g, destino), **resultado;
    if(noorigem==NULL || nodestino==NULL){
        *n=0;
        return NULL;
    }
    for (int i = 0; i < g->tamanho; i++){
        g->nos[i]->anterior=NULL;
        g->nos[i]->p_acumulado=0;
    }
    nodestino->anterior=NULL;
    nodestino->p_acumulado=0;
    nodestino->dataatualizada=&partida;
    valido=heap_insere(h, noorigem, 1);
    if(valido==0){
        heap_apaga(h);
        *n=0;
        return NULL;
    }
    while (strcmp(h->elementos[1]->no->cidade, destino)!=0){
        prio=h->elementos[1]->prioridade;
        aux=heap_remove(h);
        if(aux==NULL){
            heap_apaga(h);
            *n=0;
            return NULL;
        }
        for (int i = 0; i < aux->tamanho; i++){
            atual=mktime(aux->dataatualizada);
            p=mktime(&aux->arestas[i]->partida);
            c=mktime(&aux->arestas[i]->chegada);
            //if(difftime(p, atual)<0) continue;
            existe=0;
            for (int j = 0; j < h->tamanho; j++){
                if (strcmp(h->elementos[j+1]->no->cidade, aux->arestas[i]->destino->cidade)==0){
                    existe=1;
                }
            }
            if(existe==0){
                aux->arestas[i]->destino->anterior=aux;
                aux->arestas[i]->destino->dataatualizada=&aux->arestas[i]->chegada;
                aux->arestas[i]->destino->p_acumulado=(aux->p_acumulado+difftime(p, c)+difftime(p, atual));
                valido=heap_insere(h, aux->arestas[i]->destino, prio+1);
                if(valido==0){
                    heap_apaga(h);
                    *n=0;
                    return NULL;
                }
            } 
        }  
        if(h->tamanho<1){
            heap_apaga(h);
            *n=0;
            return NULL;
        }
    }
    resultado=(no_grafo**)malloc(((int)h->elementos[1]->prioridade)*sizeof(no_grafo*));
    if(resultado==NULL){
        heap_apaga(h);
        *n=0;
        return NULL;
    }
    nodestino=h->elementos[1]->no;
    for (int i = 0; i < ((int)h->elementos[1]->prioridade); i++){
        resultado[((int)h->elementos[1]->prioridade)-1-i]=nodestino;
        nodestino=nodestino->anterior;
    }
    *n=((int)h->elementos[1]->prioridade);
    heap_apaga(h);
    return resultado;
}

aresta_grafo **atualiza_lugares(char *ficheiro, grafo *g, int *n)
{
    FILE *f=fopen(ficheiro, "r");
    if(f==NULL){
        *n=0;
        return NULL;
    }
    char aux[100], *codigo;
    int lugares, cont=1, flag=0;
    aresta_grafo **resultado;
    resultado=(aresta_grafo**)malloc(cont*sizeof(aresta_grafo*));
    if(resultado==NULL){
        *n=0;
        return NULL;
    }
    while (fgets(aux, 100, f)!=NULL){
        codigo=strtok(aux, ",");
        lugares=atoi(strtok(NULL, ","));
        for (int i = 0; i < g->tamanho; i++){
            for (int j = 0; j < g->nos[i]->tamanho; j++){
                if(strcmp(g->nos[i]->arestas[j]->codigo, codigo)==0){
                    if(lugares==0){
                        flag=1;
                        resultado=(aresta_grafo**)realloc(resultado, cont*sizeof(aresta_grafo*));
                        if(resultado==NULL){
                            *n=0;
                            return NULL;
                        }
                        resultado[cont-1]=g->nos[i]->arestas[j];
                        for (int k = j; k < g->nos[i]->tamanho-1; k++){
                            g->nos[i]->arestas[k]=g->nos[i]->arestas[k+1];
                        }
                        g->nos[i]->tamanho--;
                        cont++;
                    }
                    g->nos[i]->arestas[j]->lugares=lugares;
                }
            }
            
        }   
    }
    fclose(f);
    *n=cont-1;
    if(flag==0){
        free(resultado);
        return NULL;
    }
    return resultado;
}

grafo *grafo_importa(const char *nome_ficheiro)
{
    if (nome_ficheiro == NULL)
        return NULL;

    FILE *f = fopen(nome_ficheiro, "r");
    if (f == NULL)
        return NULL;

    grafo *g = grafo_novo();
    if (g == NULL)
    {
        fclose(f);
        return NULL;
    }
    char str[500] = {0}, codigo[15] = {0}, origem[50] = {0}, destino[50] = {0}, companhia[4] = {0};
    int lugares = 0;
    double preco = 0;
    struct tm partida, chegada;

    char *token;
    no_grafo *no_origem, *no_destino;
    int ret;
    while (fgets(str, 500 * sizeof(char), f) != NULL)
    {
        str[strlen(str) - 1] = '\0';

        token = strtok(str, ",");
        if (!token)
            break;
        strcpy(codigo, token);

        token = strtok(NULL, ",");
        strcpy(origem, token);

        token = strtok(NULL, ",");
        strcpy(destino, token);

        token = strtok(NULL, ",");
        partida.tm_year = atoi(token);

        token = strtok(NULL, ",");
        partida.tm_mon = atoi(token) - 1;

        token = strtok(NULL, ",");
        partida.tm_mday = atoi(token);

        token = strtok(NULL, ",");
        partida.tm_hour = atoi(token);

        token = strtok(NULL, ",");
        partida.tm_min = atoi(token);

        token = strtok(NULL, ",");
        chegada.tm_year = atoi(token);

        token = strtok(NULL, ",");
        chegada.tm_mon = atoi(token) - 1;

        token = strtok(NULL, ",");
        chegada.tm_mday = atoi(token);

        token = strtok(NULL, ",");
        chegada.tm_hour = atoi(token);

        token = strtok(NULL, ",");
        chegada.tm_min = atoi(token);

        token = strtok(NULL, ",");
        preco = atof(token);

        token = strtok(NULL, ",");
        lugares = atoi(token);

        token = strtok(NULL, ",");
        strcpy(companhia, token);

        // procura no grafo um nó com o nome da origem
        no_origem = encontra_no(g, origem);
        // se o nó ainda não existe, cria-o e insere-o no grafo
        if (!no_origem)
        {

            no_origem = no_insere(g, origem);

            if (!no_origem)
            {
                fclose(f);
                return NULL;
            }
        }
        // procura no grafo um nó com o nome do destino
        no_destino = encontra_no(g, destino);

        // se o nó ainda não existe, cria-o e insere-o no grafo
        if (!no_destino)
        {
            no_destino = no_insere(g, destino);

            if (!no_destino)
            {
                fclose(f);
                return NULL;
            }
        }

        if (existe_aresta(no_origem, no_destino, codigo) == 0)
        {

            ret = cria_aresta(no_origem, no_destino, codigo, companhia, partida, chegada, preco, lugares);

            if (ret == -1)
            {
                fclose(f);
                return NULL;
            }
        }
    }

    fclose(f);
    return g;
}