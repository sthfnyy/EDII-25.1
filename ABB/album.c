#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include "interface.h"

// Funcao para alocar album
Album *alocarAlbum(infoAlbum dados)
{
    Album *no;
    no = (Album*) malloc (sizeof(Album)); // aloca memoria para o album
    if(no == NULL)
    {
        printf("Erro de acesso a memoria.\n"); // verifica se houve erro de alocação
        exit(1); 
    }
    no->esq = NULL; // lado esquerdo vazio
    no->dir = NULL; // lado direito vazio
    no->musicas = NULL; // ainda sem musicas
    no->info = dados; // preenche os dados do album
    return no; // retorna o ponteiro para o novo album
}

// Preenche os dados do album, tipo titulo e ano
infoAlbum preencherDadosAlbum()
{
    infoAlbum dados;
    printf("Digite o titulo do album: "); scanf("%s", dados.titulo); // recebe o titulo do album
    printf("Digite o ano de lancamento do album: "); scanf("%d", &dados.anoLancamento); // recebe o ano de lançamento
    dados.qtdMusicas = 0; // comeca com 0 musicas
    return dados; // retorna os dados preenchidos
}

// Cadastra um novo album na arvore
int inserirAlbum(Album **raizAlbum, Album *no)
{
    int cadastrado = 1;
    if(*raizAlbum == NULL)
    {
        *raizAlbum = no; // insere o album na raiz ou na subarvore
    }
    else
    {
        // verifica onde inserir o album, comparando pelo titulo
        if (strcmp((*raizAlbum)->info.titulo, no->info.titulo) > 0)
        {
            cadastrado = inserirAlbum(&(*raizAlbum)->esq, no); // insere na subarvore esquerda
        }
        else if(strcmp((*raizAlbum)->info.titulo, no->info.titulo) < 0)
        {
            cadastrado = inserirAlbum(&(*raizAlbum)->dir, no); // insere na subarvore direita
        }
        else
        {
            cadastrado = 0; // album já existe
        }
    }
    return(cadastrado); // retorna 1 se o album foi cadastrado, 0 se já existia
}

// Procura um album pelo titulo
Album *buscarAlbum(Album *raiz, char *titulo)
{
    Album *encontrado;
    encontrado = NULL; 
    if (raiz != NULL)
    {
        if(strcmp(raiz->info.titulo, titulo) == 0)
        {
            encontrado = raiz; // encontra o album
        }
        else if(strcmp(raiz->info.titulo, titulo) > 0)
        {
            encontrado = buscarAlbum(raiz->esq, titulo); // busca na subarvore esquerda
        } 
        else
        {
            encontrado = buscarAlbum(raiz->dir, titulo); // busca na subarvore direita
        }     
    }
    return(encontrado); // retorna o album encontrado ou NULL se não encontrado
}

// Mostra todos os albuns na arvore
void mostrarAlbuns(Album *raiz)
{
    if (raiz != NULL)
    {
        mostrarAlbuns(raiz->esq); // exibe albuns da subarvore esquerda
        printf("\nTitulo: %s, Ano Lancamento: %d, Quantidade de Musicas: %d \n", raiz->info.titulo, raiz->info.anoLancamento, raiz->info.qtdMusicas); // exibe os dados do album
        mostrarAlbuns(raiz->dir); // exibe albuns da subarvore direita
    }
}

// Libera os albuns da memoria
void liberarAlbum(Album **raiz)
{
    if(*raiz != NULL)
    {
        liberarAlbum(&(*raiz)->esq); // libera a subarvore esquerda
        liberarAlbum(&(*raiz)->dir); // libera a subarvore direita
        liberarMusica(&(*raiz)->musicas); // libera as musicas associadas ao album
        free(*raiz); // libera o espaço de memória do album
    }
}

