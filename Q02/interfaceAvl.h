#include <stdio.h>
#include <stdlib.h>

// Estrutura que guarda os dados da musica da playlist
typedef struct infoMusicaPlaylist
{
    char tituloMusica[50]; // titulo da musica
    char tituloAlbum[50];   // titulo do album
    char nomeArtista[50];   // nome do artista
}infoMusicaPlaylist;

// Estrutura de um no da arvore de musicas da playlist
typedef struct musicaplaylist MusicaPlaylist;
struct musicaplaylist
{
    infoMusicaPlaylist info; // dados da musica
    MusicaPlaylist *esq, *dir; // ponteiros pra esquerda e direita da arvore
    int altura; // altura do nó (para balanceamento)
};

// Estrutura que guarda os dados da playlist
typedef struct infoPlaylist
{
    char nome[50]; // nome da playlist
    MusicaPlaylist *musicas; // arvore de musicas
}infoPlaylist;

// Estrutura de um no da arvore de playlists
typedef struct playlist Playlist;
struct playlist
{
    infoPlaylist info; // dados da playlist
    int altura; // altura do nó (para balanceamento)
    Playlist *esq, *dir; // ponteiros pra esquerda e direita da arvore
};

// Estrutura que guarda os dados da musica
typedef struct infoMusica
{
    char titulo[50]; // nome da musica
    int qtdMinutos;  // duracao dela em minutos
}infoMusica;

// Estrutura de um no da arvore de musicas
typedef struct musica Musica;
struct musica
{
    infoMusica info; // dados da musica
    Musica *esq, *dir; // ponteiros pra esquerda e direita da arvore
    int altura; // altura do nó (para balanceamento)
};

// Estrutura que guarda os dados do album
typedef struct infoAlbum
{
    char titulo[50]; // nome do album
    int anoLancamento; // ano que saiu
    int qtdMusicas; // quantas musicas tem
}infoAlbum;

// Estrutura de um no da arvore de albuns
typedef struct album Album;
struct album
{
    infoAlbum info; // dados do album
    Album *esq, *dir; // ponteiros pra esquerda e direita
    int altura; // altura do nó (para balanceamento)
    Musica *musicas; // ponteiro pra arvore de musicas desse album
};

// Estrutura que guarda os dados do artista
typedef struct infoArtista
{
    char nome[50]; // nome do artista
    char tipo[50]; // tipo (solo, banda...)
    char estilo[50]; // estilo musical
    int numAlbuns; // quantos albuns tem
}infoArtista;

// Estrutura de um no da arvore de artistas
typedef struct artista Artista;
struct artista
{
    infoArtista info; // dados do artista
    Artista *esq, *dir; // ponteiros pra esquerda e direita
    int altura; // altura do nó (para balanceamento)
    Album *albuns;  // ponteiro pra arvore de albuns desse artista
};

// Funcoes relacionadas a Artista
Artista *alocaArtista(infoArtista dados); // cria novo artista
infoArtista preencherDadosArtista(); // adicionar dados de artista
int inserirArtista(Artista **raiz, Artista *no); // cadastra na arvore
void balanceamentoArtista(Artista **raiz); // balanceamento da arvore de artistas
int fatorBalanceamentoArtista(Artista *raiz); // calcula o fator de balanceamento de um nó
int alturaArtista(Artista *raiz); // retorna a altura de um nó
void atualizarAlturaArtista(Artista **raiz); // atualiza a altura de um nó
void rotacionarEsqArtista(Artista **raiz); // rotaciona a árvore para a esquerda
void rotacionarDirArtista(Artista **raiz); // rotaciona a árvore para a direita
Artista *buscarArtista(Artista *raiz, char *nome);  // procura artista
void acessarArtista(Artista *no, Playlist *raizP); // menu do artista
void mostrarArtistas(Artista *raiz); // mostra todos artistas
void mostrarArtistasEstilo(Artista *raiz, char *estilo); // mostra artistas de um estilo
void mostrarArtistasTipo(Artista *raiz, char *tipo); // mostra artistas de um tipo
void mostrarArtistasTipoEstilo(Artista *raiz, char *tipo, char *estilo); // mostra por tipo e estilo
void liberarArtistas(Artista **raiz); // libera memoria dos artistas
void mostrarAlbunsDeArtistasPorAno(Artista *raiz, int ano); // mostra albuns por ano

// Funcoes relacionadas a Album
Album *alocarAlbum(infoAlbum dados); // cria novo album
infoAlbum preencherDadosAlbum(); // preenche os dados do album
int inserirAlbum(Album **raizAlbum, Album *no); // cadastra album na arvore
void balanceamentoAlbum(Album **raiz); // balanceamento da arvore de albuns
int fatorBalanceamentoAlbum(Album *raiz); // calcula o fator de balanceamento de um nó
int alturaAlbum(Album *raiz); // retorna a altura de um nó
void atualizarAlturaAlbum(Album **raiz); // atualiza a altura de um nó
void rotacionarEsqAlbum(Album **raiz); // rotaciona a árvore para a esquerda
void rotacionarDirAlbum(Album **raiz); // rotaciona a árvore para a direita
Album *buscarAlbum(Album *raiz, char *titulo);  // procura album
void acessarAlbum(Album *no, Playlist *raizP); // menu do album
void mostrarAlbuns(Album *raiz); // mostra todos albuns
void liberarAlbum(Album **raiz); // libera memoria dos albuns
void mostrarAlbunsPorAno(Artista *raiz, int ano); // mostra todos albuns de todos artistas por ano
void mostrarAlbunsPorAnoDeUMArtista(Album *raiz, int ano); // mostra albuns de um artista por ano

