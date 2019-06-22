/*
Nome do Arquivo: movimentar.c
Programador: Daniel Gonçalves
Data de criação: 2019
Última modificação: 2019

Descrição: arquivo onde estão implementadas as funções de alocação e dos mecanismos de movimentação dos pedestres 

*/

#include<stdio.h>
#include<stdlib.h>
#include"prototipos.h"

static void verifica_erro(int a, int b, int ped);//função que verifica se algum pedestre está em uma posição inválida
static int valid_cell(int a, int b);//função responsavel por contar a qtd de celulas validas para movimentação
static void storage_cell(float **vet, int a, int b);//função responsavel por armazenar os valores, as linhas e colunas da vizinhança no vetor
static void organiza_vetor(float **vet,int tamanho);//função que organizar em ordem crescente um vetor com tamanho 'tamanho'
static void troca(float *i, float *h);//função responsavel por trocar os valores entre duas variaveis ou posições de um vetor
static int comparation(float **vet, int tamanho);//função para determinar a menor celula da vizinhança de um pedestre
static int function_panic(Pessoas *P);//função para panico entre os pedestres
static void X_decide(Pessoas *p, Pessoas *p1);//função responsável por decidir qual pedestre irá se mover
static node *aloca(Pessoas *pes);//função para se alocar elementos da lista de conflitos	
static void libera(node *LISTA);//função para desalocar a memória dinâmicamente alocada de uma lista

void pedestre_alocar(){//função para alocar os pedestres na sala
	for(int ped=0; ped<PEDESTRES;){//ped serve para determinar qual pedestre esta sendo alocado no momento
		int a = rand()%lin;//a variavel a recebe o resto da divisao de um numero aleatorio por lin, ou seja, pode ser de 0 até lin-1
		int b = rand()%col;//a variavel b recebe o resto da divisao de um numero aleatorio por col, ou seja, pode ser de 0 até col-1
		
		if(ped == PEDESTRES)
			break;//caso a qtd de pedestres tenha sido atingida, paramos o laço
		if(piso.mat[a][b] == PAREDE || piso.mat[a][b] == VALOR_PORTA)
			continue;//caso a posição que foi escolhida aleatoriamente for uma parede ou uma porta, pula
		else if(sala.mat[a][b] >= 2)
			continue;//caso a posição já estiver ocupada, pula
		else{
			sala.mat[a][b] = 2+ped;//na posição escolhida para o pedestre na matriz sala sera colocado o numero do pedestre + 2  
			Pedestre[ped].num = ped;//armazena o numero real do pedestre
			Pedestre[ped].linha_atual = a;//armazena a linha atual do pedestre
			Pedestre[ped].coluna_atual = b;//armazena a coluna atual do pedestre
			Pedestre[ped].na_sala = 1;//inicializa a variavel que indica a presença do pedestre na sala, 1 = presente, 0 = saiu
			Pedestre[ped].mover = 1;//inicializa a variavel que indica se o pedestre pode se mover, 1 = pode se mover, 0 = n pode
			
			ped++;//incrementa a variavel que indica o pedestre
		}
	}
}

