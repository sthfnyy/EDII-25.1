#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "interface_avp.h"

// Preenche a struct infoCEP com o valor do CEP
void preencherCEP(infoCEP *dados, int numCEP) 
{
    dados->cep = numCEP;
}

// retorna BLACK se nó for NULL, senão cor do nó
int corCEP(CEP *noAtual) 
{
    int resultado;
    if (noAtual == NULL)
        resultado = BLACK;
    else
        resultado = noAtual->cor;
    return resultado;
}

// inverte a cor do nó e de seus filhos, se existirem
void inverterCorCEP(CEP *noAtual) 
{
    noAtual->cor = !noAtual->cor;
    if (noAtual->esq)
        noAtual->esq->cor = !noAtual->esq->cor;
    if (noAtual->dir)
        noAtual->dir->cor = !noAtual->dir->cor;
}

// rotação à esquerda em CEP
CEP *rotacionaEsquerdaCEP(CEP *noAtual) 
{
    CEP *novoNo = noAtual->dir;
    noAtual->dir   = novoNo->esq;
    novoNo->esq    = noAtual;
    novoNo->cor    = noAtual->cor;
    noAtual->cor   = RED;
    return novoNo;
}

// rotação à direita em CEP
CEP *rotacionaDireitaCEP(CEP *noAtual) 
{
    CEP *novoNo = noAtual->esq;
    noAtual->esq   = novoNo->dir;
    novoNo->dir    = noAtual;
    novoNo->cor    = noAtual->cor;
    noAtual->cor   = RED;
    return novoNo;
}

// Aloca e inicializa um novo nó rubro-negro para CEP
CEP *alocarCEP(infoCEP dados, int *resp) 
{
    CEP *novoNo = (CEP*) malloc(sizeof(CEP));
    if (novoNo == NULL) 
    {
        *resp = 0;
        printf("Erro ao alocar memória para CEP.\n");
    } else {
        novoNo->infoCEP = dados;
        novoNo->esq     = NULL;
        novoNo->dir     = NULL;
        novoNo->cor     = RED;
        *resp           = 1;
    }
    return novoNo;
}


int inserirCEP(CEP **raiz, infoCEP dados)
{
    int inseriu = 0;
    // Função responsavel pela busca ddo local de inserção do nó
    *raiz = inserirNOCEP(&(*raiz),dados,&inseriu);
    if ((*raiz) != NULL)
        (*raiz)->cor = BLACK;

    return inseriu;
}



// Insere um CEP na RBT, faz rotações e flip de cores — só um return no fim
CEP *inserirNOCEP(CEP **raiz, infoCEP dados, int *resp) 
{
    *resp = 1;
    int resp_2;
    if (*raiz == NULL){
        CEP *novoNo;
        novoNo = alocarCEP(dados, &resp_2);
        if (resp_2 != 0)
            (*raiz) = novoNo;

    // Caso onde o dado que vai ser inserido já existe
    }else{ 
        if(dados.cep == (*raiz)->infoCEP.cep){
            *resp = 0;
        }
        else{
            if (dados.cep < (*raiz)->infoCEP.cep){
                (*raiz)->esq = inserirNOCEP(&(*raiz)->esq,dados,resp);
            }else{
                (*raiz)->dir = inserirNOCEP(&(*raiz)->dir,dados,resp);
            }
        }

        // caso 1: filho direito vermelho + filho esquerdo preto → roda à esquerda
        if (corCEP((*raiz)->dir) == RED && corCEP((*raiz)->esq) == BLACK) 
        {
            *raiz = rotacionaEsquerdaCEP(*raiz);
        }
        // caso 2: filho esquerdo e neto esquerdo vermelhos → roda à direita
        if (corCEP((*raiz)->esq) == RED && corCEP((*raiz)->esq->esq) == RED) 
        {
            *raiz = rotacionaDireitaCEP(*raiz);
        }
        // caso 3: dois filhos vermelhos → inverte cores
        if (corCEP((*raiz)->esq) == RED && corCEP((*raiz)->dir) == RED) 
        {
            inverterCorCEP(*raiz);
        }
    }

    return (*raiz);
}

// procura recursivamente por valor de CEP na ABB de CEPs
CEP *buscarCEP(CEP *raiz, int valorCEP) 
{
    CEP *resultado;
    if (raiz == NULL) 
    {
        resultado = NULL;
    }
    else if (valorCEP == raiz->infoCEP.cep) 
    {
        resultado = raiz;
    }
    else if (valorCEP < raiz->infoCEP.cep) 
    {
        resultado = buscarCEP(raiz->esq, valorCEP);
    }
    else {
        resultado = buscarCEP(raiz->dir, valorCEP);
    }
    return resultado;
}

