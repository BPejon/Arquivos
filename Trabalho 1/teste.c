#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

typedef struct registro{
	//Campo variavel nunca vem \0
	//maximo de 105 byteofset
	int tamcidadeMae;		//tamanho do campo cidade mae 	//será usado para definir se está logicamante definido ou nao
	char cidadeMae[70];		//cidade da mae
	int tamcidadeBebe;		//tamanho do campo cidade bb	
	char cidadeBebe[70];	//cidade do bebe

	//Tamanhos fixos - começam no byteofset 105
	int idNascimento;		//id do nascimento  										não tem nulo
	int idadeMae;			//idade da mae         										-1
	char dataNascimento[10];//data de nascimento 			AAAA-MM-DD 					\0$4 e lixo
	char sexoBebe;			//sigla pro sexo do bb utilizar '0', '1', '2'				0 = nulo
	char estadoMae[2];			//sigla estado da mae 										\0$
	char estadoBebe[2];		//sigla estado bb     										\0$

	//char erroDataNascimento[10] = '\0$$$$$$$$$';
}REGISTRO;


//Escreve o registro no arquivo
void escreve_reg_arq(REGISTRO reg, FILE *arq){
	//Escrita cidade mae
	fwrite(&reg.tamcidadeMae, sizeof(int), 1, arq);
	

	//Escrita cidade bebe
	fwrite(&reg.tamcidadeBebe, sizeof(int), 1, arq);

	//se cidade mae não for nulo escreve na memória
	if(reg.tamcidadeMae != 0)
		fwrite(reg.cidadeMae, sizeof(char), reg.tamcidadeMae, arq);
	if(reg.tamcidadeBebe != 0)
		fwrite(reg.cidadeBebe, sizeof(char), reg.tamcidadeBebe,arq);

	//Adicionar o lixo após os nomes das cidades
	//O total de bytes possivel de ser armazenado pelas duas cidades juntas é 97 bytes
	//Portanto os bytes de lixo serão 97 menos o total já escrito pelas duas cidades
	char lixo = '$';
	int bytes_faltantes = 97 - (reg.tamcidadeBebe + reg.tamcidadeMae);
	while(bytes_faltantes != 0){
		fwrite(&lixo, sizeof(char), 1, arq);
		--bytes_faltantes;	
	}


	//Id nascimento
	fwrite(&reg.idNascimento, sizeof(int), 1, arq);

	//Idade mae
	fwrite(&reg.idadeMae, sizeof(int), 1, arq);

	//Data de Nascimento
	fwrite(reg.dataNascimento, sizeof(char), 10, arq);

	//Sexo bb
	fwrite(&reg.sexoBebe, sizeof(char), 1, arq);

	//Estado Mae
	fwrite(reg.estadoMae, sizeof(char), 2, arq);

	//Estado Filho
	fwrite(reg.estadoBebe, sizeof(char), 2, arq);
}

//Lê os conteudos das linhas no arquivo .csv e os guarda na struct Registro, e em seguida salva em disco
//Retorna a quantidade de itens salvos
int le_entrada( const char *leitura, FILE *arq){
	FILE *ler;

	//abre o arquivo e verifica sua integridade
	if ((ler = fopen(leitura, "r")) == NULL){
		printf("Falha no carregamento do arquivo.\n");
		return -1;
	}

	//guardar os dados obtidos
	REGISTRO reg;

	int qtd = 0; //indica a quantidade de arquivos adcionados

	//ignora a primeira linha do arquivo e seu primeiro \n
	char lixostring[200];
	fgets(lixostring, 200, ler);
	char lixo;

	//lê o arquivo
	//ALTERAR PARA FSCANF O ARQUIVO .CSV
	char erroEstado[2] = {"\0$"};
	char erroDatadeNascimento[10] = {"\0$$$$$$$$$"};
	int i =0;
	int situacao;
	while((situacao = fscanf(ler,"%[^,]", reg.cidadeMae )) != EOF){

		//Salva a cidade da mae
		//se for vazio retornará 0
		if(situacao == 0 ){
			reg.tamcidadeMae = 0;
		}
		else{ //nao é vazio
			reg.tamcidadeMae = strlen(reg.cidadeMae);

		}

		fscanf(ler,"%c", &lixo); //pega a vírgula

		//Salva a cidade do bebe
		if((fscanf(ler,"%[^,]", reg.cidadeBebe)) == 0){
			reg.tamcidadeBebe = 0;
		}
		else{
			reg.tamcidadeBebe = strlen(reg.cidadeBebe);
		}

		fscanf(ler,"%c", &lixo); //pega a vírgula

		//Salva o id nascimento
		fscanf(ler,"%d", &reg.idNascimento);

		fscanf(ler,"%c", &lixo); //pega a vírgula

		//Salva idade mae
		if((fscanf(ler,"%d", &reg.idadeMae)) == 0 ){
			reg.idadeMae = -1;
		}else{
			//Não precisa fazer nada	
		}

		fscanf(ler,"%c", &lixo); //pega a vírgula

		//Salva a data de nascimento
		if((fscanf(ler,"%[^,]", reg.dataNascimento)) == 0){
			strcpy(reg.dataNascimento,erroDatadeNascimento);
		}else{
			//não muda nada
		}

		fscanf(ler,"%c", &lixo); //pega a vírgula

		//Salva o sexo bebe
		if( (fscanf(ler,"%[^,]", &reg.sexoBebe)) == 0){
			reg.sexoBebe = '0';
		}

		fscanf(ler,"%c", &lixo); //pega a vírgula

		//Salva o estado da Mae
		if((fscanf(ler,"%[^,]", reg.estadoMae)) == 0){
			strcpy(reg.estadoMae, erroEstado);
		}

		fscanf(ler,"%c", &lixo); //pega a vírgula

		//Salva o estado do bebe
		//como é o último, pega até o \n
		if((fscanf(ler,"%[^\n]", reg.estadoBebe)) == 0){
			strcpy(reg.estadoBebe, erroEstado);
		} 
		

		fscanf(ler,"%c", &lixo); //pega o \n
		++i;
		++qtd;
		escreve_reg_arq(reg, arq);
	}

	return qtd;
}


