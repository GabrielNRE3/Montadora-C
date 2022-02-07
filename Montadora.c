//EDITAR A FUNÇÃO GERENCIAR RESERVAS - A PARTE DE VENDAS, DEIXAR O NÚMERO DE REGISTRO NEGATIVO DOS CARROS USADOS.
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include <ctype.h>
//Início dos structs e unions

//struct - concessionaria.bin
typedef struct endereco {
char logradouro[80];
char bairro[15];
char CEP[10];
char cidade[15];
char estado[3];
char fone[12];
char email[40];
} endereco;

typedef struct infocarro {
char sigla;
int regcarro;
} infocarro;

typedef union dadoscarro {
char sigla;
infocarro reservado;
} dadoscarro;

typedef struct loja {
int regloja;
char nome[30];
char CNPJ[19];
endereco end;
int sold;
int reserved;
dadoscarro tabela[3];
} loja;

// struct - carro.bin
typedef struct infoloja {
    char sigla;
    char CNPJ[19];
} infoloja;
typedef union dadosloja {
    char sigla;
    infoloja reserva;
} dadosloja;
typedef struct montadora {
    int regcarro; 
    char modelo[20];
    char cor[10];
    float valor;
    dadosloja status;
} montadora;

typedef struct data {
int dia;
int mes;
int ano;
} data;

typedef struct historicoVendas {
int reghist;
int regcarro;
char modelo[20];
char cor[10];
float valor;
int regloja;
char nome[30];
char cnpj[19];
data dataVenda;
}historicoVendas;

// Fim da Lista de Struct

// Protótipos das Funções : As funções são chamadas diversas vezes em diversas partes do programa, logo não consegui organiza-las de um modo cronológico
void alocar(loja **p, int tam);         //Função Usada Para Alocar Memória Para o ponteiro do tipo loja
int checar();       //Determina a Quantidade de Dados do tipo loja guardados no arquivo concessionária.bin
void cadastrar(loja *p, int qtd);       //Recebe os dados que serão armazenados no ponteiro do tipo loja
int autenticacao(char *auxr,int qtd,int k);			//Função usada para impedir dados iguais de serem registrados
void maiuscula(char *p,int tam);        //Transforma um string em letras maíusculas
int verificar(loja *p,int qtd);        //Exibição dos dados gravados no arquivo concessionária.bin levando em conta apenas seu CNPJ e exibe tabelas apenas se sua sigla for 'R'
void alocar_carro(montadora **p, int tam);      //Função Usada Para Alocar Memória Para o ponteiro do tipo montadora
int checar_carro();         //Determina a Quantidade de Dados do tipo montadora guardados no arquivo carro.bin
void registrar(montadora *p, int qtd);      //Recebe os dados que serão armazenados no ponteiro do tipo montadora
void mostrar_geral(montadora *p,int qtd);       //Exibição do dados gravados no arquivo carro.bin
void mostrar_status(montadora *p,int qtd);      //Exibição do dados gravados no arquivo carro.bin levando em conta seu status
void mostrar_modelo(montadora *p, int qtd);         //Exibição do dados gravados no arquivo carro.bin levando em conta seu modelo
void reservar(loja *pl,montadora *pm,int qtd,int qtd_carro);        //Usando o CNPJ da concessionária e o número de registro do carro é linkado ambos os arquivos alterando alguns dados de ambos, realizando a reserva
void atualizar_loja(loja *p,int aux,char *modo);       //Grava no Arquivo as mudanças nos dados no arquivo concessionária.bin, ou grava novos dados. Dependendo do valor de "modo"
void atualizar_montadora(montadora *p,int aux,char *modo);         //Grava no Arquivo as mudanças nos dados no arquivo carro.bin, ou grava novos dados. Dependendo do valor de "modo"
void atualizar_historico(historicoVendas *p,int aux,char *modo); // Grava no arquivo historico.bin os dados que o ponteiro do tipo historicoVendas aponta.
void mostrar_reservas(montadora *p,int qtd);        //Exibição dos dados que estão em reserva
void gerenciar_reservas(loja *pl,montadora *pm,historicoVendas *ph,int qtd_historico,int qtd,int qtd_carro);      //Permite realizar a operação de liberar ou comprar
void alocar_historico(historicoVendas **p, int tam);		//Reserva memória para o ponteiro do tipo historicoVendas.
int checar_historico(); 		// Encontra o número de dados gravados no histórico.
void historico(loja *pl,montadora *pm,historicoVendas *ph,int auxl,int auxm,int qtd_historico,int valor,char *modelo,char *cor); 		// O ponteiro adquire os valores que serão gravados
void mostrar_hmodelo(historicoVendas *p,int qtd);		 // Mostra os vendidos baseado no modelo.
void mostrar_hcnpj(historicoVendas *p,int qtd);			//Mostra os vendidos baseado no cnpj da concessionária.
void alterar_concessionaria(loja *pl,montadora *pm,historicoVendas *ph,int qtd,int qtd_carro);		//Altera os dados da concessionária, alterando em TODOS os lugares possíveis.
void alterar_carro(montadora *pm,historicoVendas *ph,int qtd_carro);		//Altera os dados do carro, atenando em TODOS os lugares possíveis.

// Fim dos Protótipos das Funções