// imprime em ordem (in-order) os CEPs com sua cor
void imprimirCEP(CEP *raiz) 
{
    if (raiz != NULL) 
    {
        imprimirCEP(raiz->esq);
        printf("CEP: %08d | cor: %s\n",
               raiz->infoCEP.cep,
               (raiz->cor == RED) ? "RED" : "BLACK");
        imprimirCEP(raiz->dir);
    }
}

// libera todos os nós da árvore rubro-negra de CEPs
void liberarArvoreCEP(CEP *raiz) 
{
    if (raiz != NULL) 
    {
        liberarArvoreCEP(raiz->esq);
        liberarArvoreCEP(raiz->dir);
        free(raiz);
    }
}

CEP *balancear(CEP *noAtual)
{

    if (corCEP(noAtual->dir) == RED) 
    {
        noAtual = rotacionaEsquerdaCEP(noAtual);
    }
    
    if (noAtual->esq != NULL && corCEP(noAtual->esq) == RED && corCEP(noAtual->esq->esq) == RED) 
    {
        noAtual = rotacionaDireitaCEP(noAtual);
    }

    if (corCEP(noAtual->esq) == RED && corCEP(noAtual->dir) == RED) 
    {
        inverterCorCEP(noAtual);
    }

    return noAtual;
}

CEP *move2EsqRED(CEP *noAtual)
{
    inverterCorCEP(noAtual);
    if (corCEP(noAtual->dir->esq) == RED)
    {
        noAtual->dir = rotacionaDireitaCEP(noAtual->dir);
        noAtual = rotacionaEsquerdaCEP(noAtual);
        inverterCorCEP(noAtual);
    }
    return noAtual;
}


CEP *move2DirRED(CEP *noAtual)
{
    inverterCorCEP(noAtual);
    if (corCEP(noAtual->esq->esq) == RED)
    {
        noAtual = rotacionaDireitaCEP(noAtual);
        inverterCorCEP(noAtual);
    }
    return noAtual;
}

CEP *removerMenor(CEP *noAtual)
{
    if (noAtual->esq == NULL)
    {
        free(noAtual);
        return NULL;
    }
    if (corCEP(noAtual->esq) == BLACK && corCEP(noAtual->esq->esq) == BLACK)
        noAtual = move2EsqRED(noAtual);

    noAtual->esq = removerMenor(noAtual->esq);
    return balancear(noAtual);
}

CEP *procurarMenor(CEP *atual)
{
    CEP *no1 = atual;
    CEP *no2 = atual->esq;

    while (no2 != NULL)
    {
        no1 = no2;
        no2 = no2->esq;
    }
    return no1;
}

CEP *removeNoCEP(CEP *noAtual, int cep)
{
    if (cep < noAtual->infoCEP.cep) 
    {
        if (noAtual->esq != NULL) 
        {
            if (corCEP(noAtual->esq) == BLACK && corCEP(noAtual->esq->esq) == BLACK)
                noAtual = move2EsqRED(noAtual);
            noAtual->esq = removeNoCEP(noAtual->esq, cep);
        }
    } else {
        
        if (corCEP(noAtual->esq) == RED)
            noAtual = rotacionaDireitaCEP(noAtual);

        if (cep == noAtual->infoCEP.cep && noAtual->dir == NULL) 
        {
            free(noAtual);
            return NULL;
        }

        if (noAtual->dir != NULL) 
        {

            if (corCEP(noAtual->dir) == BLACK && corCEP(noAtual->dir->esq) == BLACK)
                noAtual = move2DirRED(noAtual);

            if (cep == noAtual->infoCEP.cep) {
                CEP *aux = procurarMenor(noAtual->dir);
                noAtual->infoCEP = aux->infoCEP; // copiar toda a struct se necessário
                noAtual->dir = removeNoCEP(noAtual->dir, aux->infoCEP.cep);
            } else {
                noAtual->dir = removeNoCEP(noAtual->dir, cep);
            }
        }
    }

    return balancear(noAtual); // importante para manter as propriedades da árvore
}


int removerCEP(CEP **raiz, int valorCEP)
{

    int removido = 0; // 0 = falhou, 1 = removeu

    if (*raiz == NULL) 
    {
        return removido; // árvore vazia
    }


    if (buscarCEP(*raiz, valorCEP)) 
    {
        *raiz = removeNoCEP(*raiz, valorCEP);
        
        removido = 1;
        if (*raiz != NULL) 
        {
            (*raiz)->cor = BLACK; // raiz sempre preta
        }
    }

    return removido;
}