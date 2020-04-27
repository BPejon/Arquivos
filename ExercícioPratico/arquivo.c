#include <stdio.h>
#include <stdlib.h>

/**
[SCC0215Turma1]
Exercício Prático do Dia 24/03 e 26/03
Aluno: Breno Pejon Rodrigues	
NºUSP: 10801152
*/

//Printa o menu de opções
//retorna a opção selecionada
int menu(){
	int escolha;

	printf("1 - Gravação de Dados\n");
	printf("2 - Observar dados\n");
	printf("3 - Sair\n");

	scanf("%d" , &escolha);
	return escolha;
}

//limpa buffer
void flush_in(){
	int ch;
	while( (ch = fgetc(stdin)) != EOF && ch != '\n'){}
}


//Salvar estruturas uma por uma para não errar na memória que seu programa está a usar
//ler tambem campo a campo
typedef struct Registro{
	//fgets salva a string um \n e \0, portanto possuem +2 para poder comportar 100, 50 e 13 respectivamente
	char nome_produto[102];
	char marca_fabricante[52];
	char codigo[15];			 
	float valor;
	
}REGISTRO;


//Escreve no arquivo desejado uma mensagem com o tamanho desejado
//Escreve na memória uma por uma para não alterar o tamanho do arquivo
void escreve_arquivo(FILE *arquivo, REGISTRO reg){

	fwrite(reg.nome_produto, sizeof(char), 102, arquivo);
	fwrite(reg.marca_fabricante, sizeof(char), 52, arquivo);
	fwrite(reg.codigo, sizeof(char), 15, arquivo);
	fwrite(&reg.valor, sizeof(float), 1, arquivo);

	return;
}



//Eu devo passar um novo registro alocado dinamicamente, caso contrário, ele passara o registro porém ao término da função ele se apagará e não passará nada
//REGISTRO *cria_novo_registro(){}

//Função que lê um novo registro
//Dessa maneira não preciso alocar dinamicamente o meu registro!!
void cria_novo_registro(REGISTRO *reg){	

	//lê na ordem estruturada na struct
	//Fgets salva o /n lido, portanto deve-se tratar o último byte e tirá-lo

	
	//limpa o buffer
	flush_in();

	printf("Nome do Produto: ");
	fgets(reg->nome_produto, 102, stdin);

	printf("Marca do Fabricante: ");
	fgets(reg->marca_fabricante, 52, stdin);
	

	printf("Código do Produto: ");
	fgets(reg->codigo, 15, stdin);


	printf("Preço do Produto: ");	
	scanf("%f", &reg->valor);


	printf("\n");					

	return;
}

void le_registro(FILE *arq_lei){
	REGISTRO reg;
	//-printf printa até o \0 portanto não haverá lixo

	
	//lê todos os dados do registro, em seguida, printa-os
	while(fread(reg.nome_produto, sizeof(char), 102, arq_lei) == 102){
		fread(reg.marca_fabricante, sizeof(char), 52, arq_lei);
		fread(reg.codigo, sizeof(char), 15, arq_lei);
		fread(&reg.valor, sizeof(float), 1, arq_lei);

		printf("Nome do produto: %s", reg.nome_produto);
		printf("Marca do fabricante: %s", reg.marca_fabricante);
		printf("Código do produto: %s" , reg.codigo);
		printf("Preço do Produto: %0.2f", reg.valor);
		printf("\n--------------------------------------\n");

	}
	
}



int main (int arg, char **argv){

	//Abre o menu de opções:
	int escolha = menu();

	//Se escolheu a opção um, então:
	if(escolha == 1){
		//criar um novo arquivo
		FILE *arquivo;
		arquivo = fopen("registro_produto.bin", "w+");

		//gravar em um novo registro
		int gravar = 0;
		REGISTRO registro;

		//permite a gravação dos dados até que o usuario preciona -1
		//Lê e grava de 1 em 1
		while(gravar != 2){
			//Lê o novo registro do teclado e armazena-o em registro
			//passar com ponteiro para salvar os dados
			cria_novo_registro(&registro);

			//Escreve o que está no registro no arquivo dos produtos registrados
			escreve_arquivo(arquivo, registro);

			printf("1 - Adcionar novo Produto\n");
			printf("2 - Sair e Salvar\n");
			scanf("%d" , &gravar);
		}
		fclose(arquivo);
	}

	//Se escolheu ler o arquivo
	else if(escolha == 2){
		FILE *leitura;
		leitura = fopen("registro_produto.bin", "rb");

		le_registro(leitura);

		fclose(leitura);
	}

	//se escolheu 3 ou nenhuma delas
	else if (escolha == 3){
		
	}	

	return 0;
}