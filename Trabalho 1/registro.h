#ifndef REGISTRO_H
#define REGISTRO_H
/**
[SCC0215Turma1]
Aluno: Breno Pejon Rodrigues	
NºUSP: 10801152
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

	typedef struct registro REGISTRO;

	//Printa as informações do bebê da funcionalidade 2
	void print_registro(REGISTRO reg);
	
	//Teste para verificar os dados salvos
	void print_reg_teste(REGISTRO reg);

	//Escreve o resgistro reg no arquivo binário arq
	void escreve_reg_arq(REGISTRO reg, FILE *arq);

	//Le o arquivo .csv passado em leitura e salva seus dados em arq
	int le_entrada( FILE *leitura, FILE *arq);

	//Lê todos os registros do arquivo binário passado em arq
	void le_todos_registros(FILE *arq);
	
#endif