int main() //As quatro funções chamadas antes do "do" são realizadas para evitar a chamada das mesmas durante várias funções
{// Antes dos menus, são chamadas funções e utilizado de variáveis e ponteiros para alocar a memória e identificar a quantia de itens dentro dos arquivos 
    loja *pl=NULL;
    montadora *pm=NULL;
    historicoVendas *ph=NULL;
    char sn;
    int qtd=0, es,esc,esr,esh,ese,qtd_carro=0,qtd_historico=0,aux;
    setlocale(LC_ALL,"Portuguese");
    alocar(&pl,1);
    alocar_carro(&pm,1);
    alocar_historico(&ph,1);
    qtd = checar();
    qtd_carro = checar_carro();
    qtd_historico = checar_historico();
    menu: // O menu principal está contido dentro de um ciclo do while, porém os "sub menus" estão fazendo uso de goto. O motivo disto é apenas a organização que achei mais fácil de identificar. Os goto fora do menu também são formas de organizar idéias que achei melhor, assim como a abertura dos arquivos no início da função
    do{
        system("cls");
		printf("Escolha uma opção abaixo: \n\n[1]Cadastrar Concessionária\n[2]Acesso de Concessionárias\n[3]Registrar Novo Carro\n[4]Acesso ao Acervo de Carros\n[5]Reservas\n[6]Verificar Histórico\n[7]Alterar Registros\n[8]Encerrar\n\n");
		scanf("%i",&es);
        fflush(stdin);
        switch(es)
		{
			case 1:
            do{
            	sn='s';
                if(qtd <5)
                {
	                cadastrar(pl,qtd+1);
	                qtd=checar();
	                printf("\nDeseja cadastrar mais uma concessionária ? <S/N> ");
	                scanf("%c",&sn);
	                fflush(stdin);
                }
                else
                {
                    printf("\nNão é possível cadastrar mais concessionárias \n");
                    system("pause");
                    sn = 'n';
                }         
            }while(sn!='n' && sn!= 'N');
            break;

            case 2:
            aux=verificar(pl,qtd);
            break;

            case 3:
            do{
            	sn='s';         
                if(qtd_carro <50)
                {
	                registrar(pm,qtd_carro+1);
                    atualizar_montadora(pm,0,"ab");
	                qtd_carro++;
	                printf("\nDeseja registrar mais carros? <S/N> ");
	                scanf("%c",&sn);
	                fflush(stdin);
                }
                else
                {
                    printf("\nNão é possível adicionar mais carros \n");
                    system("pause");
                    sn = 'n';
		        }
            }while(sn!='n' && sn!= 'N');
            break;

            case 4:
            menuc: 
            system("cls");
            printf("[1]Total\n[2]Parcial: Livre ou Reservado\n[3]Parcial: Modelo\n[4]Retornar ao Menu Principal\n");
            scanf("%i",&esc);
            fflush(stdin);
            switch(esc)
            {
            	case 1:
				mostrar_geral(pm,qtd_carro);
				goto menuc;
				break;
				
				case 2:
	            mostrar_status(pm,qtd_carro);
	            goto menuc;
	            break;
	            
	            case 3:
	            mostrar_modelo(pm,qtd_carro);
	            goto menuc;
    	        break;
    	        
    	        default:
    	        goto menu;
			}
            break;

            case 5:
			menur:
            system("cls");
            printf("[1]Acervo de Carros Total\n[2]Verificar Reservas\n[3]Reservar Novo Carro\n[4]Gerenciar Reservas\n[5]Retornar ao Menu Principal\n");
			scanf("%i",&esr);
        	fflush(stdin);
            switch(esr)
			{
                case 1:
                mostrar_geral(pm,qtd_carro);
                goto menur;
                break;
                
                case 2:
                mostrar_reservas(pm,qtd_carro);
                goto menur;
                break;
                
                case 3:
                reservar(pl,pm,qtd,qtd_carro);
                goto menur;
                break;
                
                case 4:
                gerenciar_reservas(pl,pm,ph,qtd_historico,qtd,qtd_carro);
                goto menur;
				break;
				default:
				goto menu;
				break;
            }
            break;

            case 6:
            menuh:
            qtd_historico = checar_historico();
            system("cls");
            printf("[1]Consulta por CNPJ\n[2]Consulta por Modelo\n[3]Retornar ao Menu Principal\n");
			scanf("%i",&esh);
        	fflush(stdin);
        	switch(esh)
        	{
        		case 1:
        		mostrar_hcnpj(ph,qtd_historico);
        		goto menuh;
        		break;
        		
        		case 2:
        		mostrar_hmodelo(ph,qtd_historico);
        		goto menuh;
        		break;
        		
        		default:
				goto menu;
				break;
			}
			break;

            case 7:
			menue:
            system("cls");
            printf("Modificar: \n\n[1]Concessionária\n[2]Carro\n[3]Retornar ao Menu Principal\n");
			scanf("%i",&ese);
        	fflush(stdin);
        	switch(ese)
        	{
        		case 1:
        		alterar_concessionaria(pl,pm,ph,qtd,qtd_carro);

        		goto menue;
        		break;
        		
        		case 2:
				alterar_carro(pm,ph,qtd_carro);
        		goto menue;
        		break;
        		
        		default:
				goto menu;
				break;
			}
			break;
    	}
    }while(es<8); // Fim do Menu
}//Fim da Main

// Conteúdo Das Funções

void alocar(loja **p, int tam)
{
    if((*p=(loja*)realloc(*p, tam*sizeof(loja))) == NULL)
    {
        printf("Erro");
        exit(1);
    }
}

int checar()
{
    FILE *fp = NULL;
    long int cont = 0;
    if((fp=fopen("concessionaria.bin","rb"))==NULL)
        return cont;
    else
    {
  	    fseek(fp,0,2);   
  	    cont=ftell(fp)/sizeof(loja);
  	    fclose(fp);
        return cont;     
    }
}

void cadastrar(loja *p, int qtd) // Alguns dados impedem de serem duplicados pois isso implicaria em conflito ao comparar os dados aos obtidos nos arquivos. Note que os únicos campos que isso se aplica é onde é impossível dois campos serem iguais
{// da valor ao ponteiro do tipo loja que será gravado no arquivo pela função atualizar_concessionaria
	int i, tamn=0, taml=0,tamb=0,tamc=0,tame=0,taem=0,tamCNPJ=0,tamf=0,cont=0;
    char auxn[30], auxl[80], auxb[15], auxc[15], auxe[3], auxem[40],auxCNPJ[19],auxf[12];
	    printf("\nRegistro: %i",qtd);
    	printf("\nNome da concessionária: ");
	    gets(auxn);
    	fflush(stdin);
    	tamn+=strlen(auxn)+1;
    	maiuscula(auxn,tamn);
	    cont = autenticacao(auxn,qtd,0);
    	if(cont==1)
	    {
		    strcpy(p->nome, auxn);
    	}
   		else
        	goto fim;
    	printf("\nCNPJ da concessionária: ");
    	gets(auxCNPJ);
    	fflush(stdin);
    	tamCNPJ+=strlen(auxCNPJ)+1;
    	maiuscula(auxCNPJ,tamCNPJ);
    	cont = autenticacao(auxCNPJ,qtd,1);
    	if(cont==1)
	    {
		    strcpy(p->CNPJ, auxCNPJ);
    	}
   		else
        	goto fim;
    	printf("\nInsira os endereços requisitados abaixo: ");
    	printf("\nLogradouro:");
    	gets(auxl);
    	fflush(stdin);
    	taml+=strlen(auxl)+1;
    	strcpy(p->end.logradouro, auxl);
    	maiuscula(p->end.logradouro,taml);
    	printf("\nBairro:");
    	gets(auxb);
    	fflush(stdin);
    	tamb+=strlen(auxb)+1;
    	strcpy(p->end.bairro, auxb);
    	maiuscula(p->end.bairro,tamb);
    	printf("\nCEP:");
    	gets(p->end.CEP);
    	fflush(stdin);
    	printf("\nCidade:");
    	gets(auxc);
    	fflush(stdin);
    	tamc+=strlen(auxc)+1;
    	strcpy(p->end.cidade, auxc);
    	maiuscula(p->end.cidade,tamc);
    	printf("\nEstado:");
    	gets(auxe);
    	fflush(stdin);
    	tame+=strlen(auxe)+1;
	    strcpy(p->end.estado, auxe);
    	maiuscula(p->end.estado,tame);
	    printf("\nTelefone:");
    	gets(auxf);
    	fflush(stdin);
    	cont = autenticacao(auxf,qtd,2);
    	if(cont==1)
	    {
		    strcpy(p->end.fone, auxf);
    	}
   		else
        	goto fim;
    	printf("\nEmail:");
    	gets(auxem);
    	fflush(stdin);
    	tame+=strlen(auxem)+1;
    	maiuscula(auxem,tame);
    	cont = autenticacao(auxem,qtd,3);
    	if(cont==1)
	    {
		    strcpy(p->end.email, auxem);
    	}
   		else
        	goto fim;
    	p->regloja=qtd;
	    p->sold=0;
    	p->reserved=0;
	    for(i=0;i<3;i++)
		{
    		(p->tabela+i)->sigla='L';
    	}
		system("cls");
		printf("Concessionária cadastrada com sucesso\n");
    	atualizar_loja(p,0,"ab");
    	if(qtd==10) // O goto utilizado quando é encontrado um erro está dentro de uma situação impossível, logo somente através do goto é possível acessar esta área. O goto além de receber preferência por organização, redução do tamanho do código, ele também é o modo mais simples de dar um feedback para o usuário sobre seu erro.
		{
        	fim:
        	system("cls");
	        printf("Não é possível cadastrar a concessionária, dados já utilizados\n");
    	    system("pause");
    	}
    
}