void basic_moviment(){//função para realizar o movimento basico dos pedestres

	for(int ped=0; ped<PEDESTRES;){//laço para percorrer o vetor de pedestres
		if(Pedestre[ped].na_sala == 0){//caso na_sala for 0, significa q o pedestre ja saiu do local
			ped++;//aumentamos a variavel ped
			continue;//e pulamos o pedestre
		}
		Pedestre[ped].mover = 1;//reinicia a variavel que indica que o pedestre, a principio, possui permissao para se mover

		int pnc = function_panic(&Pedestre[ped]);//função para determinar panico nos pedestres, 5% de chance de ocorrer
		if(pnc == 1){//caso pnc for igual a 1, significa que o pedestre entrou em panico
			//printf("Pedestres %d entrou em pânico\n\n",ped+2);
			ped++;//incrementamos a variavel ped
			continue;//e passamos pra frente
		}

		int a = Pedestre[ped].linha_atual;//a recebe a linha atual do pedestre
		int b = Pedestre[ped].coluna_atual;//b recebe a coluna atual do pedestre

		verifica_erro(a,b,ped);//função que verifica se algum pedestre está em uma posição inválida

		int valid = valid_cell(a,b);//valid armazena a qtd de celulas validas, que sera o tamanho de um vetor
		if(valid == 0){//caso o pedestre estiver encurralado e não puder se mover para local algum
			Pedestre[ped].mover = 0;//determinamos que ele é incapaz de se mover
			continue;//e passamos para o próximo pedestre
		}
		
		float **celulas = NULL;//ponteiro para ponteiro pra matriz que armazenará os valores e a posição da célula
		celulas = malloc(valid*sizeof(float *));//alocamos um bloco de memória de tamanho valid para o ponteiro de ponteiro
		for(int i=0;i<valid;i++)
			celulas[i] = malloc(3*sizeof(float));//para cada posição do bloco alocamos um novo bloco de 3 posições

		storage_cell(celulas,a,b);//função responsavel por fazer esse armazenamento
		organiza_vetor(celulas,valid);//organizar os valores da vizinhamça em ordem crescente

		int chosen = comparation(celulas,valid);//retorna a celula para a qual o pedestre irá se mover

		Pedestre[ped].linha_mover = (int) celulas[chosen][1];//a variavel que indica a linha para onde o pedestre irá se mover recebe o valor amazenado na segunda coluna da matriz celulas
		Pedestre[ped].coluna_mover = (int) celulas[chosen][2];//a variavel que indica a coluna para onde o pedestre irá se mover recebe o valor armazenado na terceira coluna da matriz celulas

		for(int i=0; i<valid; i++)//desaloca a memória de cada linha
			free(celulas[i]);
		free(celulas);//desaloca a memoria do vetor de linhas
		ped++;
	}
}


void verifica_erro(int a, int b, int ped){//função que verifica se algum pedestre está em uma posição inválida
	//função aplicada apenas para pedestres que já n tenham saido da sala e que podem se movimentar
	
	if(a==0 || a==lin-1){//verifica se o pedestre está em uma linha invalida
		printf("\n\nWARNING WARNING WARNING\nLINHA (%d) inválida para o pedestre %d\n\n",a,ped);
		exit(0);
	}
	if(b==0 || b==col-1){//verifica se o pedestre está em uma coluna invalida
		printf("\n\nWARNING WARNING WARNING\nCOLUNA (%d) inválida para o pedestre %d\n\n",b,ped);
		exit(0);
	}
}

int valid_cell(int a, int b){//função responsavel por contar a qtd de celulas validas para movimentação
	int valid = 0;

	for(int c=-1; c<2; c++){
		for(int d=-1; d<2; d++){//juntos percorrem a vizinhança do pedestre	
			if(sala.mat[a+c][b+d] >= 2 || piso.mat[a+c][b+d] == PAREDE)
				continue;//caso a posição ja estiver ocupada, ou for uma parede, passamos pra frente
			else
				valid++; 
		}
	}
	return valid;
}

void storage_cell(float **vet, int a, int b){//função responsavel por armazenar os valores, as linhas e colunas da vizinhança no vetor
	int i = 0;//a variavel i é responsavel por percorrer a matriz de armazenamento, linha por linha

	for(int c=-1; c<2; c++){
		for(int d=-1; d<2; d++){//juntos percorrem a vizinhança do pedestre	
	 		if(sala.mat[a+c][b+d] >= 2 || piso.mat[a+c][b+d] == PAREDE)
				continue;//caso a posição ja estiver ocupada, ou for uma parede, passamos pra frente
			else{
				vet[i][0] = piso.mat[a+c][b+d];//armazena o valor da celula
				vet[i][1] = a+c; //armazena a linha da celula
				vet[i][2] = b+d; //armazena a coluna da celula
				i++;//essa variavel serve para percorrer o vetor
			}			
		}
	}
}

void organiza_vetor(float **vet,int tamanho){//função que organizar em ordem crescente um vetor com tamanho 'tamanho'

	//para organizar esse vetor em ordem crescente, verificações serão feitas se comparando determinada posição com sua posterior, devido a esse fato, a ultima não sofrera o processo, pois caso contrario uma falha de segmentação acontecerá
	int fim;
	do{
		fim = 0;//atribuimos zero para a variavel, caso todo o vetor esteja organizado, ela n recebe 1, o que indica o fim do laço
		for(int n=0; n<tamanho-1; n++){//ajudara a percorrer o vetor, da primeira posição até a penultima
			if(vet[n][0] > vet[n+1][0]){//caso a posição em analise for maior que a proxima
				troca(&vet[n][0],&vet[n+1][0]);//troca os valores de posição
				troca(&vet[n][1],&vet[n+1][1]);//troca as linhas de posição
				troca(&vet[n][2],&vet[n+1][2]);//trocas as colunas de posição
				fim = 1;//muda pra 1 indicando que pelo menos uma operação de reorganização foi realizada e pra ter certeza que está tudo nos conformes, é necessário mais uma repetição, pelo menos
			}
		}
	}while(fim == 1);//vai parar quando nenhuma operação tiver sido realizado no vetor nesses ciclo
}

