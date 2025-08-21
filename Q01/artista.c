#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "interface.h"

// Funcao para alocar artista
Artista *alocaArtista(infoArtista dados)
{
    Artista *no;
    no = (Artista *) malloc(sizeof(Artista));
    if (no == NULL)
    {
        printf("Erro de acesso a memoria.\n");
        exit(1);
    }
    no->esq = NULL;   // esquerda comeca vazio
    no->dir = NULL;   // direita comeca vazio
    no->albuns = NULL;  // inicializa album vazio
    no->info = dados;
    return no;
}

// Preenche as informacoes do artista: nome, estilo, tipo, numAlbuns 
infoArtista preencherDadosArtista()
{
    infoArtista dados;
    printf("Digite o nome do artista: "); scanf("%s", dados.nome);
    printf("Digite o estilo do artista: "); scanf("%s", dados.estilo);
    printf("Digite o tipo: "); scanf("%s", dados.tipo);
    dados.numAlbuns = 0; // ainda sem albuns cadastrados
    return dados;
}

// Tenta colocar um artista novo na arvore
int inserirArtista(Artista **raiz, Artista *no)
{
    int inserido = 1;
    if(*raiz == NULL)
    {
        *raiz = no; // coloca ele na raiz
    }
    else
    {
        // decide se vai pra esquerda ou direita
        if(strcmp((*raiz)->info.nome, no->info.nome) > 0) 
        {
            inserido = inserirArtista(&(*raiz)->esq, no);
        }
        else if(strcmp((*raiz)->info.nome, no->info.nome) < 0)
        {
            inserido = inserirArtista(&(*raiz)->dir, no);
        }
        else
        {
            inserido = 0; // ja existe
        }
    }
    return(inserido);
}

// Procura um artista pelo nome
Artista *buscarArtista(Artista *raiz, char *nome)
{
    Artista *encontrado;
    encontrado = NULL; 
    if (raiz != NULL)
    {
        if (strcmp(raiz->info.nome, nome) == 0)
        {
            encontrado = raiz; 
        }
        else if (strcmp(raiz->info.nome, nome) > 0)
        {
            encontrado = buscarArtista(raiz->esq, nome);
        } 
        else
        {
            encontrado = buscarArtista(raiz->dir, nome);
        }     
    }
    return(encontrado);
}

// Mostra todos os artistas da arvore
void mostrarArtistas(Artista *raiz)
{
    if(raiz != NULL)
    {
        mostrarArtistas(raiz->esq);
        printf("\nNome: %s, Estilo: %s, Tipo: %s, Albuns Lancados: %d\n", raiz->info.nome, raiz->info.estilo, raiz->info.tipo, raiz->info.numAlbuns);
        mostrarArtistas(raiz->dir);
    }
}

// Mostra so os artistas de um estilo especifico
void mostrarArtistasEstilo(Artista *raiz, char *estilo)
{
    if(raiz != NULL)
    {
        mostrarArtistasEstilo(raiz->esq, estilo);
        if(strcmp(raiz->info.estilo, estilo) == 0)
        {
            
            printf("\nNome: %s, Estilo: %s, Tipo: %s, Albuns Lancados: %d ", raiz->info.nome, raiz->info.estilo, raiz->info.tipo, raiz->info.numAlbuns);
        }
        mostrarArtistasEstilo(raiz->dir, estilo);
    }
}

// Mesma coisa do anterior, mas com o tipo (solo, banda, etc)
void mostrarArtistasTipo(Artista *raiz, char *tipo)
{
    if(raiz != NULL)
    {
        mostrarArtistasTipo(raiz->esq, tipo);
        if(strcmp(raiz->info.tipo, tipo) == 0)
        {
            printf("\nNome: %s, Estilo: %s, Tipo: %s, Quantidade de Albuns: %d ", raiz->info.nome, raiz->info.estilo, raiz->info.tipo, raiz->info.numAlbuns);
        }
        mostrarArtistasTipo(raiz->dir, tipo);
    }
}

// Mostra artistas que combinam tipo e estilo ao mesmo tempo
void mostrarArtistasTipoEstilo(Artista *raiz, char *tipo, char *estilo)
{
    if(raiz != NULL)
    {
        mostrarArtistasTipoEstilo(raiz->esq, tipo, estilo);
        if(strcmp(raiz->info.tipo, tipo) == 0 && strcmp(raiz->info.estilo, estilo) == 0)
        {
            printf("\nNome: %s, Estilo: %s, Tipo: %s, Quantidade de Albuns: %d ", raiz->info.nome, raiz->info.estilo, raiz->info.tipo, raiz->info.numAlbuns);
        }
        mostrarArtistasTipoEstilo(raiz->dir, tipo, estilo);
    }
}

// Libera o que foi alocado na arvore de artistas
void liberarArtistas(Artista **raiz)
{
    if(*raiz != NULL)
    {
        liberarArtistas(&(*raiz)->esq);
        liberarArtistas(&(*raiz)->dir);
        liberarAlbum(&(*raiz)->albuns); // libera os albuns do artista tambem
        free(*raiz); // apaga o artista da memoria
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
    int cont, ano;
    int opcao;
    do 
    {
        printf("\nMenu do Artista %s:\n", no->info.nome);
        printf("0 - Sair\n");
        printf("1 - Cadastrar album\n");
        printf("2 - Mostrar todos os Albuns do Artista\n");
        printf("3 - Mostrar todos os Albuns do Artista de um determinado ano\n");
        printf("4 - Mostrar todas as Musicas de um determinado Album\n");
        printf("5 - Acessar album\n");
        printf("Escolha uma opcao: "); scanf("%d", &opcao);
        switch (opcao) 
        {
            case 0:
                printf("Saindo...\n");
                break;
            case 1:
                dados = preencherDadosAlbum();
                album = alocarAlbum(dados);
                if(inserirAlbum(&no->albuns, album))
                {
                    printf("Album %s cadastrado com sucesso.\n", album->info.titulo);                        
                    no->info.numAlbuns++;
                } 
                else 
                {
                    printf("Album %s ja existe na arvore. Tente outro valor.\n", album->info.titulo);
                    free(album);
                }    
                break;
            case 2:
                mostrarAlbuns(no->albuns);
                printf("\n");
                break;
            case 3:
                printf("Digite o ano do Album: "); scanf("%d", &ano);
                mostrarAlbunsPorAnoDeUMArtista(no->albuns, ano);
                printf("\n");
                break;
            case 4:
                printf("Digite o titulo do Album: "); scanf("%s", titulo);
                album = buscarAlbum(no->albuns, titulo);
                if(album != NULL)
                {
                    mostrarMusicas(album->musicas);
                    printf("\n");
                }else
                {
                    printf("\nAlbum %s nao encontrado!\nCadastre o album primeiro.\n", titulo);
                }
                break;
            case 5:
                printf("Digite o titulo do Album: ");scanf("%s", titulo);
                album = buscarAlbum(no->albuns, titulo);
                if(album != NULL)
                {
                    acessarAlbum(album, raizP);
                }
                else
                {
                    printf("\nAlbum %s nao encontrado!\nCadastre o album primeiro.\n", titulo);
                }
                break;
            default:
                printf("Opcao invalida!\n");
        }
    } while (opcao != 0);
}