#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "interface_b.h"


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


Pessoa *alocarPessoa(infoPessoa dado, Pessoa *fEsq, Pessoa *fCen) 
{
    Pessoa *novo = (Pessoa*) malloc(sizeof(Pessoa));
    if (novo != NULL) {
        novo->info_um = dado;
        novo->qntd_info = 1;
        novo->esq = NULL;
        novo->esq = fEsq;
        novo->cen = NULL;
        novo->cen = fCen;
        novo->dir = NULL;
    }
    return novo;
}


void inserir_infoPessoa(Pessoa **novoNo, infoPessoa dado, Pessoa *subArvInfo) 
{
    if (strcmp(dado.nome, (*novoNo)->info_um.nome) > 0) {
        (*novoNo)->info_dois = dado;
        (*novoNo)->dir = subArvInfo;
    } else {
        (*novoNo)->info_dois = (*novoNo)->info_um;
        (*novoNo)->info_um = dado;
        (*novoNo)->dir = (*novoNo)->cen;
        (*novoNo)->cen = subArvInfo;
    }
    (*novoNo)->qntd_info = 2;
}

Pessoa *quebrarNoPessoa(Pessoa **no, infoPessoa dado, infoPessoa *sobe, Pessoa *filhoDir) 
{
    Pessoa *maior;

    if (strcmp(dado.nome, (*no)->info_dois.nome) > 0) {
        *sobe = (*no)->info_dois;
        maior = alocarPessoa(dado, (*no)->dir, filhoDir);
        (*no)->qntd_info = 1;
    } else if (strcmp(dado.nome, (*no)->info_um.nome) > 0) {
        *sobe = dado;
        maior = alocarPessoa((*no)->info_dois, filhoDir, (*no)->dir);
        (*no)->qntd_info = 1;
    } else {
        *sobe = (*no)->info_um;
        maior = alocarPessoa((*no)->info_dois, (*no)->cen, (*no)->dir);
        (*no)->info_um = dado;
        (*no)->cen = filhoDir;
        (*no)->qntd_info = 1;
    }

    return maior;
}

Pessoa *inserirPessoa(Pessoa **raiz, Pessoa *pai, infoPessoa dado, infoPessoa *sobe)
{
    Pessoa *maiorNo = NULL;

    if (*raiz == NULL) {
        *raiz = alocarPessoa(dado, NULL, NULL);
    } else if ((*raiz)->esq == NULL) {
        if ((*raiz)->qntd_info == 1) {
            inserir_infoPessoa(raiz, dado, NULL);
        } else {
            maiorNo = quebrarNoPessoa(raiz, dado, sobe, NULL);
            if (pai == NULL) {
                *raiz = alocarPessoa(*sobe, *raiz, maiorNo);
                maiorNo = NULL;
            }
        }
    } else {
        if (strcmp(dado.nome, (*raiz)->info_um.nome) < 0) {
            maiorNo = inserirPessoa(&(*raiz)->esq, *raiz, dado, sobe);
        } else if ((*raiz)->qntd_info == 1 || strcmp(dado.nome, (*raiz)->info_dois.nome) < 0) {
            maiorNo = inserirPessoa(&(*raiz)->cen, *raiz, dado, sobe);
        } else {
            maiorNo = inserirPessoa(&(*raiz)->dir, *raiz, dado, sobe);
        }

        if (maiorNo != NULL) {
            if ((*raiz)->qntd_info == 1) {
                inserir_infoPessoa(raiz, *sobe, maiorNo);
                maiorNo = NULL;
            } else {
                maiorNo = quebrarNoPessoa(raiz, *sobe, sobe, maiorNo);
                if (pai == NULL) {
                    *raiz = alocarPessoa(*sobe, *raiz, maiorNo);
                    maiorNo = NULL;
                }
            }
        }
    }

    return maiorNo;
}

