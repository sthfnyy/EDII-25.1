#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include "interfaceAvl.h"

// Cria uma nova musica na memoria
Musica *alocarMusica(infoMusica dados)
{
    Musica *no;
    no = (Musica*) malloc (sizeof(Musica));  // Aloca memória para um nó de música
    if(no == NULL)
    {
        printf("Erro de acesso a memoria.\n");  // Se não conseguir alocar, exibe erro
        exit(1); 
    }
    no->esq = NULL; // esquerda comeca vazia
    no->dir = NULL; // direita tambem
    no->info = dados; // Atribui os dados passados para o nó
    return no;  // Retorna o nó alocado
}

// Preenche os dados da musica, tipo o titulo e duracao
infoMusica preencherDadosMusica()
{
    infoMusica dados;
    printf("Digite o titulo da musica: "); scanf("%s", dados.titulo); // Lê o título da música
    printf("Digite a quantidade de minutos da musica: "); scanf("%d", &dados.qtdMinutos); // Lê a duração
    return dados;  // Retorna os dados preenchidos
}

// Cadastra uma nova musica na arvore
int inserirMusica(Musica **raizMusica, Musica *no)
{
    int inserido = 1;
    if(*raizMusica == NULL)
    {
        *raizMusica = no; // Se a raiz está vazia, coloca o novo nó nela
    }
    else
    {
        // Compara os títulos das músicas para decidir se o nó vai para a esquerda ou direita
        if (strcmp((*raizMusica)->info.titulo, no->info.titulo) > 0)
        {
            inserido = inserirMusica(&(*raizMusica)->esq, no); // Vai para a subárvore esquerda
        }
        else if(strcmp((*raizMusica)->info.titulo, no->info.titulo) < 0)
        {
            inserido = inserirMusica(&(*raizMusica)->dir, no); // Vai para a subárvore direita
        }
        else
        {
            inserido = 0; // A música já existe na árvore
        }
    }
    if((*raizMusica) != NULL)
    {
        if(inserido)
        {
            balanceamentoMusica(raizMusica);  // Balanceia a árvore após a inserção
            atualizarAlturaMusica(raizMusica);  // Atualiza a altura da árvore
        }
    }
    return(inserido);  // Retorna se a música foi inserida com sucesso
}

void balanceamentoMusica(Musica **raiz)
{
    int fb;
    fb = fatorBalanceamentoMusica(*raiz);  // Calcula o fator de balanceamento do nó
    if(fb == 2)
    {
        int fbEsq = fatorBalanceamentoMusica((*raiz)->esq);  // Calcula o fator de balanceamento da subárvore esquerda
        if(fbEsq < 0)
        {
            rotacionarEsqMusica(&(*raiz)->esq);  // Rotaciona a subárvore esquerda para a esquerda se necessário
        }
        rotacionarDirMusica(raiz);  // Rotaciona o nó para a direita
    }
    else if(fb == -2)
    {
        int fbDir = fatorBalanceamentoMusica((*raiz)->dir);  // Calcula o fator de balanceamento da subárvore direita
        if(fbDir > 0)
        {
            rotacionarDirMusica(&(*raiz)->dir);  // Rotaciona a subárvore direita para a direita se necessário
        }
        rotacionarEsqMusica(raiz);  // Rotaciona o nó para a esquerda
    }
}

int fatorBalanceamentoMusica(Musica *raiz)
{
    int fb;
    fb = alturaMusica(raiz->esq) - alturaMusica(raiz->dir);  // Calcula o fator de balanceamento como a diferença de altura
    return fb;  // Retorna o fator de balanceamento
}

int alturaMusica(Musica *raiz)
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
            altura = altEsq + 1;  // Se a subárvore esquerda for mais alta, usa a altura da esquerda
        }
        else
        {
            altura = altDir + 1;  // Caso contrário, usa a altura da direita
        }
    }
    else
    {
        altura = -1;  // Se o nó for NULL, a altura é -1
    }
    return altura;  // Retorna a altura do nó
}

