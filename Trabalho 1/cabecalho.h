#ifndef CABECALHO_H
#define CABECALHO_H

/**
[SCC0215Turma1]
USP - Trabalho Organizão de Arquivos - 2020
Aluno: Breno Pejon Rodrigues	
NºUSP: 10801152
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

	typedef struct cabecalho CABECALHO;
	/**Inicializa o arquivo cabeçalho com tudo inicialmente zerado
	   Recebe o arquivo binario arq como entrada
	*/
	void inicializa_status(FILE *arq);

	//Le os 128 bytes do cabecalho do arquivo binario arq
	void le_cabecalho(FILE *arq);

	//Altera o status do cabeçalho para inválido
	void status_invalido(FILE *arq);

	//Altera o status do cabeçalho para válido
	void status_valido(FILE *arq);

	//escreve o cabeçalho baseado no cabeçalho cab no arquivo binario arq
	void escreve_cabecalho(CABECALHO cab, FILE* arq);

	//Retorna o status atual do cabecalho
	char verifica_status(FILE *arq);

	//insere a quantidade de registros e o RRN deles ao final da leitura completa do arquivo .csv na primeira função
	void insere_qtd_registro(int qtd, FILE *arq);


#endif