void imprimirInOrdemPessoa(Pessoa *raiz) 
{
    if (raiz != NULL){

        // Percorre a esquerda
        imprimirInOrdemPessoa(raiz->esq);

        // Visita o primeiro valor
        printf("1. nome:%s | CPF:%s\n", 
            raiz->info_um.nome,
            raiz->info_um.CPF);

        // Percorre o centro
        imprimirInOrdemPessoa(raiz->cen);

        // Se for nó 2-info, visita segundo valor e percorre direita
        if (raiz->qntd_info == 2) {
            printf("2. nome:%s | CPF: %s\n", 
                raiz->info_dois.nome
                , raiz->info_dois.CPF);
            imprimirInOrdemPessoa(raiz->dir);
        }
    }
}

void imprimirInOrdemPessoaComRaiz(Pessoa *raiz) 
{
    printf("Valores em ordem: \n");
    imprimirInOrdemPessoa(raiz);
    printf("\n");

    if (raiz) {
        printf("Raiz: ");
        if (raiz->qntd_info == 1)
            printf("[%s]\n", raiz->info_um.nome);
        else
            printf("[%s | %s]\n", raiz->info_um.nome, raiz->info_dois.nome);
    } else {
        printf("Árvore vazia.\n");
    }
}


Pessoa* buscarPessoa(Pessoa *raiz, char *valor) 
{
    Pessoa *resultado = NULL;

    if (raiz != NULL) {
        if (strcmp(raiz->info_um.nome, valor) == 0) {
            resultado = raiz;
        } else if (raiz->qntd_info == 2 && strcmp(raiz->info_dois.nome, valor) == 0) {
            resultado = raiz;
        } else if (strcmp(valor, raiz->info_um.nome) < 0) {
            resultado = buscarPessoa(raiz->esq, valor);
        } else if (raiz->qntd_info == 1 || strcmp(valor, raiz->info_dois.nome) < 0) {
            resultado = buscarPessoa(raiz->cen, valor);
        } else {
            resultado = buscarPessoa(raiz->dir, valor);
        }
    }

    return resultado;
}

void liberar_arvore_pessoa(Pessoa **raiz) 
{
    if (*raiz) {
        liberar_arvore_pessoa(&(*raiz)->esq);
        liberar_arvore_pessoa(&(*raiz)->cen);
        if ((*raiz)->qntd_info == 2)
            liberar_arvore_pessoa(&(*raiz)->dir);
        free(*raiz);
        *raiz = NULL;
    }
}


// Consulta pessoas nascidas em um CEP específico
Pessoa *consultaPessoaCEPNacimento(Pessoa *raiz, int CEP, int *encontrado) 
{
    // percorre árvore e verifica se o CEP de nascimento corresponde
    Pessoa *aux = NULL;
    if (raiz != NULL)
    {
        if (raiz->info_um.cepCidadeNatal == CEP)
        {
            aux = raiz;
            *encontrado = 1;
        }else if(raiz->qntd_info == 2 && raiz->info_dois.cepCidadeNatal == CEP)
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
            aux = consultaPessoaCEPNacimento(raiz->cen, CEP, encontrado);
        }
        if (*encontrado == 0)
        {
            aux = consultaPessoaCEPNacimento(raiz->dir, CEP, encontrado);
        }
    }
    return aux;
}

// Consulta pessoas que moram atualmente em um CEP
Pessoa *consultaPessoaCEPAtual(Pessoa *raiz, int CEP, int *encontrado) 
{
    Pessoa *aux = NULL;
    if (raiz != NULL)
    {
        if (raiz->info_um.cepCidadeAtual == CEP)
        {
            aux = raiz;
            *encontrado = 1;
        }else if(raiz->qntd_info == 2 && raiz->info_dois.cepCidadeAtual == CEP)
        {
            aux = raiz;
            *encontrado = 1;

        }
        // verifica primeiro nos ramos da esquerda e depois direita, caso for encontrado a varaivel incontrado já terá sido setada para 1
        if (*encontrado == 0)
        {
            aux = consultaPessoaCEPAtual(raiz->esq, CEP, encontrado);
        }
        if (*encontrado == 0)
        {
            aux = consultaPessoaCEPAtual(raiz->cen, CEP, encontrado);
        }
        if (*encontrado == 0)
        {
            aux = consultaPessoaCEPAtual(raiz->dir, CEP, encontrado);
        }
    }
    return aux;
}