int autenticacao(char *auxr,int qtd,int k) // A função por si só não impede os dados de serem registrados, mas dá a condicional para realizar isso na função cadastrar
{
    loja *aux=NULL;
    int cont=0, i=0;
    FILE *fp=NULL;
    alocar(&aux,1);
    if((fp=fopen("concessionaria.bin","rb"))==NULL)// Autenticação é uma função que interage exclusivamente com a struct loja. Pois somente nela há dados que o usuário pode editar que não podem ser duplicados no programa
        cont=1;
    else
    {// É utilizado a variável k como auxiliar que determina qual campo está sendo autenticado. Seu valor é definido no momento da chamada da função, seu valor é definido manualmente
		if(k==0)
		{	
        	do{
            	fseek(fp,i*sizeof(loja),0);
		  	    fread(aux,sizeof(loja),1,fp);
    	    	i++;
        	  	if(i==qtd)
	  	            break;
      		}while(strcmp(aux->nome,auxr)!=0);
    	if(strcmp(aux->nome,auxr)==0)
        	cont=0;
    	if(i==qtd)
    		cont=1;
    	}
		if(k==1)
		{	
        	do{
            	fseek(fp,i*sizeof(loja),0);
		  	    fread(aux,sizeof(loja),1,fp);
    	      	i++;
          		if(i==qtd)
	              	break;
      		}while(strcmp(aux->CNPJ,auxr)!=0);
    	if(strcmp(aux->CNPJ,auxr)==0)
        	cont=0;
    	if(i==qtd)
    		cont=1;
    	}
		if(k==2)
		{	
        	do{
          	    fseek(fp,i*sizeof(loja),0);
	  	        fread(aux,sizeof(loja),1,fp);
        	  	i++;
       	  		if(i==qtd)
	            	break;
      		}while(strcmp(aux->end.fone,auxr)!=0);
    	if(strcmp(aux->end.fone,auxr)==0)
        	cont=0;
    	if(i==qtd)
    		cont=1;
    	}
		if(k==3)
		{	
        	do{
            	fseek(fp,i*sizeof(loja),0);
		  	    fread(aux,sizeof(loja),1,fp);
    	   	  	i++;
       	  		if(i==qtd)
	            	break;
      		}while(strcmp(aux->end.email,auxr)!=0);
    	if(strcmp(aux->end.email,auxr)==0)
        	cont=0;
    	if(i==qtd)
    		cont=1;
    	}
    	fclose(fp);
	}
	return cont;
}

void maiuscula(char *p,int tam) //É necessário deixar as letras maiúsculas para casos de comparação de dados dentro do programa. Para manter consistência foi aplicado a todos os dados que entram nos arquivos .bin
{
	int i;
	for(i=0;i<tam;i++)
  		if(*(p+i)>='a' && *(p+i)<='z')
    		*(p+i)-=32;
}

int verificar(loja *p,int qtd) // Esta função é do tipo int somente para retornar um valor que é utilizado na função alterar_concessionária, nos demais casos que é usada este valor não implica nada
{
    char cnpj[19];
    int i,cont=-1;
    FILE *fp=NULL;
    system("cls");
    printf("Insira o CNPJ\n");
    gets(cnpj);
    fflush(stdin);
    if((fp=fopen("concessionaria.bin","rb"))==NULL)
        printf("\nErro ao abrir o arquivo");
    else
    {
  	    for(i=0;i<qtd;)
  	    {
            do{
                fseek(fp,i*sizeof(loja),0);
	  	        fread(p,sizeof(loja),1,fp);
	  	        i++;
	  	        if(i==qtd)
	  	            break;
            }while(strcmp(p->CNPJ,cnpj)!=0);
            if(strcmp(p->CNPJ,cnpj)==0)
            {
          	    system("cls"); // Abaixo estão todas as possibilidades relacionado as tabelas, assim são apenas exibidos a quantia exata de tabelas baseado na quantia de reservados. Note que o número de reserva das tabelas NÃO REFLETEM o espaço do arquivo que ocupam, por exemplo, se a tabela [0] e tabela [2] estarem reservados, o programa irá exibir "Reserva 1" e "Reserva 2", ao invés de "Reserva 1" e "Reserva 3"
                if(p->tabela[0].reservado.sigla=='R' && p->tabela[1].reservado.sigla=='R' && p->tabela[2].reservado.sigla=='R')
                    printf("Nome: %s\nVendidos: %i\nReservados: %i\nReserva 1:%c - %i\nReserva 2:%c - %i\nReserva 3:%c - %i\n",p->nome, p->sold, p->reserved, (p->tabela)->reservado.sigla, (p->tabela)->reservado.regcarro, (p->tabela+1)->reservado.sigla, (p->tabela+1)->reservado.regcarro, (p->tabela+2)->reservado.sigla, (p->tabela+2)->reservado.regcarro);
				if(p->tabela[0].reservado.sigla=='R' && p->tabela[1].reservado.sigla=='R' && p->tabela[2].reservado.sigla!='R')
                    printf("Nome: %s\nVendidos: %i\nReservados: %i\nReserva 1:%c - %i\nReserva 2:%c - %i\n",p->nome, p->sold, p->reserved, (p->tabela)->reservado.sigla, (p->tabela)->reservado.regcarro, (p->tabela+1)->reservado.sigla, (p->tabela+1)->reservado.regcarro);
				if(p->tabela[0].reservado.sigla=='R' && p->tabela[1].reservado.sigla!='R' && p->tabela[2].reservado.sigla!='R')
                    printf("Nome: %s\nVendidos: %i\nReservados: %i\nReserva 1:%c - %i\n",p->nome, p->sold, p->reserved, (p->tabela)->reservado.sigla, (p->tabela)->reservado.regcarro);
				if(p->tabela[0].reservado.sigla=='R' && p->tabela[1].reservado.sigla!='R' && p->tabela[2].reservado.sigla=='R')
                    printf("Nome: %s\nVendidos: %i\nReservados: %i\nReserva 1:%c - %i\nReserva 2:%c - %i\n",p->nome, p->sold, p->reserved, (p->tabela)->reservado.sigla, (p->tabela)->reservado.regcarro, (p->tabela+2)->reservado.sigla, (p->tabela+2)->reservado.regcarro);				
				if(p->tabela[0].reservado.sigla!='R' && p->tabela[1].reservado.sigla=='R' && p->tabela[2].reservado.sigla!='R')
                    printf("Nome: %s\nVendidos: %i\nReservados: %i\nReserva 1:%c - %i\nReserva 2:%c - %i\n",p->nome, p->sold, p->reserved, (p->tabela+1)->reservado.sigla, (p->tabela+1)->reservado.regcarro, (p->tabela+2)->reservado.sigla, (p->tabela+2)->reservado.regcarro);
				if(p->tabela[0].reservado.sigla!='R' && p->tabela[1].reservado.sigla=='R' && p->tabela[2].reservado.sigla!='R')
                    printf("Nome: %s\nVendidos: %i\nReservados: %i\nReserva 1:%c - %i\n",p->nome, p->sold, p->reserved, (p->tabela+1)->reservado.sigla, (p->tabela+1)->reservado.regcarro);
				if(p->tabela[0].reservado.sigla!='R' && p->tabela[1].reservado.sigla!='R' && p->tabela[2].reservado.sigla=='R')
                    printf("Nome: %s\nVendidos: %i\nReservados: %i\nReserva 1:%c - %i\n",p->nome, p->sold, p->reserved, (p->tabela+2)->reservado.sigla, (p->tabela+2)->reservado.regcarro);
				if(p->tabela[0].reservado.sigla!='R' && p->tabela[1].reservado.sigla!='R' && (p->tabela+2)->reservado.sigla!='R')
                    printf("Nome: %s\nVendidos: %i\nReservados: %i\n",p->nome, p->sold, p->reserved);
				cont = i-1; // O Valor retornado caso o CNPJ seja encontrado, se trata do valor que multiplica o sizeof na parte do offset da função fseek
			}
            break;
        }
        if(strcmp(p->CNPJ,cnpj)!=0)
        	printf("\nNão há nenhuma concessionária com este CNPJ cadastrada\n");
        fclose(fp);
        system("pause");
        return cont;
    }
}

