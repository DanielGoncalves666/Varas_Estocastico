/*
Nome do Arquivo: imprimir.c
Projeto: Varas Estocástico
Programador: Daniel Gonçalves

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

void imprimirOctave(){//função responsável por imprimir a sala e os comandos necessários para gerar uma imagem no ocatave.
	
	if(caracter >=91)//verifica se o caracter é um simbolo nao valido ou letra minuscula, caso verdade, pulamos algumas unidades para evitar imprimirmos os primeiros
		printf("%c = [",(char) (caracter+6));//somamos 6 unidades para retirarmos a variável de dentro dessa faixa
	else
		printf("%c = [",(char) caracter);
		
	for(int a=0; a<lin; a++){
		printf("[ ");
		for(int b=0; b<col; b++){//percorre a sala
			if(piso.mat[a][b] == PAREDE)
				printf("2 ");//se caso a posição for uma parede
			else if(sala.mat[a][b] == 0 || sala.mat[a][b] == 1)
				printf("0 ");//caso for uma célula vazia
			else if(sala.mat[a][b] > 1)
				printf("1 ");//caso for um pedestre
		}
		printf("]\n");
	}
	printf("];\n\n");
	caracter++;
}

void imprimirComandos(){//função responsável por imprimir os camandos que gerarão as imagens no ocatave
	printf("\n\n");
	for(int a=65; a<=caracter; a++){
		if(a == 91){//pula símbolos não alfabéticos entre maiúsculas e minúsculas
			a = 97;
			caracter+=5;//incremeta a quantiade de símbolos pulados
		}
		printf("imagesc(%c); colormap(map);\n", (char) a);
	}
}