// Encontra o menor nome (alfabeticamente) na subárvore (sucessor para remoção)
infoPessoa encontrar_sucessorPessoa(Pessoa *no) 
{
    Pessoa *atual = no;
    while (atual->esq != NULL)
        atual = atual->esq;

    return atual->info_um;
}

// Rebalanceamento após remoção quando um filho fica vazio
void rebalancearPessoa(Pessoa **pai, Pessoa **filho_vazio) 
{
    Pessoa *p = *pai;
    Pessoa *f = *filho_vazio;

    // Filho vazio à esquerda
    if (f == p->esq) {
        if (p->cen->qntd_info == 2) {
            f->info_um = p->info_um;
            f->qntd_info = 1;

            p->info_um = p->cen->info_um;
            p->cen->info_um = p->cen->info_dois;
            p->cen->qntd_info = 1;
        } else {
            f->info_um = p->info_um;
            f->info_dois = p->cen->info_um;
            f->qntd_info = 2;

            free(p->cen);
            p->cen = NULL;

            if (p->qntd_info == 2) {
                p->info_um = p->info_dois;
                p->qntd_info = 1;
                p->cen = p->dir;
                p->dir = NULL;
            } else {
                **pai = *f;
                free(f);
                *filho_vazio = NULL;
            }
        }
    }

    // Filho vazio no centro
    else if (f == p->cen) {
        if (p->esq->qntd_info == 2) {
            f->info_um = p->info_um;
            f->qntd_info = 1;

            p->info_um = p->esq->info_dois;
            p->esq->qntd_info = 1;
        } else if (p->qntd_info == 2 && p->dir && p->dir->qntd_info == 2) {
            f->info_um = p->info_dois;
            f->qntd_info = 1;

            p->info_dois = p->dir->info_um;
            p->dir->info_um = p->dir->info_dois;
            p->dir->qntd_info = 1;
        } else {
            p->esq->info_dois = p->info_um;
            p->esq->qntd_info = 2;

            free(p->cen);
            p->cen = NULL;

            if (p->qntd_info == 2) {
                p->info_um = p->info_dois;
                p->qntd_info = 1;
                p->cen = p->dir;
                p->dir = NULL;
            } else {
                **pai = *p->esq;
                free(p->esq);
                *filho_vazio = NULL;
            }
        }
    }

    // Filho vazio à direita
    else if (f == p->dir && p->qntd_info == 2) {
        if (p->cen->qntd_info == 2) {
            f->info_um = p->info_dois;
            f->qntd_info = 1;

            p->info_dois = p->cen->info_dois;
            p->cen->qntd_info = 1;
        } else {
            p->cen->info_dois = p->info_dois;
            p->cen->qntd_info = 2;

            free(p->dir);
            p->dir = NULL;
            p->qntd_info = 1;
        }
    }
}

// Função principal de remoção 
int removerPessoa(Pessoa **raiz, char *valor)
{
    if (*raiz == NULL)
        return 0;

    // Caso 1: nó folha
    if ((*raiz)->esq == NULL) {
        if ((*raiz)->qntd_info == 1) {
            if (strcmp((*raiz)->info_um.nome, valor) == 0) {
                free(*raiz);
                *raiz = NULL;
                return 1;
            }
        } else {
            if (strcmp((*raiz)->info_um.nome, valor) == 0) {
                (*raiz)->info_um = (*raiz)->info_dois;
                (*raiz)->qntd_info = 1;
                return 1;
            } else if (strcmp((*raiz)->info_dois.nome, valor) == 0) {
                (*raiz)->qntd_info = 1;
                return 1;
            }
        }
        return 0;
    }

    // Caso 2: valor está em nó interno
    if (strcmp((*raiz)->info_um.nome, valor) == 0) {
        infoPessoa sucessor = encontrar_sucessorPessoa((*raiz)->cen);
        (*raiz)->info_um = sucessor;
        int ok = removerPessoa(&(*raiz)->cen, sucessor.nome);
        if ((*raiz)->cen && (*raiz)->cen->qntd_info == 0)
            rebalancearPessoa(raiz, &(*raiz)->cen);
        return ok;
    } else if ((*raiz)->qntd_info == 2 && strcmp((*raiz)->info_dois.nome, valor) == 0) {
        infoPessoa sucessor = encontrar_sucessorPessoa((*raiz)->dir);
        (*raiz)->info_dois = sucessor;
        int ok = removerPessoa(&(*raiz)->dir, sucessor.nome);
        if ((*raiz)->dir && (*raiz)->dir->qntd_info == 0)
            rebalancearPessoa(raiz, &(*raiz)->dir);
        return ok;
    }

    // Caso 3: valor está nas subárvores
    int sucesso = 0;
    if (valor < (*raiz)->info_um.nome) {
        sucesso = removerPessoa(&(*raiz)->esq, valor);
        if ((*raiz)->esq && (*raiz)->esq->qntd_info == 0)
            rebalancearPessoa(raiz, &(*raiz)->esq);
    } else if ((*raiz)->qntd_info == 1 || valor < (*raiz)->info_dois.nome) {
        sucesso = removerPessoa(&(*raiz)->cen, valor);
        if ((*raiz)->cen && (*raiz)->cen->qntd_info == 0)
            rebalancearPessoa(raiz, &(*raiz)->cen);
    } else {
        sucesso = removerPessoa(&(*raiz)->dir, valor);
        if ((*raiz)->dir && (*raiz)->dir->qntd_info == 0)
            rebalancearPessoa(raiz, &(*raiz)->dir);
    }

    return sucesso;
}

