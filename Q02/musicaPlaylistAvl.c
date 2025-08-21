#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include "interfaceAvl.h"

// Funcao para alocar musica
MusicaPlaylist *alocarMusicaP(infoMusicaPlaylist dados)
{
    MusicaPlaylist *no;
    no = (MusicaPlaylist*) malloc (sizeof(MusicaPlaylist));  // Aloca memoria para um no de musica
    if(no == NULL)
    {
        printf("Erro de acesso a memoria.\n");  // Exibe erro se nao conseguir alocar memoria
        exit(1); 
    }
    no->esq = NULL;  // Inicializa ponteiro da esquerda como NULL
    no->dir = NULL;  // Inicializa ponteiro da direita como NULL
    no->info = dados;  // Atribui os dados passados ao no
    return no;  // Retorna o no alocado
}

// Preenche as informacoes da musica inserida na playlist: nome do artista, titulo do album, titulo da musica
infoMusicaPlaylist preencherDadosMusicaP(char *nomeArtista, char *tituloAlbum, char *tituloMusica)
{
    infoMusicaPlaylist dados;
    strcpy(dados.nomeArtista, nomeArtista);  // Copia nome do artista
    strcpy(dados.tituloAlbum, tituloAlbum);  // Copia titulo do album
    strcpy(dados.tituloMusica, tituloMusica);  // Copia titulo da musica
    return dados;  // Retorna os dados preenchidos
}

// Insere uma musica na arvore de musicas de uma playlist
int inserirMusicaP(MusicaPlaylist **raizMusicap, MusicaPlaylist *no)
{
    int inserido = 1;
    if(*raizMusicap == NULL)
    {
        *raizMusicap = no;  // Se a raiz estiver vazia, coloca o no nela
    }
    else
    {
        // Compara os titulos para decidir onde inserir o no
        if(strcmp((*raizMusicap)->info.tituloMusica, no->info.tituloMusica) > 0) 
        {
            inserido = inserirMusicaP(&(*raizMusicap)->esq, no);  // Insere na subarvore esquerda
        }
        else if(strcmp((*raizMusicap)->info.tituloMusica, no->info.tituloMusica) < 0)
        {
            inserido = inserirMusicaP(&(*raizMusicap)->dir, no);  // Insere na subarvore direita
        }
        else
        {
            inserido = 0;  // Se ja existir, retorna 0
        }
    }
    if((*raizMusicap) != NULL)
    {
        if(inserido)
        {
            balanceamentoMusicaPlaylist(raizMusicap);  // Balanceia a arvore apos insercao
            atualizarAlturaMusicaPlaylist(raizMusicap);  // Atualiza a altura dos nos
        }
    }
    return(inserido);  // Retorna se a musica foi inserida com sucesso
}

// Realiza o balanceamento da arvore de musicas
void balanceamentoMusicaPlaylist(MusicaPlaylist **raiz)
{
    int fb;
    fb = fatorBalanceamentoMusicaPlaylist(*raiz);  // Calcula o fator de balanceamento
    if(fb == 2)  // Caso a arvore esteja desbalanceada para a direita
    {
        int fbEsq = fatorBalanceamentoMusicaPlaylist((*raiz)->esq);  // Calcula o fator de balanceamento da subarvore esquerda
        if(fbEsq < 0)  // Se o fator de balanceamento da subarvore esquerda for negativo, realiza uma rotacao
        {
            rotacionarEsqMusicaPlaylist(&(*raiz)->esq);  // Rotaciona a subarvore esquerda
        }
        rotacionarDirMusicaPlaylist(raiz);  // Rotaciona a raiz para a direita
    }
    else if(fb == -2)  // Caso a arvore esteja desbalanceada para a esquerda
    {
        int fbDir = fatorBalanceamentoMusicaPlaylist((*raiz)->dir);  // Calcula o fator de balanceamento da subarvore direita
        if(fbDir > 0)  // Se o fator de balanceamento da subarvore direita for positivo, realiza uma rotacao
        {
            rotacionarDirMusicaPlaylist(&(*raiz)->dir);  // Rotaciona a subarvore direita
        }
        rotacionarEsqMusicaPlaylist(raiz);  // Rotaciona a raiz para a esquerda
    }
}

