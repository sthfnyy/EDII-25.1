#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "interface_b.h"

// Preenche a struct infoCEP com o valor do CEP
void preencherCEP(infoCEP *dados, int numCEP) 
{
    dados->cep = numCEP;
}

CEP *alocarCEP(infoCEP dado, CEP *fEsq, CEP *fCen) 
{
    CEP *novo = (CEP*) malloc(sizeof(CEP));
    if (novo != NULL) {
        novo->info_um = dado;
        novo->qntd_info = 1;
        novo->esq = fEsq;
        novo->cen = fCen;
        novo->dir = NULL;
    }
    return novo;
}

// Insere uma segunda informação no nó 2-3 (transforma em nó 2-informações)
void inserir_infoCEP(CEP **novoNo, infoCEP dado, CEP *subArvInfo) 
{
    if (dado.cep > (*novoNo)->info_um.cep) {
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

// Realiza a quebra de um nó cheio e retorna o novo nó
CEP *quebrarNoCEP(CEP **no, infoCEP dado, infoCEP *sobe, CEP *filhoDir) 
{
    CEP *maior;

    if (dado.cep > (*no)->info_dois.cep) {
        *sobe = (*no)->info_dois;
        maior = alocarCEP(dado, (*no)->dir, filhoDir);
        (*no)->qntd_info = 1;
    } else if (dado.cep > (*no)->info_um.cep) {
        *sobe = dado;
        maior = alocarCEP((*no)->info_dois, filhoDir, (*no)->dir);
        (*no)->qntd_info = 1;
    } else {
        *sobe = (*no)->info_um;
        maior = alocarCEP((*no)->info_dois, (*no)->cen, (*no)->dir);
        (*no)->info_um = dado;
        (*no)->cen = filhoDir;
        (*no)->qntd_info = 1;
    }

    return maior;
}

// Função principal de inserção na árvore 2-3
CEP *inserirCEP(CEP **raiz, CEP *pai, infoCEP dado, infoCEP *sobe)
{
    CEP *maiorNo = NULL;

    if (*raiz == NULL) {
        *raiz = alocarCEP(dado, NULL, NULL);
    
        // Caso: nó folha
    } else if ((*raiz)->esq == NULL) {
        if ((*raiz)->qntd_info == 1) {
            inserir_infoCEP(raiz, dado, NULL);
        } else {
            maiorNo = quebrarNoCEP(raiz, dado, sobe, NULL);
            // Se está na raiz, cria novo nó raiz
            if (pai == NULL) {
                *raiz = alocarCEP(*sobe, *raiz, maiorNo);
                maiorNo = NULL;
            }
        }
    // Caso: nó interno, desce até a posição correta
    } else {
        if (dado.cep < (*raiz)->info_um.cep) {
            maiorNo = inserirCEP(&(*raiz)->esq, *raiz, dado, sobe);
        } else if ((*raiz)->qntd_info == 1 || dado.cep < (*raiz)->info_dois.cep) {
            maiorNo = inserirCEP(&(*raiz)->cen, *raiz, dado, sobe);
        } else {
            maiorNo = inserirCEP(&(*raiz)->dir, *raiz, dado, sobe);
        }
        // Se houve quebra, tenta inserir na raiz atual ou promove para cima
        if (maiorNo != NULL) {
            if ((*raiz)->qntd_info == 1) {
                inserir_infoCEP(raiz, *sobe, maiorNo);
                maiorNo = NULL;
            } else {
                maiorNo = quebrarNoCEP(raiz, *sobe, sobe, maiorNo);
                if (pai == NULL) {
                    *raiz = alocarCEP(*sobe, *raiz, maiorNo);
                    maiorNo = NULL;
                }
            }
        }
    }

    return maiorNo;
}

void imprimirInOrdemCEP(CEP *raiz) 
{
    if (raiz != NULL){

        // Percorre a esquerda
        imprimirInOrdemCEP(raiz->esq);

        // Visita o primeiro valor
        printf("1. CEP:%d\n", 
            raiz->info_um.cep);

        // Percorre o centro
        imprimirInOrdemCEP(raiz->cen);

        // Se for nó 2-info, visita segundo valor e percorre direita
        if (raiz->qntd_info == 2) {
            printf("2. CEP:%d\n", 
                raiz->info_dois.cep);
            imprimirInOrdemCEP(raiz->dir);
        }
    }
}

void imprimirInOrdemCEPComRaiz(CEP *raiz) 
{
    printf("Valores em ordem: \n");
    imprimirInOrdemCEP(raiz);
    printf("\n");

    if (raiz) {
        printf("Raiz: ");
        if (raiz->qntd_info == 1)
            printf("[%d]\n", raiz->info_um.cep);
        else
            printf("[%d | %d]\n", raiz->info_um.cep, raiz->info_dois.cep);
    } else {
        printf("Árvore vazia.\n");
    }
}


CEP* buscarCEP(CEP *raiz, int valor) 
{
    CEP *resultado = NULL;

    if (raiz != NULL) {
        if (raiz->info_um.cep == valor) {
            resultado = raiz;
        } else if (raiz->qntd_info == 2 && raiz->info_dois.cep==valor) {
            resultado = raiz;
        } else if (valor < raiz->info_um.cep) {
            resultado = buscarCEP(raiz->esq, valor);
        } else if (raiz->qntd_info == 1 || valor < raiz->info_dois.cep) {
            resultado = buscarCEP(raiz->cen, valor);
        } else {
            resultado = buscarCEP(raiz->dir, valor);
        }
    }

    return resultado;
}

void liberar_arvore_CEPs(CEP **raiz) 
{
    if (*raiz) {
        liberar_arvore_CEPs(&(*raiz)->esq);
        liberar_arvore_CEPs(&(*raiz)->cen);
        if ((*raiz)->qntd_info == 2)
            liberar_arvore_CEPs(&(*raiz)->dir);
        free(*raiz);
        *raiz = NULL;
    }
}

// Função auxiliar: encontrar menor da subárvore 
infoCEP encontrar_sucessorCEP(CEP *no) 
{
    CEP *atual = no;
    while (atual->esq != NULL)
        atual = atual->esq;

    return atual->info_um;
}

// Função auxiliar: rebalancear após remoção
void rebalancearCEP(CEP **pai, CEP **filho_vazio) 
{
    CEP *p = *pai;
    CEP *f = *filho_vazio;

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

// Remove um CEP da árvore, tratando todos os casos (folha, nó interno, etc.)
int removerCEP(CEP **raiz, int valor)
{
    if (*raiz == NULL)
        return 0;

    // Caso 1: nó folha
    if ((*raiz)->esq == NULL) {
        if ((*raiz)->qntd_info == 1) {
            if ((*raiz)->info_um.cep == valor) {
                free(*raiz);
                *raiz = NULL;
                return 1;
            }
        } else {
            if ((*raiz)->info_um.cep == valor) {
                (*raiz)->info_um = (*raiz)->info_dois;
                (*raiz)->qntd_info = 1;
                return 1;
            } else if ((*raiz)->info_dois.cep == valor) {
                (*raiz)->qntd_info = 1;
                return 1;
            }
        }
        return 0;
    }

    // Caso 2: valor está em nó interno
    if ((*raiz)->info_um.cep == valor) {
        infoCEP sucessor = encontrar_sucessorCEP((*raiz)->cen);
        (*raiz)->info_um = sucessor;
        int ok = removerCEP(&(*raiz)->cen, sucessor.cep);
        if ((*raiz)->cen && (*raiz)->cen->qntd_info == 0)
            rebalancearCEP(raiz, &(*raiz)->cen);
        return ok;
    } else if ((*raiz)->qntd_info == 2 && (*raiz)->info_dois.cep == valor) {
        infoCEP sucessor = encontrar_sucessorCEP((*raiz)->dir);
        (*raiz)->info_dois = sucessor;
        int ok = removerCEP(&(*raiz)->dir, sucessor.cep);
        if ((*raiz)->dir && (*raiz)->dir->qntd_info == 0)
            rebalancearCEP(raiz, &(*raiz)->dir);
        return ok;
    }

    // Caso 3: valor está nas subárvores
    int sucesso = 0;
    if (valor < (*raiz)->info_um.cep) {
        sucesso = removerCEP(&(*raiz)->esq, valor);
        if ((*raiz)->esq && (*raiz)->esq->qntd_info == 0)
            rebalancearCEP(raiz, &(*raiz)->esq);
    } else if ((*raiz)->qntd_info == 1 || valor < (*raiz)->info_dois.cep) {
        sucesso = removerCEP(&(*raiz)->cen, valor);
        if ((*raiz)->cen && (*raiz)->cen->qntd_info == 0)
            rebalancearCEP(raiz, &(*raiz)->cen);
    } else {
        sucesso = removerCEP(&(*raiz)->dir, valor);
        if ((*raiz)->dir && (*raiz)->dir->qntd_info == 0)
            rebalancearCEP(raiz, &(*raiz)->dir);
    }

    return sucesso;
}