void atualizarAlturaMusica(Musica **raiz)
{
    if((*raiz) != NULL)
    {
        atualizarAlturaMusica(&(*raiz)->esq);  // Atualiza a altura da subárvore esquerda
        atualizarAlturaMusica(&(*raiz)->dir);  // Atualiza a altura da subárvore direita
        (*raiz)->altura = alturaMusica(*raiz);  // Atualiza a altura do nó atual
    }
}

void rotacionarEsqMusica(Musica **raiz)
{
    Musica *aux;
    aux = (*raiz)->dir;  // Armazena o nó à direita
    (*raiz)->dir = aux->esq;  // O nó à direita passa a ser a subárvore esquerda do nó auxiliar
    aux->esq = *raiz;  // O nó auxiliar passa a ser a subárvore esquerda
    *raiz = aux;  // O nó auxiliar se torna a raiz da subárvore
}

void rotacionarDirMusica(Musica **raiz)
{
    Musica *aux;
    aux = (*raiz)->esq;  // Armazena o nó à esquerda
    (*raiz)->esq = aux->dir;  // O nó à esquerda passa a ser a subárvore direita do nó auxiliar
    aux->dir = *raiz;  // O nó auxiliar passa a ser a subárvore direita
    *raiz = aux;  // O nó auxiliar se torna a raiz da subárvore
}

// Mostra todas as musicas em ordem
void mostrarMusicas(Musica *raiz)
{
    if (raiz != NULL)
    {
        mostrarMusicas(raiz->esq);  // Mostra as músicas na subárvore esquerda
        printf("Musica: %s, %d min \n", raiz->info.titulo, raiz->info.qtdMinutos);  // Exibe a música
        mostrarMusicas(raiz->dir);  // Mostra as músicas na subárvore direita
    }
}

// Libera a memoria usada pelas musicas
void liberarMusica(Musica **raiz)
{
    if(*raiz != NULL)
    {
        liberarMusica(&(*raiz)->esq);  // Libera a memória da subárvore esquerda
        liberarMusica(&(*raiz)->dir);  // Libera a memória da subárvore direita
        free(*raiz);  // Libera o nó atual
    }
}

// Busca uma musica por titulo em todos os artistas
void buscarMusicaEmArtista(Artista *raiz, char *tituloMusica)
{
    if (raiz != NULL)
    {
        buscarMusicaEmArtista(raiz->esq, tituloMusica);  // Busca na subárvore esquerda
        buscarMusicaEmAlbum(raiz->albuns, tituloMusica, raiz->info.nome);  // Busca nos álbuns do artista
        buscarMusicaEmArtista(raiz->dir, tituloMusica);  // Busca na subárvore direita
    }
}

// Busca a musica dentro dos albuns de um artista
void buscarMusicaEmAlbum(Album *raiz, char *tituloMusica, char *nomeArtista)
{
    if (raiz != NULL)
    {
        buscarMusicaEmAlbum(raiz->esq, tituloMusica, nomeArtista);  // Busca na subárvore esquerda
        buscarMusicaEmMusicas(raiz->musicas, tituloMusica, nomeArtista, raiz->info.titulo, raiz->info.anoLancamento);  // Busca nas músicas do álbum
        buscarMusicaEmAlbum(raiz->dir, tituloMusica, nomeArtista);  // Busca na subárvore direita
    }
}

// Verifica se a musica ta dentro da lista de musicas de um album
void buscarMusicaEmMusicas(Musica *raiz, char *tituloMusica, char *nomeArtista, char *tituloAlbum, int anoAlbum)
{
    if (raiz != NULL)
    {
        buscarMusicaEmMusicas(raiz->esq, tituloMusica, nomeArtista, tituloAlbum, anoAlbum);  // Busca na subárvore esquerda
        if (strcmp(raiz->info.titulo, tituloMusica) == 0)  // Se encontrar a música, exibe as informações
        {
            printf("Artista: %s, Album: %s, Ano: %d, Musica: %s\n", nomeArtista, tituloAlbum, anoAlbum, raiz->info.titulo);
        }
        buscarMusicaEmMusicas(raiz->dir, tituloMusica, nomeArtista, tituloAlbum, anoAlbum);  // Busca na subárvore direita
   }
}