// Calcula o fator de balanceamento da arvore de musicas
int fatorBalanceamentoMusicaPlaylist(MusicaPlaylist *raiz)
{
    int fb;
    fb = alturaMusicaPlaylist(raiz->esq) - alturaMusicaPlaylist(raiz->dir);  // Fator de balanceamento = altura esquerda - altura direita
    return fb;  // Retorna o fator de balanceamento
}

// Calcula a altura de um no da arvore de musicas
int alturaMusicaPlaylist(MusicaPlaylist *raiz)
{
    int altura;
    if(raiz != NULL)
    {
        int altEsq = -1, altDir = -1;
        if(raiz->esq != NULL)
        {
            altEsq = raiz->esq->altura;  // Obtém a altura da subarvore esquerda
        }
        if(raiz->dir != NULL)
        {
            altDir = raiz->dir->altura;  // Obtém a altura da subarvore direita
        }
        if(altEsq > altDir)
        {
            altura = altEsq + 1;  // Se a subarvore esquerda for mais alta, incrementa 1
        }
        else
        {
            altura = altDir + 1;  // Caso contrário, incrementa 1 na altura da subarvore direita
        }
    }
    else
    {
        altura = -1;  // Se o no for NULL, a altura é -1
    }
    return altura;  // Retorna a altura calculada
}

// Atualiza a altura de todos os nos a partir de um no da arvore de musicas
void atualizarAlturaMusicaPlaylist(MusicaPlaylist **raiz)
{
    if((*raiz) != NULL)
    {
        atualizarAlturaMusicaPlaylist(&(*raiz)->esq);  // Atualiza a altura da subarvore esquerda
        atualizarAlturaMusicaPlaylist(&(*raiz)->dir);  // Atualiza a altura da subarvore direita
        (*raiz)->altura = alturaMusicaPlaylist(*raiz);  // Atualiza a altura do no atual
    }
}

// Rotaciona um no para a esquerda
void rotacionarEsqMusicaPlaylist(MusicaPlaylist **raiz)
{
    MusicaPlaylist *aux;
    aux = (*raiz)->dir;  // Armazena o nó à direita
    (*raiz)->dir = aux->esq;  // O nó à direita passa a ser a subarvore esquerda do nó auxiliar
    aux->esq = *raiz;  // O nó auxiliar passa a ser a subarvore esquerda
    *raiz = aux;  // O nó auxiliar se torna a raiz da subarvore
}

// Rotaciona um no para a direita
void rotacionarDirMusicaPlaylist(MusicaPlaylist **raiz)
{
    MusicaPlaylist *aux;
    aux = (*raiz)->esq;  // Armazena o nó à esquerda
    (*raiz)->esq = aux->dir;  // O nó à esquerda passa a ser a subarvore direita do nó auxiliar
    aux->dir = *raiz;  // O nó auxiliar passa a ser a subarvore direita
    *raiz = aux;  // O nó auxiliar se torna a raiz da subarvore
}

// Mostrar musicas de uma playlist;
void mostrarMusicasP(MusicaPlaylist *raiz)
{
    if(raiz != NULL)
    {
        mostrarMusicasP(raiz->esq);  // Exibe as músicas da subarvore esquerda
        printf("\nNome do artista: %s Album: %s Musica: %s, Altura: %d \n", raiz->info.nomeArtista, raiz->info.tituloAlbum, raiz->info.tituloMusica, raiz->altura);  // Exibe os dados da música
        mostrarMusicasP(raiz->dir);  // Exibe as músicas da subarvore direita
    }
}

// Verificar se o no eh folha
int ehFolha(MusicaPlaylist *raiz)
{
    int folha = 0;
    if(raiz->esq == NULL && raiz->dir == NULL)
    {
        folha = 1;  // Se não tiver filhos, é uma folha
    }
    return(folha);  // Retorna se o nó é uma folha
}

// Verificar se o nó tem só um filho
MusicaPlaylist *soUmFilho(MusicaPlaylist *raiz)
{
    MusicaPlaylist *filho;
    filho = NULL;
    if(raiz->dir == NULL)
    {
        filho = raiz->esq;  // Se não tiver filho à direita, retorna o filho à esquerda
    }
    else if(raiz->esq == NULL)
    {
        filho = raiz->dir;  // Se não tiver filho à esquerda, retorna o filho à direita
    }        
    return(filho);  // Retorna o filho
}

