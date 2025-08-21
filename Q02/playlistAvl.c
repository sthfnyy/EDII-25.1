#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "interfaceAvl.h"

// Funcao para alocar playlist
Playlist *alocarPlaylist(infoPlaylist dados)
{
    Playlist *no;
    no = (Playlist*) malloc (sizeof(Playlist));  // Aloca memoria para um no de playlist
    if(no == NULL)
    {
        printf("Erro de acesso a memoria.\n");  // Exibe erro se não conseguir alocar memória
        exit(1); 
    }
    no->esq = NULL;  // Inicializa ponteiro da esquerda como NULL
    no->dir = NULL;  // Inicializa ponteiro da direita como NULL
    no->info = dados;  // Atribui os dados passados ao no
    return no;  // Retorna o no alocado
}

// Preenche as informacoes da playlist: nome
infoPlaylist preencherDadosPlaylist()
{
    infoPlaylist dados;
    printf("Digite o nome da playlist: "); scanf("%s", &dados.nome);  // Solicita o nome da playlist
    dados.musicas = NULL;  // Inicializa a árvore de músicas como NULL
    return dados;  // Retorna os dados preenchidos
}

// Insere uma playlist na árvore
int inserirPlaylist(Playlist **raizPlaylist, Playlist *no)
{
    int inserido = 1;
    if(*raizPlaylist == NULL)
    {
        *raizPlaylist = no;  // Se a raiz estiver vazia, coloca o nó nela
    }
    else
    {
        // Decide se a playlist vai para a esquerda ou direita com base no nome
        if(strcmp((*raizPlaylist)->info.nome, no->info.nome) > 0) 
        {
            inserido = inserirPlaylist(&(*raizPlaylist)->esq, no);  // Insere na subárvore esquerda
        }
        else if(strcmp((*raizPlaylist)->info.nome, no->info.nome) < 0)
        {
            inserido = inserirPlaylist(&(*raizPlaylist)->dir, no);  // Insere na subárvore direita
        }
        else
        {
            inserido = 0;  // Se já existir, retorna 0
        }
    }
    if((*raizPlaylist) != NULL)
    {
        if(inserido)
        {
            balanceamentoPlaylist(raizPlaylist);  // Balanceia a árvore após inserção
            atualizarAlturaPlaylist(raizPlaylist);  // Atualiza a altura dos nós
        }
    }
    return(inserido);  // Retorna se a playlist foi inserida com sucesso
}

// Realiza o balanceamento da árvore de playlists
void balanceamentoPlaylist(Playlist **raiz)
{
    int fb;
    fb = fatorBalanceamentoPlaylist(*raiz);  // Calcula o fator de balanceamento
    if(fb == 2)  // Caso a árvore esteja desbalanceada para a direita
    {
        int fbEsq = fatorBalanceamentoPlaylist((*raiz)->esq);  // Calcula o fator de balanceamento da subárvore esquerda
        if(fbEsq < 0)
        {
            rotacionarEsqPlaylist(&(*raiz)->esq);  // Rotaciona a subárvore esquerda
        }
        rotacionarDirPlaylist(raiz);  // Rotaciona a raiz para a direita
    }
    else if(fb == -2)  // Caso a árvore esteja desbalanceada para a esquerda
    {
        int fbDir = fatorBalanceamentoPlaylist((*raiz)->dir);  // Calcula o fator de balanceamento da subárvore direita
        if(fbDir > 0)
        {
            rotacionarDirPlaylist(&(*raiz)->dir);  // Rotaciona a subárvore direita
        }
        rotacionarEsqPlaylist(raiz);  // Rotaciona a raiz para a esquerda
    }
}

// Calcula o fator de balanceamento da árvore de playlists
int fatorBalanceamentoPlaylist(Playlist *raiz)
{
    int fb;
    fb = alturaPlaylist(raiz->esq) - alturaPlaylist(raiz->dir);  // Fator de balanceamento = altura esquerda - altura direita
    return fb;  // Retorna o fator de balanceamento
}

// Calcula a altura de um nó da árvore de playlists
int alturaPlaylist(Playlist *raiz)
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

// Atualiza a altura de todos os nós a partir de um nó da árvore de playlists
void atualizarAlturaPlaylist(Playlist **raiz)
{
    if((*raiz) != NULL)
    {
        atualizarAlturaPlaylist(&(*raiz)->esq);  // Atualiza a altura da subárvore esquerda
        atualizarAlturaPlaylist(&(*raiz)->dir);  // Atualiza a altura da subárvore direita
        (*raiz)->altura = alturaPlaylist(*raiz);  // Atualiza a altura do nó atual
    }
}

// Rotaciona um nó para a esquerda
void rotacionarEsqPlaylist(Playlist **raiz)
{
    Playlist *aux;
    aux = (*raiz)->dir;  // Armazena o nó à direita
    (*raiz)->dir = aux->esq;  // O nó à direita passa a ser a subárvore esquerda do nó auxiliar
    aux->esq = *raiz;  // O nó auxiliar passa a ser a subárvore esquerda
    *raiz = aux;  // O nó auxiliar se torna a raiz da subárvore
}

