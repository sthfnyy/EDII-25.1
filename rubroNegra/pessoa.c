#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "interface_avp.h"

// Preenche a struct infoCidade com nome e população
void preencherPessoa(infoPessoa *info_pessoa, 
                        int cepCidadeNatal,
                        int cepCidadeAtual,
                        char *nome,
                        char *CPF,
                        char *nascimento) {
    strncpy(info_pessoa->nome, nome, TAM_STRING - 1);
    info_pessoa->nome[TAM_STRING - 1] = '\0';

    strncpy(info_pessoa->CPF, CPF, TAM_STRING - 1);
    info_pessoa->CPF[TAM_STRING - 1] = '\0';

    strncpy(info_pessoa->nascimento, nascimento, TAM_STRING - 1);
    info_pessoa->nascimento[TAM_STRING - 1] = '\0';

    info_pessoa->cepCidadeNatal = cepCidadeNatal;
    info_pessoa->cepCidadeAtual = cepCidadeAtual;
}

// retorna BLACK se nó for NULL, senão cor do nó
int corPessoa(Pessoa *noAtual)
{
    int resultado;
    if(noAtual == NULL)
        resultado = BLACK;
    else
        resultado = noAtual->cor;
    return resultado;
}

// inverte a cor do nó e de seus filhos, se existirem
void inverterCorPessoa(Pessoa *noAtual) {
    noAtual->cor = !noAtual->cor;
    if (noAtual->esq)
        noAtual->esq->cor = !noAtual->esq->cor;
    if (noAtual->dir)
        noAtual->dir->cor = !noAtual->dir->cor;
}

Pessoa *rotacionaEsquerdaPessoa(Pessoa *noAtual)
{
    Pessoa *novoNo = noAtual->dir;
    noAtual->dir = novoNo->esq;
    novoNo->esq = noAtual;
    novoNo->cor = noAtual->cor;
    noAtual->cor = RED;
    return novoNo;
}

Pessoa *rotacionaDireitaPessoa(Pessoa *noAtual)
{
    Pessoa *novoNo = noAtual->esq;
    noAtual->esq = novoNo->dir;
    novoNo->dir = noAtual;
    novoNo->cor = noAtual->cor;
    noAtual->cor = RED;
    return novoNo;
}

// Aloca e inicializa um novo nó da árvore rubro-negra para Cidade
Pessoa *alocarPessoa(infoPessoa dados, int *resp)
{
    Pessoa *novoNo = (Pessoa*) malloc(sizeof(Pessoa));
    if (novoNo == NULL) 
    {
        *resp = 0;
        printf("Erro ao alocar memoria para Pessoa.\n");
    }else{
        novoNo->infoPessoa = dados;
        novoNo->esq = NULL;
        novoNo->dir = NULL;
        novoNo->cor = RED;
        *resp = 1;
    }
    return novoNo;
}

int inserirPessoa(Pessoa **raiz, infoPessoa dados) 
{
    int inseriu = 0;
    // Função responsavel pela busca ddo local de inserção do nó
    *raiz = inserirNOPessoa(&(*raiz),dados,&inseriu);
    if ((*raiz) != NULL)
        (*raiz)->cor = BLACK;

    return inseriu;
}

