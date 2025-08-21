#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define STRING           50   // tamanho máximo das strings
#define MIN_TITLE_LEN     5   // tamanho mínimo do título
#define MAX_TITLE_LEN    30   // tamanho máximo do título gerado
#define MIN_MINUTOS       1   // duração mínima (em minutos)
#define MAX_MINUTOS      10   // duração máxima (em minutos)

typedef struct infoMusica {
    char titulo[STRING];   // nome da música
    int  qtdMinutos;       // duração em minutos
} infoMusica;

// Gera uma string aleatória de tamanho entre MIN_TITLE_LEN e MAX_TITLE_LEN
void gerarStringAleatoria(char *str) {
    const char alfabeto[] =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "0123456789";
    int tamAlfabeto = strlen(alfabeto);
    int lenTitulo  = rand() % (MAX_TITLE_LEN - MIN_TITLE_LEN + 1)
                      + MIN_TITLE_LEN;

    for (int i = 0; i < lenTitulo; i++) {
        str[i] = alfabeto[rand() % tamAlfabeto];
    }
    str[lenTitulo] = '\0';
}

// Preenche o struct infoMusica com dados aleatórios
void gerarMusicaAleatoria(infoMusica *musica) {
    gerarStringAleatoria(musica->titulo);
    musica->qtdMinutos = rand() % (MAX_MINUTOS - MIN_MINUTOS + 1)
                         + MIN_MINUTOS;
}

// Salva N músicas em "<N>_musicas.txt" no formato CSV: título,duração
void salvarMusicasEmArquivo(int N) {
    char nomeArquivo[64];
    snprintf(nomeArquivo, sizeof(nomeArquivo), "%d_musicas.txt", N);

    FILE *fp = fopen(nomeArquivo, "w");
    if (!fp) {
        perror("Erro ao criar o arquivo");
        exit(EXIT_FAILURE);
    }

    infoMusica mus;
    for (int i = 0; i < N; i++) {
        gerarMusicaAleatoria(&mus);
        fprintf(fp, "%s,%d\n",
                mus.titulo,
                mus.qtdMinutos);
    }

    fclose(fp);
    printf("Arquivo \"%s\" gerado com sucesso!\n", nomeArquivo);
}

int main() {
    srand((unsigned)time(NULL));  // semente do RNG

    int N;
    printf("Quantas músicas deseja gerar? ");
    if (scanf("%d", &N) != 1 || N <= 0) {
        fprintf(stderr, "Entrada inválida.\n");
        return 1;
    }

    salvarMusicasEmArquivo(N);
    return 0;
}