void erro_entrada( int argc){
	//caso n se insira nada
	if(argc< 2){
		printf("Falha no carregamento do arquivo.\n");
		exit(0);
	}

	//caso nao seja inserido um arquivo ou só inserido um comando
	if(argc< 3 ){
		printf("Falha no carregamento do arquivo.\n");
		exit(0);
	}
}

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

void status_valido(FILE *arq){
	char status = '1';
	rewind(arq);
	fwrite(&status, sizeof(char), 1, arq);
}

void le_cabecalho(FILE *arq){
	rewind(arq);

	CABECALHO cab;
	fread(&cab.status, sizeof(char), 1, arq);
	fread(&cab.RRNproxRegistro, sizeof(int), 1, arq);
	fread(&cab.numeroRegistrosInseridos, sizeof(int), 1, arq);
	fread(&cab.numeroRegistrosRemovidos, sizeof(int), 1, arq);
	fread(&cab.numeroRegistrosAtualizados, sizeof(int), 1, arq);

	//pula o lixo
	fseek(arq, 111, SEEK_CUR);
}

//Printa as informaçoes do bebe
void print_registro(REGISTRO reg){
	char sexo[10];
	char ignorado[] = "IGNORADO\0";
	char masculino[] = "MASCULINO\0";
	char feminino[] = "FEMININO\0";

	//printar o sexo do bebe
	if( reg.sexoBebe == '0'){
		strcpy(sexo, ignorado);
	}
	else if(reg.sexoBebe == '1'){
		strcpy(sexo, masculino);
	}
	else{
		strcpy(sexo, feminino);
	}

	/*Para printar sem adicionar /0 na memória utilizei o macete
	//printf("%.*s", quantidade de bytes para printar, o que ira printar)
	*/

	//Printar a cidade do bebe
	if(reg.tamcidadeBebe == 0){
		printf("Nasceu em -");
	}
	else
		printf("Nasceu em %.*s", reg.tamcidadeBebe, reg.cidadeBebe);

	//Ṕrintar o estado
	if(reg.estadoBebe[0] == '\0'){
		printf("/-, ");
	}else
		printf("/%.*s, ", 2, reg.estadoBebe);

	//printar a data de nascimento
	if(reg.dataNascimento[0] == '\0'){
		printf("em -, ");
	}else
		printf("em %.*s, ", 10, reg.dataNascimento);

	//Printar o sexo do bebe
	printf("um bebê de sexo %s.\n", sexo);
}

//ta bugado o print do estado
void print_reg_teste(REGISTRO reg){
	//printf("%.*s", quantidade de bytes para printar, o que ira printar)
	printf("Tam_cid_mae = %d ---- cidade Mae = %.*s ---- idade Mae: %d\n", reg.tamcidadeMae, reg.tamcidadeMae, reg.cidadeMae, reg.idadeMae);
	printf("tamcidadeBebe = %d ---- cidade bebê  = %.*s\n", reg.tamcidadeBebe,reg.tamcidadeBebe, reg.cidadeBebe );
	printf("id nascimento = %d --- Data = %.*s ---- Sexo = %c\n", reg.idNascimento,10 , reg.dataNascimento, reg.sexoBebe);
	printf("estadoMae = %.*s--- Estado bb = %.*s\n", 2, reg.estadoMae, 2 , reg.estadoBebe);
	printf("Estado mae sozinho %s\n", reg.estadoMae );
	printf("-----------------------------------------------------------\n");

}