void troca(float *i, float *h){//função responsavel por trocar os valores entre duas variaveis ou posições de um vetor
	float aux;
	aux = *i;//aux recebe o valor de *i
	*i = *h;//*i recebe o valor de *h
	*h = aux;//*h recebe o valor de *i
}

int comparation(float **vet, int tamanho){//função para determinar a menor celula da vizinhança de um pedestre

	int ultimo = 0;//indica a ultima posição que teve valor semelhante a celula no indice 0, que a principio é ela propria
	int n=1;//indica a posição pela qual a comparação ira começar
	for(; n<tamanho-1; n++){
		if(vet[0][0] == vet[n][0]){//se caso o valor da menor celula for igual ao valor da celula em vet[n]
			// a coluna 0 é onde os valores de cada celula estão armazenados
			ultimo = n;//com isso saberemos o indice do ultimo e por conseguinte a qtd de celulas com msm valor
		}
	}//esse for serve para verificar se há alguma celula com valor igual a do indice 0
	
	return rand()%(ultimo+1);//o resto dessa divisao sera o indice do valor que foi escolhida para que o pedestre se movimente
			  //ultimo+1 indica o ultimo indice + o do primeiro valor
}

int function_panic(Pessoas *P){//função para panico entre os pedestres
	//recebe o endereço da estrutura especifica do pedestre

	int sem = rand()%100;//um numero aleatorio é gerado e entao dividido por 100, de modo a podermos fazer operações de porcentagem
	//printf("%d\n",sem);
	if(sem < 5){//caso o numero gerado for menos que 5, ou seja, se estiver na margem de 5% prevista no modelo, o pedestre entra em panico
		P->mover = 0;//como o pedestre entrou em panico, logo ele n pode se mover
		//panico++;//incrementa-se a variavel para permitir a geração de numeros diferentes
		return 1;//retorna o valor 1, significando que o pedestre entrou em panico
	}else{
		//panico++;//incrementa-se a variavel para permitir a geração de numeros diferentes
		return 0;//retorna 0, significando que o pedestre não entrou em panico
	}
	//um erro que foi cometido durante o desenvolvimento dessa função foi colocar panico++ no fim da função, esperando que o restante dela seria executado. Porém, deve-se atentar ao fato que quando é executado um return, o comando do programa é devolvido para a main
}

