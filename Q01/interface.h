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
    Album *albuns;  // ponteiro pra arvore de albuns desse artista
};

// Funcoes relacionadas a Artista
Artista *alocaArtista(infoArtista dados); // cria novo artista
infoArtista preencherDadosArtista(); // adicionar dados de artista
int inserirArtista(Artista **raiz, Artista *no); // cadastra na arvore
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
infoAlbum preencherDadosAlbum(); 
int inserirAlbum(Album **raizAlbum, Album *no); // cadastra album na arvore
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
void mostrarMusicas(Musica *raiz); // mostra todas musicas
void liberarMusica(Musica **raiz); // libera memoria das musicas
void buscarMusicaEmArtista(Artista *raiz, char *tituloMusica); // busca musica em todos artistas
void buscarMusicaEmAlbum(Album *raiz, char *tituloMusica, char *nomeArtista); // busca musica dentro dos albuns
void buscarMusicaEmMusicas(Musica *raiz, char *tituloMusica, char *nomeArtista, char *tituloAlbum, int anoAlbum); // busca musica nos dados da musica
Musica *buscarMusica(Musica *raiz, char *titulo);
int removerMusica(Musica **raiz, char *titulo);
int ehfolhaMusica(Musica *raiz);
Musica *soUmFilhoMusica(Musica *raiz);
Musica **menorDaDirMusica(Musica **raiz);

// Funcoes relacionadas a Playlist
Playlist *alocarPlaylist(infoPlaylist dados); // cria nova playlist
infoPlaylist preencherDadosPlaylist(); // preenche dados da playlist
int inserirPlaylist(Playlist **raizPlaylist, Playlist *no); // cadastra na arvore de playlists
void menuPlaylist(Playlist **raizPlaylist, Artista *raizArtista); // menu de interação da playlist
void mostrarPlaylists(Playlist *raiz); // mostra todas as playlists
Playlist *buscarPlaylist(Playlist *raiz, char *nome); // busca playlist pelo nome
void liberarPlaylists(Playlist **raiz); // libera memoria das playlists
int removerPlaylist(Playlist **raiz, char *titulo); // remove playlist da arvore
int ehfolhaPlaylist(Playlist *raiz); // verifica se o nó é folha
Playlist *soUmFilhoPlaylist(Playlist *raiz); // verifica se o nó tem um só filho
Playlist **menorDaDirPlaylist(Playlist **raiz); // encontra o menor nó da subarvore direita

// Funcoes relacionadas a Musica da Playlist
MusicaPlaylist *alocarMusicaP(infoMusicaPlaylist dados); // cria nova musica na playlist
infoMusicaPlaylist preencherDadosMusicaP(char *nomeArtista, char *tituloAlbum, char *tituloMusica); // preenche dados da musica da playlist
int inserirMusicaP(MusicaPlaylist **raizMusicap, MusicaPlaylist *no); // insere musica na arvore
void mostrarMusicasP(MusicaPlaylist *raiz); // mostra todas as musicas na playlist
int ehFolha(MusicaPlaylist *raiz); // verifica se o nó é folha
MusicaPlaylist *soUmFilho(MusicaPlaylist *raiz); // verifica se o nó tem um filho
MusicaPlaylist **menorDaDir(MusicaPlaylist **raiz); // encontra o menor nó da subarvore direita
MusicaPlaylist *buscarMusicaNasPlaylists(Playlist *raiz, char *nome); // busca musica nas playlists
MusicaPlaylist *buscarMusicaPlaylist(MusicaPlaylist *raiz, char *nome); // busca musica na playlist
int removerNo(MusicaPlaylist **raiz, char *titulo_musica); // remove musica na playlist
void liberarMusicaP(MusicaPlaylist **raiz); // libera memoria das musicas na playlist
