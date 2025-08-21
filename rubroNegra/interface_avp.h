#ifndef INTERFACE_AVP_H
#define INTERFACE_AVP_H

#define TAM_STRING 50
#define RED 1
#define BLACK 0 


typedef struct cidade Cidade;
typedef struct cep CEP;

// Estrutura infoEstado
typedef struct infoEstado 
{
    char nome_estado[TAM_STRING];
    char nome_capital[TAM_STRING];
    int num_cidade;
    int populacao;
    Cidade *arv_cidade;  // ponteiro para árvore de cidades
} infoEstado;

// Estrutura Estado (lista duplamente encadeada)
typedef struct estado 
{
    infoEstado infoEstado;  // struct embutida (não ponteiro)
    struct estado *prox;
    struct estado *ant;
} Estado;

typedef struct infoCidade 
{
    char nome_cidade[TAM_STRING];
    int populacao_c;
    CEP *avr_CEP;
} infoCidade;

typedef struct cidade Cidade;
struct cidade 
{
    infoCidade infoCidade;
    int cor;
    Cidade *esq;
    Cidade *dir;
};

// Estrutura infoCEP
typedef struct infoCEP 
{
    int cep;
} infoCEP;

// Declaração adiantada para CEP
typedef struct cep CEP;

struct cep 
{
    infoCEP infoCEP;
    int cor;
    CEP *esq;
    CEP *dir;
};

// Estrutura infoPessoa
typedef struct infoPessoa 
{
    int cepCidadeNatal;
    int cepCidadeAtual;
    char nome[TAM_STRING];
    char CPF[TAM_STRING];
    char nascimento[TAM_STRING];
} infoPessoa;

// Declaração adiantada para infoPessoa
typedef struct pessoa Pessoa;

struct pessoa 
{
    infoPessoa infoPessoa;
    int cor;
    Pessoa *esq;
    Pessoa *dir;
};

// Protótipos de estado
Estado* alocar_estado();
void preencher_estado(infoEstado *estado, char *nome_estado, char *nome_capital, int n_cidade, int populacao);
int inserir_estado_ordenado(Estado **inicio, Estado *novo);
void imprimir_estados(Estado *inicio);
Estado* buscar_estado(Estado *inicio, const char *nome_busca, int *flag);
Estado* buscar_estado_Por_CEP(Estado *inicio, int intCEP);
void liberar_lista_estados(Estado *inicio);
Estado *retorna_estados_com_maior_populacao(Estado *atual);
Cidade *buscar_Cidade_nos_Estados(Estado *inicio, char *nome_cidade);

// Protótipos das funções para cidade
// interface_avp.h

void preencherCidade(infoCidade *cidade, char *nome, int pop);
int cor(Cidade *noAtual);
void inverterCor(Cidade *noAtual);
Cidade *rotacionaEsquerda(Cidade *noAtual);
Cidade *rotacionaDireita(Cidade *noAtual);
Cidade *alocarCidade(infoCidade dados, int *resp);
int inserirCidade(Cidade **raiz, infoCidade dados);
Cidade *buscarCidade(Cidade *raiz, const char *nome_cidade);
void imprimir(Cidade *raiz);
void liberar_arvore_cidades(Cidade *raiz);
int cadastrarCEPEmCidade(Cidade *raizCidades, const char *nomeCidade, infoCEP dadosCEP);
int atualizarCidade(Cidade *CidadeAtual, infoCidade novosDados);
Cidade *buscarCidadePorCEP(Cidade *raiz, int valorCEP);
Cidade *cidade_mais_populosa_de_um_estado_sem_ser_a_capital(Cidade *raiz, char *nome_da_capital);

// Protótipos para árvore rubro-negra de CEP
void preencherCEP(infoCEP *dados, int numCEP);
int corCEP(CEP *noAtual);
void inverterCorCEP(CEP *noAtual);
CEP *rotacionaEsquerdaCEP(CEP *noAtual);
CEP *rotacionaDireitaCEP(CEP *noAtual);
CEP *alocarCEP(infoCEP dados, int *resp);
int inserirCEP(CEP **raiz, infoCEP dados);
CEP *inserirNOCEP(CEP **raiz, infoCEP dados, int *resp);
CEP *buscarCEP(CEP *raiz, int valorCEP);
void imprimirCEP(CEP *raiz);
void liberarArvoreCEP(CEP *raiz);
//função de remoção
CEP *removeNoCEP(CEP *noAtual, int valorCEP);
int removerCEP(CEP **raiz, int valorCEP);
CEP *move2EsqRED(CEP *noAtual);
CEP *move2DirRED(CEP *noAtual);
CEP *balancear(CEP *noAtual);
CEP *removerMenor(CEP *noAtual);
CEP *procurarMenor(CEP *atual);

// Funções para pessoa
void preencherPessoa(infoPessoa *info_pessoa, int cepCidadeNatal,int cepCidadeAtual, char *nome, char *CPF, char *nascimento);
int corPessoa(Pessoa *noAtual);
void inverterCorPessoa(Pessoa *noAtual);
Pessoa *rotacionaEsquerdaPessoa(Pessoa *noAtual);
Pessoa *rotacionaDireitaPessoa(Pessoa *noAtual);
int inserirPessoa(Pessoa **raiz, infoPessoa dados);
Pessoa *inserirNOPessoa(Pessoa **raiz, infoPessoa dados, int *resp);
Pessoa *alocarCidadePessoa(infoPessoa dados, int *resp);
void imprimirPessoa(Pessoa *raiz);
Pessoa *buscarPessoaNome(Pessoa *raiz, char *nome);
Pessoa *consultaPessoaCEPNacimento(Pessoa *raiz, int CEP, int *encontrado);
Pessoa *consultaPessoaCEPAtual(Pessoa *raiz, int CEP, int *encontrado);
int consultaPessoaCPF(Pessoa *raiz, const char *CPF);
Pessoa *removeNoPessoa(Pessoa *noAtual, char *nome);
int removerPessoa(Pessoa **raiz, char *nome);
Pessoa *move2EsqREDPessoa(Pessoa *noAtual) ;
Pessoa *move2DirREDPessoa(Pessoa *noAtual);
Pessoa *balancearPessoa(Pessoa *noAtual);
Pessoa *removerMenorPessoa(Pessoa *noAtual);
Pessoa *procurarMenorPessoa(Pessoa *atual);
int Quantas_pessoas_não_moram_na_cidade_natal(Pessoa *raiz, Estado *estados);
int numero_de_pessoas_nascidas_em_uma_determinada_cidade_nao_mora_na_cidade_natal(Pessoa *raiz, Cidade *cidade);
int numero_de_pessoas_que_moram_em_uma_determinada_cidade_nao_nasceram_nela(Pessoa *raiz, Cidade *cidade);



#endif