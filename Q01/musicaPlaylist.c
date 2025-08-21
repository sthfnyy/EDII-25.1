#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "interface.h"

// Funcao para alocar musica
MusicaPlaylist *alocarMusicaP(infoMusicaPlaylist dados)
{
    MusicaPlaylist *no;
    no = (MusicaPlaylist*) malloc (sizeof(MusicaPlaylist)); // aloca memoria para a musica
    if(no == NULL)
    {
        printf("Erro de acesso a memoria.\n"); // caso ocorra erro de alocação
        exit(1); 
    }
    no->esq = NULL; // esquerda comeca vazia
    no->dir = NULL; // direita tambem vazia
    no->info = dados; // preenche os dados da musica
    return no; // retorna o ponteiro para a musica
}

// Preenche as informacoes da musica inserida na playlist: nome do artista, titulo do album, titulo da musica
infoMusicaPlaylist preencherDadosMusicaP(char *nomeArtista, char *tituloAlbum, char *tituloMusica)
{
    infoMusicaPlaylist dados;
    strcpy(dados.nomeArtista, nomeArtista); // copia o nome do artista
    strcpy(dados.tituloAlbum, tituloAlbum); // copia o titulo do album
    strcpy(dados.tituloMusica, tituloMusica); // copia o titulo da musica
    return dados; // retorna os dados preenchidos
}

// Insere uma musica na arvore de musicas de uma playlist
int inserirMusicaP(MusicaPlaylist **raizMusicap, MusicaPlaylist *no)
{
    int inserido = 1;
    if(*raizMusicap == NULL)
    {
        *raizMusicap = no; // insere o nó na raiz da árvore
    }
    else
    {
        // decide se vai pra esquerda ou direita
        if(strcmp((*raizMusicap)->info.tituloMusica, no->info.tituloMusica) > 0) 
        {
            inserido = inserirMusicaP(&(*raizMusicap)->esq, no); // insere na subarvore esquerda
        }
        else if(strcmp((*raizMusicap)->info.tituloMusica, no->info.tituloMusica) < 0)
        {
            inserido = inserirMusicaP(&(*raizMusicap)->dir, no); // insere na subarvore direita
        }
        else
        {
            inserido = 0; // já existe
        }
    }
    return(inserido); // retorna 1 se inserido, 0 se já existia
}

// Mostrar musicas de uma playlist;
void mostrarMusicasP(MusicaPlaylist *raiz)
{
    if(raiz != NULL)
    {
        mostrarMusicasP(raiz->esq); // exibe musicas da subarvore esquerda
        printf("\nNome do artista: %s Album: %s Musica: %s\n", raiz->info.nomeArtista, raiz->info.tituloAlbum, raiz->info.tituloMusica); // exibe as informações da musica
        mostrarMusicasP(raiz->dir); // exibe musicas da subarvore direita
    }
}

// Verificar se o no eh folha
int ehFolha(MusicaPlaylist *raiz)
{
    int folha = 0;
    if(raiz->esq == NULL && raiz->dir == NULL) // verifica se o nó é uma folha
    {
        folha = 1;
    }
    return(folha); // retorna 1 se for folha, 0 caso contrário
}

// Verificar se tem so um filho
MusicaPlaylist *soUmFilho(MusicaPlaylist *raiz)
{
    MusicaPlaylist *filho;
    filho = NULL;
    if(raiz->dir == NULL) // verifica se tem apenas um filho à esquerda
    {
        filho = raiz->esq;
    }
    else if(raiz->esq == NULL) // verifica se tem apenas um filho à direita
    {
        filho = raiz->dir;        
    }
    return(filho); // retorna o filho, ou NULL se não houver
}

// Encontra o menor da direita
MusicaPlaylist **menorDaDir(MusicaPlaylist **raiz) // encontra o menor valor na subarvore direita
{
    MusicaPlaylist **menor;
    menor = raiz;
    if((*raiz)->esq != NULL)  
    {
        while((*menor)->esq != NULL)
        {
            menor = &((*menor)->esq); // percorre até o menor valor
        }
    }
    return(menor); // retorna o menor nó da subarvore direita
}

// Busca uma musica nas playlists
MusicaPlaylist *buscarMusicaNasPlaylists(Playlist *raiz, char *nome)
{
    MusicaPlaylist *encontrado;
    encontrado = NULL;

    if(raiz != NULL)
    {
        encontrado = buscarMusicaPlaylist(raiz->info.musicas, nome); // busca na árvore de músicas da playlist
        if(encontrado == NULL)
        {
            encontrado = buscarMusicaNasPlaylists(raiz->esq, nome); // busca na subarvore esquerda
            if(encontrado == NULL)
            {
                encontrado = buscarMusicaNasPlaylists(raiz->dir, nome); // busca na subarvore direita
            }
        }
    }
    return(encontrado); // retorna a musica encontrada ou NULL
}

// Busca uma musica na playlist
MusicaPlaylist *buscarMusicaPlaylist(MusicaPlaylist *raiz, char *nome)
{
    MusicaPlaylist *encontrado;
    encontrado = NULL; 
    if (raiz != NULL)
    {
        if (strcmp(raiz->info.tituloMusica, nome) == 0) // verifica se o título da música é o procurado
        {
            encontrado = raiz; // encontrou a música
        }
        else if (strcmp(raiz->info.tituloMusica, nome) > 0)
        {
            encontrado = buscarMusicaPlaylist(raiz->esq, nome); // busca na subarvore esquerda
        } 
        else
        {
            encontrado = buscarMusicaPlaylist(raiz->dir, nome); // busca na subarvore direita
        }     
    }
    return(encontrado); // retorna a música encontrada ou NULL
}

// Remove a musica pelo titulo
int removerNo(MusicaPlaylist **raiz, char *titulo_musica)
{
    int removido = 1;
    if((*raiz) != NULL)
    {
        if(strcmp((*raiz)->info.tituloMusica, titulo_musica) == 0) // verifica se encontrou a música
        {
            MusicaPlaylist *aux, *filho;
            if(ehFolha(*raiz)) // se for uma folha
            {
                aux = *raiz;
                *raiz = NULL;
                free(aux);
            }
            else
            {
                if((filho = soUmFilho(*raiz)) != NULL) // se tiver um filho
                {
                    aux = *raiz;
                    *raiz = filho;
                    free(aux);
                }
                else
                { 
                    // caso tenha 2 filhos
                    MusicaPlaylist **menor;
                    menor = menorDaDir(&((*raiz)->dir)); // encontra o menor da subarvore direita
                    (*raiz)->info = (**menor).info; // copia os dados do menor nó
                    aux = *menor;
                    *menor = (**menor).dir; // ajusta a subarvore
                    free(aux);
                }
            }
        }
        else 
        {
            if (strcmp((*raiz)->info.tituloMusica, titulo_musica) > 0)
            {
                removido = removerNo(&((*raiz)->esq), titulo_musica); // busca na subarvore esquerda
            } 
            else
            {
                removido = removerNo(&((*raiz)->dir), titulo_musica); // busca na subarvore direita
            }  
        }
    }
    else
    {
        removido = 0; // não encontrou
    } 
    return (removido); // retorna 1 se removido, 0 se não encontrado
}

// Libera o que foi alocado na arvore de musicas da playlist
void liberarMusicaP(MusicaPlaylist **raiz)
{
    if(*raiz != NULL)
    {
        liberarMusicaP(&(*raiz)->esq); // libera a subarvore esquerda
        liberarMusicaP(&(*raiz)->dir); // libera a subarvore direita
        free(*raiz); // libera a memória da música
    }
}