void alocar_carro(montadora **p, int tam)
{
    if((*p=(montadora*)realloc(*p, tam*sizeof(montadora))) == NULL)
    {
        printf("Erro");
        exit(1);
    }
}

int checar_carro()
{
    FILE *fp = NULL;
    long int cont2 = 0;
    if((fp=fopen("carro.bin","rb"))==NULL)
        return cont2;
    else
    {
  	    fseek(fp,0,2);   
  	    cont2=ftell(fp)/sizeof(montadora);
  	    fclose(fp);     
    }
    return cont2;
}

void registrar(montadora *p, int qtd) // Não há necessidade do uso da função autenticação porque todos os dados que o usuário pode inserir não necessitam ser únicos
{//Dá valor ao ponteiro do tipo montadora, que então é salvo pela função atualizar_montadora.
	int i,tamm=0,tamc=0;
	char auxm[20],auxc[10];
    p->regcarro=qtd;
    printf("\nRegistro: %i",p->regcarro);
    printf("\nModelo do carro: ");
    gets(auxm);
    fflush(stdin);
    tamm+=strlen(auxm)+1;
    strcpy(p->modelo, auxm);
    maiuscula(p->modelo,tamm);
    printf("\nCor: ");
    gets(auxc);
    fflush(stdin);
    tamc+=strlen(auxc)+1;
    strcpy(p->cor, auxc);
    maiuscula(p->cor,tamc);
    printf("\nValor: ");
    scanf("%f", &p->valor);
    fflush(stdin);
    p->status.sigla = 'L';
	system("cls");
	printf("Carro adicionado ao acervo com sucesso\n");
	
}

void mostrar_geral(montadora *p,int qtd)
{
    int i;
    FILE *fp=NULL;
    system("cls");
    if((fp=fopen("carro.bin","rb"))==NULL)
    {
        printf("\nErro ao abrir o arquivo");
        system("pause");
    }
    else
    {	
  		for(i=0;i<qtd;)
  	  	{ 
	  		fseek(fp,i*sizeof(montadora),0);
	  		fread(p,sizeof(montadora),1,fp);
			if(p->regcarro>0)
			{
				if(p->status.sigla=='L')   
        			printf("\nNúmero de Registro: %i\nModelo: %s\nCor: %s\nValor %.2f\nStatus: %c\n",p->regcarro, p->modelo, p->cor ,p->valor, p->status.sigla);
     		   	else
        			printf("\nNúmero de Registro: %i\nModelo: %s\nCor: %s\nValor %.2f\nStatus: %c - %s\n",p->regcarro, p->modelo, p->cor ,p->valor, p->status.reserva.sigla, p->status.reserva.CNPJ);
			}
			i++;
		}
	    fclose(fp);
    }
	system("pause");
}

void mostrar_status(montadora *p,int qtd)
{
    int i;
    char aux;
    FILE *fp=NULL;
    system("cls");
    if((fp=fopen("carro.bin","rb"))==NULL)
        printf("\nErro ao abrir o arquivo");
    else
    {
        printf("Checar acervo de carros livres [L] ou reservados [R] ? \n");
        scanf("%c", &aux);
        fflush(stdin);
        aux = toupper(aux);
        switch(aux)
        {
            case 'L':
  	            for(i=0;i<qtd;i++)
  	            {
					fseek(fp,i*sizeof(montadora),0);
	  	            fread(p,sizeof(montadora),1,fp);
					if(p->regcarro>0)
					{
		            	if(aux==p->status.sigla)
                        	printf("\nNúmero de Registro: %i\nModelo: %s\nCor: %s\nValor %.2f\nStatus: %c\n",p->regcarro, p->modelo, p->cor ,p->valor, p->status.sigla);
	            	}
				}
                fclose(fp);
                break;

	        case 'R':
                
  	            for(i=0;i<qtd;i++)
	            {
					fseek(fp,i*sizeof(montadora),0);
	  	            fread(p,sizeof(montadora),1,fp);
					if(p->regcarro>0)
					{
		            	if(aux==p->status.reserva.sigla)
                        	printf("\nNúmero de Registro: %i\nModelo: %s\nCor: %s\nValor %.2f\nStatus: %c - %s\n",p->regcarro, p->modelo, p->cor ,p->valor, p->status.reserva.sigla, p->status.reserva.CNPJ);
	            	}
				}        
		        fclose(fp);
                break;

                default:
                break;
        }
        system("pause");
    }
}

void mostrar_modelo(montadora *p,int qtd)
{
    int i,tam=0;
    char aux[20];
    FILE *fp=NULL;
    system("cls");
    if((fp=fopen("carro.bin","rb"))==NULL)
        printf("\nErro ao abrir o arquivo");
    else
    {
        printf("Insira o modelo do carro: \n");
        gets(aux);
        fflush(stdin);
    	tam+=strlen(aux)+1;
    	maiuscula(aux,tam);
  	    for(i=0;i<qtd;i++)
  	    {
			fseek(fp, i*sizeof(montadora),0);
	  	    fread(p,sizeof(montadora),1,fp);
			if(p->regcarro>0)
			{
            	if(strcmp(aux,p->modelo)==0)
             	   printf("\nNúmero de Registro: %i\nModelo: %s\nCor: %s\nValor %.2f\nStatus: %c\n",p->regcarro, p->modelo, p->cor ,p->valor, p->status.sigla);
			}
		}
    	system("pause");
        fclose(fp);
    }
}

