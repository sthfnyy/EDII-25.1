#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include "interfaceAvl.h"

// Funcao para alocar album
Album *alocarAlbum(infoAlbum dados)
{
    Album *no;
    no = (Album*) malloc (sizeof(Album));  // Aloca memória para um nó de álbum
    if(no == NULL)
    {
        printf("Erro de acesso a memoria.\n");  // Exibe erro se não conseguir alocar memória
        exit(1); 
    }
    no->esq = NULL;  // Inicializa ponteiro da esquerda como NULL
    no->dir = NULL;  // Inicializa ponteiro da direita como NULL
    no->musicas = NULL;  // Inicializa a árvore de músicas como NULL
    no->info = dados;  // Atribui os dados do álbum ao nó
    return no;  // Retorna o nó alocado
}

// Preenche os dados do album, tipo titulo e ano
infoAlbum preencherDadosAlbum()
{
    infoAlbum dados;
    printf("Digite o titulo do album: "); scanf("%s", dados.titulo);  // Solicita o título do álbum
    printf("Digite o ano de lancamento do album: "); scanf("%d", &dados.anoLancamento);  // Solicita o ano de lançamento
    dados.qtdMusicas = 0;  // Inicializa a quantidade de músicas como 0
    return dados;  // Retorna os dados preenchidos
}

// Cadastra um novo album na arvore
int inserirAlbum(Album **raizAlbum, Album *no)
{
    int cadastrado = 1;
    if(*raizAlbum == NULL)
    {
        *raizAlbum = no;  // Se a raiz for NULL, insere o novo nó na raiz
    }
    else
    {
        // Verifica onde inserir o nó (árvore binária de busca)
        if (strcmp((*raizAlbum)->info.titulo, no->info.titulo) > 0)
        {
            cadastrado = inserirAlbum(&(*raizAlbum)->esq, no);  // Insere na subárvore esquerda
        }
        else if(strcmp((*raizAlbum)->info.titulo, no->info.titulo) < 0)
        {
            cadastrado = inserirAlbum(&(*raizAlbum)->dir, no);  // Insere na subárvore direita
        }
        else
        {
            cadastrado = 0;  // Caso o álbum já exista, não insere
        }
    }
    if((*raizAlbum) != NULL)
    {
        if(cadastrado)
        {
            balanceamentoAlbum(raizAlbum);  // Realiza o balanceamento após a inserção
            atualizarAlturaAlbum(raizAlbum);  // Atualiza a altura dos nós da árvore
        }
    }
    return(cadastrado);  // Retorna se a inserção foi bem-sucedida
}

// Realiza o balanceamento da árvore de álbums
void balanceamentoAlbum(Album **raiz)
{
    int fb;
    fb = fatorBalanceamentoAlbum(*raiz);  // Calcula o fator de balanceamento
    if(fb == 2)  // Se o fator de balanceamento for 2 (desbalanceado à direita)
    {
        int fbEsq = fatorBalanceamentoAlbum((*raiz)->esq);  // Calcula o fator de balanceamento da subárvore esquerda
        if(fbEsq < 0)
        {
            rotacionarEsqAlbum(&(*raiz)->esq);  // Rotaciona a subárvore esquerda à esquerda
        }
        rotacionarDirAlbum(raiz);  // Rotaciona a árvore para a direita
    } 
    else if(fb == -2)  // Se o fator de balanceamento for -2 (desbalanceado à esquerda)
    {
        int fbDir = fatorBalanceamentoAlbum((*raiz)->dir);  // Calcula o fator de balanceamento da subárvore direita
        if(fbDir > 0)
        {
            rotacionarDirAlbum(&(*raiz)->dir);  // Rotaciona a subárvore direita à direita
        }
        rotacionarEsqAlbum(raiz);  // Rotaciona a árvore para a esquerda
    }
}

// Calcula o fator de balanceamento de um álbum
int fatorBalanceamentoAlbum(Album *raiz)
{
    int fb;
    fb = alturaAlbum(raiz->esq) - alturaAlbum(raiz->dir);  // Fator de balanceamento = altura esquerda - altura direita
    return fb;  // Retorna o fator de balanceamento
}

