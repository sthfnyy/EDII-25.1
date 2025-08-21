#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include "interface.h"

// Cria uma nova musica na memoria
Musica *alocarMusica(infoMusica dados)
{
    Musica *no;
    no = (Musica*) malloc (sizeof(Musica));
    if(no == NULL)
    {
        printf("Erro de acesso a memoria.\n");
        exit(1); 
    }
    no->esq = NULL; // esquerda comeca vazia
    no->dir = NULL; // direita tambem
    no->info = dados;
    return no;
}

// Preenche os dados da musica, tipo o titulo e duracao
infoMusica preencherDadosMusica()
{
    infoMusica dados;
    printf("Digite o titulo da musica: "); scanf("%s", dados.titulo);
    printf("Digite a quantidade de minutos da musica: "); scanf("%d", &dados.qtdMinutos);
    return dados;
}

// Cadastra uma nova musica na arvore
int inserirMusica(Musica **raizMusica, Musica *no)
{
    int inserido = 1;
    if(*raizMusica == NULL)
    {
        *raizMusica = no; // adiciona na arvore
    }
    else
    {
        // compara os titulos pra decidir onde colocar
        if (strcmp((*raizMusica)->info.titulo, no->info.titulo) > 0)
        {
            inserido = inserirMusica(&(*raizMusica)->esq, no);
        }
        else if(strcmp((*raizMusica)->info.titulo, no->info.titulo) < 0)
        {
            inserido = inserirMusica(&(*raizMusica)->dir, no);
        }
        else
        {
            inserido = 0; // ja existe
        }
    }
    return(inserido);
}

// Mostra todas as musicas em ordem
void mostrarMusicas(Musica *raiz)
{
    if (raiz != NULL)
    {
        mostrarMusicas(raiz->esq);
        printf("Musica: %s, %d min\n", raiz->info.titulo, raiz->info.qtdMinutos);
        mostrarMusicas(raiz->dir);
    }
}

// Libera a memoria usada pelas musicas
void liberarMusica(Musica **raiz)
{
    if(*raiz != NULL)
    {
        liberarMusica(&(*raiz)->esq);
        liberarMusica(&(*raiz)->dir);
        free(*raiz); // apaga o no da memoria
    }
}

// Busca uma musica por titulo em todos os artistas
void buscarMusicaEmArtista(Artista *raiz, char *tituloMusica)
{
    if (raiz != NULL)
    {
        buscarMusicaEmArtista(raiz->esq, tituloMusica);
        buscarMusicaEmAlbum(raiz->albuns, tituloMusica, raiz->info.nome);
        buscarMusicaEmArtista(raiz->dir, tituloMusica);
    }
}

// Busca a musica dentro dos albuns de um artista
void buscarMusicaEmAlbum(Album *raiz, char *tituloMusica, char *nomeArtista)
{
    if (raiz != NULL)
    {
        buscarMusicaEmAlbum(raiz->esq, tituloMusica, nomeArtista);
        buscarMusicaEmMusicas(raiz->musicas, tituloMusica, nomeArtista, raiz->info.titulo, raiz->info.anoLancamento);
        buscarMusicaEmAlbum(raiz->dir, tituloMusica, nomeArtista);
    }
}

// Verifica se a musica ta dentro da lista de musicas de um album
void buscarMusicaEmMusicas(Musica *raiz, char *tituloMusica, char *nomeArtista, char *tituloAlbum, int anoAlbum)
{
    if (raiz != NULL)
    {
        buscarMusicaEmMusicas(raiz->esq, tituloMusica, nomeArtista, tituloAlbum, anoAlbum);
        if (strcmp(raiz->info.titulo, tituloMusica) == 0)
        {
            printf("Artista: %s, Album: %s, Ano: %d, Musica: %s\n", nomeArtista, tituloAlbum, anoAlbum, raiz->info.titulo);
        }
        buscarMusicaEmMusicas(raiz->dir, tituloMusica, nomeArtista, tituloAlbum, anoAlbum);
   }
}

Musica *buscarMusica(Musica *raiz, char *titulo)
{
    Musica *encontrado;
    encontrado = NULL; 
    if (raiz != NULL)
    {
        if(strcmp(raiz->info.titulo, titulo) == 0)
        {
            encontrado = raiz; // achou
        }
        else if(strcmp(raiz->info.titulo, titulo) > 0)
        {
            encontrado = buscarMusica(raiz->esq, titulo);
        } 
        else
        {
            encontrado = buscarMusica(raiz->dir, titulo);
        }     
    }
    return(encontrado);
}

int ehfolhaMusica(Musica *raiz)
{
    int folha = 0;
    if(raiz->esq == NULL && raiz->dir == NULL)
    {
        folha = 1;
    }
    return(folha);
}

Musica *soUmFilhoMusica(Musica *raiz)
{
    Musica *filho;
    filho = NULL;
    if(raiz->dir == NULL)
    {
        filho = raiz->esq;
    }
    else if(raiz->esq == NULL)
    {
        filho = raiz->dir;        
    }
    return(filho);
}

Musica **menorDaDirMusica(Musica **raiz)
{
    Musica **menor;
    menor = raiz;
    if((*raiz)->esq != NULL)  
    {
        while((*menor)->esq != NULL)
        {
            menor = &((*menor)->esq);
        }
    }
    return(menor);
}

// Remove uma musica da arvore
int removerMusica(Musica **raiz, char *titulo) 
{
    int removido = 1;
    if((*raiz) != NULL)
    {
        if(strcmp((*raiz)->info.titulo, titulo) == 0)
        {
            Musica *aux, *filho;
            if(ehfolhaMusica(*raiz))
            {
                aux = *raiz;
                *raiz = NULL;
                free(aux);
            }
            else
            {
                if((filho = soUmFilhoMusica(*raiz)) != NULL)
                {
                    aux = *raiz;
                    *raiz = filho;
                    free(aux);
                }
                else
                { 
                    //caso tenha 2 filhos
                    Musica **menor;
                    menor = menorDaDirMusica(&((*raiz)->dir));
                    (*raiz)->info = (**menor).info;
                    aux = *menor;
                    *menor = (**menor).dir;
                    free(aux);
                }
            }
        }
        else 
        {
            if (strcmp((*raiz)->info.titulo, titulo) > 0)
            {
                removido = removerMusica(&((*raiz)->esq), titulo);
            } 
            else
            {
                removido = removerMusica(&((*raiz)->dir), titulo);
            }  
        }
    }
    else
    {
        removido = 0;
    } 
    return (removido);
}