// Rotaciona um nó para a direita
void rotacionarDirPlaylist(Playlist **raiz)
{
    Playlist *aux;
    aux = (*raiz)->esq;  // Armazena o nó à esquerda
    (*raiz)->esq = aux->dir;  // O nó à esquerda passa a ser a subárvore direita do nó auxiliar
    aux->dir = *raiz;  // O nó auxiliar passa a ser a subárvore direita
    *raiz = aux;  // O nó auxiliar se torna a raiz da subárvore
}

// Mostra todas as playlists da árvore
void mostrarPlaylists(Playlist *raiz)
{
    if(raiz != NULL)
    {
        mostrarPlaylists(raiz->esq);  // Exibe as playlists da subárvore esquerda
        printf("\nPlaylist: %s \n", raiz->info.nome);  // Exibe os dados da playlist
        mostrarPlaylists(raiz->dir);  // Exibe as playlists da subárvore direita
    }
}

// Libera a memória alocada na árvore de playlists
void liberarPlaylists(Playlist **raiz)
{
    if(*raiz != NULL)
    {
        liberarPlaylists(&(*raiz)->esq);  // Libera a subárvore esquerda
        liberarPlaylists(&(*raiz)->dir);  // Libera a subárvore direita
        liberarMusicaP(&(*raiz)->info.musicas);  // Libera a árvore de músicas da playlist
        free(*raiz);  // Libera o nó da memória
    }
}

// Procura uma playlist pelo nome
Playlist *buscarPlaylist(Playlist *raiz, char *nome)
{
    Playlist *encontrado;
    encontrado = NULL; 
    if (raiz != NULL)
    {
        if (strcmp(raiz->info.nome, nome) == 0)
        {
            encontrado = raiz;  // Se encontrar a playlist, retorna o nó
        }
        else if (strcmp(raiz->info.nome, nome) > 0)
        {
            encontrado = buscarPlaylist(raiz->esq, nome);  // Busca na subárvore esquerda
        } 
        else
        {
            encontrado = buscarPlaylist(raiz->dir, nome);  // Busca na subárvore direita
        }     
    }
    return(encontrado);  // Retorna a playlist encontrada ou NULL
}

// Verifica se o nó é uma folha
int ehfolhaPlaylist(Playlist *raiz)
{
    int folha = 0;
    if(raiz->esq == NULL && raiz->dir == NULL)
    {
        folha = 1;  // Se não tiver filhos, é uma folha
    }
    return(folha);  // Retorna se o nó é uma folha
}