Pessoa *inserirNOPessoa(Pessoa **raiz, infoPessoa dados, int *resp) 
{
    *resp = 1;
    int resp_2;
    if (*raiz == NULL){
        Pessoa *novoNo;
        novoNo = alocarPessoa(dados, &resp_2);
        if (resp_2 != 0)
            (*raiz) = novoNo;

    // Caso onde o dado que vai ser inserido já existe
    }else{ 
        if(strcmp(dados.nome, (*raiz)->infoPessoa.nome) == 0){
            *resp = 0;
        }
        else{
            if (strcmp(dados.nome, (*raiz)->infoPessoa.nome) < 0){
                (*raiz)->esq = inserirNOPessoa(&(*raiz)->esq,dados,resp);
            }else{
                (*raiz)->dir = inserirNOPessoa(&(*raiz)->dir,dados,resp);
            }
        }

        // caso 1: filho direito vermelho + filho esquerdo preto → roda à esquerda
        if (corPessoa((*raiz)->dir) == RED && corPessoa((*raiz)->esq) == BLACK) 
        {
            *raiz = rotacionaEsquerdaPessoa(*raiz);
        }
        // caso 2: filho esquerdo e neto esquerdo vermelhos → roda à direita
        if (corPessoa((*raiz)->esq) == RED && corPessoa((*raiz)->esq->esq) == RED) 
        {
            *raiz = rotacionaDireitaPessoa(*raiz);
        }
        // caso 3: dois filhos vermelhos → inverte cores
        if (corPessoa((*raiz)->esq) == RED && corPessoa((*raiz)->dir) == RED) 
        {
            inverterCorPessoa(*raiz);
        }
    }

    return (*raiz);
}

// imprime em ordem (in-order) os CEPs com sua cor
void imprimirPessoa(Pessoa *raiz) 
{
    if (raiz != NULL) 
    {
        imprimirPessoa(raiz->esq);
        printf("Pessoa: %s, cor: %s\n",
               raiz->infoPessoa.nome,
               (raiz->cor == RED) ? "RED" : "BLACK");
        imprimirPessoa(raiz->dir);
    }
}

// procura recursivamente por valor de CEP na ABB de CEPs
Pessoa *buscarPessoaNome(Pessoa *raiz, char *nome) 
{
    Pessoa *resultado;
    if (raiz == NULL) 
    {
        resultado = NULL;
    }
    else{
        int cmp = strcmp(nome, raiz->infoPessoa.nome); 
        if (cmp == 0) 
        {
            resultado = raiz;
        }
        else if (cmp < 0) 
        {
            resultado = buscarPessoaNome(raiz->esq, nome);
        }
        else {
            resultado = buscarPessoaNome(raiz->dir, nome);
        }
    }
    return resultado;
}

// procura recursivamente por valor de CEP de Nacimento das pessoas na ABRB de Pessoas
Pessoa *consultaPessoaCEPNacimento(Pessoa *raiz, int CEP, int *encontrado) 
{
    Pessoa *aux = NULL;
    if (raiz != NULL) 
    {
        if (raiz->infoPessoa.cepCidadeNatal == CEP) 
        {
            aux = raiz;
            *encontrado = 1;
        }
        // verifica primeiro nos ramos da esquerda e depois direita, caso for encontrado a varaivel incontrado já terá sido setada para 1
        if (*encontrado == 0)
        {
            aux = consultaPessoaCEPNacimento(raiz->esq, CEP, encontrado);
        }
        if (*encontrado == 0)
        {
            aux = consultaPessoaCEPNacimento(raiz->dir, CEP, encontrado);
        }
    }
    return aux;
}

// procura recursivamente por valor de CEP de Atual das pessoas na ABRB de Pessoas
Pessoa *consultaPessoaCEPAtual(Pessoa *raiz, int CEP, int *encontrado) 
{
    Pessoa *aux = NULL;
    if (raiz != NULL) 
    {
        if (raiz->infoPessoa.cepCidadeAtual == CEP) 
        {
            aux = raiz;
            *encontrado = 1;
        }
        // Ferifica primeiro nos ramos da esquerda e depois direita, caso for encontrado a varaivel incontrado já terá sido setada para 1
        if (*encontrado == 0)
        {
            aux = consultaPessoaCEPAtual(raiz->esq, CEP, encontrado);
        }
        if (*encontrado == 0)
        {
            aux = consultaPessoaCEPAtual(raiz->dir, CEP, encontrado);
        }
    }
    return aux;
}

int consultaPessoaCPF(Pessoa *raiz, const char *CPF)
{
    int resultado = 0;
    if (raiz == NULL) 
    {
        resultado = 0; // não encontrado
    }
    else if (strcmp(raiz->infoPessoa.CPF, CPF) == 0) 
    {
        resultado = 1; // encontrado
    }
    else 
    {
        resultado = consultaPessoaCPF(raiz->esq, CPF);
        if (resultado == 0) 
        {
            resultado = consultaPessoaCPF(raiz->dir, CPF);
        }
    }
    return resultado;
}

