#include "registro.h"
/*
	Arquivo que contém os códigos associados aos registros de dados utilizados no Trabalho de Orgazinação de Arquivos - 2020	

*/
//Registro que guarda as informações
//128 bytes
//não colocar \0 - colocar apenas na leitura
typedef struct registro{
	//Campo variavel nunca vem \0
	//maximo de 105 byteofset
	int tamcidadeMae;		//tamanho do campo cidade mae 	//será usado para definir se está logicamante definido ou nao
	char cidadeMae[60];		//cidade da mae
	int tamcidadeBebe;		//tamanho do campo cidade bb	
	char cidadeBebe[60];	//cidade do bebe

	//Tamanhos fixos - começam no byteofset 105
	int idNascimento;		//id do nascimento  										não tem nulo
	int idadeMae;			//idade da mae         										-1
	char dataNascimento[10];//data de nascimento 			AAAA-MM-DD 					\0$4 e lixo
	char sexoBebe;			//sigla pro sexo do bb utilizar '0', '1', '2'				0 = nulo
	char estadoMae[2];		//sigla estado da mae 										\0$
	char estadoBebe[2];		//sigla estado bb     										\0$
}REGISTRO;



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



//Printa as informações para conferir se estão corretas
void print_reg_teste(REGISTRO reg){
	//printf("%.*s", quantidade de bytes para printar, o que ira printar)
	printf("Tam_cid_mae = %d ---- cidade Mae = %.*s ---- idade Mae: %d\n", reg.tamcidadeMae, reg.tamcidadeMae, reg.cidadeMae, reg.idadeMae);
	printf("tamcidadeBebe = %d ---- cidade bebê  = %.*s\n", reg.tamcidadeBebe,reg.tamcidadeBebe, reg.cidadeBebe );
	printf("id nascimento = %d --- Data = %.*s ---- Sexo = %c\n", reg.idNascimento,10 , reg.dataNascimento, reg.sexoBebe);
	printf("estadoMae = %.*s--- Estado bb = %.*s\n", 2, reg.estadoMae, 2 , reg.estadoBebe);
	printf("Estado mae sozinho %s\n", reg.estadoMae );
	printf("-----------------------------------------------------------\n");

}

//Escreve o registro no arquivo
//Todos os erros serão tratados aqui, ou seja, não haverá mensagens de erro nos registros
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
	char barrazero = '\0';
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
	if(reg.dataNascimento[0] == '\0'){
		fwrite(&barrazero, sizeof(char), 1, arq); 	//escreve o \0
		for(int i =0; i<9 ; ++i)					//escreve o lixo restante
			fwrite(&lixo, sizeof(char), 1, arq);
	}else{
		fwrite(reg.dataNascimento, sizeof(char), 10, arq);
	}

	//Sexo bb
	fwrite(&reg.sexoBebe, sizeof(char), 1, arq);

	//Estado Mae
	if(reg.estadoMae[0] == '\0'){
		fwrite(&barrazero, sizeof(char), 1, arq);
		fwrite(&lixo, sizeof(char), 1, arq);
	}else{
		fwrite(reg.estadoMae, sizeof(char), 2, arq);
	}

	//Estado Filho
	if(reg.estadoBebe[0] == '\0'){
		fwrite(&barrazero, sizeof(char), 1, arq);
		fwrite(&lixo, sizeof(char), 1, arq);
	}else{
		fwrite(reg.estadoBebe, sizeof(char), 2, arq);
	}
}

//Lê os conteudos das linhas no arquivo .csv e os guarda na struct Registro, e em seguida salva em disco
//Retorna a quantidade de itens salvos
int le_entrada( FILE*ler, FILE *arq){

	//guardar os dados obtidos
	REGISTRO reg;

	int qtd = 0; //indica a quantidade de arquivos adcionados

	//ignora a primeira linha do arquivo e seu primeiro \n
	char lixostring[200];
	char barrazero = '\0';
	fgets(lixostring, 200, ler);

	//caracter para armazenar as vírgulas e \n nas linhas do arquivo .csv
	char lixo;

	//lê o arquivo
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
			//strcpy não funciona, pois copiará até o \0, deixando o lixo de lado
			//Salva um \0 na data de Nascimento, depois o lixo restante será tratado na hora de imprimir
			strcpy(reg.dataNascimento, &barrazero);
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
			strcpy(reg.estadoMae, &barrazero);
		}

		fscanf(ler,"%c", &lixo); //pega a vírgula

		//Salva o estado do bebe
		//como é o último, pega até o \n
		if((fscanf(ler,"%[^\n]", reg.estadoBebe)) == 0){
			strcpy(reg.estadoBebe, &barrazero);
		} 
		

		fscanf(ler,"%c", &lixo); //pega o \n

		escreve_reg_arq(reg, arq);
		++qtd;
	}

	return qtd;
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

	//saida para caso não existam registros
	if(existe_registro == 0)
		printf("Registro inexistente.\n");
	

}