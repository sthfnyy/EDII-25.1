#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "interface_avp.h"

// Preenche a struct infoCidade com nome e população
void preencherCidade(infoCidade *cidade, char *nome_cidade, int populacaoC) {
    strncpy(cidade->nome_cidade, nome_cidade, TAM_STRING - 1);
    cidade->nome_cidade[TAM_STRING - 1] = '\0';
    cidade->populacao_c = populacaoC;
    //cidade->avr_CEP = NULL;
}

// retorna BLACK se nó for NULL, senão cor do nó
int cor(Cidade *noAtual)
{
    int resultado;
    if(noAtual == NULL)
        resultado = BLACK;
    else
        resultado = noAtual->cor;
    return resultado;
}

// inverte a cor do nó e de seus filhos, se existirem
void inverterCor(Cidade *noAtual) {
    noAtual->cor = !noAtual->cor;
    if (noAtual->esq)
        noAtual->esq->cor = !noAtual->esq->cor;
    if (noAtual->dir)
        noAtual->dir->cor = !noAtual->dir->cor;
}

Cidade *rotacionaEsquerda(Cidade *noAtual)
{
    Cidade *novoNo = noAtual->dir;
    noAtual->dir = novoNo->esq;
    novoNo->esq = noAtual;
    novoNo->cor = noAtual->cor;
    noAtual->cor = RED;
    return novoNo;
}

Cidade *rotacionaDireita(Cidade *noAtual)
{
    Cidade *novoNo = noAtual->esq;
    noAtual->esq = novoNo->dir;
    novoNo->dir = noAtual;
    novoNo->cor = noAtual->cor;
    noAtual->cor = RED;
    return novoNo;
}

// Aloca e inicializa um novo nó da árvore rubro-negra para Cidade
Cidade *alocarCidade(infoCidade dados, int *resp)
{
    Cidade *novoNo = (Cidade*) malloc(sizeof(Cidade));
    if (novoNo == NULL) 
    {
        *resp = 0;
        printf("Erro ao alocar memoria para cidade.\n");
        return NULL;
    }
    novoNo->infoCidade = dados;
    novoNo->esq = NULL;
    novoNo->dir = NULL;
    novoNo->cor = RED;
    novoNo->infoCidade.avr_CEP = NULL;
    *resp = 1;
    return novoNo;
}

int inserirCidade_old(Cidade **raiz, infoCidade dados) 
{
    int inseriu = 0;

    if (*raiz == NULL) {
        // cria a raiz
        *raiz = alocarCidade(dados, &inseriu);
        if (*raiz)
            (*raiz)->cor = BLACK;  // raiz sempre preta
    }
    else {
        int inseriuSub = 0;
        if (strcmp(dados.nome_cidade, (*raiz)->infoCidade.nome_cidade) < 0) 
        {
            inseriuSub = inserirCidade(&(*raiz)->esq, dados);
        }
        else if (strcmp(dados.nome_cidade, (*raiz)->infoCidade.nome_cidade) > 0) 
        {
            inseriuSub = inserirCidade(&(*raiz)->dir, dados);
        }
        else {
            inseriuSub = 0;  // já existe
        }
        inseriu = inseriuSub;
    }

    // só faz rotações e flip se inseriu algo
    if (inseriu) 
    {
        // caso 1: filho direito vermelho + filho esquerdo preto → roda à esquerda
        if (cor((*raiz)->dir) == RED && cor((*raiz)->esq) == BLACK) 
        {
            *raiz = rotacionaEsquerda(*raiz);
        }
        // caso 2: filho esquerdo e neto esquerdo vermelhos → roda à direita
        if (cor((*raiz)->esq) == RED && cor((*raiz)->esq->esq) == RED) 
        {
            *raiz = rotacionaDireita(*raiz);
        }
        // caso 3: dois filhos vermelhos → inverte cores
        if (cor((*raiz)->esq) == RED && cor((*raiz)->dir) == RED) 
        {
            inverterCor(*raiz);
        }
    }

    return inseriu;
}

Cidade *inserirNOCidade(Cidade **raiz, infoCidade dados, int *resp) 
{
    *resp = 1;
    int resp_2;
    if (*raiz == NULL){
        Cidade *novoNo;
        novoNo = alocarCidade(dados, &resp_2);
        if (resp_2 != 0)
            (*raiz) = novoNo;

    // Caso onde o dado que vai ser inserido já existe
    }else{ 
        if(strcmp(dados.nome_cidade, (*raiz)->infoCidade.nome_cidade) == 0){
            *resp = 0;
        }
        else{
            if (strcmp(dados.nome_cidade, (*raiz)->infoCidade.nome_cidade) < 0){
                (*raiz)->esq = inserirNOCidade(&(*raiz)->esq,dados,resp);
            }else{
                (*raiz)->dir = inserirNOCidade(&(*raiz)->dir,dados,resp);
            }
        }

        // caso 1: filho direito vermelho + filho esquerdo preto → roda à esquerda
        if (cor((*raiz)->dir) == RED && cor((*raiz)->esq) == BLACK) 
        {
            *raiz = rotacionaEsquerda(*raiz);
        }
        // caso 2: filho esquerdo e neto esquerdo vermelhos → roda à direita
        if (cor((*raiz)->esq) == RED && cor((*raiz)->esq->esq) == RED) 
        {
            *raiz = rotacionaDireita(*raiz);
        }
        // caso 3: dois filhos vermelhos → inverte cores
        if (cor((*raiz)->esq) == RED && cor((*raiz)->dir) == RED) 
        {
            inverterCor(*raiz);
        }
    }

    return (*raiz);
}

