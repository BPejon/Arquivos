

//Registro que guarda as informações
//128 bytes
//não colocar \0 - colocar apenas na leitura
typedef struct registro{
	int tamcidadeMae;		//tamanho do campo cidade mae 	//será usado para definir se está logicamante definido ou nao
	char cidadeMae[60];		//cidade da mae
	int tamcidadeBebe;		//tamanho do campo cidade bb	
	char cidadeBebe[60];	//cidade do bebe

	//Tamanhos fixos - começam no byteofset 105
	int idNascimento;		//id do nascimento
	int idadeMae;			//idade da mae
	char dataNascimento[10];//data de nascimento AAAA-MM-DD nulo \0 e lixo
	char sexoBebe;			//sigla pro sexo do bb utilizar '0'(nulo), '1', '2'
	char estadoMae;			//sigla estado da mae \0$
	char estadoBebe;		//sigla estado bb     \0$
}REGISTRO;


//Calcular rrn - somar 1- 0 é o cabeçalho
