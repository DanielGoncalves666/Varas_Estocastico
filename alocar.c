/*
Nome do Arquivo: alocar.c
Programador: Daniel Gonçalves
Data de criação: 2019
Última modificação: 2019

Descrição: arquivo responsável pela implementação das funções de alocação dinâmica das matrizes usadas no programa.
*/

#include<stdio.h>
#include<stdlib.h>
#include"prototipos.h"

void aloca_int_mat(mat_int *M){//função para alocar uma matriz de inteiros "dentro" do ponteiro para ponteiros de uma estrutura mat_int
	(*M).mat = malloc(lin*sizeof(int *));	//o endereço de um vetor de  ponteiros para inteiros é passado para (*M).mat
	for(int a=0;a<col;a++){
		(*M).mat[a] = malloc(col*sizeof(int));//cada posição do vetor de ponteiros recebe o endereço de um vetor de inteiros
	}						//(*M).mat == M->mat
}

void aloca_float_mat(mat_float *M){//função para alocar uma matriz de reais "dentro" do ponteiro para ponteiros de um struct mat_float
	(*M).mat = malloc(lin*sizeof(float *));	//o endereço de um vetor de ponteiros para reais é passado para (*M).mat
	for(int a=0;a<col;a++){
		(*M).mat[a] = malloc(col*sizeof(float));//cada posição do vetor de ponteiros recebe o endereço de um vetor de reais
	}
}

void alocar_tudo(){//função para alocar todas as matrizes
	aloca_int_mat(&sala);//aloca a matriz sala
	aloca_float_mat(&piso);//aloca a matriz piso
	for(int b=0; b<QTD_PORTAS; b++){
		aloca_float_mat(&campo_piso[b]);//aloca uma matriz para cada posição de campo_piso
	}
}

void desaloca(){//função para desalocar as matrizes básicas
	free(sala.mat);
	free(piso.mat);
	for(int camada=0; camada<QTD_PORTAS; camada++){
		free(campo_piso[camada].mat);
	}
}