int inserirCidade(Cidade **raiz, infoCidade dados)
{
    int inseriu = 0;
    // Função responsavel pela busca ddo local de inserção do nó
    *raiz = inserirNOCidade(&(*raiz),dados,&inseriu);
    if ((*raiz) != NULL)
        (*raiz)->cor = BLACK;

    return inseriu;
}

// em cidade.c ou utils.c

// Retorna 1 se inseriu com sucesso, 0 se cidade não existe ou CEP duplicado
int cadastrarCEPEmCidade(Cidade *raizCidades, const char *nomeCidade, infoCEP dadosCEP)
{
    // 1) Procura a cidade
    Cidade *cidade = buscarCidade(raizCidades, nomeCidade);
    if (!cidade) return 0;

    // 2) Insere na RBT de CEPs da cidade
    //    (usa a raizCEPs que acabamos de inicializar em alocarCidade)
    return inserirCEP(&cidade->infoCidade.avr_CEP, dadosCEP);
}


// procura recursivamente por nome na ABB de Cidades --- essa busca retorna o valor, por isso no menu tem que ser igual a null ?
Cidade *buscarCidade(Cidade *raiz, const char *nome_cidade) 
{
    Cidade *resultado;

    if (raiz == NULL) 
    {
        resultado = NULL;
    }
    else {
        int cmp = strcmp(nome_cidade, raiz->infoCidade.nome_cidade);
        if (cmp == 0) 
        {
            resultado = raiz;
        }
        else if (cmp < 0) 
        {
            resultado = buscarCidade(raiz->esq, nome_cidade);
        }
        else {
            resultado = buscarCidade(raiz->dir, nome_cidade);
        }
    }
    return resultado;
}

Cidade *buscarCidadePorCEP(Cidade *raiz, int valorCEP){ //essa também retorna o valor, por isso no menu tem que ser igual a null ?
    Cidade *resultado;
    CEP *auxCPF;

    if (raiz == NULL) 
    {
        resultado = NULL;
    }
    else {
        auxCPF = buscarCEP(raiz->infoCidade.avr_CEP, valorCEP);
        if (auxCPF) 
        {
            resultado = raiz;
        }
        else 
        {
            resultado = buscarCidadePorCEP(raiz->esq, valorCEP);
            if (resultado == NULL){
                resultado = buscarCidadePorCEP(raiz->dir, valorCEP);
            }
        }
    }
    return resultado;
}

void imprimir(Cidade *raiz) 
{
    if (raiz != NULL) 
    {
        imprimir(raiz->esq);
        printf("Cidade: %s | Populacao: %d | Cor: %s\n",
               raiz->infoCidade.nome_cidade,
               raiz->infoCidade.populacao_c,
               (raiz->cor == RED) ? "RED" : "BLACK");
        imprimir(raiz->dir);
    }
}

int atualizarCidade(Cidade *CidadeAtual, infoCidade novosDados) 
{
    int resultado = 0;
    if (CidadeAtual != NULL) 
    {
        // Atualiza os dados da cidade
        strncpy(CidadeAtual->infoCidade.nome_cidade, novosDados.nome_cidade, TAM_STRING - 1);
        CidadeAtual->infoCidade.nome_cidade[TAM_STRING - 1] = '\0';
        CidadeAtual->infoCidade.populacao_c = novosDados.populacao_c;
        resultado = 1;
    }
    
    return resultado;
}

// libera todos os nós da árvore rubro-negra de Cidades
void liberar_arvore_cidades(Cidade *raiz) 
{
    if (raiz != NULL) 
    {
        liberar_arvore_cidades(raiz->esq);
        liberar_arvore_cidades(raiz->dir);
        free(raiz);
    }
}

Cidade *cidade_mais_populosa_de_um_estado_sem_ser_a_capital(Cidade *raiz, char *nome_da_capital) 
{

    // Começamos supondo que o maior válido é NULL
    Cidade* maiorAtual = NULL;

    if (raiz != NULL){
        // Busca recursiva nos dois filhos
        Cidade *maiorEsq = cidade_mais_populosa_de_um_estado_sem_ser_a_capital(raiz->esq, nome_da_capital);
        Cidade *maiorDir = cidade_mais_populosa_de_um_estado_sem_ser_a_capital(raiz->dir, nome_da_capital);

        // Verifica se o valor atual é diferente de n
        if (strcmp(raiz->infoCidade.nome_cidade, nome_da_capital) != 0)
            maiorAtual = raiz;

        // Compara com o da esquerda
        if (maiorEsq != NULL && (maiorAtual == NULL || maiorEsq->infoCidade.populacao_c > maiorAtual->infoCidade.populacao_c))
            maiorAtual = maiorEsq;

        // Compara com o da direita
        if (maiorDir != NULL && (maiorAtual == NULL || maiorDir->infoCidade.populacao_c > maiorAtual->infoCidade.populacao_c))
            maiorAtual = maiorDir;
    }

    return maiorAtual;
}