#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_TITLE    50    // tamanho máximo para o título
#define MIN_YEAR     1960  // ano mínimo de lançamento
#define MAX_YEAR     2025  // ano máximo de lançamento
#define MIN_TRACKS   5     // número mínimo de faixas
#define MAX_TRACKS   20    // número máximo de faixas

typedef struct infoAlbum {
    char titulo[MAX_TITLE];
    int anoLancamento;
    int qtdMusicas;
} infoAlbum;

// Gera uma string aleatória de tamanho fixo (20 caracteres)
void gerarStringAleatoria(char *str) {
    const char alfabeto[] =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "0123456789"
    ;
    int tamAlfabeto = strlen(alfabeto);
    for (int i = 0; i < 20; i++) {
        str[i] = alfabeto[rand() % tamAlfabeto];
    }
    str[20] = '\0';
}

// Preenche um álbum com dados aleatórios
void gerarAlbumAleatorio(infoAlbum *album) {
    gerarStringAleatoria(album->titulo);
    album->anoLancamento = rand() % (MAX_YEAR - MIN_YEAR + 1) + MIN_YEAR;
    album->qtdMusicas    = rand() % (MAX_TRACKS - MIN_TRACKS + 1) + MIN_TRACKS;
}

// Salva N álbuns em um arquivo TXT cujo nome é "<N>_albuns.txt"
void salvarAlbunsEmArquivo(int N) {
    char nomeArquivo[64];
    snprintf(nomeArquivo, sizeof(nomeArquivo), "%d_albuns.txt", N);

    FILE *fp = fopen(nomeArquivo, "w");
    if (!fp) {
        perror("Erro ao criar o arquivo");
        exit(EXIT_FAILURE);
    }

    infoAlbum alb;
    for (int i = 0; i < N; i++) {
        gerarAlbumAleatorio(&alb);
        // Formato: título,ano,qtd_músicas
        fprintf(fp, "%s,%d,%d\n",
                alb.titulo,
                alb.anoLancamento,
                alb.qtdMusicas
        );
    }

    fclose(fp);
    printf("Arquivo \"%s\" gerado com sucesso!\n", nomeArquivo);
}

int main() {
    srand((unsigned)time(NULL));

    int N;
    printf("Quantos álbuns deseja gerar? ");
    if (scanf("%d", &N) != 1 || N <= 0) {
        printf("Número inválido.\n");
        return 1;
    }

    salvarAlbunsEmArquivo(N);
    return 0;
}
