#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "interface.h"

// Funcao para alocar playlist
Playlist *alocarPlaylist(infoPlaylist dados)
{
    Playlist *no;
    no = (Playlist*) malloc (sizeof(Playlist)); // aloca memoria para a nova playlist
    if(no == NULL)
    {
        printf("Erro de acesso a memoria.\n"); // caso ocorra erro de alocação
        exit(1); 
    }
    no->esq = NULL; // inicializa a subarvore esquerda vazia
    no->dir = NULL; // inicializa a subarvore direita vazia
    no->info = dados; // preenche as informações da playlist
    return no; // retorna o ponteiro para a nova playlist
}

// Preenche as informacoes da playlist: nome
infoPlaylist preencherDadosPlaylist()
{
    infoPlaylist dados;
    printf("Digite o nome da playlist: "); scanf("%s", &dados.nome); // recebe o nome da playlist
    dados.musicas = NULL; // inicia sem músicas
    return dados; // retorna os dados preenchidos
}

// Cadastra uma nova playlist na arvore binária
int inserirPlaylist(Playlist **raizPlaylist, Playlist *no)
{
    int inserido = 1;
    if(*raizPlaylist == NULL)
    {
        *raizPlaylist = no; // coloca o no na raiz da árvore
    }
    else
    {
        // decide se vai inserir na subarvore esquerda ou direita
        if(strcmp((*raizPlaylist)->info.nome, no->info.nome) > 0) 
        {
            inserido = inserirPlaylist(&(*raizPlaylist)->esq, no); // insere à esquerda
        }
        else if(strcmp((*raizPlaylist)->info.nome, no->info.nome) < 0)
        {
            inserido = inserirPlaylist(&(*raizPlaylist)->dir, no); // insere à direita
        }
        else
        {
            inserido = 0; // já existe
        }
    }
    return(inserido); // retorna 1 se inserido, 0 se já existia
}

// Mostra todas as playlists da arvore
void mostrarPlaylists(Playlist *raiz)
{
    if(raiz != NULL)
    {
        mostrarPlaylists(raiz->esq); // exibe playlists da subarvore esquerda
        printf("\nPlaylist: %s\n", raiz->info.nome); // exibe o nome da playlist
        mostrarPlaylists(raiz->dir); // exibe playlists da subarvore direita
    }
}

// Libera o que foi alocado na arvore de playlists
void liberarPlaylists(Playlist **raiz)
{
    if(*raiz != NULL)
    {
        liberarPlaylists(&(*raiz)->esq); // libera a subarvore esquerda
        liberarPlaylists(&(*raiz)->dir); // libera a subarvore direita
        liberarMusicaP(&(*raiz)->info.musicas); // libera as músicas associadas à playlist
        free(*raiz); // libera a memória da playlist
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
            encontrado = raiz; // encontrou a playlist
        }
        else if (strcmp(raiz->info.nome, nome) > 0)
        {
            encontrado = buscarPlaylist(raiz->esq, nome); // busca à esquerda
        } 
        else
        {
            encontrado = buscarPlaylist(raiz->dir, nome); // busca à direita
        }     
    }
    return(encontrado); // retorna a playlist encontrada ou NULL
}

// Verifica se o nó é uma folha (não tem filhos)
int ehfolhaPlaylist(Playlist *raiz)
{
    int folha = 0;
    if(raiz->esq == NULL && raiz->dir == NULL) // verifica se não tem filhos
    {
        folha = 1;
    }
    return(folha); // retorna 1 se for folha, 0 caso contrário
}

// Verifica se o nó tem apenas um filho
Playlist *soUmFilhoPlaylist(Playlist *raiz)
{
    Playlist *filho;
    filho = NULL;
    if(raiz->dir == NULL) // se não tem filho à direita
    {
        filho = raiz->esq; // o filho é o da esquerda
    }
    else if(raiz->esq == NULL) // se não tem filho à esquerda
    {
        filho = raiz->dir; // o filho é o da direita        
    }
    return(filho); // retorna o único filho ou NULL se não tiver filhos
}

// Encontra o menor nó da subarvore direita
Playlist **menorDaDirPlaylist(Playlist **raiz)
{
    Playlist **menor;
    menor = raiz;
    if((*raiz)->esq != NULL)  
    {
        while((*menor)->esq != NULL)
        {
            menor = &((*menor)->esq); // vai para o nó mais à esquerda
        }
    }
    return(menor); // retorna o menor nó da subarvore direita
}