// Encontra o menor da direita
MusicaPlaylist **menorDaDir(MusicaPlaylist **raiz)  // A raiz é o valor à direita
{
    MusicaPlaylist **menor;
    menor = raiz;
    if((*raiz)->esq != NULL)  
    {
        while((*menor)->esq != NULL)
        {
            menor = &((*menor)->esq);  // Vai para a subárvore esquerda até encontrar o menor nó
        }
    }
    return(menor);  // Retorna o menor nó
}

// Busca uma musica em todas as playlists
MusicaPlaylist *buscarMusicaNasPlaylists(Playlist *raiz, char *nome)
{
    MusicaPlaylist *encontrado;
    encontrado = NULL;

    if(raiz != NULL)
    {
        encontrado = buscarMusicaPlaylist(raiz->info.musicas, nome);  // Busca na playlist atual
        if(encontrado == NULL)
        {
            encontrado = buscarMusicaNasPlaylists(raiz->esq, nome);  // Busca na subárvore esquerda
            if(encontrado == NULL)
            {
                encontrado = buscarMusicaNasPlaylists(raiz->dir, nome);  // Busca na subárvore direita
            }
        }
    }
    return(encontrado);  // Retorna a música encontrada ou NULL
}

// Busca uma musica dentro de uma playlist
MusicaPlaylist *buscarMusicaPlaylist(MusicaPlaylist *raiz, char *nome)
{
    MusicaPlaylist *encontrado;
    encontrado = NULL; 
    if (raiz != NULL)
    {
        if (strcmp(raiz->info.tituloMusica, nome) == 0)
        {
            encontrado = raiz;  // Se encontrar a música, retorna o nó
        }
        else if (strcmp(raiz->info.tituloMusica, nome) > 0)
        {
            encontrado = buscarMusicaPlaylist(raiz->esq, nome);  // Busca na subárvore esquerda
        } 
        else
        {
            encontrado = buscarMusicaPlaylist(raiz->dir, nome);  // Busca na subárvore direita
        }     
    }
    return(encontrado);  // Retorna o nó encontrado ou NULL
}

// Remove a musica pelo titulo
int removerNo(MusicaPlaylist **raiz, char *titulo_musica)
{
    int removido = 1;
    if((*raiz) != NULL)
    {
        if(strcmp((*raiz)->info.tituloMusica, titulo_musica) == 0)
        {
            MusicaPlaylist *aux, *filho;
            if(ehFolha(*raiz))
            {
                aux = *raiz;
                *raiz = NULL;
                free(aux);  // Libera o nó folha
            }
            else
            {
                if((filho = soUmFilho(*raiz)) != NULL)
                {
                    aux = *raiz;
                    *raiz = filho;
                    free(aux);  // Substitui o nó por seu único filho
                }
                else
                { 
                    // Caso tenha dois filhos
                    MusicaPlaylist **menor;
                    menor = menorDaDir(&((*raiz)->dir));  // Encontra o menor da subárvore direita
                    (*raiz)->info = (**menor).info;  // Substitui os dados do nó atual pelo do menor
                    aux = *menor;
                    *menor = (**menor).dir;
                    free(aux);  // Libera o nó substituído
                }
            }
        }
        else 
        {
            if (strcmp((*raiz)->info.tituloMusica, titulo_musica) > 0)
            {
                removido = removerNo(&((*raiz)->esq), titulo_musica);  // Busca na subárvore esquerda
            } 
            else
            {
                removido = removerNo(&((*raiz)->dir), titulo_musica);  // Busca na subárvore direita
            }  
        }
    }
    else
    {
        removido = 0;  // Caso o nó não exista
    }
    if((*raiz) != NULL)
    {
        if(removido)
        {
            balanceamentoMusicaPlaylist(raiz);  // Balanceia a árvore após remoção
            atualizarAlturaMusicaPlaylist(raiz);  // Atualiza a altura após remoção
        }
    }
    return (removido);  // Retorna se a remoção foi bem-sucedida
}

// Libera o que foi alocado na arvore de musicas da playlist
void liberarMusicaP(MusicaPlaylist **raiz)
{
    if(*raiz != NULL)
    {
        liberarMusicaP(&(*raiz)->esq);  // Libera a subárvore esquerda
        liberarMusicaP(&(*raiz)->dir);  // Libera a subárvore direita
        free(*raiz);  // Libera o nó atual
    }
}
