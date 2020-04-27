#include <stdio.h>
#include <stdlib.h>
#include "cabecalho.h"
#include "registro.h"


/**
[SCC0215Turma1]
Exercício Prático do Dia 24/03 e 26/03
Aluno: Breno Pejon Rodrigues	
NºUSP: 10801152
*/



void menu(){
	printf("1 - Escrita de Dados\n");
	printf("2 - Leitura de Dados\n");
	printf("3 - Sair\n");
}

int main(int agrc, char **argv){
	menu();

	int opcao;
	scanf("%d", &opcao);

	while(opcao != 3){
		//opcao 1
		if(opcao == 1){
			//ler - strtok
			//%[^,] lê até a virgula (n le a virgula)
			//identificar valores nulos ( ,, ou ,\n)
			//verificar o que volta o scanf - se for 0 - nulo
			//não há /r


		}

		//opção3
		if(opcao == 2){

		}



	}



	return 0;
}