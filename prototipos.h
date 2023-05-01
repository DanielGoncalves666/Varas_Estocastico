/*
Nome do Arquivo: prototipos.h
Projeto: Varas Estocástico
Programador: Daniel Gonçalves

Descrição: Arquivo de cabeçaho reponsável pela definição de constantes, variáveis, estruturas e funções globais. Todas com escopo de programa.
*/

#ifndef PROTOTIPOS_H
#define PROTOTIPOS_H

// - - - - - - - - - - - - - - - - - - - - - - - - - - - Constantes - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - //

#define QTD_TESTES 92				//define a qtd de testes que serão realizados
#define PEDESTRES 30				//define a qtd de pedestres que serão distribuídos
#define QTD_PORTAS 1				//define a qtd de portas e consequentemente de camadas
#define PAREDE 500				//define os valores para as paredes
#define VALOR_PORTA 1				//define o valor a ser atribuído para a célula de uma porta

// - - - - - - FOGO - - - - - - //
#define ZONA_NEUTRA 10	 			//limite da zona onde nenhum foco de incêndio pode começar						//fogo
#define ZONA_QUENTE 16				//limite da zona ideal para focos de incêndio começarem							//fogo
#define QTD_FOCOS 4				//define a quantidade de focos de incêndio								//fogo
#define VALOR_FOGO 400				//define o valor de uma célula pegando fogo								//fogo
#define PROBABILIDADE 25			//define a probabilidade de uma célula na vizinhança de um foco pegar fogo (por mil)			//fogo

//constantes próprias do modelo estocástico
#define QTD_ELIT 3 		//quantidade de células da vizinhança de um pedestre que farão parte do processo estocástico

// - - - - - - - - - - - - - - - - - - - - - - - - - - - Estruturas de Dados - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - //

typedef struct{				//estrutura de dados padrão para os pedestres
	int num, na_sala, mover;	//numero do pedestre (0 até PEDESTRES-1), se ele está na sala, se ele pode se mover
	int linha_atual, coluna_atual;	//posição atual do pedestre em linha e coluna, respectivamente
	int linha_mover, coluna_mover;	//posição ao qual o pedestre irá se mover, em linha e coluna, respectivamente
}Pessoas;

typedef struct{
	int **mat;
}mat_int;

typedef struct{
	float **mat;
}mat_float;

typedef struct node{//estrutrura do tipo fila
	int num;
	struct node *prox;
}node;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - Variáveis e Vetores Globais - - - - - - - - - - - - - - - - - - - - - - - - - - - //
extern float DIST_ELIT;		//valor máximo das células onde a movimentação estocástica começa a ocorrer
extern char caracter;		//variável onde será armazenada o caractere que será usado para imprimir a matriz do octave

extern int lin, col;	//qtd de linhas, qtd de colunas
extern int local_port[QTD_PORTAS*2];	//cria-se um vetor que armazena a linha e a coluna, respectivamente, para cada porta
extern int semente;	//variavel para armazenar o valor que sera usado pela rand(), para gerar numeros pseudo aleatorios

extern Pessoas Pedestre[PEDESTRES];	//estrutura para a quantidade PEDESTRES de indivíduos
extern mat_int sala, fogo, fogo_aux; //estrutura para a sala, onde os pedestres serao alocados						//fogo
extern mat_float campo_piso[QTD_PORTAS];	//estrutura para o campo de piso.
extern mat_float piso, piso_original;		//estrutura para o piso final, obtido a partir das matrizes do campo_piso					//fogo

// - - - - - - - - - - - - - - - - - - - - - - - - - - - Prototipos das Funções - - - - - - - - - - - - - - - - - - - - - - - - - - - - - //

//imprimir.c
extern void ler_inicio();	//função para ler as entradas necessárias (lin, col, local_portas)
extern void imprimir_piso(mat_float *M);//função para imprimir uma camada de campo de piso
extern void imprimir_sala(mat_int *M);//função para imprimir uma matriz de inteiros, mais especificamente a nossa sala
extern void imprimir_sala_pedestres(mat_int *M, mat_float *N);//função para imprimir a sala com os pedestres de uma maneira visualmente amiga
extern void imprimirOctave();//função responsável por imprimir a sala e os comandos necessários para gerar uma imagem no ocatave.
extern void imprimirComandos();//função responsável por imprimir os camandos que gerarão as imagens no ocatave
extern void imprimirSalaCompacta(mat_int *M, mat_float *N);

//piso.c
extern void inicializar_campo_piso();//função para inicializar a matriz campo_piso
extern void inicializar_mat_int(mat_int *M, int valor, int aux);//função para inicializar uma matriz de inteiros
extern void inserir_port();//função para inserir as portas em sua respectiva camada da matriz campo_piso
extern void distribuir_piso();//função que reune as duas funções de atribuição de valores ao piso
extern void piso_final();//junta todos os valores em um piso final, sendo que os valores menores tem a prioridade
extern void maiorCampoPiso();//função que determina o maior campo de piso
extern int contarVizin(mat_float *M, float num, int a, int b);//função que conta a quantidade de células na vizinhança com um determinado valor		//fogo
extern void copiarPiso(mat_float *M, mat_float *N);//função que copia o campo de piso para outra							//fogo

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

//fogo.c
extern void inserir_fogo();//função responsável por inserir os focos de incêndio na sala
extern void fogo_espalhar();//função responsável por espalhar o fogo pela sala

#endif
