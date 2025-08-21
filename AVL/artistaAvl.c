#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include "interfaceAvl.h"

// Funcao para alocar artista
Artista *alocaArtista(infoArtista dados)
{
    Artista *no;
    no = (Artista *) malloc(sizeof(Artista));  // Aloca memória para um nó de Artista
    if (no == NULL)
    {
        printf("Erro de acesso a memoria.\n");  // Exibe erro se não conseguir alocar memória
        exit(1);
    }
    no->esq = NULL;   // Inicializa a subárvore esquerda como NULL
    no->dir = NULL;   // Inicializa a subárvore direita como NULL
    no->albuns = NULL;  // Inicializa a árvore de álbuns como NULL
    no->info = dados;  // Atribui os dados do artista ao nó
    return no;  // Retorna o nó alocado
}

// Preenche as informacoes do artista: nome, estilo, tipo, numAlbuns
infoArtista preencherDadosArtista()
{
    infoArtista dados;
    printf("Digite o nome do artista: "); scanf("%s", dados.nome);  // Solicita o nome do artista
    printf("Digite o estilo do artista: "); scanf("%s", dados.estilo);  // Solicita o estilo musical
    printf("Digite o tipo: "); scanf("%s", dados.tipo);  // Solicita o tipo de artista (solo, banda, etc.)
    dados.numAlbuns = 0; // Inicializa o número de álbuns como 0
    return dados;  // Retorna os dados preenchidos
}

// Tenta colocar um artista novo na arvore
int inserirArtista(Artista **raiz, Artista *no)
{
    int inserido = 1;
    if(*raiz == NULL)
    {
        *raiz = no;  // Se a raiz for NULL, insere o artista
    }
    else
    {
        // Compara os nomes dos artistas para decidir onde inserir na árvore binária de busca
        if(strcmp((*raiz)->info.nome, no->info.nome) > 0) 
        {
            inserido = inserirArtista(&(*raiz)->esq, no);  // Insere na subárvore esquerda
        }
        else if(strcmp((*raiz)->info.nome, no->info.nome) < 0)
        {
            inserido = inserirArtista(&(*raiz)->dir, no);  // Insere na subárvore direita
        }
        else
        {
            inserido = 0;  // Caso o artista já exista, não insere
        }
    }
    if((*raiz) != NULL)
    {
        if(inserido)
        {
            balanceamentoArtista(raiz);  // Realiza balanceamento da árvore após a inserção
            atualizarAlturaArtista(raiz);  // Atualiza a altura dos nós
        }
    }
    return(inserido);  // Retorna se a inserção foi bem-sucedida
}

// Realiza o balanceamento da árvore de artistas
void balanceamentoArtista(Artista **raiz)
{
    int fb;
    fb = fatorBalanceamentoArtista(*raiz);  // Calcula o fator de balanceamento
    if(fb == 2)  // Se o fator de balanceamento for 2 (desbalanceado à direita)
    {
        int fbEsq = fatorBalanceamentoArtista((*raiz)->esq);  // Calcula o fator de balanceamento da subárvore esquerda
        if(fbEsq < 0)
        {
            rotacionarEsqArtista(&(*raiz)->esq);  // Rotaciona a subárvore esquerda à esquerda
        }
        rotacionarDirArtista(raiz);  // Rotaciona a árvore para a direita
    }
    else if(fb == -2)  // Se o fator de balanceamento for -2 (desbalanceado à esquerda)
    {
        int fbDir = fatorBalanceamentoArtista((*raiz)->dir);  // Calcula o fator de balanceamento da subárvore direita
        if(fbDir > 0)
        {
            rotacionarDirArtista(&(*raiz)->dir);  // Rotaciona a subárvore direita à direita
        }
        rotacionarEsqArtista(raiz);  // Rotaciona a árvore para a esquerda
    }
}

// Calcula o fator de balanceamento de um artista
int fatorBalanceamentoArtista(Artista *raiz)
{
    int fb;
    fb = alturaArtista(raiz->esq) - alturaArtista(raiz->dir);  // Fator de balanceamento = altura esquerda - altura direita
    return fb;  // Retorna o fator de balanceamento
}

// Calcula a altura de um nó da árvore de artistas
int alturaArtista(Artista *raiz)
{
    int altura;
    if(raiz != NULL)
    {
        int altEsq = -1, altDir = -1;
        if(raiz->esq != NULL)
        {
            altEsq = raiz->esq->altura;  // Obtém a altura da subárvore esquerda
        }
        if(raiz->dir != NULL)
        {
            altDir = raiz->dir->altura;  // Obtém a altura da subárvore direita
        }
        if(altEsq > altDir)
        {
            altura = altEsq + 1;  // Se a subárvore esquerda for mais alta, incrementa 1
        }
        else
        {
            altura = altDir + 1;  // Caso contrário, incrementa 1 na altura da subárvore direita
        }
    }
    else
    {
        altura = -1;  // Se o nó for NULL, a altura é -1
    }
    return altura;  // Retorna a altura calculada
}

