/*
Nome do Arquivo: fogo.c
Programador: Daniel Gonçalves
Data de criação: 2019

Descrição: arquivo onde estão implementadas as funções relacionadas à propagação de focos de incêndio
*/

#include<stdio.h>
#include<stdlib.h>
#include"prototipos.h"

void inserir_fogo(){//função responsável por inserir os focos de incêndio na sala
	int cont = 0;//variável usada para contar em quantas células a verificação já passou
	int num = 0;//variavel que recebe o número aleatorio
	int break_auxiliar;//variável para auxiliar na parada do for que percorre as linhas

	for(int a=0; a<QTD_FOCOS; ){//quantidade de focos
		if(cont == 0){//o número aleatório é apenas gerado de cont estiver zerado
			num = rand()%(lin-2)*(col-2);//num recebe um número aleatorio localizado entre 0 e o produto das qtd de linhas e colunas que não sejam paredes
		}
		
		for(int i=1; i<lin-1; i++){
			for(int h=1; h<col-1;h++){//percorre a matriz nas posições que não são paredes
				break_auxiliar = 0;//inicializamos indicando que não se deve parar o laço
				if(sala.mat[i][h] > 0 || fogo.mat[i][h] > 0){//caso a posição esteja ocupada por um pedestre ou foco de incêndio
					continue;
				}
				if(piso.mat[i][h] > ZONA_NEUTRA && piso.mat[i][h] <= ZONA_QUENTE){//caso a célula esteja na zona determinada onde focos de incêndio podem começar
					if(cont == num){//caso a váriavel de contagem for igual ao número sorteado
						fogo.mat[i][h] = VALOR_FOGO;//atribui o valor do fogo na matriz	
						a++;//seu aumento indica a introdução com sucesso de um novo foco
						break_auxiliar = 1;//inicializamos indicando que se deve parar o laço
						cont = 0;//reiniciamos a varável de contagem
						break;				
					}else{
						cont++;//incrementamos a variável
					}
				}
			}
			if(break_auxiliar == 1)
				break;
		}
	}
}

void fogo_espalhar(){//função responsável por espalhar o fogo pela sala
	inicializar_mat_int(&fogo_aux,0,0);//inicializa a matriz auxiliar
	
	for(int a=0; a<lin; a++){
		for(int b=0; b<col; b++){//percorre a matriz a procura de focos
			if(fogo.mat[a][b] == VALOR_FOGO){//verifica se existe um foco na célula em questão
				fogo_aux.mat[a][b] = fogo.mat[a][b];//a matriz aux recebe o foco de incendio na posição determinada
				for(int c=-1; c<2; c++){
					for(int d=-1; d<2; d++){//percorre a vizinhança
						if(piso.mat[a+c][b+d] == PAREDE || piso.mat[a+c][b+d] == VALOR_PORTA || sala.mat[a+c][b+d] > 0 || fogo.mat[a+c][b+d] == VALOR_FOGO || piso.mat[a+c][b+d] <=5.0)
							continue;//caso a célula da vizinhança for uma parede ou uma porta ou estiver ocupada por um pedestre ou se já existir um foco naquela posição, ouuuuu se caso a célula estiver próxima de uma saída
						
						int sem = rand()%1000;//a variavel 'sem' recebe um numero aleatorio, entre 0 e 99

						if(sem < PROBABILIDADE)//quase sem for menor que o valor setado para a probabilidade
							fogo_aux.mat[a+c][b+d] = VALOR_FOGO;//um foco é iniciado
					}
				}
			}
		}
	}
	
	for(int a=0; a<lin; a++){
		for(int b=0; b<col; b++){
			fogo.mat[a][b] = fogo_aux.mat[a][b];//os valores da matriz_aux são passados para a matriz original	
		}
	}
}