Pessoa *removeNoPessoa(Pessoa *noAtual, char *nome) 
{
    if (strcmp(nome, noAtual->infoPessoa.nome) < 0) 
    {
        if (noAtual->esq != NULL) 
        {
            if (corPessoa(noAtual->esq) == BLACK && corPessoa(noAtual->esq->esq) == BLACK)
                noAtual = move2EsqREDPessoa(noAtual);
            noAtual->esq = removeNoPessoa(noAtual->esq, nome);
        }
    } else {
        
        if (corPessoa(noAtual->esq) == RED)
            noAtual = rotacionaDireitaPessoa(noAtual);

        if (strcmp(nome, noAtual->infoPessoa.nome) == 0 && noAtual->dir == NULL) 
        {
            free(noAtual);
            return NULL;
        }

        if (noAtual->dir != NULL) 
        {

            if (corPessoa(noAtual->dir) == BLACK && corPessoa(noAtual->dir->esq) == BLACK)
                noAtual = move2DirREDPessoa(noAtual);

            if (strcmp(nome, noAtual->infoPessoa.nome) == 0) {
                Pessoa *aux = procurarMenorPessoa(noAtual->dir);
                noAtual->infoPessoa = aux->infoPessoa; // copiar toda a struct se necessário
                noAtual->dir = removeNoPessoa(noAtual->dir, aux->infoPessoa.nome);
            } else {
                noAtual->dir = removeNoPessoa(noAtual->dir, nome);
            }
        }
    }

    return balancearPessoa(noAtual); // importante para manter as propriedades da árvore
}

int removerPessoa(Pessoa **raiz, char *nome) 
{
    int removido = 0; // 0 = falhou, 1 = removeu

    if (*raiz == NULL) 
    {
        return removido; // árvore vazia
    }


    if (buscarPessoaNome(*raiz, nome)) 
    {
        *raiz = removeNoPessoa(*raiz, nome);
        
        removido = 1;
        if (*raiz != NULL) 
        {
            (*raiz)->cor = BLACK; // raiz sempre preta
        }
    }

    return removido;
}


Pessoa *move2EsqREDPessoa(Pessoa *noAtual) 
{
    inverterCorPessoa(noAtual);
    if (corPessoa(noAtual->dir->esq) == RED) 
    {
        noAtual->dir = rotacionaDireitaPessoa(noAtual->dir);
        noAtual = rotacionaEsquerdaPessoa(noAtual);
        inverterCorPessoa(noAtual);
    }
    return noAtual;
}

Pessoa *move2DirREDPessoa(Pessoa *noAtual) 
{
    inverterCorPessoa(noAtual);
    if (corPessoa(noAtual->esq->esq) == RED) 
    {
        noAtual = rotacionaDireitaPessoa(noAtual);
        inverterCorPessoa(noAtual);
    }
    return noAtual;
}

Pessoa *balancearPessoa(Pessoa *noAtual) 
{
    if (corPessoa(noAtual->dir) == RED) 
    {
        noAtual = rotacionaEsquerdaPessoa(noAtual);
    }
    
    if (noAtual->esq != NULL && corPessoa(noAtual->esq) == RED && corPessoa(noAtual->esq->esq) == RED) 
    {
        noAtual = rotacionaDireitaPessoa(noAtual);
    }

    if (corPessoa(noAtual->esq) == RED && corPessoa(noAtual->dir) == RED) 
    {
        inverterCorPessoa(noAtual);
    }

    return noAtual;
}

Pessoa *removerMenorPessoa(Pessoa *noAtual)
{
    if (noAtual->esq == NULL) 
    {
        free(noAtual);
        return NULL;
    }
    
    if (corPessoa(noAtual->esq) == BLACK && corPessoa(noAtual->esq->esq) == BLACK) 
        noAtual = move2EsqREDPessoa(noAtual);
    
    noAtual->esq = removerMenorPessoa(noAtual->esq);
    
    return balancearPessoa(noAtual);
}