int Quantas_pessoas_não_moram_na_cidade_natal(Pessoa *raiz, Estado *estados) 
{
    int cont = 0;
    Estado *auxEstado;
    Cidade *auxCidade1, *auxCidade2;
    infoCidade *auxinfoCidade1, *auxinfoCidade2;

    if (raiz != NULL) 
    {

        // Buscarndo cidade natal
        auxEstado = buscar_estado_Por_CEP(estados, raiz->info_um.cepCidadeAtual);
        auxCidade1 = buscarCidadePorCEP(auxEstado->infoEstado.arv_cidade, raiz->info_um.cepCidadeNatal);

        if (buscarCEP(auxCidade1->info_um.avr_CEP, raiz->info_um.cepCidadeNatal) != NULL) {
            auxinfoCidade1 = &auxCidade1->info_um;
        }else{
            if (raiz->qntd_info == 2) {
                auxinfoCidade1 = &auxCidade1->info_dois;
            }
        }

        // Buscar cidade onde mora
        auxEstado = buscar_estado_Por_CEP(estados, raiz->info_um.cepCidadeAtual);
        auxCidade2 = buscarCidadePorCEP(auxEstado->infoEstado.arv_cidade, raiz->info_um.cepCidadeAtual);

        if (buscarCEP(auxCidade2->info_um.avr_CEP, raiz->info_um.cepCidadeNatal) != NULL) {
            auxinfoCidade2 = &auxCidade2->info_um;
        }else{
            if (raiz->qntd_info == 2) {
                auxinfoCidade2 = &auxCidade2->info_dois;
            }
        }

        if(auxCidade1 != NULL && auxCidade2 != NULL){

            if(strcmp(auxinfoCidade2->nome_cidade, auxinfoCidade2->nome_cidade) != 0){
                // Se a cidade natal for maior que a cidade onde mora, incrementa o contador
                cont += 1;
            }
        }

        if (raiz->qntd_info == 2) {
            auxEstado = buscar_estado_Por_CEP(estados, raiz->info_dois.cepCidadeAtual);
            auxCidade1 = buscarCidadePorCEP(auxEstado->infoEstado.arv_cidade, raiz->info_dois.cepCidadeNatal);

            if (buscarCEP(auxCidade1->info_um.avr_CEP, raiz->info_dois.cepCidadeNatal) != NULL) {
                auxinfoCidade1 = &auxCidade1->info_um;
            }else{
                if (raiz->qntd_info == 2) {
                    auxinfoCidade1 = &auxCidade1->info_dois;
                }
            }

            // Buscar cidade onde mora
            auxEstado = buscar_estado_Por_CEP(estados, raiz->info_dois.cepCidadeAtual);
            auxCidade2 = buscarCidadePorCEP(auxEstado->infoEstado.arv_cidade, raiz->info_dois.cepCidadeAtual);

            if (buscarCEP(auxCidade2->info_um.avr_CEP, raiz->info_dois.cepCidadeNatal) != NULL) {
                auxinfoCidade2 = &auxCidade2->info_um;
            }else{
                if (raiz->qntd_info == 2) {
                    auxinfoCidade2 = &auxCidade2->info_dois;
                }
            }

            if(auxCidade1 != NULL && auxCidade2 != NULL){

                if(strcmp(auxinfoCidade2->nome_cidade, auxinfoCidade2->nome_cidade) != 0){
                    // Se a cidade natal for maior que a cidade onde mora, incrementa o contador
                    cont += 1;
                }
            }
        }

        cont += Quantas_pessoas_não_moram_na_cidade_natal(raiz->esq, estados);
        cont += Quantas_pessoas_não_moram_na_cidade_natal(raiz->dir, estados);

    }

    return cont;
}

