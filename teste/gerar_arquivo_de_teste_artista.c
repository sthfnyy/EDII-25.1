#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define STRING 50

typedef struct Info
{
    char nome[STRING];
    char tipo[STRING];
    char estilo[STRING];
} Info;

// Estrutura do No de artistas
typedef struct artista Artista;
struct artista
{
    Info infoArtista;
    Artista *esq, *dir;
};

// Função para gerar um nome aleatório
void gerarStringAleatoria(char *nome) {
    const char alfabeto[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789"; // Alfabeto de caracteres possíveis
    int tamanhoAlfabeto = strlen(alfabeto);

    // Preenche a string com caracteres aleatórios até o limite de 20 caracteres
    for (int i = 0; i < 20; i++) {
        nome[i] = alfabeto[rand() % tamanhoAlfabeto]; // Escolhe um caractere aleatório do alfabeto
    }

    nome[20] = '\0'; // Garante que a string termine corretamente com o terminador nulo
}



// Função para gerar dados aleatórios para o artista
void gerarArtistaAleatorio(Artista *artista)
{
    gerarStringAleatoria(artista->infoArtista.nome);

    const char *tipos[] = {"Solo", "Dueto", "Grupo", "Orquestra"};
    snprintf(artista->infoArtista.tipo, STRING, "%s", tipos[rand() % 4]);

    const char *estilos[] = {"Pop", "Rock", "Jazz", "Eletrônico", "Folk", "Blues"};
    snprintf(artista->infoArtista.estilo, STRING, "%s", estilos[rand() % 6]);
}

// Função para salvar os artistas em um arquivo txt
void salvarArtistasEmArquivo()
{
    FILE *arquivo = fopen("10000_artistas.txt", "w");
    if (arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo.\n");
        exit(1);
    }

    Artista artista;
    srand(time(NULL)); // Inicializa o gerador de números aleatórios

    for (int i = 0; i < 10000; i++)
    {
        gerarArtistaAleatorio(&artista);
        fprintf(arquivo, "%s,%s,%s\n", artista.infoArtista.nome, artista.infoArtista.tipo, artista.infoArtista.estilo);
    }

    fclose(arquivo);
    printf("Arquivo gerado com sucesso!\n");
}

int main()
{
    salvarArtistasEmArquivo();
   return 0;
}
