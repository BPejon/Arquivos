#include "cabecalho.h"

/**
[SCC0215Turma1]
USP - Trabalho Organizão de Arquivos - 2020
Aluno: Breno Pejon Rodrigues	
NºUSP: 10801152

	Arquivo cujo conteúdo trata-se de funções dedicadas a tratar do cabeçalho do arquivo binário do sistema
*/



//Registro de Cabeçalho, contém 128 bytes
typedef struct cabecalho{
	char status;					//armazena o status do arquivo
	int RRNproxRegistro;			//armazena o proximo rrn da pilha
	int numeroRegistrosInseridos;	//indica quantos registros foram inseridos (decrementa quando há arquivos logicamente removidos)
	int numeroRegistrosRemovidos;	//indica quantos registros foram logicamente removidos
	int numeroRegistrosAtualizados;	//indica registros atualizados
	//111 bytes de restos com lixo de $$
}CABECALHO;

//Inicializa o cabeçalho com tudo inicializado
void inicializa_status(FILE *arq){
	CABECALHO cab;
	cab.status = '0';
	cab.RRNproxRegistro = 0;
	cab.numeroRegistrosInseridos = 0;
	cab.numeroRegistrosRemovidos = 0;
	cab.numeroRegistrosAtualizados = 0;

	//inicializa cabecalho com status 0
	fwrite(&cab.status, sizeof(char), 1, arq);
	fwrite(&cab.RRNproxRegistro, sizeof(int), 1, arq);
	fwrite(&cab.numeroRegistrosInseridos, sizeof(int), 1, arq);
	fwrite(&cab.numeroRegistrosRemovidos, sizeof(int), 1, arq);
	fwrite(&cab.numeroRegistrosAtualizados, sizeof(int), 1, arq);

	//complementa com 111bytes de lixo
	char lixo = '$';
	for(int i = 0; i<111; ++i)
		fwrite(&lixo, sizeof(char), 1 , arq);
	
}
//le o cabeçalho
void le_cabecalho(FILE *arq){
	CABECALHO cab;
	fread(&cab.status, sizeof(char), 1, arq);
	fread(&cab.RRNproxRegistro, sizeof(int), 1, arq);
	fread(&cab.numeroRegistrosInseridos, sizeof(int), 1, arq);
	fread(&cab.numeroRegistrosRemovidos, sizeof(int), 1, arq);
	fread(&cab.numeroRegistrosAtualizados, sizeof(int), 1, arq);

	//pula o lixo
	fseek(arq, 111, SEEK_CUR);
}

//altera o status para 0
void status_invalido(FILE *arq){
	char status = '0';
	rewind(arq);	//volta para o inicio do arquivo
	fwrite(&status, sizeof(char), 1, arq);
}

//altera o status para 1
void status_valido(FILE *arq){
	char status = '1';
	rewind(arq);	//volta para o começo do arquivo
	fwrite(&status, sizeof(char), 1, arq);
}

void escreve_cabecalho(CABECALHO cab, FILE* arq){
	rewind(arq);
	char status = '0'; //continua invalido
	fwrite(&status, sizeof(char), 1, arq);
	fwrite(&cab.RRNproxRegistro, sizeof(int), 1, arq);
	fwrite(&cab.numeroRegistrosInseridos , sizeof(int),1,arq);
	fwrite(&cab.numeroRegistrosRemovidos , sizeof(int),1,arq);
	fwrite(&cab.numeroRegistrosAtualizados, sizeof(int),1,arq);

	//pula o lixo
	fseek(arq, 111, SEEK_CUR);

}


//verificar o status do arquivo
char verifica_status(FILE *arq){
	rewind(arq);
	char status;
	fread(&status, sizeof(char),1,arq);

	return status;
}

//Insere quantos registros foram inseridos após terminado a leitura de todo arquivo .csv
//Além de inserir o RRN, que será igual à quantidade dos registros
void insere_qtd_registro(int qtd, FILE *arq){
	rewind(arq);

	//pula o status
	fseek(arq, 1, SEEK_CUR);

	//o RRN será o mesmo da quantidade no casi 1
	fwrite(&qtd, sizeof(int), 1, arq);
	fwrite(&qtd, sizeof(int), 1, arq);
	//o resto continua o mesmo
}