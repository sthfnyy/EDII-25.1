#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "interfaceAvl.h"


// programa principal que gerencia tudo
int main() 
{
    Artista *raiz;
    raiz = NULL; // comeca com a arvore de artistas vazia
    int opcao;
    int ano, cont;
    char nome[50];
    char estilo[50];
    infoArtista dados;
    Artista *artista;
    Album *albumEncontrado;
    Playlist *raizPlaylist;
    raizPlaylist = NULL;



    do 
    {
        // menu principal do sistema
        printf("\nMenu:\n");
        printf("0 - Sair\n");
        printf("1 - Cadastrar artista\n");
        printf("2 - Mostrar artistas\n");
        printf("3 - Mostrar artistas de um estilo\n");
        printf("4 - Mostrar artistas de um tipo\n");
        printf("5 - Mostrar artistas de um tipo e estilo\n");
        printf("6 - Acessar artista\n"); // acessar um artista p/ ver e mexer nos albuns dele
        printf("7 - Mostrar todos os albuns de um ano especifico de todos os artistas cadastrados\n");
        printf("8 - Mostrar os detalhes de uma musica especifica\n");
        printf("9 - Playlists\n"); //acessar um menu para as playlists
        printf("Escolha uma opcao: "); scanf("%d", &opcao);
        switch (opcao) 
        {
            case 0:
                printf("Saindo...\n");
                break;
            case 1:
                dados = preencherDadosArtista();
                artista = alocaArtista(dados);
                if (inserirArtista(&raiz, artista)) 
                {
                    printf("Artista %s inserido com sucesso.\n", artista->info.nome);
                } 
                else 
                {
                    printf("Artista com valor %s ja existe na arvore. Tente outro valor.\n", artista->info.nome);
                    free(artista);
                }
                break;
            case 2:
                // mostrar todos os artistas da arvore
                mostrarArtistas(raiz);
                printf("\n");
                break;
            case 3:
                printf("Digite o estilo dos artistas: "); scanf("%s", estilo);
                mostrarArtistasEstilo(raiz, estilo);
                printf("\n");
                break;
            case 4:   
                printf("Digite o tipo dos artistas: "); scanf("%s", nome);
                mostrarArtistasTipo(raiz, nome);
                printf("\n");
                break;
            case 5:
                printf("Digite o tipo dos artistas: "); scanf("%s", nome);
                printf("Digite o estilo dos artistas: "); scanf("%s", estilo);
                mostrarArtistasTipoEstilo(raiz, nome, estilo);
                printf("\n");
                break;
            case 6:
                // entra no menu do artista para menu interativos dos albuns dele
                printf("Digite o nome do Artista: "); scanf("%s", nome);
                artista = buscarArtista(raiz, nome);
                if(artista != NULL)
                {
                    acessarArtista(artista, raizPlaylist);
                }
                else
                {
                    printf("\nArtista %s nao encontrado!\nCadastre o artista primeiro.\n", nome);
                }
                break;
            case 7:
                printf("\nDigite o ano do Album: "); scanf("%d", &ano);
                mostrarAlbunsPorAno(raiz, ano); 
                printf("\n");
                break;
            case 8:
                printf("\nDigite o nome da musica: "); scanf("%s", nome);
                buscarMusicaEmArtista(raiz, nome);
                break;
            case 9:
                menuPlaylist(&raizPlaylist, raiz);//menu para playlists
            default:
                printf("Opcao invalida!\n");
        }
    } while (opcao != 0);
    liberarArtistas(&raiz); // no final libera memoria
    liberarPlaylists(&raizPlaylist); //libera memoria de playlist
    return 0;
}