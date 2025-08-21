#include "interface.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define STRING 50


// Preenche as informações do artista: nome, estilo, tipo, numAlbuns 
infoArtista preencherDadosArtistaAutomatico(char linha[150]) {
    infoArtista dados;

    // Separar a linha por vírgulas
    char *token = strtok(linha, ",");
    strncpy(dados.nome, token, STRING);

    token = strtok(NULL, ",");
    strncpy(dados.tipo, token, STRING);

    token = strtok(NULL, ",");
    strncpy(dados.estilo, token, STRING);

    dados.numAlbuns = 0; // ainda sem albuns cadastrados
    return dados;
}

// Preenche as infromações do album: titulo, ano, qtdMusicas
infoAlbum preencherDadosAlbumAutomatico(char linha[150]) {
    infoAlbum dados;

    // Separar a linha por vírgulas
    char *token = strtok(linha, ",");
    strncpy(dados.titulo, token, STRING);

    token = strtok(NULL, ",");
    dados.anoLancamento = atoi(token);

    token = strtok(NULL, ",");
    dados.qtdMusicas = 0;

    return dados;
}

// Preenche as informações da musica: titulo, duracao
infoMusica preencherDadosMusicaAutomatico(char linha[150]) {
    infoMusica dados;

    // Separar a linha por vírgulas
    char *token = strtok(linha, ",");
    strncpy(dados.titulo, token, STRING);

    token = strtok(NULL, ",");
    dados.qtdMinutos = atoi(token);

    return dados;
}

// Função para ler o arquivo .txt e preencher a árvore binária
double lerArquivoDeInsercao(const char *nomeArquivoArtista, const char *nomeArquivoAlbum, const char *nomeArquivoMusicas, Artista **raiz) {
    // Ler arquivo
    FILE *arquivo = fopen(nomeArquivoArtista, "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        exit(1);
    }

    infoArtista dados;
    Artista *artista;
    double total_de_tempo = 0.0;
    int contArtirstas = 0;

    char linha[150];
    
    while (fgets(linha, sizeof(linha), arquivo)) {

        // Remover o caractere de nova linha
        linha[strcspn(linha, "\n")] = '\0';
        
        // Contar tempo inicial
        clock_t inicio = clock();

        dados = preencherDadosArtistaAutomatico(linha);

        artista = alocaArtista(dados);


        int flag =  inserirArtista(raiz, artista);

        // if (flag){
        //     printf("Artista %s cadastrado com sucesso.\n", artista->info.nome);
        // }else{
        //     printf("Não cadastrado.\n");
        //     free(artista);
        // }

        clock_t fim = clock();

        total_de_tempo += fim - inicio;

        // Inserir o albuns

        // Ler arquivo de album
        FILE *arquivo_album = fopen(nomeArquivoAlbum, "r");
        if (arquivo_album == NULL) {
            printf("Erro ao abrir o arquivo.\n");
            exit(1);
        }


        while (fgets(linha, sizeof(linha), arquivo_album)) {
            // Remover o caractere de nova linha
            linha[strcspn(linha, "\n")] = '\0';
            
            infoAlbum dados_album = preencherDadosAlbumAutomatico(linha);
            
            Album *album = alocarAlbum(dados_album);
            if(inserirAlbum(&artista->albuns, album)){
                // printf("Album %s cadastrado com sucesso.\n", album->info.titulo);
                artista->info.numAlbuns++;    
            }

            // Inserir o musicas
            // Ler arquivo de musicas
            FILE *arquivo_musicas = fopen(nomeArquivoMusicas, "r");
            if (arquivo_musicas == NULL) {
                printf("Erro ao abrir o arquivo.\n");
                exit(1);
            }
            while (fgets(linha, sizeof(linha), arquivo_musicas)) {
                // Remover o caractere de nova linha
                linha[strcspn(linha, "\n")] = '\0';
                
                infoMusica dados_musica = preencherDadosMusicaAutomatico(linha);
                
                Musica *musica = alocarMusica(dados_musica);
                if (inserirMusica(&album->musicas, musica)){
                    // printf("Musica %s cadastrada com sucesso.\n", musica->info.titulo);
                    album->info.qtdMusicas++;
                }
            }
            fclose(arquivo_musicas);
        }
        fclose(arquivo_album);
        
        contArtirstas++;
        // printf("Artista n: %d\n", contArtirstas);


    }
    // Contar tempo final
    double tempo = (double)(total_de_tempo) / CLOCKS_PER_SEC;
    printf("Tempo de insercao: %.10f segundos\n", tempo);

    fclose(arquivo);
    return tempo;
}