Musica *buscarMusica(Musica *raiz, char *titulo)
{
    Musica *encontrado;
    encontrado = NULL; 
    if (raiz != NULL)
    {
        if(strcmp(raiz->info.titulo, titulo) == 0)
        {
            encontrado = raiz;  // Se encontrar a música, retorna o nó
        }
        else if(strcmp(raiz->info.titulo, titulo) > 0)
        {
            encontrado = buscarMusica(raiz->esq, titulo);  // Busca na subárvore esquerda
        } 
        else
        {
            encontrado = buscarMusica(raiz->dir, titulo);  // Busca na subárvore direita
        }     
    }
    return(encontrado);  // Retorna o nó encontrado ou NULL
}

// Verifica se o nó é folha (não tem filhos)
int ehfolhaMusica(Musica *raiz)
{
    int folha = 0;
    if(raiz->esq == NULL && raiz->dir == NULL)
    {
        folha = 1;  // Se não tiver filhos, é folha
    }
    return(folha);  // Retorna se é folha
}

// Verifica se o nó tem um único filho
Musica *soUmFilhoMusica(Musica *raiz)
{
    Musica *filho;
    filho = NULL;
    if(raiz->dir == NULL)
    {
        filho = raiz->esq;  // Se não tiver filho direito, retorna o filho esquerdo
    }
    else if(raiz->esq == NULL)
    {
        filho = raiz->dir;  // Se não tiver filho esquerdo, retorna o filho direito
    }        
    return(filho);  // Retorna o filho
}

// Encontra o nó mais à esquerda da subárvore direita
Musica **menorDaDirMusica(Musica **raiz)
{
    Musica **menor;
    menor = raiz;
    if((*raiz)->esq != NULL)  
    {
        while((*menor)->esq != NULL)
        {
            menor = &((*menor)->esq);  // Vai para a subárvore esquerda até encontrar o menor nó
        }
    }
    return(menor);  // Retorna o menor nó encontrado
}

// Remove uma musica da arvore
int removerMusica(Musica **raiz, char *titulo) 
{
    int removido = 1;
    if((*raiz) != NULL)
    {
        if(strcmp((*raiz)->info.titulo, titulo) == 0)
        {
            Musica *aux, *filho;
            if(ehfolhaMusica(*raiz))  // Se for folha, apenas remove
            {
                aux = *raiz;
                *raiz = NULL;
                free(aux);
            }
            else
            {
                if((filho = soUmFilhoMusica(*raiz)) != NULL)  // Se tiver um filho, remove e substitui pelo filho
                {
                    aux = *raiz;
                    *raiz = filho;
                    free(aux);
                }
                else
                { 
                    // Se tiver dois filhos, encontra o menor da subárvore direita
                    Musica **menor;
                    menor = menorDaDirMusica(&((*raiz)->dir));
                    (*raiz)->info = (**menor).info;
                    aux = *menor;
                    *menor = (**menor).dir;
                    free(aux);
                }
            }
        }
        else 
        {
            if (strcmp((*raiz)->info.titulo, titulo) > 0)
            {
                removido = removerMusica(&((*raiz)->esq), titulo);  // Busca na subárvore esquerda
            } 
            else
            {
                removido = removerMusica(&((*raiz)->dir), titulo);  // Busca na subárvore direita
            }  
        }
    }
    else
    {
        removido = 0;  // Se o nó for NULL, não há nada a remover
    }
    if((*raiz) != NULL)
    {
        if(removido)
        {
            balanceamentoMusica(raiz);  // Balanceia a árvore após a remoção
            atualizarAlturaMusica(raiz);  // Atualiza a altura após a remoção
        }
    }
    return (removido);  // Retorna se a remoção foi bem-sucedida
}