void reservar(loja *pl,montadora *pm,int qtd,int qtd_carro) // Foi feito um outro menu apenas para não sobrecarregar o menu principal
{
	char sn,cnpj[19];
    int i=0,j=0,aux=0,aux_nr=0,menu=0,k=0;
    FILE *fpl=NULL;
    FILE *fpm=NULL;
    system("cls");
    printf("Deseja ver o acervo de carros?[s/n] \n");
    scanf("%c",&sn);
    fflush(stdin);
    system("cls");
	switch(sn)
	{
		case 'S':
		case 's':
		printf("Modo de Acesso: \n[1]Status\n[2]Modelo\n[3]Geral\n");
    	scanf("%i",&menu);
    	switch(menu)
		{ // Este menu possibilita o usuário a ver o acervo de carros de 3 modos. Por status, modelo ou todos.
    		case 1:
	    	fflush(stdin);
    		mostrar_status(pm,qtd_carro);
    		break;
    	
	    	case 2:
    		fflush(stdin);
    		mostrar_modelo(pm,qtd_carro);
	    	break;
    	
    		case 3:
    		fflush(stdin);
	    	mostrar_geral(pm,qtd_carro);	
    		break;
    	
			default:
			printf("Acesso ao acervo cancelado\n");
			system("pause");
			break;
		}
		break;

		default:
		break;
	}
    printf("Insira o CNPJ da concessionária que fará a reserva: \n");
    gets(cnpj);
    fflush(stdin);
    if((fpl=fopen("concessionaria.bin","rb"))==NULL)
        printf("\nErro ao abrir o arquivo");
    else
    {
    	if((fpm=fopen("carro.bin","rb"))==NULL)
        	printf("\nErro ao abrir o arquivo");
        do{// Como o CNPJ é único, é possível usar um ciclo do while para posicionar o ponteiro no arquivo na posição correta. 
            fseek(fpl,i*sizeof(loja),0);
	  	    fread(pl,sizeof(loja),1,fpl);
	  	    i++;
	  	    if(i==qtd)// Caso o arquivo seja lido por completo e não seja encontrado o CNPJ desejado, o ciclo é quebrado
	  	        break;
        }while(strcmp(pl->CNPJ,cnpj)!=0);
    	if(strcmp(pl->CNPJ,cnpj)==0) // Se o CNPJ for encontrado a função continua sua execução
    		if(pl->reserved<3)
	    	{
	    		system("cls");
	    		printf("Insira o número de registro do carro que deseja reservar\n");
		    	scanf("%i", &aux_nr);
	   		 	fflush(stdin);
	        	do{// Tme o mesmo propósito do ciclo acima, exceto que o ponteiro que é deslocado é o da montadora e não o da loja, e que o ciclo irá se repetir até achar o número de registro desejado
    	    	    fseek(fpm,j*sizeof(montadora),0);
		  		    fread(pm,sizeof(montadora),1,fpm);
	  			    if(j==qtd_carro)// Caso o arquivo seja completamente lido e o número de registro não seja encontrado, o ciclo é quebrado
		  		        break;
		  		    j++;
    	  	  	}while(pm->regcarro!=aux_nr);
		      	if(pm->regcarro!=aux_nr)
       			{
        			system("cls");
    				printf("Não há nenhum carro com o número de registro inserido \n");
				}
				if(pm->regcarro>0) // Esta verificação impede que carros com número de registro negativo (Vendidos) possam ser reservados
				{
					if(pm->regcarro==aux_nr) // Caso o número de registro digitado seja encontrado, o programa continua sua execução
					{
						if(pm->status.sigla=='L')
						{
							for(k=0;k<3;)
							{
								if(pl->tabela[k].reservado.sigla!='R')
								{
									(pl->tabela+k)->reservado.sigla='R';
									(pl->tabela+k)->reservado.regcarro=aux_nr;
									pl->reserved+=1;
									pm->status.reserva.sigla='R';
									strcpy(pm->status.reserva.CNPJ,cnpj);
									atualizar_loja(pl,i-1,"rb+");
									atualizar_montadora(pm,j-1,"rb+");
									printf("\nReserva concluída\n");
									break;
								}
								k++;				
							}
						}
						else // Caso o número já esteja reservado será exibido uma mensagem impedindo a reserva
						{
							system("cls");
							printf("O carro que deseja reservar já está reservado\n");
						}
					}
				}
				else // No caso de número de registro negativo será exibido esta mensagem
				{
					system("cls");
					printf("Não há nenhum carro com o número de registro inserido \n");
				}

				}
			}
		if(strcmp(pl->CNPJ,cnpj)!=0)// Caso o CNPJ não seja encontrado a mensagem abaixo será exibida
		{
			system("cls");
			printf("\nConcessionária com o CNPJ: '%s' não registrada \n",cnpj);
		}
		system("pause");
		fclose(fpl);
		fclose(fpm);
}
// As três funções abaixo funcionam do mesmo modo, apenas são usadas para ponteiros diferentes.
// Caso seja passado o modo "rb+" o programa desloca o ponteiro para a posição inserida e atualiza os dados.
// Caso seja passado o modo "ab" o programa apenas salva um novo dado no fim do arquivo.
void atualizar_loja(loja *p,int aux,char *modo)
{
    FILE *fp=NULL;
    if((fp=fopen("concessionaria.bin",modo))==NULL)
        printf("\nErro ao abrir o arquivo");
    else
	{
  		if(strcmp(modo,"rb+")==0) 
  			fseek(fp,aux*sizeof(loja),0);
  		fwrite(p,sizeof(loja),1,fp); 
  	}
    fclose(fp);
}

void atualizar_montadora(montadora *p,int aux,char *modo)
{
    FILE *fp=NULL;
    if((fp=fopen("carro.bin",modo))==NULL)
        printf("\nErro ao abrir o arquivo");
    else
	{
  		if(strcmp(modo,"rb+")==0)
  			fseek(fp,aux*sizeof(montadora),0);
  		fwrite(p,sizeof(montadora),1,fp);
  	}
    fclose(fp);
}

void atualizar_historico(historicoVendas *p,int aux,char *modo)
{
    FILE *fp=NULL;
    if((fp=fopen("historico.bin",modo))==NULL)
        printf("\nErro ao abrir o arquivo");
    else
	{
  		if(strcmp(modo,"rb+")==0)
  			fseek(fp,aux*sizeof(historicoVendas),0);
  		fwrite(p,sizeof(historicoVendas),1,fp);
  	}
    fclose(fp);
}

void mostrar_reservas(montadora *p,int qtd)
{
    int i;
    FILE *fp=NULL;
    system("cls");
    if((fp=fopen("carro.bin","rb"))==NULL)
    {
        printf("\nErro ao abrir o arquivo");
        system("pause");
    }
    else
    {	
  		for(i=0;i<qtd;)
  	  	{ 
	  		fseek(fp,i*sizeof(montadora),0);
	  		fread(p,sizeof(montadora),1,fp);
			if(p->status.reserva.sigla=='R')   
        		printf("\nNúmero de Registro: %i\nModelo: %s\nCor: %s\nValor %.2f\nStatus: %c - %s\n",p->regcarro, p->modelo, p->cor ,p->valor, p->status.reserva.sigla, p->status.reserva.CNPJ);
			i++;
		}
	    fclose(fp);
    }
	system("pause");	
}

void gerenciar_reservas(loja *pl,montadora *pm,historicoVendas *ph,int qtd_historico,int qtd,int qtd_carro)
{// Os ciclos do while movem o ponteiro, assim como já realizado em funções anteriores.
    int i=0,j=0,aux=0,aux_nr=0, es=0,k,valor=0;
    char modelo[20],cor[10];
    FILE *fpl=NULL;
    FILE *fpm=NULL;
    system("cls");
    mostrar_reservas(pm,qtd_carro);
    printf("\nInsira o número de registro do carro que deseja gerenciar \n");
    scanf("%i", &aux_nr);
    fflush(stdin);
    if((fpl=fopen("concessionaria.bin","rb"))==NULL)
        printf("\nErro ao abrir o arquivo");
    else
    {
    	if((fpm=fopen("carro.bin","rb"))==NULL)
        	printf("\nErro ao abrir o arquivo");
        else
		{
        do{
            fseek(fpm,i*sizeof(montadora),0);
	  	    fread(pm,sizeof(montadora),1,fpm);
	  	    i++;
	  	    if(i==qtd_carro)
	  	        break;
        }while(pm->regcarro!=aux_nr);
        if(pm->regcarro!=aux_nr)
        	printf("Registro não encontrado\n");
    	if(pm->regcarro==aux_nr)
    		if(pm->status.reserva.sigla=='R')
	    	{
                do{
                    fseek(fpl,j*sizeof(loja),0);
	  	            fread(pl,sizeof(loja),1,fpl);
        	  	    j++;
        	  	    if(j==qtd)
	  	               break;
                }while(strcmp(pm->status.reserva.CNPJ,pl->CNPJ)!=0);
	    		system("cls");
	    		printf("\nModelo: %s\nCor: %s\nValor %.2f\nStatus: %c - %s\n", pm->modelo, pm->cor ,pm->valor, pm->status.reserva.sigla, pm->status.reserva.CNPJ);
                printf("\n[1]Comprar\n[2]Desfazer Reserva\n[3]Cancelar\n");
		    	scanf("%i", &es);
	   		 	fflush(stdin);
	   		 	k=0;
	        	switch(es) //Os "break" são utilizados para finalizar o ciclo for, já que as atualizações são feitas em apenas um índice da tabela
                {
                    case 1:
					for(k=0;k<3;)
					{
						if((pl->tabela+k)->reservado.regcarro==pm->regcarro) // Ao vender o carro ele NÃO aparecerá nas funções que mostram o acervo de carros, apenas será mostrado no historico de vendas. Devido ao reg do carro ser -1. O PDF não solicitou isto, mas no comentário da Etapa dois isto foi mencionado que poderia ser feito.
						{//caso queiram ver se o programa funcionaria sem esta alteração minha, é necessário apenas apagar as linhas:1058 e 1059. 
							(pl->tabela+k)->reservado.sigla='L'; //Esta linha é necessária para a execução correta da função verificar
							(pl->tabela+k)->sigla='L'; 
							pl->reserved-=1;
							pl->sold+=1;
							strcpy(modelo,pm->modelo);
							strcpy(cor,pm->cor);
							valor=pm->valor;
							strcpy(pm->modelo,"VAGO");
							strcpy(pm->cor,"VAGO");
							pm->valor=0;
							pm->status.sigla='L';
							atualizar_loja(pl,j-1,"rb+");
							atualizar_montadora(pm,i-1,"rb+");
							fclose(fpm);
							fclose(fpl);
							historico(pl,pm,ph,j-1,i-1,qtd_historico,valor,modelo,cor);
							printf("\nCompra realizada\n");
							break;
						}
						k++;
					}
                    break;
                    
                    case 2:
					for(k=0;k<3;)
					{
						if(pl->tabela[k].reservado.regcarro==pm->regcarro)
						{
							(pl->tabela+k)->reservado.sigla='L'; //Esta linha é necessária para a execução correta da função verificar
							(pl->tabela+k)->sigla='L'; 
							pl->reserved-=1;
							pm->status.sigla='L';
							atualizar_loja(pl,j-1,"rb+");
							atualizar_montadora(pm,i-1,"rb+");
							fclose(fpl);
							fclose(fpm);
							printf("\nReserva cancelada\n");
							break;
						}
						k++;
					}
                    break;
                    
                    default: 
                    printf("\nAlterações Desfeitas\n");
                    break;
                }
            }
		system("pause");
    	}
	}
}