// Atualiza a altura de todos os nós a partir de um nó da árvore de artistas
void atualizarAlturaArtista(Artista **raiz)
{
    if((*raiz) != NULL)
    {
        atualizarAlturaArtista(&(*raiz)->esq);  // Atualiza a altura da subárvore esquerda
        atualizarAlturaArtista(&(*raiz)->dir);  // Atualiza a altura da subárvore direita
        (*raiz)->altura = alturaArtista(*raiz);  // Atualiza a altura do nó atual
    }
}

// Rotaciona um nó da árvore de artistas para a esquerda
void rotacionarEsqArtista(Artista **raiz)
{
    Artista *aux;
    aux = (*raiz)->dir;  // Armazena o nó à direita
    (*raiz)->dir = aux->esq;  // O nó à direita passa a ser a subárvore esquerda do nó auxiliar
    aux->esq = *raiz;  // O nó auxiliar passa a ser a subárvore esquerda
    *raiz = aux;  // O nó auxiliar se torna a raiz da subárvore
}

// Rotaciona um nó da árvore de artistas para a direita
void rotacionarDirArtista(Artista **raiz)
{
    Artista *aux;
    aux = (*raiz)->esq;  // Armazena o nó à esquerda
    (*raiz)->esq = aux->dir;  // O nó à esquerda passa a ser a subárvore direita do nó auxiliar
    aux->dir = *raiz;  // O nó auxiliar passa a ser a subárvore direita
    *raiz = aux;  // O nó auxiliar se torna a raiz da subárvore
}

// Procura um artista pelo nome
Artista *buscarArtista(Artista *raiz, char *nome)
{
    Artista *encontrado;
    encontrado = NULL; 
    if (raiz != NULL)
    {
        if(strcmp(raiz->info.nome, nome) == 0)
        {
            encontrado = raiz;  // Se encontrar, retorna o nó
        }
        else if(strcmp(raiz->info.nome, nome) > 0)
        {
            encontrado = buscarArtista(raiz->esq, nome);  // Busca na subárvore esquerda
        } 
        else
        {
            encontrado = buscarArtista(raiz->dir, nome);  // Busca na subárvore direita
        }     
    }else
    {
        printf("raiz == NULL\n");  // Caso o nó seja NULL
    }
    return(encontrado);  // Retorna o artista encontrado ou NULL
}

// Mostra todos os artistas da arvore
void mostrarArtistas(Artista *raiz)
{
    if(raiz != NULL)
    {
        mostrarArtistas(raiz->esq);  // Exibe os artistas da subárvore esquerda
        printf("\nNome: %s, Estilo: %s, Tipo: %s, Albuns Lancados: %d, Altura: %d\n", raiz->info.nome, raiz->info.estilo, raiz->info.tipo, raiz->info.numAlbuns, raiz->altura);  // Exibe os dados do artista
        mostrarArtistas(raiz->dir);  // Exibe os artistas da subárvore direita
    }
}

// Mostra so os artistas de um estilo especifico
void mostrarArtistasEstilo(Artista *raiz, char *estilo)
{
    if(raiz != NULL)
    {
        mostrarArtistasEstilo(raiz->esq, estilo);  // Exibe os artistas da subárvore esquerda com o estilo solicitado
        if(strcmp(raiz->info.estilo, estilo) == 0)
        {
            printf("\nNome: %s, Estilo: %s, Tipo: %s, Albuns Lancados: %d ", raiz->info.nome, raiz->info.estilo, raiz->info.tipo, raiz->info.numAlbuns);  // Exibe o artista caso o estilo seja o solicitado
        }
        mostrarArtistasEstilo(raiz->dir, estilo);  // Exibe os artistas da subárvore direita com o estilo solicitado
    }
}

// Mesma coisa do anterior, mas com o tipo (solo, banda, etc)
void mostrarArtistasTipo(Artista *raiz, char *tipo)
{
    if(raiz != NULL)
    {
        mostrarArtistasTipo(raiz->esq, tipo);  // Exibe os artistas da subárvore esquerda com o tipo solicitado
        if(strcmp(raiz->info.tipo, tipo) == 0)
        {
            printf("\nNome: %s, Estilo: %s, Tipo: %s, Quantidade de Albuns: %d ", raiz->info.nome, raiz->info.estilo, raiz->info.tipo, raiz->info.numAlbuns);  // Exibe o artista caso o tipo seja o solicitado
        }
        mostrarArtistasTipo(raiz->dir, tipo);  // Exibe os artistas da subárvore direita com o tipo solicitado
    }
}