// Mostra todos os albuns lancados em um certo ano de TODOS os artistas
void mostrarAlbunsPorAno(Artista *raiz, int ano)
{
    if (raiz != NULL)
    {
        mostrarAlbunsPorAno(raiz->esq, ano); // verifica a subarvore esquerda
        mostrarAlbunsPorAnoDeUMArtista(raiz->albuns, ano); // mostra os albuns do ano para esse artista
        mostrarAlbunsPorAno(raiz->dir, ano); // verifica a subarvore direita
    }
}

// Mostra todos os albuns de UM artista especifico
void mostrarTodosAlbunsdeUmArtista(Artista *raiz, char *nomeArtista)
{
    if (raiz != NULL)
    {
        mostrarTodosAlbunsdeUmArtista(raiz->esq, nomeArtista); // verifica a subarvore esquerda
        if (strcmp(raiz->info.nome, nomeArtista) == 0) // verifica se é o artista procurado
        {
            printf("Artista: %s\n", raiz->info.nome); // exibe o nome do artista
            mostrarAlbuns(raiz->albuns); // mostra os albuns do artista
        }
        mostrarTodosAlbunsdeUmArtista(raiz->dir, nomeArtista); // verifica a subarvore direita
    }
}

// Mostra os albuns de UM ARTISTA de UM ANO especifico
void mostrarAlbunsPorAnoDeUMArtista(Album *raiz, int ano)
{
    if (raiz != NULL)
    {
        mostrarAlbunsPorAnoDeUMArtista(raiz->esq, ano); // verifica a subarvore esquerda
        if (raiz->info.anoLancamento == ano) // verifica se o ano do album é o ano procurado
        {
            printf("\nAlbum: %s, Ano: %d, Quantidade de musicas: %d\n", raiz->info.titulo, raiz->info.anoLancamento, raiz->info.qtdMusicas); // exibe os dados do album
        }
        mostrarAlbunsPorAnoDeUMArtista(raiz->dir, ano); // verifica a subarvore direita
    }
}

// Menu interativo para um album especifico
void acessarAlbum(Album *no, Playlist *raizP)
{
    char titulo[50];
    int opcao, ano, removido;
    infoMusica dados;
    Musica *musica;
    MusicaPlaylist *musicaP;
    do 
    {
        // Menu com opções para o album
        printf("\nMenu do Album %s:\n", no->info.titulo);
        printf("0 - Sair\n");
        printf("1 - Cadastrar musica\n");
        printf("2 - Mostrar musicas\n");
        printf("3 - Remover musica\n");
        printf("Escolha uma opcao: "); scanf("%d", &opcao);
        switch (opcao) 
        {
            case 0:
                printf("Saindo...\n");
                break;
            case 1:
                // Cadastrar nova musica
                dados = preencherDadosMusica();
                musica  = alocarMusica(dados);
                if(inserirMusica(&no->musicas, musica)) // insere a musica na árvore
                {
                    printf("Musica %s cadastrada com sucesso.\n", musica->info.titulo); // sucesso
                    no->info.qtdMusicas++; // atualiza a quantidade de músicas do álbum
                } 
                else 
                {
                    printf("Musica %s ja existe na arvore. Tente outro valor.\n", musica->info.titulo); // falha ao inserir
                    free(musica);
                }    
                break;
            case 2:
                // Exibe as músicas do álbum
                printf("\nLista de musicas em ordem: ");
                mostrarMusicas(no->musicas);
                printf("\n");
                break;
            case 3:
                // Remove uma música
                printf("Digite o titulo da musica a ser removida: "); scanf("%s", titulo);
                musicaP = buscarMusicaNasPlaylists(raizP, titulo); // verifica se a musica está em alguma playlist
                if(musicaP == NULL)
                {
                    removido =  removerMusica(&no->musicas, titulo); // remove da árvore de músicas
                    if(removido)
                    {
                        printf("Musica %s removida com sucesso.\n", titulo);
                    }
                    else
                    {
                        printf("Falha ao remover.\n");
                    }
                }
                else
                {
                    printf("Musica inserida em uma playlist!\n"); // se a música estiver em uma playlist, não pode ser removida
                }
                break;
            default:
                printf("Opcao invalida!\n");
        }
    } while (opcao != 0);
}