void movimentar_em_X(){//função responsável por impedir a movimentação em X

	for(int a=1; a<lin-1; a++){
		for(int b=1; b<col-1; b++){
			if(sala.mat[a][b] >= 2){//localiza-se os pedestres na sala
				int p = sala.mat[a][b]-2;//atribui o numero real do pedestre à variavel p
				if(Pedestre[p].mover == 0)
					continue;	//se o pedestre estiver impossibilitado de se mover, o ignoramos
				for(int c=1; c<2; c++){
					for(int d=-1; d<2; d++){//fazemos uma busca por pedestres na vizinhança
						if(((c+1)%2 == 0 && (d+1)%2 == 0) || (a+c == a && b+d == b) || piso.mat[a+c][b+d] == 1.0)
							continue; //se a celula for das diagonais, a do pedestre ou uma porta, ignoramos
						if(Pedestre[p].linha_mover == a || Pedestre[p].coluna_mover == b)
							continue;//se o pedestre for se mover para uma celula em que n ocorre a movimentação em x, ignoramos
						if(sala.mat[a+c][b+d] >= 2 && sala.mat[a+c][b+d] != PAREDE){
							int p1 = sala.mat[a+c][b+d]-2;//atribui o numero real do pedestre na vizinhança à variavel p1
							if(Pedestre[p1].mover == 0 || Pedestre[p1].na_sala == 0)
								continue; //se o pedestre não estiver na sala ou impossibilitado de se mover, ignoramos
							if(Pedestre[p1].coluna_atual == b){ //se o pedestre p1 estiver na msm coluna do p
								if(Pedestre[p1].linha_atual == a-1 && Pedestre[p].linha_mover == a-1){
									//caso o pedestre p1 estiver localizado na linha superior em relação ao pedestre p ----> (a-1)(b)
									//caso o pedestre p pretender se mover para a linha superior	
									if(Pedestre[p1].linha_mover == a && Pedestre[p1].coluna_mover == b-1){//caso o pedestre p1 for se mover para (a)(b-1)
										X_decide(&Pedestre[p],&Pedestre[p1]);
									}else if(Pedestre[p1].linha_mover == a && Pedestre[p1].coluna_mover == b+1){//caso o pedestre p1 for se mover para (a)(b+1)
										X_decide(&Pedestre[p],&Pedestre[p1]);
									}
								}else if(Pedestre[p1].linha_atual == a+1 && Pedestre[p].linha_mover == a+1){
									//caso o pedestre p1 estiver localizado na linha inferior em relação ao pedestre p ----> (a+1)(b)
									//caso o pedestre p pretender se mover para a linha inferior
									if(Pedestre[p1].linha_mover == a && Pedestre[p1].coluna_mover == b-1){//caso o pedestre p1 for se mover para (a)(b-1)
										X_decide(&Pedestre[p],&Pedestre[p1]);
									}else if(Pedestre[p1].linha_mover == a && Pedestre[p1].coluna_mover == b+1){//caso o pedestre p1 for se mover para (a)(b+1)
										X_decide(&Pedestre[p],&Pedestre[p1]);
									}
								}	
							}
							else if(Pedestre[p1].linha_atual == a){//se o pedestre p1 estiver na msm linha do p
								if(Pedestre[p1].coluna_atual == b-1 && Pedestre[p].coluna_mover == b-1){
									//caso o pedestre p1 estiver localizado na coluna à esquerda em relação ao pedestre p ----> (a)(b-1)
									//caso o pedestre p pretender se mover para a coluna à esquerda
									if(Pedestre[p1].coluna_mover == b && Pedestre[p1].linha_mover == a-1){//caso o pedestre p1 for se mover para (a-1)(b)
										X_decide(&Pedestre[p],&Pedestre[p1]);
									}else if(Pedestre[p1].coluna_mover == b && Pedestre[p1].linha_mover == a+1){//caso o pedestre p1 for se mover para (a+1)(b)
										X_decide(&Pedestre[p],&Pedestre[p1]);
									}
								}else if(Pedestre[p1].coluna_atual == b+1 && Pedestre[p].coluna_mover == b+1){
									//caso o pedestre p1 estiver localizado na coluna à direita em relação ao pedestre p ----> (a)(b+1)
									//caso o pedestre p pretender se mover para a coluna à direita
									if(Pedestre[p1].coluna_mover == b && Pedestre[p1].linha_mover == a-1){//caso o pedeste p1 for se mover para (a-1)(b)
										X_decide(&Pedestre[p],&Pedestre[p1]);
									}else if(Pedestre[p1].coluna_mover == b && Pedestre[p1].linha_mover == a+1){//caso o pedestre p1 for se mover para (a+1)(b)
										X_decide(&Pedestre[p],&Pedestre[p1]);
									}
								}
							}
						}		
					}
				}
			}
		}
	}	
}
/*
Pedestre[p1]							
(a-1)(b-1)	(a-1)(b)	(a-1)(b+1)


(a)(b-1)	   p		(a)(b+1)
	
	
(a+1)(b-1)	(a+1)(b) 	(a+1)(b+1)
*/

void X_decide(Pessoas *p, Pessoas *p1){//função responsável por decidir qual pedestre irá se mover
	int r = rand();
	//printf("Movimento em X\t");
	if(r%2 == 0){
		//printf("Pedestre %d ira mover. Pedestre %d ira aguardar.\n",p->num+2,p1->num+2);
		p1->mover = 0;//o pedestre p1 n irá se mover, apenas o pedestre p
	}else{
		//printf("Pedestre %d ira mover. Pedestre %d ira aguardar.\n",p1->num+2,p->num+2);
		p->mover = 0;//o pedestre p n irá se mover, apenas o pedestre p1
	}
}

