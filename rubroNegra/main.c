#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "interface_avp.h"

void menu_cidade(Cidade *auxCidade, Pessoa *pessoas, Estado *estados){
    int opcao, resp, n_CEP, encontrado;
    CEP *auxCEP;
    infoCEP auxinfoCEP;
    Pessoa *auxPessoa, *auxPessoa_2;
    Estado *auxEstado;

    do{

        printf("Cidade: %s | Populacao: %d\n",
            auxCidade->infoCidade.nome_cidade,
            auxCidade->infoCidade.populacao_c);

        printf("\nMenu:\n");
        printf("0 - Sair\n");
        printf("1 - Cadastrar CEP\n");
        printf("2 - Mostrar CEPs\n");
        printf("3 - Buscar CEP\n");
        printf("4 - Remover CEP\n");
        printf("Escolha uma opcao: "); scanf("%d", &opcao);

        switch (opcao) 
        {
            case 0:
                printf("Saindo...\n");
                break;
            case 1:
                // Informar dados
                printf("Informe o numero do CEP: \n"); scanf("%d", &n_CEP);  

                //auxCEP = buscarCEP(auxCidade->infoCidade.avr_CEP, n_CEP);
                // Busca em todos os estados se o CEP já existe
                auxEstado = buscar_estado_Por_CEP(estados, n_CEP);

                if (auxEstado == NULL){
                    
                    preencherCEP(&auxinfoCEP, n_CEP);
                    resp = inserirCEP(&auxCidade->infoCidade.avr_CEP, auxinfoCEP);

                    if (resp == 1){
                        printf("CEP Cadastrado !!!\n");    
                    }else{
                        printf("Não foi possivel cadastrar o CEP %d !!!\n", n_CEP);
                    }
                    
                }else{
                    printf("Não foi possivel cadastrar o CEP %d pos ele já existe!!!\n", n_CEP);
                }                
                break;
            case 2:
                printf("\nÁrvore de CEPs (in-order):\n");
                imprimirCEP(auxCidade->infoCidade.avr_CEP);
                break;
            case 3:
                printf("Informe o numero do CEP: \n"); scanf("%d", &n_CEP);
                auxCEP = buscarCEP(auxCidade->infoCidade.avr_CEP, n_CEP);
                if (auxCEP){
                    printf("CEP: %08d, cor: %s\n",
                        auxCEP->infoCEP.cep,
                        (auxCEP->cor == RED) ? "RED" : "BLACK");
                }else{
                    printf("Estado nao encontrado!!!\n");
                }
                break;
            case 4:
                printf("Informe o numero do CEP: \n"); scanf("%d", &n_CEP);
                
                auxPessoa = consultaPessoaCEPNacimento(pessoas, n_CEP, &encontrado);
                auxPessoa_2 = consultaPessoaCEPAtual(pessoas, n_CEP, &encontrado);

                if ((auxPessoa == NULL) && (auxPessoa_2 == NULL)){

                    resp = removerCEP(&auxCidade->infoCidade.avr_CEP, n_CEP);
                    // TODO: necessario verificar se não tem nem uma pessoa com esse CEP
                    
                    if (resp==1){
                        printf("Remocao feita com sucesso!!!\n");
                    }else{
                        printf("Remocao deu erro!!!\n");
                    }
                }else{
                    printf("Remocao CEP utilizado por alguma pessoa!!!\n");
                }
                break;
        }
    }while (opcao != 0);
}