// Funcoes relacionadas a Musica
Musica *alocarMusica(infoMusica dados); // cria nova musica
infoMusica preencherDadosMusica(); // coloca dados da musica
int inserirMusica(Musica **raizMusica, Musica *no); // cadastra na arvore
void balanceamentoMusica(Musica **raiz); // balanceamento da arvore de musicas
int fatorBalanceamentoMusica(Musica *raiz); // calcula o fator de balanceamento de um nó
int alturaMusica(Musica *raiz); // retorna a altura de um nó
void atualizarAlturaMusica(Musica **raiz); // atualiza a altura de um nó
void rotacionarEsqMusica(Musica **raiz); // rotaciona a árvore para a esquerda
void rotacionarDirMusica(Musica **raiz); // rotaciona a árvore para a direita
void mostrarMusicas(Musica *raiz); // mostra todas musicas
void liberarMusica(Musica **raiz); // libera memoria das musicas
void buscarMusicaEmArtista(Artista *raiz, char *tituloMusica); // busca musica em todos artistas
void buscarMusicaEmAlbum(Album *raiz, char *tituloMusica, char *nomeArtista); // busca musica dentro dos albuns
void buscarMusicaEmMusicas(Musica *raiz, char *tituloMusica, char *nomeArtista, char *tituloAlbum, int anoAlbum); // busca musica nos dados da musica
Musica *buscarMusica(Musica *raiz, char *titulo); // busca musica pelo título
int removerMusica(Musica **raiz, char *titulo); // remove musica da arvore
int ehfolhaMusica(Musica *raiz); // verifica se o nó é folha
Musica *soUmFilhoMusica(Musica *raiz); // verifica se o nó tem um só filho
Musica **menorDaDirMusica(Musica **raiz); // encontra o menor nó da subarvore direita

// Funcoes relacionadas a Playlist
Playlist *alocarPlaylist(infoPlaylist dados); // cria nova playlist
infoPlaylist preencherDadosPlaylist(); // preenche dados da playlist
int inserirPlaylist(Playlist **raizPlaylist, Playlist *no); // cadastra na arvore de playlists
void balanceamentoPlaylist(Playlist **raiz); // balanceamento da arvore de playlists
int fatorBalanceamentoPlaylist(Playlist *raiz); // calcula o fator de balanceamento de um nó
int alturaPlaylist(Playlist *raiz); // retorna a altura de um nó
void atualizarAlturaPlaylist(Playlist **raiz); // atualiza a altura de um nó
void rotacionarEsqPlaylist(Playlist **raiz); // rotaciona a árvore para a esquerda
void rotacionarDirPlaylist(Playlist **raiz); // rotaciona a árvore para a direita
void menuPlaylist(Playlist **raizPlaylist, Artista *raizArtista); // menu de interação da playlist
void mostrarPlaylists(Playlist *raiz); // mostra todas as playlists
Playlist *buscarPlaylist(Playlist *raiz, char *nome); // busca playlist pelo nome
void liberarPlaylists(Playlist **raiz); // libera memoria das playlists
int removerPlaylist(Playlist **raiz, char *titulo); // remove playlist da arvore
int ehfolhaPlaylist(Playlist *raiz); // verifica se o nó é folha
Playlist *soUmFilhoPlaylist(Playlist *raiz); // verifica se o nó tem um filho
Playlist **menorDaDirPlaylist(Playlist **raiz); // encontra o menor nó da subarvore direita

// Funcoes relacionadas a Musica da Playlist
MusicaPlaylist *alocarMusicaP(infoMusicaPlaylist dados); // cria nova musica na playlist
infoMusicaPlaylist preencherDadosMusicaP(char *nomeArtista, char *tituloAlbum, char *tituloMusica); // preenche dados da musica da playlist
int inserirMusicaP(MusicaPlaylist **raizMusicap, MusicaPlaylist *no); // insere musica na arvore
void balanceamentoMusicaPlaylist(MusicaPlaylist **raiz); // balanceamento da arvore de musicas da playlist
int fatorBalanceamentoMusicaPlaylist(MusicaPlaylist *raiz); // calcula o fator de balanceamento de um nó
int alturaMusicaPlaylist(MusicaPlaylist *raiz); // retorna a altura de um nó
void atualizarAlturaMusicaPlaylist(MusicaPlaylist **raiz); // atualiza a altura de um nó
void rotacionarEsqMusicaPlaylist(MusicaPlaylist **raiz); // rotaciona a árvore para a esquerda
void rotacionarDirMusicaPlaylist(MusicaPlaylist **raiz); // rotaciona a árvore para a direita
void mostrarMusicasP(MusicaPlaylist *raiz); // mostra todas as musicas na playlist
int ehFolha(MusicaPlaylist *raiz); // verifica se o nó é folha
MusicaPlaylist *soUmFilho(MusicaPlaylist *raiz); // verifica se o nó tem um só filho
MusicaPlaylist **menorDaDir(MusicaPlaylist **raiz); // encontra o menor nó da subarvore direita
MusicaPlaylist *buscarMusicaNasPlaylists(Playlist *raiz, char *nome); // busca musica nas playlists
MusicaPlaylist *buscarMusicaPlaylist(MusicaPlaylist *raiz, char *nome); // busca musica na playlist
int removerNo(MusicaPlaylist **raiz, char *titulo_musica); // remove musica na playlist
void liberarMusicaP(MusicaPlaylist **raiz); // libera memoria das musicas na playlist