// Calcula a altura de um nó da árvore de álbums
int alturaAlbum(Album *raiz)
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

// Atualiza a altura de todos os nós a partir de um nó da árvore de álbums
void atualizarAlturaAlbum(Album **raiz)
{
    if((*raiz) != NULL)
    {
        atualizarAlturaAlbum(&(*raiz)->esq);  // Atualiza a altura da subárvore esquerda
        atualizarAlturaAlbum(&(*raiz)->dir);  // Atualiza a altura da subárvore direita
        (*raiz)->altura = alturaAlbum(*raiz);  // Atualiza a altura do nó atual
    }
}

// Rotaciona um nó da árvore de álbums para a esquerda
void rotacionarEsqAlbum(Album **raiz)
{
    Album *aux;
    aux = (*raiz)->dir;  // Armazena o nó à direita
    (*raiz)->dir = aux->esq;  // O nó à direita passa a ser a subárvore esquerda do nó auxiliar
    aux->esq = *raiz;  // O nó auxiliar passa a ser a subárvore esquerda
    *raiz = aux;  // O nó auxiliar se torna a raiz da subárvore
}

// Rotaciona um nó da árvore de álbums para a direita
void rotacionarDirAlbum(Album **raiz)
{
    Album *aux;
    aux = (*raiz)->esq;  // Armazena o nó à esquerda
    (*raiz)->esq = aux->dir;  // O nó à esquerda passa a ser a subárvore direita do nó auxiliar
    aux->dir = *raiz;  // O nó auxiliar passa a ser a subárvore direita
    *raiz = aux;  // O nó auxiliar se torna a raiz da subárvore
}

// Procura um álbum pelo título
Album *buscarAlbum(Album *raiz, char *titulo)
{
    Album *encontrado;
    encontrado = NULL; 
    if (raiz != NULL)
    {
        if(strcmp(raiz->info.titulo, titulo) == 0)
        {
            encontrado = raiz;  // Se encontrar, retorna o nó
        }
        else if(strcmp(raiz->info.titulo, titulo) > 0)
        {
            encontrado = buscarAlbum(raiz->esq, titulo);  // Busca na subárvore esquerda
        } 
        else
        {
            encontrado = buscarAlbum(raiz->dir, titulo);  // Busca na subárvore direita
        }     
    }
    return(encontrado);  // Retorna o álbum encontrado ou NULL
}

// Mostra todos os álbuns na árvore em ordem
void mostrarAlbuns(Album *raiz)
{
    if (raiz != NULL)
    {
        mostrarAlbuns(raiz->esq);  // Exibe os álbuns da subárvore esquerda
        printf("\nTitulo: %s, Ano Lancamento: %d, Quantidade de Musicas: %d, Altura: %d \n", raiz->info.titulo, raiz->info.anoLancamento, raiz->info.qtdMusicas, raiz->altura);  // Exibe os dados do álbum
        mostrarAlbuns(raiz->dir);  // Exibe os álbuns da subárvore direita
    }
}

// Libera a memória dos álbuns
void liberarAlbum(Album **raiz)
{
    if(*raiz != NULL)
    {
        liberarAlbum(&(*raiz)->esq);  // Libera a subárvore esquerda
        liberarAlbum(&(*raiz)->dir);  // Libera a subárvore direita
        liberarMusica(&(*raiz)->musicas);  // Libera a árvore de músicas do álbum
        free(*raiz);  // Libera o nó do álbum
    }
}

// Mostra todos os álbuns lançados em um certo ano de TODOS os artistas
void mostrarAlbunsPorAno(Artista *raiz, int ano)
{
    if (raiz != NULL)
    {
        mostrarAlbunsPorAno(raiz->esq, ano);  // Exibe os álbuns da subárvore esquerda
        mostrarAlbunsPorAnoDeUMArtista(raiz->albuns, ano);  // Exibe os álbuns do artista atual no ano especificado
        mostrarAlbunsPorAno(raiz->dir, ano);  // Exibe os álbuns da subárvore direita
    }
}

