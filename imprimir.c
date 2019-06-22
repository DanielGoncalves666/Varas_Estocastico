/*
Nome do Arquivo: imprimir.c
Programador: Daniel Gonçalves
Data de criação: 2019
Última modificação: 2019

Descrição: arquivo onde estão implementadas as funções de saída e entrada de dados
*/

#include<stdio.h>
#include<stdlib.h>
#include"prototipos.h"

void ler_inicio(){	//função para ler as entradas necessárias (lin, col, local_portas)
	scanf("%d %d",&lin,&col);
	
	for(int b=0; b<QTD_PORTAS*2; b+=2){//para armazenar as portas
		//a variável 'b' indica qual dos dados está sendo armazenado, b para linha e b+1 para coluna
		scanf("%d",&local_port[b]);	
		scanf("%d",&local_port[b+1]);
	}
}

void imprimir_piso(mat_float *M){//função para imprimir uma camada de campo de piso
	for(int a=0; a<lin; a++){
		for(int b=0; b<col; b++){
			printf("%.1f\t", M->mat[a][b]);
		}
		printf("\n\n\n");
	}
}

void imprimir_sala(mat_int *M){//função para imprimir uma matriz de inteiros, mais especificamente a nossa sala
	for(int a=0; a<lin; a++){
		for(int b=0; b<col; b++){
			printf("%d\t",M->mat[a][b]);
		}
		printf("\n\n\n");
	}
}

void imprimir_sala_pedestres(mat_int *M, mat_float *N){//função para imprimir a sala com os pedestres de uma maneira visualmente amiga
	for(int a=0; a<lin; a++){
		for(int b=0; b<col; b++){
			if((*N).mat[a][b] == 1.0)
				printf(" \t");//caso a célula for uma porta, imprime um espaço
			else if(a == 0 || a == lin-1 || b == 0 || b == col-1)
				printf("%d\t",PAREDE);//caso for uma das células da borda, imprime o valor de PAREDE
			else if((*M).mat[a][b] == 0)
				printf(" \t");//caso for uma célula vazia, imprime um espaço
			else
				printf("%d\t", (*M).mat[a][b]);//imprime o numero relativo do pedestre
		}
		printf("\n\n\n");
	}
}