Pessoa *procurarMenorPessoa(Pessoa *atual) 
{
    Pessoa *no1 = atual;
    Pessoa *no2 = atual->esq;

    while (no2 != NULL) 
    {
        no1 = no2;
        no2 = no2->esq;
    }
    return no1;
}

int Quantas_pessoas_não_moram_na_cidade_natal(Pessoa *raiz, Estado *estados) 
{
    int cont = 0;
    Estado *auxEstado;
    Cidade *auxCidade1, *auxCidade2;

    if (raiz != NULL) 
    {

        // Buscarndo cidade natal
        auxEstado = buscar_estado_Por_CEP(estados, raiz->infoPessoa.cepCidadeAtual);
        auxCidade1 = buscarCidadePorCEP(auxEstado->infoEstado.arv_cidade, raiz->infoPessoa.cepCidadeNatal);

        // Buscar cidade onde mora
        auxEstado = buscar_estado_Por_CEP(estados, raiz->infoPessoa.cepCidadeAtual);
        auxCidade2 = buscarCidadePorCEP(auxEstado->infoEstado.arv_cidade, raiz->infoPessoa.cepCidadeAtual);


        if(auxCidade1 != NULL && auxCidade2 != NULL){
            if(strcmp(auxCidade1->infoCidade.nome_cidade, auxCidade2->infoCidade.nome_cidade) != 0){
                // Se a cidade natal for maior que a cidade onde mora, incrementa o contador
                cont += 1;
            }
        }

        cont += Quantas_pessoas_não_moram_na_cidade_natal(raiz->esq, estados);
        cont += Quantas_pessoas_não_moram_na_cidade_natal(raiz->dir, estados);

    }

    return cont;
}

int numero_de_pessoas_nascidas_em_uma_determinada_cidade_nao_mora_na_cidade_natal(Pessoa *raiz, Cidade *cidade){
    int cont = 0;
    CEP *auxCEPNatal, *auxCEPAtual;
    if (raiz != NULL) 
    {
        auxCEPNatal = buscarCEP(cidade->infoCidade.avr_CEP, raiz->infoPessoa.cepCidadeNatal);
        auxCEPAtual = buscarCEP(cidade->infoCidade.avr_CEP, raiz->infoPessoa.cepCidadeAtual);
        // A cidade natal deve ser encontrada enquanto a cidade atual não deve ser encontrada
        if(auxCEPNatal != NULL && auxCEPAtual == NULL){
            cont += 1;
        }

        cont += numero_de_pessoas_nascidas_em_uma_determinada_cidade_nao_mora_na_cidade_natal(raiz->esq, cidade);
        cont += numero_de_pessoas_nascidas_em_uma_determinada_cidade_nao_mora_na_cidade_natal(raiz->dir, cidade);

    }
    return cont;
}

int numero_de_pessoas_que_moram_em_uma_determinada_cidade_nao_nasceram_nela(Pessoa *raiz, Cidade *cidade){
    int cont = 0;
    CEP *auxCEPNatal, *auxCEPAtual;
    if (raiz != NULL) 
    {
        auxCEPNatal = buscarCEP(cidade->infoCidade.avr_CEP, raiz->infoPessoa.cepCidadeNatal);
        auxCEPAtual = buscarCEP(cidade->infoCidade.avr_CEP, raiz->infoPessoa.cepCidadeAtual);
        // A cidade natal não deve ser encontrada equanto a cidade atual deve ser encontrada
        if(auxCEPNatal == NULL && auxCEPAtual != NULL){
            cont += 1;
        }

        cont += numero_de_pessoas_que_moram_em_uma_determinada_cidade_nao_nasceram_nela(raiz->esq, cidade);
        cont += numero_de_pessoas_que_moram_em_uma_determinada_cidade_nao_nasceram_nela(raiz->dir, cidade);

    }
    return cont;
}


