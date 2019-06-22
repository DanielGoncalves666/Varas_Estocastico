/*
Nome do Arquivo: piso.c
Programador: Daniel Gonçalves
Data de criação: 2019
Última modificação: 2019

Descrição: arquivo onde estão implementadas as funções de destribuição do campo de piso.
*/

#include<stdio.h>
#include<stdlib.h>
#include"prototipos.h"

static float regra[3][3]={{1.5,1.0,1.5},
			  {1.0,0.0,1.0},
			  {1.5,1.0,1.5}};


static void inicializar_mat_float(mat_float *M, int valor, int aux);//função para inicializar uma matriz de floats
static void vizin_port(mat_float *M);//função para atribuir valor as celulas vizinhas das portas
static void campo_geral(mat_float *M);//preenche o restante do campo de piso

void inicializar_mat_float(mat_float *M, int valor, int aux){//função para inicializar uma matriz de floats
	//o primeiro parametro é um ponteiro para o tipo mat_float
	//o segundo é o valor com o qual a matriz sera preenchida
	//o terceiro é uma variavel auxiliar que indicara a abrangencia do preenchimento, com borda(0) ou sem as bordas(1 ou mais)
	for(int a=0+aux; a<lin-aux; a++){
		for(int b=0+aux; b<col-aux; b++){
			(*M).mat[a][b] = valor;	//atribui a cada posição o valor estabelecido	
		}
	}
}

void inicializar_campo_piso(){//função para inicializar a matriz campo_piso
	for(int a=0; a<QTD_PORTAS; a++){//vai formatar cada camada da estrutura, sendo cada uma indicada por a
		inicializar_mat_float(&campo_piso[a],PAREDE,0);//preenchera toda a matriz com o valor de PAREDE
		inicializar_mat_float(&campo_piso[a],0,1);	//preenchera toda a matriz, menos as bordas, com o valor 0
	}
}

void inicializar_mat_int(mat_int *M, int valor, int aux){//função para inicializar uma matriz de inteiros
	//o primeiro parametro é um ponteiro para o tipo mat_int
	//o segundo é o valor com o qual a matriz sera preenchida
	//o terceiro é uma variavel auxiliar que indicara a abrangencia do preenchimento, com borda(0) ou sem as bordas(1 ou mais)
	for(int a=0+aux; a<lin-aux; a++){
		for(int b=0+aux; b<col-aux; b++){
			(*M).mat[a][b] = valor;	//atribui a cada posição o valor estabelecido	
		}
	}
}

void inserir_port(){//função para inserir as portas em sua respectiva camada da matriz campo_piso
	for(int a=0, b=0; a<QTD_PORTAS; a++, b+=2){
		//a variavel 'a' realiza a contagem de camadas
		//a variavel 'b' indica a posição no vetor no qual estao armazenados os dados sobre as portas, b para linha e b+1 para coluna
		campo_piso[a].mat[local_port[b]][local_port[b+1]] = VALOR_PORTA;
	}
}

void vizin_port(mat_float *M){//função para atribuir valor as celulas vizinhas das portas
	for(int a=0; a<lin; a++){
		for(int b=0; b<col; b++){//esses dois laços de repetição e o if procuram pelo valor que indica a porta, no caso 1
			if((*M).mat[a][b] == VALOR_PORTA){
				if(a == 0){//se caso a porta estiver na parte superior
					for(int c=-1; c<2; c++){//percorre posições na frente da porta
						if((*M).mat[a+1][b+c] == PAREDE)
							continue;//caso a celula estiver com valor de PAREDE, ignoramos
						else
							(*M).mat[a+1][b+c] = regra[2][c+1]+VALOR_PORTA;
						//a+1 indica a linha imediatamente abaixo da porta, assim como o 2 em 'regra'
						//a soma b+c vai percorrer as tres posições possiveis
						//c+1 indica a posição relativa na coluna do vetor 'regra', o +1 serve pra deixar o valor entre 0 e 2
					}
				}
				if(a == lin-1){//caso a porta esteja na parte inferior
					for(int c=-1; c<2; c++){//percorre posições na frente da porta
						if((*M).mat[a-1][b+c] == PAREDE)
							continue;//caso a celula estiver com valor de PAREDE, ignoramos
						else
							(*M).mat[a-1][b+c] = regra[0][c+1]+VALOR_PORTA;
						//a-1 indica a linha imediatamente acima da porta, assim como o 0 em 'regra'
						//a soma b+c vai percorrer as tres posições possiveis
						//c+1 indica a posição relativa na coluna do vetor 'regra', o +1 serve pra deixar o valor entre 0 e 2
					}
				}
				if(b == 0){//caso a porta esteja na esquerda
					for(int d=-1; d<2; d++){//percorre as posições na frente da porta
						if((*M).mat[a+d][b+1] == PAREDE)
							continue;//caso a celula estiver com valor de PAREDE, ignoramos
						else
							(*M).mat[a+d][b+1] = regra[d+1][2]+VALOR_PORTA;
						//b+1 indica a coluna imediatamente a direita da porta, assim como o 2 em 'regra'
						//a soma a+d vai percorrer as tres posições possiveis
						//b+1 indica a posição relativa na linha do vetor 'regra', o +1 serve para deixar o valor entre 0 e 2
					}
				}
				if(b == col-1){//caso a porta esteja na direita
					for(int d=-1; d<2; d++){//percorre as posições na frente da porta
						if((*M).mat[a+d][b-1] == PAREDE)
							continue;//caso a celula estiver com valor de PAREDE, ignoramos
						else
							(*M).mat[a+d][b-1] = regra[d+1][0]+VALOR_PORTA;
						//b-1 indica a coluna imediatamente a esquerda da porta, assim como o 0 em 'regra'
						//a soma a+d vai percorrer as tres posições possiveis
						//b-1 indica a posição relativa na linha do vetor 'regra', 0 +1 serve para deixar o valor entre 0 e 2
					}
				}
			}
		}
	}
}