// Verifica se o nó tem apenas um filho
Playlist *soUmFilhoPlaylist(Playlist *raiz)
{
    Playlist *filho;
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

// Encontra o menor nó à direita
Playlist **menorDaDirPlaylist(Playlist **raiz)
{
    Playlist **menor;
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

// Remove uma playlist pelo nome
int removerPlaylist(Playlist **raiz, char *titulo)
{
    int removido = 1;
    if((*raiz) != NULL)
    {
        if(strcmp((*raiz)->info.nome, titulo) == 0)
        {
            Playlist *aux, *filho;
            if(ehfolhaPlaylist(*raiz))
            {
                aux = *raiz;
                *raiz = NULL;
                free(aux);  // Libera o nó folha
            }
            else
            {
                if((filho = soUmFilhoPlaylist(*raiz)) != NULL)
                {
                    aux = *raiz;
                    *raiz = filho;
                    free(aux);  // Substitui o nó por seu único filho
                }
                else
                { 
                    // Caso tenha dois filhos
                    Playlist **menor;
                    menor = menorDaDirPlaylist(&((*raiz)->dir));  // Encontra o menor nó da subárvore direita
                    (*raiz)->info = (**menor).info;  // Substitui os dados do nó atual pelo do menor
                    aux = *menor;
                    *menor = (**menor).dir;
                    free(aux);  // Libera o nó substituído
                }
            }
        }
        else 
        {
            if (strcmp((*raiz)->info.nome, titulo) > 0)
            {
                removido = removerPlaylist(&((*raiz)->esq), titulo);  // Busca na subárvore esquerda
            } 
            else
            {
                removido = removerPlaylist(&((*raiz)->dir), titulo);  // Busca na subárvore direita
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
            balanceamentoPlaylist(raiz);  // Balanceia a árvore após remoção
            atualizarAlturaPlaylist(raiz);  // Atualiza a altura após remoção
        }
    }
    return (removido);  // Retorna se a remoção foi bem-sucedida
}

// Menu para gerenciamento das playlists
void menuPlaylist(Playlist **raizPlaylist, Artista *raizArtista)
{
    int opcao;
    char nome[50];
    Playlist *no;
    infoPlaylist dados;
    MusicaPlaylist *noMusica;
    infoMusicaPlaylist dadosMusicaP;
    Artista *artista;
    Album *album;
    Musica *musica;
    do 
    {
        printf("0 - Sair\n");
        printf("1 - Cadastrar playlist\n");
        printf("2 - Mostrar todas as playlists\n");
        printf("3 - Mostrar dados de uma playlist\n");
        printf("4 - Adicionar musica\n");
        printf("5 - Remover musica de uma playlist\n");
        printf("6 - Remover playlist\n");
        printf("Escolha uma opcao: "); scanf("%d", &opcao);
        switch (opcao) 
        {
            case 0:
                printf("Saindo...\n");
                break;
            case 1:
                dados = preencherDadosPlaylist();  // Preenche os dados da playlist
                no = alocarPlaylist(dados);  // Aloca a nova playlist
                if(inserirPlaylist(raizPlaylist, no))  // Insere a playlist na árvore
                {
                    printf("Playlist %s cadastrada com sucesso.\n", no->info.nome);                    
                }
                else
                {
                    printf("Playlist %s ja existe na arvore. Tente outro valor.\n", no->info.nome);
                    free(no);  // Libera a memória se a playlist já existir
                }
                break;
            case 2:
                mostrarPlaylists(*raizPlaylist);  // Exibe todas as playlists
                printf("\n");
                break;
            case 3:
                printf("Digite o nome da playlist: "); scanf("%s", nome);  // Solicita o nome da playlist
                no = buscarPlaylist(*raizPlaylist, nome);  // Busca a playlist
                if(no)
                {
                    mostrarMusicasP(no->info.musicas);  // Exibe as músicas da playlist
                }
                printf("\n");
                break;
            case 4:
                printf("Digite o nome da playlist: "); scanf("%s", nome);
                no = buscarPlaylist(*raizPlaylist, nome);  // Busca a playlist
                if(no)
                {
                    printf("Digite o nome do artista: "); scanf("%s", nome);  // Solicita o nome do artista
                    artista = buscarArtista(raizArtista, nome);  // Busca o artista
                    if(artista)
                    {
                        printf("Digite o titulo do album: "); scanf("%s", nome);  // Solicita o título do álbum
                        album = buscarAlbum(artista->albuns, nome);  // Busca o álbum
                        if(album)
                        {
                            printf("Digite o titulo da musica: "); scanf("%s", nome);  // Solicita o título da música
                            musica = buscarMusica(album->musicas, nome);  // Busca a música
                            if(musica)
                            {
                                dadosMusicaP = preencherDadosMusicaP(artista->info.nome, album->info.titulo, musica->info.titulo);  // Preenche os dados da música
                                noMusica = alocarMusicaP(dadosMusicaP);  // Aloca a nova música
                                int inseriu = inserirMusicaP(&(no->info.musicas), noMusica);  // Insere a música na playlist
                                if(inseriu)
                                {
                                    printf("Musica %s cadastrada com sucesso.\n", noMusica->info.tituloMusica);  // Confirma a inserção
                                }
                                else
                                {
                                    printf("Musica %s ja existe na arvore. Tente outro valor.\n", noMusica->info.tituloMusica);  // Informa se já existir
                                    free(noMusica);
                                }
                            }
                            else
                            {
                                printf("Musica nao encontrada!\n");  // Caso a música não seja encontrada
                            }
                        }
                        else
                        {
                            printf("Album nao encontrado!\n");  // Caso o álbum não seja encontrado
                        }
                    }
                    else
                    {
                        printf("Artista nao encontrado!\n");  // Caso o artista não seja encontrado
                    }
                }
                else
                {
                    printf("Playlist nao encontrada!\n");  // Caso a playlist não seja encontrada
                }
                break;
            case 5:
                printf("Digite o nome da playlist: "); scanf("%s", nome);  // Solicita o nome da playlist
                no = buscarPlaylist(*raizPlaylist, nome);  // Busca a playlist
                if(no)
                {
                    printf("Digite o nome da musica na playlist: "); scanf("%s", nome);  // Solicita o título da música na playlist
                    noMusica = buscarMusicaPlaylist(no->info.musicas, nome);  // Busca a música na playlist
                    if(noMusica)
                    {
                        if(removerNo(&no->info.musicas, nome))  // Remove a música da playlist
                        {
                            printf("Musica removida com sucesso!\n");  // Confirma remoção
                        }
                        else
                        {
                            printf("Falha ao remover!\n");  // Caso falhe na remoção
                        }
                    }
                    else
                    {
                        printf("Musica nao encontrada!\n");  // Caso a música não seja encontrada
                    }
                }
                else
                {
                    printf("Playlist nao encontrada!\n");  // Caso a playlist não seja encontrada
                }
                break;
            case 6:
                printf("Digite o nome da playlist que deseja remover: "); scanf("%s", nome);  // Solicita o nome da playlist a ser removida
                if (removerPlaylist(raizPlaylist, nome))  // Remove a playlist
                {
                    printf("Playlist %s removida com sucesso.\n", nome);  // Confirma remoção
                } 
                else 
                {
                    printf("Falha ao remover.\n");  // Caso falhe na remoção
                }
                break;
            default:
                printf("Opcao invalida!\n");  // Caso a opção seja inválida
        }
    } while (opcao != 0);  // Continua até o usuário escolher sair
}