void le_todos_registros(FILE *arq){
	REGISTRO reg;
	int qtd_lixo;
	int existe_registro = 0;

	//Lê até que não encontre mais o tamanho da cidade bebe
	while((fread(&reg.tamcidadeMae, sizeof(int), 1, arq) ) !=0 ) {
		fread(&reg.tamcidadeBebe, sizeof(int), 1, arq);
		fread(reg.cidadeMae, sizeof(char), reg.tamcidadeMae, arq);
		fread(reg.cidadeBebe, sizeof(char), reg.tamcidadeBebe, arq);

		qtd_lixo = 97 - (reg.tamcidadeMae + reg.tamcidadeBebe);
		//pular o lixo
		fseek(arq, qtd_lixo, SEEK_CUR);

		//Le id nascimento
		fread(&reg.idNascimento, sizeof(int), 1, arq);

		//Le idade mae
		fread(&reg.idadeMae, sizeof(int), 1, arq);

		//Le data nascimento
		fread(reg.dataNascimento, sizeof(char),10,arq);

		//le sexo bb
		fread(&reg.sexoBebe, sizeof(char), 1, arq);

		//le estado Mae
		fread(reg.estadoMae, sizeof(char), 2, arq);

		//le estado BB
		fread(reg.estadoBebe, sizeof(char), 2, arq);


		print_registro(reg);
		existe_registro = 1;
	}

	//saida para caso n existam registros
	if(existe_registro == 0)
		printf("Registro inexistente.\n");
	

}

void status_invalido(FILE *arq){
	char status = '0';
	rewind(arq);
	fwrite(&status, sizeof(char), 1, arq);
}

int main (int argc, char const *argv[]){
	//erro_entrada(argc);
	FILE *arq = fopen(argv[1], "rb");
	
	//falta testar
	status_invalido(arq);
	le_cabecalho(arq);
	le_todos_registros(arq);
	status_valido(arq);

	//CASO 1
	//if caso de erro aqui
	//	FILE *arq = fopen("testeeof.bin", "wb+");

	// inicializa_status(arq);
	// le_entrada(argv[1], arq);
	// status_valido(arq);

	//le entrada, passa o arquivo.csv e depois o arquivo binario a ser salvo
	fclose(arq);




	return 0;
}
// 	char string[100], string2[100];
// 	char virgula;
// 	int quantidade_string = 0;
// 	int quantidade_virgula= 0;
// 	char sexo, sexo2;
// 	char lixo;
// 	int dado, dado2;

// 	fscanf(ler,"%[^,]", &sexo);
// 	fscanf(ler,"%c", &lixo);

// 	dado = fscanf(ler,"%[^\n]", string);
// 	scanf("%c", &lixo);


// 	scanf("%[^,]", &sexo2);
// 	scanf("%c", &lixo);
// 	dado2 = scanf("%[^\n]", string2);
// 	scanf("%c", &lixo);


// 	printf("\n--------------\n");
// 	printf("%c\n", sexo );
// 	printf("%s\n", string);
// 	printf("dado = %d\n", dado );
// 	printf("%c\n", sexo2 );
// 	printf("%s\n", string2);
// 	printf("dado 2 = %d\n", dado2 );

// 	// if((scanf("%[^,]", string)) == 0){
// 	// 	printf("é igual a zero\n");

// 	// }
// 	// else {
// 	// 	printf("nao é igual a zero\n");
// 	// }

// 	// quantidade_virgula=  scanf("%c", &virgula);
// 	// //quantidade_string = scanf("%[^,]", string);


// 	int qtd = strlen(string);


// // 	printf("%s", string);	printf("\n");
// // 	printf("%c", virgula);	printf("\n");
// // 	printf("quantidade_string = %d\n", quantidade_string );
// // 	printf("quantidade_virgula = %d\n", quantidade_virgula );
// // 	printf("quantidade_string com strlen= %d\n",qtd );


// // 	quantidade_string = scanf("%[^,]", string);
// // 	quantidade_virgula=  scanf("%c", &virgula);
// // 	qtd = strlen(string);

// // 	printf("%s", string);	printf("\n");
// // 	printf("%c", virgula);	printf("\n");
// // 	printf("quantidade_string = %d\n", quantidade_string );
// // 	printf("quantidade_virgula = %d\n", quantidade_virgula );
// // 	printf("quantidade_string com strlen= %d\n",qtd );
// // /*
// // 	printf("%s", string);	printf("\n");
// // 	printf("quantidade_string = %d\n", quantidade_string );

// // */