// CRIAR FUNÇÃO QUE RECEBE ARQUIVO, RAIZ E FAZ A BUSCA 
double lerArquivoDeBusca(const char *nomeArquivo, Artista **raiz) {
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL) {
        exit(1); // ou retorne -1 se preferir sinalizar erro
    }

    Artista *artista;
    Album *album;
    char linha[150];
    double somatorio = 0;
    double tempo = 0.0;
    clock_t inicio;
    clock_t fim;


    while (fgets(linha, sizeof(linha), arquivo)) {
        // Remove o \n do final, se houver
        linha[strcspn(linha, "\n")] = 0;

        // infoArtista dados;
        char nome[STRING];
        char tipo[STRING];
        char estilo[STRING];
        char titulo[50];

        // Separar a linha por vírgulas
        char *token = strtok(linha, ",");
        strncpy(nome, token, STRING);
        //printf("Artista: %s\n", dados.nome);

        token = strtok(NULL, ",");
        strncpy(tipo, token, STRING);
    
        token = strtok(NULL, ",");
        strncpy(estilo, token, STRING);

        token = strtok(NULL, ",");
        strncpy(titulo, token, 50);

        inicio = clock(); // tempo inicial

        // if (raiz == NULL){
        //     printf("raiz == NULL\n");
        // }else{
        //     printf("raiz != NULL\n");
        // }

        artista = buscarArtista(*raiz, nome);

        // if (artista != NULL)
        // {
        //     printf("artista != NULL\n");
        // }else{
        //     printf("artista == NULL\n");
        // }

        // printf("Nome: %s\n", nome);
        // printf("Artista: %s\n", artista->info.nome);

        album = buscarAlbum(artista->albuns, titulo);

        fim = clock(); // tempo final
        
        tempo = (double)(fim - inicio) / CLOCKS_PER_SEC;
        somatorio += tempo;
        
    }
   
    printf("Tempo de busca: %.10f segundos\n", somatorio/30);
    fclose(arquivo);

    // Retorna tempo gasto em segundos
    return tempo;
}

int main() {

    Artista *raiz = NULL;
    const char *nomeArquivoArtista = "/home/sthefany/Documentos/4Periodo/ED_II/Trabalho_version_Finish/teste/7500_artistas.txt"; // Nome do arquivo
    const char *nomeArquivoAlbum = "/home/sthefany/Documentos/4Periodo/ED_II/Trabalho_version_Finish/teste/100_albuns.txt"; // Nome do arquivo
    const char *nomeArquivoMusica = "/home/sthefany/Documentos/4Periodo/ED_II/Trabalho_version_Finish/teste/3_musicas.txt"; // Nome do arquivo
    const char *nomeArquivoBusca = "/home/sthefany/Documentos/4Periodo/ED_II/Trabalho_version_Finish/teste/teste_de_busca_7500artistas.csv"; // Nome do arquivo
    double tempoInsercao = 0.0;
    double tempoBusca = 0.0;

    tempoInsercao = lerArquivoDeInsercao(nomeArquivoArtista, nomeArquivoAlbum, nomeArquivoMusica, &raiz);
    // Realizar a busca no arquivo
    tempoBusca = lerArquivoDeBusca(nomeArquivoBusca, &raiz);  

    // Garantir que a árvore esteja vazia
    liberarArtistas(&raiz);

    return 0;
}