void campo_geral(mat_float *M){//preenche o restante do campo de piso
	for(int stop=0; stop<20;){//assim que todas as celulas tiverem sido preenchidas, stop começara a ser imcrementada
		for(int a=0; a<lin; a++){
			for(int b=0; b<col; b++){//percorre-se a matriz procurando por valores diferentes de PAREDE e maiores que 1
				if((*M).mat[a][b]>1.0 && (*M).mat[a][b]!=PAREDE){
					float base = (*M).mat[a][b];//atribui o valor da celula encontrada a variavel base
					for(int c=-1; c<2; c++){
						for(int d=-1; d<2; d++){//percorre-se a vizinhança da celula em questao
							if((*M).mat[a+c][b+d]==PAREDE)
								continue;//se caso uma celula da vizinhança for parede, ignora
							if((*M).mat[a+c][b+d]>1.0){//se caso a celula ja tiver um valor atribuido e n for uma porta
								if(regra[c+1][d+1]+base < (*M).mat[a+c][b+d]){
									//caso a soma do valor da celula base com o respectivo valor de regra for menor que o valor armazenado na posição em questao, substituimos o valor
									(*M).mat[a+c][b+d] = regra[c+1][d+1]+base;
								}
							}
							if((*M).mat[a+c][b+d] == 0.0){
								//caso a celula n tiver sido preenchida
								(*M).mat[a+c][b+d] = regra[c+1][d+1]+base;
							}
							
						}
					}
				}
			}
		}
		int zero=0;//variavel para contar a presença de celulas vazias
		for(int e=0; e<lin; e++){
			for(int f=0; f<col; f++){//percorre a matriz
				if((*M).mat[e][f] == 0.0)
					zero++;//sempre que se encontra uma celula vazia, é incrementada
			}	
		}
		if(zero==0){//caso n tiver nenhuma celula vazia começa-se a incrementar a variavel stop
			stop++;
		}
	}
}

void distribuir_piso(){//função que reune as duas funções de atribuição de valores ao piso
	for(int a=0; a<QTD_PORTAS; a++){//roda enquanto 'a' for menor que a qtd de portas, ou seja, enquanto houver camadas
		vizin_port(&campo_piso[a]);//passa o endereço de cada camada de campo_piso para as funçoes	
		campo_geral(&campo_piso[a]);
	}
}

void piso_final(){//junta todos os valores em um piso final, sendo que os valores menores tem a prioridade
	for(int camada=0; camada<QTD_PORTAS; camada++){//laço para percorrer todas as camadas da estrutura campo_piso
		mat_float *p = &campo_piso[camada];	//o endereço da camada em questao é atribuido ao ponteiro p
		for(int a=0; a<lin; a++){
			for(int b=0; b<col; b++){//percorre a matriz
				if(camada==0){//se for a primeira camada a ser analisada
					piso.mat[a][b]=p->mat[a][b];//atribui-se o valor da camada para a matriz piso
				
				}else if(piso.mat[a][b] > p->mat[a][b]){//caso for outra camada, se compara o valor armazenado na posição em piso com o armazenado na mesma posição na camada, caso o da camada for menor, o valor é substituido em piso
					piso.mat[a][b]=p->mat[a][b];
				}
			}
		}
	}

}