void menu_estado(Estado *estado, Pessoa *pessoas, Estado *estados){
    int opcao, resp;
    char nome_cidade[TAM_STRING];
    Cidade *auxCidade;
    infoCidade *auxinfoCidade;

    do{

        auxCidade = NULL;

        printf("Estado: %s | Capital: %s | Cidades: %d | População: %d\n",
            estado->infoEstado.nome_estado,
            estado->infoEstado.nome_capital,
            estado->infoEstado.num_cidade,
            estado->infoEstado.populacao);

        printf("\nMenu:\n");
        printf("0 - Sair\n");
        printf("1 - Cadastrar CIDADE\n");
        printf("2 - Mostrar as CIDADES\n");
        printf("3 - Buscar CIDADE\n");
        printf("Escolha uma opcao: "); scanf("%d", &opcao);

        switch (opcao) 
        {
            case 0:
                printf("Saindo...\n");
                break;
            case 1:
                // Informar dados
                printf("Informe o nome da cidade: \n"); scanf("%s", nome_cidade);  

                auxCidade = buscarCidade(estado->infoEstado.arv_cidade, nome_cidade);

                if (auxCidade == NULL){
                    preencherCidade(auxinfoCidade, nome_cidade, 0);
                    resp = inserirCidade(&estado->infoEstado.arv_cidade, *auxinfoCidade);

                    if (resp == 1){
                        printf("Cidade Cadastrado !!!\n");
                        estado->infoEstado.num_cidade += 1;    
                    }else{
                        printf("Não foi possivel cadastrar o Cidade %s !!!\n", nome_cidade);
                    }
                    
                }else{
                    printf("Não foi possivel cadastrar o Cidade %s pos ele já existe!!!\n", nome_cidade);
                }                
                break;
            case 2:
                imprimir(estado->infoEstado.arv_cidade);
                break;
            case 3:
                printf("Informe o nome da cidade: \n"); scanf("%s", nome_cidade);
                auxCidade = buscarCidade(estado->infoEstado.arv_cidade, nome_cidade);
                if (auxCidade != NULL){
                    menu_cidade(auxCidade, pessoas, estados);
                }else{
                    printf("Cidade nao encontrado!!!\n");
                }
                break;

        }
    }while (opcao != 0);
}

