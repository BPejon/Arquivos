//Registro de Cabeçalho, contém 128 bytes
typedef struct cabecalho{
	//nao precisa pra leitura
	//verificar se status nao é valido
	char status;					//armazena o status do arquivo
	int RRNproxRegistro;			//armazena o proximo rrn da pilha
	int numeroRegistrosInseridos;	//indica quantos registros foram inseridos (decrementa quando há arquivos logicamente removidos)
	int numeroRegistrosRemovidos;	//indica quantos registros foram logicamente removidos
	int numeroRegistrosAtualizados;	//indica registros atualizados
	//111 bytes de restos com lixo de $$
}CABECALHO;


void escrita_cabecalho(){


}

void leitura_cabecalho(CABECALHO cab){


}

//verificar se o arquivo está consistente
void status_0(){


}