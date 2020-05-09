#include <stdio.h>
#include <stdlib.h>
#include "cabecalho.h"
#include "registro.h"
#include "binarionatela.h"


/**
[SCC0215Turma1]
Trabalho 1 - Organização de arquivos
Aluno: Breno Pejon Rodrigues	
NºUSP: 10801152

Funções:
	1 - Lê dados do arquivos .csv e os registra em um arquivo binario organizado
	2 - Lê dados de arquivo binário e printa na tela


*/

//limpa buffer
void flush_in(){
	int ch;	
	while( (ch = fgetc(stdin)) != EOF && ch != '\n'){}
}




int main(int argc, char **argv){
	int opcao;
	char entrada[30];
	char nome_bin[30];
	//Lê entrada de dados
	scanf("%d %s %s", &opcao, entrada, nome_bin );

	//opcao 1
	if(opcao == 1){

		//abre o arquivo binário para ser gerado
		FILE *arquivo;
		if((arquivo = fopen(nome_bin, "wb+")) == NULL){
			printf("Falha no processamento do arquivo.\n");
			exit(0);
		}

		//abre o arquivo .csv de leitura
		FILE *leitura;
		if( (leitura = fopen(entrada, "r")) == NULL){
			printf("Falha no processamento do arquivo.\n");
			fclose(arquivo);
			exit(0);
		}

		//Quantidade dos arquivos inseridos
		int quantidade;

		//muda o status do arquivo para 0
		inicializa_status(arquivo);

		//lê os dados do .csv
		quantidade = le_entrada(leitura, arquivo);
		//escreve o cabeçalho
		insere_qtd_registro(quantidade, arquivo);

		//altera o status corretamente
		status_valido(arquivo);
		//termina a função
		fclose(arquivo);
		fclose(leitura);

		//printa a saida na tela
		binarioNaTela(nome_bin);
	}

	//opção2
	if(opcao == 2){
		FILE *arq;

		//se houver falha de carregamento do arquivo
		if( (arq = fopen(entrada,"rb")  )== NULL){
			printf("Falha no processamento do arquivo.\n");
			exit(0);
		}

		//Se o status estiver inconsistente
		if(verifica_status(arq) == '0'){
			printf("Falha no processamento do arquivo.\n");
			fclose(arq);
			exit(0);
		}

		status_invalido(arq);

		//Le os primeiros 128 bytes do cabeçalho 
		le_cabecalho(arq);
		//Le todos os dados do arquivo binario
		le_todos_registros(arq);

		status_valido(arq);
		fclose(arq);
	}

	return 0;
}