#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "interface_avp.h"

//lista 

// Aloca Estado (não aloca infoEstado separadamente)
Estado* alocar_estado() 
{
    Estado* novo = (Estado*) malloc(sizeof(Estado));
    if (novo == NULL) 
    {
        printf("Erro ao alocar memoria para Estado.\n");
        exit(1);
    }
    novo->prox = NULL;
    novo->ant = NULL;

    // Inicializa arvore de cidades como NULL
    novo->infoEstado.arv_cidade = NULL;

    return novo;
}

// Preenche dados do estado
void preencher_estado(infoEstado *estado, char *nome_estado, char *nome_capital, int n_cidade, int populacao) 
{
    strcpy(estado->nome_estado, nome_estado);
    strcpy(estado->nome_capital, nome_capital);
    estado->num_cidade = n_cidade;
    estado->populacao = populacao;
    estado->arv_cidade = NULL;  // árvore de cidades começa vazia
}

// Inserção ordenada por nome do estado
int inserir_estado_ordenado(Estado **inicio, Estado *novo) 
{
    Estado *atual = *inicio;

    // Verificar duplicidade
    Estado *temp = *inicio;
    while (temp != NULL) {
        if (strcmp(temp->infoEstado.nome_estado, novo->infoEstado.nome_estado) == 0)
            return 0; // duplicado
        temp = temp->prox;
    }

    if (atual == NULL) 
    {
        *inicio = novo;
    } else{
        if (strcmp(novo->infoEstado.nome_estado, atual->infoEstado.nome_estado) < 0) {
            novo->prox = atual;
            atual->ant = novo;
            *inicio = novo;
        } else {
            while (atual->prox != NULL &&
                   strcmp(atual->prox->infoEstado.nome_estado, novo->infoEstado.nome_estado) < 0) {
                atual = atual->prox;
            }
            novo->prox = atual->prox;
            novo->ant = atual;
            if (atual->prox != NULL) {
                atual->prox->ant = novo;
            }
            atual->prox = novo;
        }
    }
    return 1;
}


int cadastrar_cidade_em_estado(Estado *lista_estados, const char *nome_estado, infoCidade dados_cidade)
{
    int resultado = 0;    // valor que será retornado
    int achou = 0;

    Estado *est = buscar_estado(lista_estados, nome_estado, &achou);

    if (achou) 
        resultado = inserirCidade(&est->infoEstado.arv_cidade, dados_cidade);
    
    return resultado;
}

// Busca estado pelo nome
Estado* buscar_estado(Estado *inicio, const char *nome_busca, int *flag) {
    Estado *atual = inicio;
    *flag = 0;
    while (atual != NULL) {
        if (strcmp(atual->infoEstado.nome_estado, nome_busca) == 0) {
            *flag = 1;
            return atual;
        }
        atual = atual->prox;
    }
    return NULL;
}

Estado* buscar_estado_Por_CEP(Estado *inicio, int intCEP) {
    Estado *atual = inicio;
    Cidade *auxCIDADE;
    while (atual != NULL) {
        auxCIDADE = buscarCidadePorCEP(atual->infoEstado.arv_cidade, intCEP);
        if (auxCIDADE) {
            return atual;
        }
        atual = atual->prox;
    }
    return NULL;
}

// em estado.c ou onde estiver imprimir_estados()
void imprimir_estados(Estado *inicio) {
    Estado *atual = inicio;
    while (atual) {
        printf("Estado: %s | Capital: %s | Cidades: %d | População: %d\n",
               atual->infoEstado.nome_estado,
               atual->infoEstado.nome_capital,
               atual->infoEstado.num_cidade,
               atual->infoEstado.populacao);

        // se houver cidades, imprime a RBT em-order
        if (atual->infoEstado.arv_cidade) {
            printf("  -> Estados em ordem:\n");
            imprimir(atual->infoEstado.arv_cidade);
        }

        atual = atual->prox;
    }
}

Estado *retorna_estados_com_maior_populacao(Estado *atual) {
    
    Estado *maiorEstado = atual;
    int maiorPopulacao = 0;

    while (atual) {
        if (maiorPopulacao < atual->infoEstado.populacao){
            maiorPopulacao = atual->infoEstado.populacao;
            maiorEstado = atual;
        }

        atual = atual->prox;
    }

    return maiorEstado;
}

void liberar_lista_estados(Estado *inicio) {
    Estado *atual = inicio;
    Estado *seguinte;

    while (atual) {
        seguinte = atual->prox;
        // primeiro libera toda a árvore de cidades
        if (atual->infoEstado.arv_cidade)
            liberar_arvore_cidades(atual->infoEstado.arv_cidade);

        // depois libera o nó de Estado
        free(atual);
        atual = seguinte;
    }
}

Cidade *buscar_Cidade_nos_Estados(Estado *inicio, char *nome_cidade) {
    Estado *atual = inicio;
    Cidade *auxCIDADE = NULL;
    int flag = 0;
    while (atual != NULL && flag == 0)   {
        auxCIDADE = buscarCidade(atual->infoEstado.arv_cidade, nome_cidade);
        if (auxCIDADE) {
            flag = 1;
        }
        atual = atual->prox;
    }
    return auxCIDADE;
}