// Mostra artistas que combinam tipo e estilo ao mesmo tempo
void mostrarArtistasTipoEstilo(Artista *raiz, char *tipo, char *estilo)
{
    if(raiz != NULL)
    {
        mostrarArtistasTipoEstilo(raiz->esq, tipo, estilo);  // Exibe os artistas da subárvore esquerda com tipo e estilo solicitados
        if(strcmp(raiz->info.tipo, tipo) == 0 && strcmp(raiz->info.estilo, estilo) == 0)
        {
            printf("\nNome: %s, Estilo: %s, Tipo: %s, Quantidade de Albuns: %d ", raiz->info.nome, raiz->info.estilo, raiz->info.tipo, raiz->info.numAlbuns);  // Exibe o artista caso o tipo e o estilo combinem
        }
        mostrarArtistasTipoEstilo(raiz->dir, tipo, estilo);  // Exibe os artistas da subárvore direita com tipo e estilo solicitados
    }
}

// Libera o que foi alocado na arvore de artistas
void liberarArtistas(Artista **raiz)
{
    if(*raiz != NULL)
    {
        liberarArtistas(&(*raiz)->esq);  // Libera a subárvore esquerda
        liberarArtistas(&(*raiz)->dir);  // Libera a subárvore direita
        liberarAlbum(&(*raiz)->albuns);  // Libera os álbuns do artista
        free(*raiz);  // Libera o nó do artista
    }
}

// Menu interativo para albuns de um artista
void acessarArtista(Artista *no, Playlist *raizP)
{
    Artista *raiz;
    raiz = NULL;
    Album *album;
    Artista *artistaEncontrado;
    infoAlbum dados;
    char titulo[50];
    char nome_artista[50];
    int cont, ano, flag = 0;
    int opcao;
    do 
    {
        printf("\nMenu do Artista %s:\n", no->info.nome);  // Exibe o nome do artista
        printf("0 - Sair\n");
        printf("1 - Cadastrar album\n");
        printf("2 - Mostrar todos os Albuns do Artista\n");
        printf("3 - Mostrar todos os Albuns do Artista de um determinado ano\n");
        printf("4 - Mostrar todas as Musicas de um determinado Album\n");
        printf("5 - Acessar album\n");
        printf("Escolha uma opcao: "); scanf("%d", &opcao);  // Solicita a opção do menu
        switch (opcao) 
        {
            case 0:
                printf("Saindo...\n");
                break;
            case 1:
                do 
                {
                    dados = preencherDadosAlbum();  // Preenche os dados do álbum
                    album = alocarAlbum(dados);  // Aloca o álbum
                    if(inserirAlbum(&no->albuns, album))  // Insere o álbum na árvore
                    {
                        printf("Album %s cadastrado com sucesso.\n", album->info.titulo);
                        no->info.numAlbuns++;  // Incrementa o número de álbuns do artista
                        flag = 1;
                    } 
                    else 
                    {
                        printf("Album %s ja existe na arvore. Tente outro valor.\n", album->info.titulo);  // Se o álbum já existir
                    }    
                } while(flag != 1);  // Repete até que o álbum seja cadastrado com sucesso
                break;
            case 2:
                mostrarAlbuns(no->albuns);  // Mostra os álbuns do artista
                printf("\n");
                break;
            case 3:
                printf("Digite o ano do Album: "); scanf("%d", &ano);  // Solicita o ano
                mostrarAlbunsPorAnoDeUMArtista(no->albuns, ano);  // Mostra os álbuns do ano solicitado
                printf("\n");
                break;
            case 4:
                printf("Digite o titulo do Album: "); scanf("%s", titulo);  // Solicita o título do álbum
                album = buscarAlbum(no->albuns, titulo);  // Busca o álbum
                if(album != NULL)
                {
                    mostrarMusicas(album->musicas);  // Exibe as músicas do álbum
                    printf("\n");
                }else
                {
                    printf("\nAlbum %s nao encontrado!\nCadastre o album primeiro.\n", titulo);  // Caso o álbum não seja encontrado
                }
                break;
            case 5:
                printf("Digite o titulo do Album: ");scanf("%s", titulo);  // Solicita o título do álbum
                album = buscarAlbum(no->albuns, titulo);  // Busca o álbum
                if(album != NULL)
                {
                    acessarAlbum(album, raizP);  // Acessa o álbum
                }
                else
                {
                    printf("\nAlbum %s nao encontrado!\nCadastre o album primeiro.\n", titulo);  // Caso o álbum não seja encontrado
                }
                break;
            default:
                printf("Opcao invalida!\n");  // Caso a opção seja inválida
        }
    } while (opcao != 0);  // Continua até que o usuário escolha sair
}