int main() {

    Estado *estados = NULL;
    Estado *novo, *auxEstado, *aux_2Estado;

    infoCidade auxInfoCidade;
    Cidade *auxCidade;
    char nome_cidade[TAM_STRING];

    int opcao;
    char nome_estado[TAM_STRING];
    char nome_capital[TAM_STRING]; 
    int n_cidade, populacao, encontrou, resp, inseriu_cidade;

    Pessoa *pessoas = NULL;
    Pessoa *auxPessoa;
    infoPessoa auxInfopessoa;
    char nome_pessoa[TAM_STRING];
    int CEPNascimento, CEPAtul; 
    char CPFPessoa[TAM_STRING];
    char DataNascimento[TAM_STRING];

    do 
    {
        encontrou = 0;
        // menu principal do sistema
        printf("\nMenu:\n");
        printf("0 - Sair\n");
        printf("1 - Cadastrar estado\n");
        printf("2 - Mostrar estados\n");
        printf("3 - Buscar estado\n");
        printf("4 - Cadastrar pessoa\n");
        printf("5 - Mostrar pessoa\n");
        printf("6 - Buscar pessoa\n");
        printf("7 - Remover pessoa\n");
        printf("8 - Qual o estado mais populoso\n");
        printf("9 - Qual a população da Capital de um determinado estado\n");
        printf("10 - Cidade mais populosa de um estado sem ser a Capital\n");
        printf("11 - Quantas pessoas não moram na cidade natal\n");
        printf("12 - Qual cidade natal de uma pessoa dado o CEP da cidade\n");
        printf("13 - Quantas pessoas nascidas em uma determinada cidade não mora na cidade natal\n");
        printf("14 - Quantas pessoas que moram em uma determinada cidade não nasceram na cidade?\n");


        printf("Escolha uma opcao: "); scanf("%d", &opcao);


        switch (opcao) 
        {
            case 0:
                printf("Saindo...\n");
                break;

            case 1:
                // Informar dados

                printf("Informe o nome do estado: \n"); scanf("%s", nome_estado);
                printf("Informe o nome da capital: \n"); scanf("%s", nome_capital);  

                n_cidade = 1;
                populacao = 0;

                novo = buscar_estado(estados, nome_estado, &encontrou);

                if (encontrou == 0)
                {
                    novo = alocar_estado();
                    preencher_estado(&novo->infoEstado, nome_estado, nome_capital,n_cidade,populacao);
                    
                    preencherCidade(&auxInfoCidade, nome_capital, 0);
                    
                    inseriu_cidade = inserirCidade(&novo->infoEstado.arv_cidade, auxInfoCidade);

                    resp = inserir_estado_ordenado(&estados, novo);

                    if (resp == 1){
                        printf("Estado Cadastrado !!!\n");    
                    }else{
                        printf("Não foi possivel cadastrar o Estado %s !!!", nome_estado);
                    }
                    
                }else{
                    printf("Não foi possivel cadastrar o Estado %s, pois ele já existe!!!", nome_estado);
                }
                break;

            case 2:
                printf("Estados Cadastrados:\n");
                imprimir_estados(estados);
                break;

            case 3:
                printf("Informe o nome do estado: \n"); scanf("%s", nome_estado);
                auxEstado = buscar_estado(estados, nome_estado, &encontrou);
                if (encontrou == 1){
                    menu_estado(auxEstado, pessoas, estados);
                }else{
                    printf("Estado nao encontrado");
                }
                break;
            case 4:
                printf("Informe o nome da pessoa: \n"); scanf("%s", nome_pessoa);
                printf("Informe o nome do CEP de onde nasceu: \n"); scanf("%d", &CEPNascimento);
                printf("Informe o nome do CEP de onde mora: \n"); scanf("%d", &CEPAtul);
                printf("Informe o CPF da pessoa: \n"); scanf("%s", CPFPessoa);
                printf("Informe o data de nascimento da pessoa (exemplo: Dia/Mes/Ano): \n"); scanf("%s", DataNascimento);

                auxPessoa = buscarPessoaNome(pessoas, nome_pessoa);

                auxEstado = buscar_estado_Por_CEP(estados, CEPNascimento);

                aux_2Estado = buscar_estado_Por_CEP(estados, CEPAtul);

                if (auxPessoa == NULL && (auxEstado != NULL && aux_2Estado != NULL)){
                    preencherPessoa(&auxInfopessoa, CEPNascimento, CEPAtul, nome_pessoa, CPFPessoa, DataNascimento);

                    resp = inserirPessoa(&pessoas, auxInfopessoa);

                    if (resp == 1){
                        auxCidade = buscarCidadePorCEP(aux_2Estado->infoEstado.arv_cidade, CEPAtul);
                        printf("Pessoa Cadastrado !!!\n");
                        aux_2Estado->infoEstado.populacao += 1;
                        auxCidade->infoCidade.populacao_c += 1;
                    }else{
                        printf("Não foi possivel cadastrar a Pessoa %s !!!\n", nome_pessoa);
                    }
                    
                }else{
                    printf("Não foi possivel cadastrar a Pessoa %s pois ele já existe ou CEPs Inexistentes!!!\n", nome_pessoa);
                }
                break;
            case 5:
                printf("\nÁrvore de Pessoas (in-order):\n");
                imprimirPessoa(pessoas);
                break;
            case 6:
                printf("Informe o nome da pessoa: \n"); scanf("%s", nome_pessoa);
                auxPessoa = buscarPessoaNome(pessoas, nome_pessoa);
                if (auxPessoa != NULL){
                    printf("Pessoa: %s | CPF: %s | CEP de nascimento: %d | CEP de atual: %d\n",
                            auxPessoa->infoPessoa.nome,
                            auxPessoa->infoPessoa.CPF,
                            auxPessoa->infoPessoa.cepCidadeNatal,
                            auxPessoa->infoPessoa.cepCidadeAtual
                        );
                }else{
                    printf("Pessoa nao encontrada!!!\n");
                }
                break;
            case 7:
                printf("Informe o nome da pessoa: \n"); scanf("%s", nome_pessoa);
                
                auxPessoa = buscarPessoaNome(pessoas, nome_pessoa);
                if (auxPessoa != NULL){
                    resp = removerPessoa(&pessoas, nome_pessoa);
                    if (resp == 1){
                        aux_2Estado = buscar_estado_Por_CEP(estados, CEPAtul);
                        auxCidade = buscarCidadePorCEP(aux_2Estado->infoEstado.arv_cidade, CEPAtul);
                        aux_2Estado->infoEstado.populacao -= 1;
                        auxCidade->infoCidade.populacao_c -= 1;
                    }
                    printf("Pessoa removida com sucesso!!!\n");
                }else{
                    printf("Pessoa nao encontrada!!!\n");
                }
                break;
            case 8:
                auxEstado = retorna_estados_com_maior_populacao(estados);
                printf("Estado mais populoso:\n");
                if (auxEstado != NULL){
                    printf("Estado: %s | Capital: %s | Cidades: %d | População: %d\n",
                        auxEstado->infoEstado.nome_estado,
                        auxEstado->infoEstado.nome_capital,
                        auxEstado->infoEstado.num_cidade,
                        auxEstado->infoEstado.populacao);
                }
                break;
            case 9:
                // Qual a população da Capital de um determinado estado?
                printf("Informe o nome do estado: \n"); scanf("%s", nome_estado);
                auxEstado = buscar_estado(estados, nome_estado, &resp);
                if (auxEstado != NULL){
                    auxCidade = buscarCidade(auxEstado->infoEstado.arv_cidade, auxEstado->infoEstado.nome_capital);
                    printf("Cidade: %s | populacao: %d\n",
                        auxCidade->infoCidade.nome_cidade,
                        auxCidade->infoCidade.populacao_c
                    );
                }else{
                    printf("Estado nao econtrado!!!\n");
                }
                break;
            case 10:
                // Qual a cidade mais populosa de um estado sem ser a Capital?
                printf("Informe o nome do estado: \n"); scanf("%s", nome_estado);
                auxEstado = buscar_estado(estados, nome_estado, &resp);
                auxCidade = cidade_mais_populosa_de_um_estado_sem_ser_a_capital(auxEstado->infoEstado.arv_cidade, auxEstado->infoEstado.nome_capital);
                printf("Cidade mais populosa de um estado sem ser a Capital:\n");
                if (auxCidade != NULL){
                    printf("Cidade: %s | populacao: %d\n",
                        auxCidade->infoCidade.nome_cidade,
                        auxCidade->infoCidade.populacao_c
                    );
                }
                break;
            case 11:
                // Quantas pessoas não moram na cidade natal
                printf("Quantas pessoas não moram na cidade natal:\n");
                resp = Quantas_pessoas_não_moram_na_cidade_natal(pessoas, estados);   
                printf("%d pessoas!!!\n",resp);
                break;

            case 12:
                // Qual cidade natal de uma pessoa dado o CEP da cidade
                printf("Informe o nome da pessoa: \n"); scanf("%s", nome_pessoa);
                auxPessoa = buscarPessoaNome(pessoas, nome_pessoa);
                if (auxPessoa != NULL){
                    auxEstado = buscar_estado_Por_CEP(estados, auxPessoa->infoPessoa.cepCidadeNatal);
                    auxCidade = buscarCidadePorCEP(auxEstado->infoEstado.arv_cidade, auxPessoa->infoPessoa.cepCidadeNatal);
                    printf("Cidade natal de %s eh %s\n", auxPessoa->infoPessoa.nome, auxCidade->infoCidade.nome_cidade);
                }else{
                    printf("Pessoa nao encontrada!!!\n");
                }
                break;

            case 13:
                // Quantas pessoas nascidas em uma determinada cidade não mora na cidade natal?
                //* Buscar pela cidade
                printf("Informe o nome da cidade: \n"); scanf("%s", nome_cidade);
                auxCidade = buscar_Cidade_nos_Estados(estados, nome_cidade);
                    if (auxCidade != NULL){
                        resp = numero_de_pessoas_nascidas_em_uma_determinada_cidade_nao_mora_na_cidade_natal(pessoas, auxCidade);
                        printf("Quantas pessoas nascidas em %s não mora na cidade natal? %d", nome_cidade, resp);                        
                    }else{
                        printf("Cidade não encontrada");
                    }
                break;

            case 14:
                // Quantas pessoas que moram em uma determinada cidade não nasceram na cidade?* Buscar pela cidade
                //* Buscar pela cidade
                printf("Informe o nome da cidade: \n"); scanf("%s", nome_cidade);
                auxCidade = buscar_Cidade_nos_Estados(estados, nome_cidade);
                    if (auxCidade != NULL){
                        resp = numero_de_pessoas_que_moram_em_uma_determinada_cidade_nao_nasceram_nela(pessoas, auxCidade);
                        printf("Quantas pessoas que moram em %s e não nasceram nessa cidade? %d", nome_cidade, resp);                        
                    }else{
                        printf("Cidade não encontrada");
                    }
                break;
            
            // case 15:
            //     modo_teste(&estados, &pessoas);
            //     break;
            default:
                printf("Opção inválida! Tente novamente.\n");
                break;
        }

    } while (opcao != 0);

    
    return 0;
}