int numero_de_pessoas_nascidas_em_uma_determinada_cidade_nao_mora_na_cidade_natal(Pessoa *raiz, infoCidade *cidade){
    int cont = 0;
    CEP *auxCEPNatal, *auxCEPAtual;
    if (raiz != NULL) 
    {
        auxCEPNatal = buscarCEP(cidade->avr_CEP, raiz->info_um.cepCidadeNatal);
        auxCEPAtual = buscarCEP(cidade->avr_CEP, raiz->info_um.cepCidadeAtual);
        // A cidade natal deve ser encontrada enquanto a cidade atual não deve ser encontrada
        if(auxCEPNatal != NULL && auxCEPAtual == NULL){
            cont += 1;
        }

        if (raiz->qntd_info == 2) {
            auxCEPNatal = buscarCEP(cidade->avr_CEP, raiz->info_dois.cepCidadeNatal);
            auxCEPAtual = buscarCEP(cidade->avr_CEP, raiz->info_dois.cepCidadeAtual);
            // A cidade natal deve ser encontrada enquanto a cidade atual não deve ser encontrada
            if(auxCEPNatal != NULL && auxCEPAtual == NULL){
                cont += 1;
            }
        }

        cont += numero_de_pessoas_nascidas_em_uma_determinada_cidade_nao_mora_na_cidade_natal(raiz->esq, cidade);
        cont += numero_de_pessoas_nascidas_em_uma_determinada_cidade_nao_mora_na_cidade_natal(raiz->cen, cidade); 
        cont += numero_de_pessoas_nascidas_em_uma_determinada_cidade_nao_mora_na_cidade_natal(raiz->dir, cidade);

    }
    return cont;
}

int numero_de_pessoas_que_moram_em_uma_determinada_cidade_nao_nasceram_nela(Pessoa *raiz, infoCidade *cidade){
    int cont = 0;
    CEP *auxCEPNatal, *auxCEPAtual;
    if (raiz != NULL) 
    {
        auxCEPNatal = buscarCEP(cidade->avr_CEP, raiz->info_um.cepCidadeNatal);
        auxCEPAtual = buscarCEP(cidade->avr_CEP, raiz->info_um.cepCidadeAtual);
        // A cidade natal deve ser encontrada enquanto a cidade atual não deve ser encontrada
        if(auxCEPNatal == NULL && auxCEPAtual != NULL){
            cont += 1;
        }

        if (raiz->qntd_info == 2) {
            auxCEPNatal = buscarCEP(cidade->avr_CEP, raiz->info_dois.cepCidadeNatal);
            auxCEPAtual = buscarCEP(cidade->avr_CEP, raiz->info_dois.cepCidadeAtual);
            // A cidade natal deve ser encontrada enquanto a cidade atual não deve ser encontrada
            if(auxCEPNatal == NULL && auxCEPAtual != NULL){
                cont += 1;
            }
        }

        cont += numero_de_pessoas_nascidas_em_uma_determinada_cidade_nao_mora_na_cidade_natal(raiz->esq, cidade);
        cont += numero_de_pessoas_nascidas_em_uma_determinada_cidade_nao_mora_na_cidade_natal(raiz->dir, cidade);

    }
    return cont;
}