void mover_msmlugar(){//função para impedir que pedestres se movimentem para a mesma posição 	
	node *lista = malloc(sizeof(node));//ponteiro para uma estrutura node, uma fila
	lista->prox = NULL;//inicializo o ponteiro dentro do primeiro elemento da lista
	
	for(int ped=0; ped<PEDESTRES; ped++){//percorre o vetor de pedestres
		lista->prox = NULL;//inicializo o ponteiro dentro do primeiro elemento da lista
		int qtd = 0;//variavel que conta a quantidade de conflitos
		if(Pedestre[ped].na_sala == 0 || Pedestre[ped].mover == 0)//se o pedestre n estiver na sala ou n for se mover, ignora
			continue;
		for(int a=0; a<PEDESTRES; a++){//passa novamente pelo vetor de pedestres para poder comparar com cada um
			if(ped == a || Pedestre[a].mover == 0 || Pedestre[a].na_sala == 0)
				continue;//se for o msm pedestre, se n for se mover ou n estiver na sala, ignora
			if(Pedestre[ped].linha_mover == Pedestre[a].linha_mover && Pedestre[ped].coluna_mover == Pedestre[a].coluna_mover){
				//se os pedestres forem se mover para a msm celula
				//printf("\nConflito entre:%d %d", ped+2 ,a+2);
				if(lista->prox == NULL){//caso a lista ainda esteja vazia

					node *prime = aloca(&Pedestre[ped]);//cria o elemento da estrura de dados para o primeiro pedestre
					lista->prox = prime;//faz que a 'cabeça' da lista aponte para esse primeiro elemento
					qtd++;//incrementa a qtd de pedestres em conflito
					
					node *second = aloca(&Pedestre[a]);//cria o elemento do struct para o segundo pedestre
					prime->prox = second;//faz com que o primeiro elemento aponte para o segundo
					qtd++;//incrementa a quantidade de pedestres em conflito
					
				}else{//caso um conflito para aquele pedestre já tenha sido estabelecido
					node *poli = aloca(&Pedestre[a]);//aloca um elemento para este pedestre
					node *tmp = lista->prox;//inicializa o ponteiro tmp com o endereço da 'cabeça' da lista
					while(tmp->prox != NULL)//procura pelo ultimo elemento que aponta para NULL
						tmp = tmp->prox;
					tmp->prox = poli;//quando encontra, faz com que esse elemento aponte para o recem criado
					qtd++;
				}
			}
		}
		
		if(qtd>=2){//se tiver mais de um pedestre na fila, assim existe um conflito
			int sem = rand()%qtd;//resto de um numero aleatorio pela qtd de pedestres
			//panico+= ped;//incrementa a variavel panico
			node *tmp = lista->prox;//da ao ponteiro tmp o endereço da 'cabeça' da lista

			for(int y=0; y<qtd; y++){
				//a variavel f indica qual pedestre esta sendo analisado	
				//printf(" \ny %d sem %d qtd %d\n",y,sem,qtd);
				//printf("%d\n",sem);
	
				if(y == sem){//o pedestre que for se mover é ignorado
					//printf("Pedestre %d vai mover.\n",tmp->num);
					if(tmp->prox != NULL)//caso o ponteiro n apontar para NULL
						tmp = tmp->prox;//faz com que tmp aponte para o proximo elemento da lista
				}else{
					//printf("Pedestre %d n vai mover.\n", tmp->num);
					Pedestre[tmp->num].mover = 0;//os pedestres que n foram escolhidos ficarão impossibilitados de se mover
					if(tmp->prox != NULL)//caso o ponteiro n apontar para NULL
						tmp = tmp->prox;//faz com que tmp aponte para o proximo elemento da lista
				}
			}
			libera(lista);//desaloca a memoria dinâmica contida na lista
		}
	}
}

node *aloca(Pessoas *pes){//função para se alocar elementos da lista de conflitos
	node *novo = malloc(sizeof(node));//aloca dinamicamente uma estrutura do tipo node
	if(novo == NULL){//se novo tiver como endereço NULL, siginifica que n há memoria suficiente
		printf("Sem memoria disponivel!\n");
		exit(1);
	}else{
		novo->num = pes->num;//armazena o numero do pedestre em conflito
		novo->prox = NULL;//indica que a estrutura n aponta para nenhuma outra
		return novo;//retorna o endereço da estrutura
	}
}

