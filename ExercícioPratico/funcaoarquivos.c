//Lê um arquivo e retorna uma uma string associada
void le_arquivo(FILE *arquivo){

	char string[500];
	int i =0;
	//lê byte a byte
	while(fread(auxiliar, sizeof(char),1, arquivo) != 0){
		string[i]= auxiliar
		++i;

	}





}

//Escreve no arquivo desejado uma mensagem com o tamanho desejado
void escreve_arquivo(FILE *arquivo, char* string, int tamanho){

	int i=0;
	while(fwrite( mensagem[i], sizeof(char), 1, arquivo) != 0){
	}
	return;
}