void alocar_historico(historicoVendas **p, int tam)
{
    if((*p=(historicoVendas*)realloc(*p, tam*sizeof(historicoVendas))) == NULL)
    {
        printf("Erro");
        exit(1);
    }
}

int checar_historico()
{
    FILE *fp = NULL;
    long int cont = 0;
    if((fp=fopen("historico.bin","rb"))==NULL)
        return cont;
    else
    {
  	    fseek(fp,0,2);   
  	    cont=ftell(fp)/sizeof(historicoVendas);
  	    fclose(fp);
        return cont;     
    }
}

void historico(loja *pl,montadora *pm,historicoVendas *ph,int auxl,int auxm,int qtd_historico,int valor,char *modelo,char *cor)
{// A função histórico recebe os dados do carro por partes para evitar gravar dados "VAGOS" e o valor 0. Mas é utilizado variáveis individuais ao invés de realizar o processo antes de alterar os dados para evitar problemas de lixo de memória já que estariamos abrindo um arquivo antes de fechá-lo na função anterior
    FILE *fpl=NULL;
    FILE *fpm=NULL;
    int dia=0,mes=0,ano=0;
    system("cls");
    system ("pause");
    if((fpl=fopen("concessionaria.bin","rb"))==NULL)
        printf("\nErro ao abrir o arquivo");
    else
    {
    	if((fpm=fopen("carro.bin","rb"))==NULL)
        	printf("\nErro ao abrir o arquivo");
        else
        {
        fseek(fpl,auxl*sizeof(loja),0);
		fread(pl,sizeof(loja),1,fpl);
		ph->regloja=pl->regloja;
		strcpy(ph->nome,pl->nome);
		strcpy(ph->cnpj,pl->CNPJ);
		fclose(fpl);     
        fseek(fpm,auxm*sizeof(montadora),0);
		fread(pm,sizeof(montadora),1,fpm);
		ph->regcarro=pm->regcarro;
		pm->regcarro=-1; // Isto evita do veículo vendido ser reservado ou visualizado por quaisquer modos
		atualizar_montadora(pm, auxm,"rb+");
		fclose(fpm);
		strcpy(ph->modelo,modelo);
		strcpy(ph->cor,cor);
		ph->valor=valor;
		qtd_historico=checar_historico()+1;
		ph->reghist=qtd_historico;
		dataValida:
		printf("\nInsira o mês da compra\n");
		scanf("%i",&mes);
		fflush(stdin);
		if(mes<13) // São utilizados goto nesta função pela sua praticidade, realizar múltiplos ciclos do while iriam aumentar os números de linha
		{
			ph->dataVenda.mes= mes;
		}
		else
			goto data;
		printf("Insira o dia da compra\n");
		scanf("%i",&dia);
		fflush(stdin);
		if(dia<32&&mes!=2&&dia>0&&mes>0)
			ph->dataVenda.dia= dia;
		if(dia<30&&mes==2&&dia>0&&mes>0)
			ph->dataVenda.dia= dia;
		if(dia>31&&mes!=2&&dia>0&&mes>0)
			goto data;
		if(dia>29&&mes==2&&dia>0&&mes>0)
			goto data;
		printf("\nInsira o ano da compra\n");
		scanf("%i",&ano);
		fflush(stdin);
		if(ano<=2021&&ano>0)
		{
			ph->dataVenda.ano= ano;
		}
		else
			goto data;
		atualizar_historico(ph,0,"ab");
			if(dia<0||mes<0||ano<0)
			{
				data:// o label data está em uma situação impossível
				system("cls");
				printf("Data impossível\n");
				system("pause");
				goto dataValida; // Este goto é utilizado para redirecionar o usuário para inserir dia, mês e ano existente. Caso o registro fosse cancelado (assim como ocorre durante o registro da concessionária) o ponteiro do tipo historicoVenda e o arquivo historico.bin poderiam guardar informações incompletas

			}
		}
	}
}

void mostrar_hmodelo(historicoVendas *p,int qtd)
{
    int i,tam=0;
    char aux[20];
    FILE *fp=NULL;
    system("cls");
    if((fp=fopen("historico.bin","rb"))==NULL)
        printf("\nNão há histórico\n");
    else
    {
        printf("Insira o modelo do carro: \n");
        gets(aux);
        fflush(stdin);
    	tam+=strlen(aux)+1;
    	maiuscula(aux,tam);
  	    for(i=0;i<qtd;i++)
  	    {
  	    	fseek(fp,i*sizeof(historicoVendas),0);
	  	    fread(p,sizeof(historicoVendas),1,fp);
            if(strcmp(aux,p->modelo)==0)
                printf("\nRegistro Histórico: %i\n\nRegistro Loja: %i\nNome: %s\nCNPJ: %s\n\nRegistro Carro: %i\nModelo: %s\nCor: %s\nValor: %.2f\n\nData: %i/%i/%i\n",p->reghist,p->regloja,p->nome,p->cnpj,p->regcarro,p->modelo,p->cor,p->valor,p->dataVenda.dia,p->dataVenda.mes,p->dataVenda.ano);
        }
        fclose(fp);
    }
    system("pause");
}

