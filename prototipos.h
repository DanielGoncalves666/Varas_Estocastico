/*
Nome do Arquivo: prototipos.h
Programador: Daniel Gonçalves
Data de Criação: 2019
Última Modificação: 2019

Descrição: Arquivo de cabeçaho reponsável pela definição de constantes, variáveis, estruturas e funções globais. Todas com escopo de programa.
*/

#ifndef PROTOTIPOS_H
#define PROTOTIPOS_H

// - - - - - - - - - - - - - - - - - - - - - - - - - - - Constantes - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - //

#define QTD_TESTES 92				//define a qtd de testes que serão realizados
#define PEDESTRES 30				//define a qtd de pedestres que serão distribuídos
#define QTD_PORTAS 2				//define a qtd de portas e consequentemente de camadas
#define PAREDE 500				//define os valores para as paredes
#define VALOR_PORTA 1				//define o valor a ser atribuído para a célula de uma porta

// - - - - - - - - - - - - - - - - - - - - - - - - - - - Variáveis e Vetores Globais - - - - - - - - - - - - - - - - - - - - - - - - - - - //

int lin, col;	//qtd de linhas, qtd de colunas
int local_port[QTD_PORTAS*2];	//cria-se um vetor que armazena a linha e a coluna, respectivamente, para cada porta
int semente;	//variavel para armazenar o valor que sera usado pela rand(), para gerar numeros pseudo aleatorios
//int panico;	//vatiavel par auxiliar na geração de numeros aleatorios na função de panico

// - - - - - - - - - - - - - - - - - - - - - - - - - - - Estruturas de Dados - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - //

typedef struct{				//estrutura de dados padrão para os pedestres
	int num, na_sala, mover;	//numero do pedestre (0 até PEDESTRES-1), se ele está na sala, se ele pode se mover
	int linha_atual, coluna_atual;	//posição atual do pedestre em linha e coluna, respectivamente
	int linha_mover, coluna_mover;	//posição ao qual o pedestre irá se mover, em linha e coluna, respectivamente
}Pessoas;
Pessoas Pedestre[PEDESTRES];	//estrutura para a quantidade PEDESTRES de indivíduos

typedef struct{
	int **mat;
}mat_int;
mat_int sala;			//estrutura para a sala, onde os pedestres serao alocados

typedef struct{
	float **mat;
}mat_float;
mat_float campo_piso[QTD_PORTAS];	//estrutura para o campo de piso.
mat_float piso;				//estrutura para o piso final, obtido a partir das matrizes do campo_piso

typedef struct node{//estrutrura do tipo fila
	int num;
	struct node *prox;
}node;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - Prototipos das Funções - - - - - - - - - - - - - - - - - - - - - - - - - - - - - //

//imprimir.c
extern void ler_inicio();	//função para ler as entradas necessárias (lin, col, local_portas)
extern void imprimir_piso(mat_float *M);//função para imprimir uma camada de campo de piso
extern void imprimir_sala(mat_int *M);//função para imprimir uma matriz de inteiros, mais especificamente a nossa sala
extern void imprimir_sala_pedestres(mat_int *M, mat_float *N);//função para imprimir a sala com os pedestres de uma maneira visualmente amiga

//piso.c
extern void inicializar_campo_piso();//função para inicializar a matriz campo_piso
extern void inicializar_mat_int(mat_int *M, int valor, int aux);//função para inicializar uma matriz de inteiros
extern void inserir_port();//função para inserir as portas em sua respectiva camada da matriz campo_piso
extern void distribuir_piso();//função que reune as duas funções de atribuição de valores ao piso
extern void piso_final();//junta todos os valores em um piso final, sendo que os valores menores tem a prioridade

//movimentar.c
extern void pedestre_alocar();//função para alocar os pedestres na sala
extern void basic_moviment();//função para realizar o movimento basico dos pedestres
extern void movimentar_em_X();//função responsável por impedir a movimentação em X
extern void mover_msmlugar();//função para impedir que pedestres se movimentem para a mesma posição
extern void pedestre_sair();//função para retirar os pedestres que já tiverem saido da sala
extern void converter_posicao();//função para atribuir a posição para o pedestre irá se mover nas variáveis que indicam onde ele está, realizando assim a movimentação
extern void ped_sala_loc();//função para alocar novamente os pedestres na sala formatada
extern int cont();//função que conta a quantidade de pedestres na sala

extern void pedestre_sair_2();//função para retirar os pedestres que já tiverem saido da sala, porém usa o vetor de coordenada das portas como ferramenta

//alocar.c
extern void aloca_int_mat(mat_int *M);//função para alocar uma matriz de inteiros "dentro" do ponteiro para ponteiros de uma estrutura mat_int
extern void aloca_float_mat(mat_float *M);//função para alocar uma matriz de reais "dentro" do ponteiro para ponteiros de um struct mat_float
extern void alocar_tudo();//função para alocar todas as matrizes
extern void desaloca();//função para desalocar as matrizes básicas


#endif