void libera(node *LISTA){//função para desalocar a memória dinâmicamente alocada de uma lista	
	if(LISTA->prox != NULL){//se a lista n estiver vazia
		node *proxNode, *atual;
		
		atual = LISTA->prox;
		while(atual != NULL){
			proxNode = atual->prox;//passa o endereço do proximo elemento da lista para proxNode
			free(atual);//libera a memoria do elemento da lista
			atual = proxNode;//passa esse endereço para o ponteiro atual
		}
	}
}

void pedestre_sair(){//função para retirar os pedestres que já tiverem saido da sala
	for(int ped=0; ped<PEDESTRES; ped++){//percorre o vetor de pedestres
		if(Pedestre[ped].na_sala == 0 || Pedestre[ped].mover == 0)//verifica se o pedestre está na sala ou se ele pode mover
			continue;					  //caso contrário, ignora
		if(piso.mat[Pedestre[ped].linha_mover][Pedestre[ped].coluna_mover] == 1.0)
			Pedestre[ped].na_sala = 0;//verifica se o pedestre irá se mover para uma célula cujo valor seja 1.0, que é o valor atribuído para portas, caso isso seja verdadeiro, retiramos o pedestre da sala ao atribuirmos 0 para a variavel na_sala	
	}
}

void pedestre_sair_2(){//função para retirar os pedestres que já tiverem saido da sala, porém usa o vetor de coordenadas das portas como ferramenta
	for(int ped=0; ped<PEDESTRES; ped++){//percorre o vetor de pedestres
		if(Pedestre[ped].na_sala == 0 || Pedestre[ped].mover == 0)//verifica se o pedestre está na está na sala ou se ele pode mover
			continue;					  //caso contrário, ignora
		for(int a=0; a<QTD_PORTAS*2; a+=2){//percorre o vetor onde as coordenadas das portas estão armazenadas
			if(Pedestre[ped].linha_mover == local_port[a] && Pedestre[ped].coluna_mover == local_port[a+1])
				Pedestre[ped].na_sala = 0;//verifica se o pedestre está se movendo para uma porta, se ele estiver, o retiramos da sala
		}
	}
}

void converter_posicao(){//função para atribuir a posição para o pedestre irá se mover nas variáveis que indicam onde ele está, realizando assim a movimentação
	for(int ped=0; ped<PEDESTRES; ped++){//percorre o vetor dos pedestres
		if(Pedestre[ped].na_sala == 0 || Pedestre[ped].mover == 0)
			continue;//caso o pedestre nao estiver na sala ou nao puder se mover, ignora 
		//if(ped == 4)
		//	printf("\n\n linha_atual = %d coluna_atual = %d\n",Pedestre[ped].linha_atual, Pedestre[ped].coluna_atual);
		Pedestre[ped].linha_atual = Pedestre[ped].linha_mover;//passa o valor de mover para atual
		Pedestre[ped].coluna_atual = Pedestre[ped].coluna_mover;//passa o valor de mover para atual
		//if(ped == 4)
			//printf("\n\n coluna_atual = %d coluna_mover = %d\n",Pedestre[ped].coluna_atual,Pedestre[ped].coluna_mover);
	}
}

void ped_sala_loc(){//função para alocar novamente os pedestres na sala formatada
	for(int ped=0; ped<PEDESTRES; ped++){//percorre o vetor dos pedestres
		if(Pedestre[ped].na_sala == 0)
			continue;//se caso o pedestre já tenha saido da sala, ignora
		//printf("%d Pedestre[ped].linha_atual = %d Pedestre[ped].coluna_atual = %d\n",ped,Pedestre[ped].linha_atual,Pedestre[ped].coluna_atual);
		sala.mat[Pedestre[ped].linha_atual][Pedestre[ped].coluna_atual] = Pedestre[ped].num + 2;
		//atribui para a determinada posição da sala o numero real do pedestre + 2
	}
}

int cont(){//função que conta a quantidade de pedestres na sala
	int qtd=0;//variavel para contar

	for(int a=0; a<lin-1; a++){
		for(int b=1; b<col-1; b++){//percorre a sala
			if(sala.mat[a][b] >= 2)//verifica se naquela posição há um pedestre
				qtd++;//incrementa a variavel 
		}
	}
	return qtd;//retorna a quantidade de pedestres na sala
}