// Mostra todos os álbuns de UM artista específico
void mostrarTodosAlbunsdeUmArtista(Artista *raiz, char *nomeArtista)
{
    if (raiz != NULL)
    {
        mostrarTodosAlbunsdeUmArtista(raiz->esq, nomeArtista);  // Exibe os álbuns da subárvore esquerda
        if (strcmp(raiz->info.nome, nomeArtista) == 0)
        {
            printf("Artista: %s\n", raiz->info.nome);  // Exibe o nome do artista
            mostrarAlbuns(raiz->albuns);  // Exibe os álbuns do artista
        }
        mostrarTodosAlbunsdeUmArtista(raiz->dir, nomeArtista);  // Exibe os álbuns da subárvore direita
    }
}

// Mostra os álbuns de UM ARTISTA de UM ANO específico
void mostrarAlbunsPorAnoDeUMArtista(Album *raiz, int ano)
{
    if (raiz != NULL)
    {
        mostrarAlbunsPorAnoDeUMArtista(raiz->esq, ano);  // Exibe os álbuns da subárvore esquerda
        if (raiz->info.anoLancamento == ano)
        {
            printf("\nAlbum: %s, Ano: %d, Quantidade de musicas: %d\n", raiz->info.titulo, raiz->info.anoLancamento, raiz->info.qtdMusicas);  // Exibe os álbuns do ano específico
        }
        mostrarAlbunsPorAnoDeUMArtista(raiz->dir, ano);  // Exibe os álbuns da subárvore direita
    }
}

// Menu interativo para um álbum específico
void acessarAlbum(Album *no, Playlist *raizP)
{
    char titulo[50];
    int opcao, ano, removido;
    infoMusica dados;
    Musica *musica;
    MusicaPlaylist *musicaP;
    do 
    {
        printf("\nMenu do Album %s:\n", no->info.titulo);  // Exibe o título do álbum
        printf("0 - Sair\n");
        printf("1 - Cadastrar musica\n");
        printf("2 - Mostrar musicas\n");
        printf("3 - Remover musica\n");
        printf("Escolha uma opcao: "); scanf("%d", &opcao);  // Solicita a opção do menu
        switch (opcao) 
        {
            case 0:
                printf("Saindo...\n");
                break;
            case 1:
                dados = preencherDadosMusica();  // Solicita os dados da música
                musica  = alocarMusica(dados);  // Aloca a música
                if(inserirMusica(&no->musicas, musica))  // Insere a música na árvore
                {
                    printf("Musica %s cadastrada com sucesso.\n", musica->info.titulo);  // Exibe sucesso
                    no->info.qtdMusicas++;  // Incrementa a quantidade de músicas
                } 
                else 
                {
                    printf("Musica %s ja existe na arvore. Tente outro valor.\n", musica->info.titulo);  // Caso a música já exista
                    free(musica);  // Libera a memória se não for possível cadastrar
                }    
                break;
            case 2:
                printf("\nLista de musicas em ordem: ");
                mostrarMusicas(no->musicas);  // Exibe as músicas do álbum
                printf("\n");
                break;
            case 3:
                printf("Digite o titulo da musica a ser removida: "); scanf("%s", titulo);  // Solicita o título da música a ser removida
                musicaP = buscarMusicaNasPlaylists(raizP, titulo);  // Verifica se a música está em alguma playlist
                if(musicaP == NULL)
                {
                    removido =  removerMusica(&no->musicas, titulo);  // Remove a música do álbum
                    if(removido)
                    {
                        printf("Musica %s removida com sucesso.\n", titulo);  // Exibe sucesso
                    }
                    else
                    {
                        printf("Falha ao remover.\n");  // Caso a remoção falhe
                    }
                }
                else
                {
                    printf("Musica inserida em uma playlist!\n");  // Caso a música esteja em uma playlist
                }
                break;
            default:
                printf("Opcao invalida!\n");  // Caso a opção seja inválida
        }
    } while (opcao != 0);  // Continua até o usuário escolher sair
}