// Remove uma playlist pelo nome
int removerPlaylist(Playlist **raiz, char *titulo)
{
    int removido = 1;
    if((*raiz) != NULL)
    {
        if(strcmp((*raiz)->info.nome, titulo) == 0) // se o nome da playlist for igual ao título procurado
        {
            Playlist *aux, *filho;
            if(ehfolhaPlaylist(*raiz)) // se for uma folha
            {
                aux = *raiz;
                *raiz = NULL; // remove a playlist
                free(aux);
            }
            else
            {
                if((filho = soUmFilhoPlaylist(*raiz)) != NULL) // se tiver um filho
                {
                    aux = *raiz;
                    *raiz = filho; // promove o filho para o lugar da raiz
                    free(aux);
                }
                else
                { 
                    // caso tenha 2 filhos
                    Playlist **menor;
                    menor = menorDaDirPlaylist(&((*raiz)->dir)); // encontra o menor nó da subarvore direita
                    (*raiz)->info = (**menor).info; // substitui os dados pela informação do menor nó
                    aux = *menor;
                    *menor = (**menor).dir; // ajusta a subarvore do menor nó
                    free(aux);
                }
            }
        }
        else 
        {
            if (strcmp((*raiz)->info.nome, titulo) > 0)
            {
                removido = removerPlaylist(&((*raiz)->esq), titulo); // busca à esquerda
            } 
            else
            {
                removido = removerPlaylist(&((*raiz)->dir), titulo); // busca à direita
            }  
        }
    }
    else
    {
        removido = 0; // não encontrou a playlist
    } 
    return (removido); // retorna 1 se removido, 0 se não encontrado
}

// Menu interativo para manipular playlists
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
                dados = preencherDadosPlaylist(); // preenche os dados da nova playlist
                no = alocarPlaylist(dados); // aloca a nova playlist
                if(inserirPlaylist(raizPlaylist, no)) // insere a playlist na árvore
                {
                    printf("Playlist %s cadastrada com sucesso.\n", no->info.nome);
                }
                else
                {
                    printf("Playlist %s ja existe na arvore. Tente outro valor.\n", no->info.nome);
                    free(no);
                }
                break;
            case 2:
                mostrarPlaylists(*raizPlaylist); // exibe todas as playlists
                printf("\n");
                break;
            case 3:
                printf("Digite o nome da playlist: "); scanf("%s", nome);
                no = buscarPlaylist(*raizPlaylist, nome); // busca a playlist pelo nome
                if(no)
                {
                    mostrarMusicasP(no->info.musicas); // mostra as músicas da playlist
                }
                printf("\n");
                break;
            case 4:
                printf("Digite o nome da playlist: "); scanf("%s", nome);
                no = buscarPlaylist(*raizPlaylist, nome); // busca a playlist pelo nome
                if(no)
                {
                    printf("Digite o nome do artista: "); scanf("%s", nome);
                    artista = buscarArtista(raizArtista, nome); // busca o artista pelo nome
                    if(artista)
                    {
                        printf("Digite o titulo do album: "); scanf("%s", nome);
                        album = buscarAlbum(artista->albuns, nome); // busca o album pelo nome
                        if(album)
                        {
                            printf("Digite o titulo da musica: "); scanf("%s", nome);
                            musica = buscarMusica(album->musicas, nome); // busca a musica pelo nome
                            if(musica)
                            {
                                dadosMusicaP = preencherDadosMusicaP(artista->info.nome, album->info.titulo, musica->info.titulo); // preenche os dados da musica
                                noMusica = alocarMusicaP(dadosMusicaP); // aloca a musica na playlist
                                int inseriu = inserirMusicaP(&(no->info.musicas), noMusica); // insere a musica na playlist
                                if(inseriu)
                                {
                                    printf("Musica %s cadastrada com sucesso.\n", noMusica->info.tituloMusica);
                                }
                                else
                                {
                                    printf("Musica %s ja existe na arvore. Tente outro valor.\n", noMusica->info.tituloMusica);
                                    free(noMusica);
                                }
                            }
                            else
                            {
                                printf("Musica nao encontrada!\n");
                            }
                        }
                        else
                        {
                            printf("Album nao encontrado!\n");
                        }
                    }
                    else
                    {
                        printf("Artista nao encontrado!\n");
                    }
                }
                else
                {
                    printf("Playlist nao encontrada!\n");
                }
                break;
            case 5:
                printf("Digite o nome da playlist: "); scanf("%s", nome);
                no = buscarPlaylist(*raizPlaylist, nome); // busca a playlist pelo nome
                if(no)
                {
                    printf("Digite o nome da musica na playlist: "); scanf("%s", nome);
                    noMusica = buscarMusicaPlaylist(no->info.musicas, nome); // busca a musica na playlist
                    if(noMusica)
                    {
                        if(removerNo(&no->info.musicas, nome)) // remove a musica da playlist
                        {
                            printf("Musica removida com sucesso!\n");
                        }
                        else
                        {
                            printf("Falha ao remover!\n");
                        }
                    }
                    else
                    {
                        printf("Musica nao encontrada!\n");
                    }
                }
                else
                {
                    printf("Playlist nao encontrada!\n");
                }
                break;
            case 6:
                printf("Digite o nome da playlist que deseja remover: "); scanf("%s", nome);
                if (removerPlaylist(raizPlaylist, nome)) // remove a playlist pelo nome
                {
                    printf("Playlist %s removida com sucesso.\n", nome);
                } 
                else 
                {
                    printf("Falha ao remover.\n");
                }
                break;
            default:
                printf("Opcao invalida!\n"); // caso a opção seja inválida
        }
    } while (opcao != 0);
}