void mostrar_hcnpj(historicoVendas *p,int qtd)
{
    int i;
    char aux[19];
    FILE *fp=NULL;
    system("cls");
    if((fp=fopen("historico.bin","rb"))==NULL)
        printf("\nNão há histórico\n");
    else
    {
        printf("Insira o CNPJ da concessionária: \n");
        gets(aux);
        fflush(stdin);
  	    for(i=0;i<qtd;i++)
  	    {
  	    	fseek(fp,i*sizeof(historicoVendas),0);
	  	    fread(p,sizeof(historicoVendas),1,fp);
            if(strcmp(aux,p->cnpj)==0)
                printf("\nRegistro Histórico: %i\n\nRegistro Loja: %i\nNome: %s\nCNPJ: %s\n\nRegistro Carro: %i\nModelo: %s\nCor: %s\nValor: %.2f\n\nData: %i/%i/%i\n",p->reghist,p->regloja,p->nome,p->cnpj,p->regcarro,p->modelo,p->cor,p->valor,p->dataVenda.dia,p->dataVenda.mes,p->dataVenda.ano);
        }
        fclose(fp);
    }	
    system("pause");
}

void alterar_concessionaria(loja *pl,montadora *pm,historicoVendas *ph,int qtd,int qtd_carro)
{//Possivelmente esta função poderia ser menor caso houvessem alguns if(fph==NULL) para evitar o programa de executar códigos que envolvem o arquivo histórico.bin. Porém foi feita utilizando o if do fopen como guia geral.
	FILE *fph=NULL;
	FILE *fpl=NULL;
	FILE *fpm=NULL;
	char snN,snCNPJ,nome[30],CNPJ[19];
	int tamn=0,tamCNPJ=0, aux=0,i=0,cont1=0,cont2=0,j,qtd_historico=0;
	system("cls");
	aux=verificar(pl,qtd);
	if (aux <0)
		goto inexistente;
	printf("Deseja alterar o nome ?\n");
	scanf("%c",&snN);
	fflush(stdin);
	printf("Deseja alterar o CNPJ ?\n");
	scanf("%c",&snCNPJ);
	fflush(stdin);
	maiuscula(&snN,1);
	maiuscula(&snCNPJ,1);
	system("cls");
	if((fpl=fopen("concessionaria.bin","rb"))==NULL)
		printf("erro ao abrir o arquivo");
    else
	{
   		if((fph=fopen("historico.bin","rb"))==NULL)//Se não haver um arquivo"historico.bin" esta função será executada
   		{
   			fseek(fpl,aux*sizeof(loja),0);
			fread(pl,sizeof(loja),1,fpl);
			if(snN=='S'&&snCNPJ=='N')
			{
				printf("Insira o novo nome da concessionária: \n");
				gets(nome);
				fflush(stdin);
  		  		tamn+=strlen(nome)+1;
  		  		maiuscula(nome,tamn);
 		   		cont1 = autenticacao(nome,qtd,0);
		    	if(cont1==1)
		   		{
			    	strcpy(pl->nome, nome);
			    	atualizar_loja(pl,aux,"rb+");
	    		}
 	  			else
	        		goto existente;				
				}
			if(snN=='N'&&snCNPJ=='S')
			{
				printf("Insira o novo CNPJ da concessionária: \n");
				gets(CNPJ);
				fflush(stdin);
 		   		cont1 = autenticacao(CNPJ,qtd,1);
		    	if(cont1==1)
		   		{
					if(pl->reserved!=0)
					{
		   				if((fpm=fopen("carro.bin","rb"))==NULL)
		   					printf("\nErro ao Abrir concessionaria.bin\n");
		   				else
		   				{			
			   				for(j=0;j<qtd_carro;)
		   					{
                    			fseek(fpm,j*sizeof(montadora),0);
	  	       		     		fread(pm,sizeof(montadora),1,fpm);
	  	            			j++;
        	  	  			  	if(strcmp(pm->status.reserva.CNPJ,pl->CNPJ)==0)
        	  		  		  	{
		   							strcpy(pm->status.reserva.CNPJ, CNPJ);
									atualizar_montadora(pm,j-1,"rb+");
								}
							}
							fclose(fpm);
						}
		   			}
			    	strcpy(pl->CNPJ, CNPJ);
			    	atualizar_loja(pl,aux,"rb+");
	    		}
 	  			else
	        		goto existente;		
			}
			if(snN=='S'&&snCNPJ=='S')
			{
				printf("Insira o novo nome da concessionária: \n");
				gets(nome);
				fflush(stdin);
  		  		tamn+=strlen(nome)+1;
  		  		maiuscula(nome,tamn);
 		   		cont1 = autenticacao(nome,qtd,0);
				printf("Insira o novo CNPJ da concessionária: \n");
				gets(CNPJ);
				fflush(stdin);
 		   		cont2 = autenticacao(CNPJ,qtd,1);
		    	if(cont1==1&&cont2==1)
		   		{
		   			if(pl->reserved!=0)
					{
		   				if((fpm=fopen("carro.bin","rb"))==NULL)
		   					printf("\nErro ao Abrir concessionaria.bin\n");
		   				else
		   				{
		   					for(j=0;j<qtd_carro;)
		   					{
                    			fseek(fpm,j*sizeof(montadora),0);
	  	           		 		fread(pm,sizeof(montadora),1,fpm);
	  	            			j++;
        	  	  			  	if(strcmp(pm->status.reserva.CNPJ,pl->CNPJ)==0)
        	  	  			  	{
		   							strcpy(pm->status.reserva.CNPJ, CNPJ);
									atualizar_montadora(pm,j-1,"rb+");
								}
							}
							fclose(fpm);
						}
		   			}
			    	strcpy(pl->nome, nome);
			    	strcpy(pl->CNPJ, CNPJ);
			    	atualizar_loja(pl,aux,"rb+");
	    		}
 	  			else
	        		goto existente;		
			}
	fclose(fpl);
	system("pause");
		}
  		else //Caso haja algo gravado no "historico.bin", esta função será executada ao invés da acima
  		{
  			qtd_historico=checar_historico();
  		  	fseek(fpl,aux*sizeof(loja),0);
			fread(pl,sizeof(loja),1,fpl);
			if(snN=='S'&&snCNPJ=='N')
			{
				printf("Insira o novo nome da concessionária: \n");
				gets(nome);
				fflush(stdin);
  		  		tamn+=strlen(nome)+1;
  		  		maiuscula(nome,tamn);
 		   		cont1 = autenticacao(nome,qtd,0);
		    	if(cont1==1)
		   		{
			    	strcpy(pl->nome, nome);
			    	for(i=0;i<qtd_historico;)
			    	{
                    	fseek(fph,i*sizeof(historicoVendas),0);
	  	            	fread(ph,sizeof(historicoVendas),1,fph);
	  	            	i++;
			    		if(pl->regloja==ph->regloja)
			  	  		{
			    			strcpy(ph->nome, nome);
			    			atualizar_historico(ph,i-1,"rb+");
			    		}
			    	}
			    	atualizar_loja(pl,aux,"rb+");
	    		}
 	  			else
	        		goto existente;				
				}
			if(snN=='N'&&snCNPJ=='S')
			{
				printf("Insira o novo CNPJ da concessionária: \n");
				gets(CNPJ);
				fflush(stdin);
 		   		cont1 = autenticacao(CNPJ,qtd,1);
		    	if(cont1==1)
		   		{
		   			if(pl->reserved!=0)
					{
		   				if((fpm=fopen("carro.bin","rb"))==NULL)
		   					printf("\nErro ao Abrir concessionaria.bin\n");
		   				else
		   				{
		   					for(j=0;j<qtd_carro;)
		   					{
                    			fseek(fpm,j*sizeof(montadora),0);
	  	            			fread(pm,sizeof(montadora),1,fpm);
	  	            			j++;
        	  	  			  	if(strcmp(pm->status.reserva.CNPJ,pl->CNPJ)==0)
								{
		   							strcpy(pm->status.reserva.CNPJ, CNPJ);
									atualizar_montadora(pm,j-1,"rb+");
								}
							}
							fclose(fpm);
						}
		   			}
			    	strcpy(pl->CNPJ, CNPJ);
			    	for(i=0;i<qtd_historico;)
			    	{
                    	fseek(fph,i*sizeof(historicoVendas),0);
	  	            	fread(ph,sizeof(historicoVendas),1,fph);
	  	            	i++;
			    		if(pl->regloja==ph->regloja)
			  	  		{
			    			strcpy(ph->cnpj, CNPJ);
			    			atualizar_historico(ph,i-1,"rb+");
			    		}
			    	}
			    	atualizar_loja(pl,aux,"rb+");
			    		
	    		}
 	  			else
	        		goto existente;		
			}
			if(snN=='S'&&snCNPJ=='S')
			{
				printf("Insira o novo nome da concessionária: \n");
				gets(nome);
				fflush(stdin);
  		  		tamn+=strlen(nome)+1;
  		  		maiuscula(nome,tamn);
 		   		cont1 = autenticacao(nome,qtd,0);
				printf("Insira o novo CNPJ da concessionária: \n");
				gets(CNPJ);
				fflush(stdin);
 		   		cont2 = autenticacao(CNPJ,qtd,1);
		    	if(cont1==1&&cont2==1)
		   		{
		   			if(pl->reserved!=0)
					{
		   				if((fpm=fopen("carro.bin","rb"))==NULL)
		   					printf("\nErro ao Abrir concessionaria.bin\n");
		   				else
		   				{
							for(j=0;j<qtd_carro;)
		   					{
                    			fseek(fpm,j*sizeof(montadora),0);
	  	          	  			fread(pm,sizeof(montadora),1,fpm);
	  	            			j++;
        	  	  			  	if(strcmp(pm->status.reserva.CNPJ,pl->CNPJ)==0)
        	  	  			  	{
		   							strcpy(pm->status.reserva.CNPJ, CNPJ);
									atualizar_montadora(pm,j-1,"rb+");
								}
							}
							fclose(fpm);
						}
		   			}
			    	strcpy(pl->nome, nome);
			    	strcpy(pl->CNPJ, CNPJ);
			    	for(i=0;i<qtd_historico;)
			    	{
                    	fseek(fph,i*sizeof(historicoVendas),0);
	  	            	fread(ph,sizeof(historicoVendas),1,fph);
	  	            	i++;
			    		if(pl->regloja==ph->regloja)
			  	  		{
			    			strcpy(ph->cnpj, CNPJ);
			    			strcpy(ph->nome, nome);
			    			atualizar_historico(ph,i-1,"rb+");
			    		}
			    	}		
			    	atualizar_loja(pl,aux,"rb+");
	    		}
 	  			else
	        		goto existente;		
			}
			if(aux==1000)
			{
				existente:
				system("cls");
				printf("Dado já registrado\n");
			}
			if(aux==2000)
			{
				inexistente:
				aux =0;
			}
	fclose(fph);
	fclose(fpl);

		}
	}
	system("pause");
}

void alterar_carro(montadora *pm,historicoVendas *ph,int qtd_carro)
{//Possivelmente esta função poderia ser menor caso houvessem alguns if(fph==NULL) para evitar o programa de executar códigos que envolvem o arquivo histórico.bin. Porém foi feita utilizando o if do fopen como guia geral.
	FILE *fph=NULL;
	FILE *fpm=NULL;
	int numreg=0,i=0,j=0,aux=0,qtd_historico=0;
	char modelo[20],cor[10];
	float valor=0;
	system("cls");
	printf("Insira o número de registro do carro que deseja alterar: \n");
	scanf("%i",&numreg);
	fflush(stdin);
	system("cls");
	printf("Escolha o que deseja modificar\n\n\n[1]Modelo\n[2]Cor\n[3]Valor\n[4]Cancelar\n");
	scanf("%i",&aux);
	if((fpm=fopen("carro.bin","rb"))==NULL)
       	printf("\nErro ao abrir o arquivo carro.bin");
    else
	{
   		if((fph=fopen("historico.bin","rb"))==NULL)
   		{
			do{
    			fseek(fpm,aux*sizeof(montadora),0);
				fread(pm,sizeof(montadora),1,fpm);
				i++;
				if(i==qtd_carro)
					break;	
			}while(numreg!=pm->regcarro);
			if(numreg==pm->regcarro)
			{
				switch(aux)
				{
					case 1: //Alterar Modelo
					printf("Insira o novo modelo:\n");
					gets(modelo);
					fflush(stdin);
					strcpy(pm->modelo,modelo);
			    	atualizar_montadora(pm,i-1,"rb+");
					break;
					
					case 2: //Alterar Cor
					printf("Insira a nova cor:\n");
					gets(cor);
					fflush(stdin);
					strcpy(pm->cor,cor);
			    	atualizar_montadora(pm,i-1,"rb+");
					break;
					
					case 3: //Alterar Valor
					printf("Insira o novo preço:\n");
					scanf("%f",&valor);
					fflush(stdin);
					pm->valor=valor;
					for(j=0;j<qtd_historico;)
			    	atualizar_montadora(pm,i-1,"rb+");
					break;
					
					default:
					break;
				}
					
			}
			else
				printf("\nNúmero de registro inválido\n");
			fclose(fph);
		}
   	 	else
		{
			qtd_historico=checar_historico();
			do{
    			fseek(fpm,aux*sizeof(montadora),0);
				fread(pm,sizeof(montadora),1,fpm);
				i++;
				if(i==qtd_carro)
					break;	
			}while(numreg!=pm->regcarro);
			if(numreg==pm->regcarro)
			{
				switch(aux)
				{
					case 1: //Alterar Modelo
					printf("Insira o novo modelo:\n");
					gets(modelo);
					fflush(stdin);
					strcpy(pm->modelo,modelo);
					for(j=0;j<qtd_historico;)
					{
						fseek(fph,j*sizeof(historicoVendas),0);
						fread(ph,sizeof(historicoVendas),1,fph);
						j++;
						if(ph->regcarro==pm->regcarro)
						{
							strcpy(ph->modelo,modelo);
							atualizar_historico(ph,j-1,"rb+");
						}
					}
			    	atualizar_montadora(pm,i-1,"rb+");
					break;
					
					case 2: //Alterar Cor
					printf("Insira a nova cor:\n");
					gets(cor);
					fflush(stdin);
					strcpy(pm->cor,cor);
					for(j=0;j<qtd_historico;)
					{
						fseek(fph,j*sizeof(historicoVendas),0);
						fread(ph,sizeof(historicoVendas),1,fph);
						j++;
						if(ph->regcarro==pm->regcarro)
						{
							strcpy(ph->cor,cor);
							atualizar_historico(ph,j-1,"rb+");
						}
					}
			    	atualizar_montadora(pm,i-1,"rb+");
					break;
					
					case 3: //Alterar Valor
					printf("Insira o novo preço:\n");
					scanf("%f",&valor);
					fflush(stdin);
					pm->valor=valor;
					for(j=0;j<qtd_historico;)
					{
						fseek(fph,j*sizeof(historicoVendas),0);
						fread(ph,sizeof(historicoVendas),1,fph);
						j++;
						if(ph->regcarro==pm->regcarro)
						{
							ph->valor=valor;
							atualizar_historico(ph,j-1,"rb+");
						}
					}
			    	atualizar_montadora(pm,i-1,"rb+");
					break;
					
					default:
					break;
				}
					
			}
			else
				printf("\nNúmero de registro inválido\n");
			fclose(fph);
		}
		fclose(fpm);
	}
	system("pause");
}