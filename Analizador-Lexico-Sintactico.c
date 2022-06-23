/*
	Proyecto #1 de Diseño de Compiladores
	Jairo Perez
  	C.I.28336214
*/

#include <stdio.h>
#include <stdlib.h>
#include "string.h"

char *tipos_id[]={"Identificador","Entero","Operador","Asignacion","Palabra reservada (ABRIR)","Palabra reservada (CERRAR)"};

FILE *archivo;
char ubic[]="Lexemas.txt";

char cadena[20], subcadena[20], lexema [20][20];
int tipo[20];
//Tipos: 1=variable/identificador, 2=Entero, 3=Operador, 4=asignacion, 5=ID_ABRIR, 6=ID_CERRAR orden=5142326

int z=0,i=0;

void save(char valor[], int tip);
void analizador_lexico(char cadena[]);
int analizador_sintactico(int *tipo, int a);
int result();

int main() {
	printf("ingrese una cadena: ");
    gets(cadena);
   	  analizador_lexico(cadena);
   	
   	//Se verifica que no haya error en el analizador lexico
   	if(i!=-1){
   		printf("\n");
   		
   		//Se verifica que no haya error en el analizador sintactico
   		switch(analizador_sintactico(tipo,z+1)){
	   		case -6:
			    printf("!No se ha encontrado un identificador de cierre...");	
	   			break;
	   		case -4:
			    printf("!El operador ingresado no es aceptado...");	
	   			break;	
	   		case -5:	
	   		case -3:
			    printf("!Falta ingresar algun valor en la operacion...");	
	   			break;	
	   		case -2:
			    printf("!No se le ha asignado ningun valor a la variable...");	
	   			break;	
	   		case -1:
			    printf("!No se ha declarado una variable...");	
	   			break;
	   		case 0:
			    printf("!Error al inicio de la cadena, no se encuentra un identificador de apertura...");	
	   			break;
	   		case 1:
				i=0;
	
				printf("cadena valida");
				printf("\n\tresultado es %s",(result()==1)?"verdadero":"falso");
				
				//Se guarda en un archivo de texto
				archivo=fopen(ubic,"w");
				fflush(stdin);
				do {
					fprintf(archivo, "\n\tLexema#%2i:%10s|Tipo:%s",i+1,lexema[i],tipos_id[tipo[i]-1]);
					i++;
				}while(i<z);
				
				fprintf(archivo,"\nEl resultado de la operacion es %s",(result()==1)?"verdadero":"falso");				
		   		break;
			default:
				printf("La cadena contiene mas elementos de los permitidos");
				break;	
		}
	}else
		printf("\n\ncadena no valida");
	
	return 0;
}

void analizador_lexico(char cadena[]){
	int j=0;
    int numero=(int)cadena[i];
	do {
		//si es un digito/entero
		if(numero>=48 && numero<=57){
			while((numero>=48 && numero<=57))	{
				subcadena[j++]=cadena[i++];
				numero=(int)cadena[i];
			}
			save(subcadena, 2);
		}
		//si es un identificador/variable
		else if((numero>=65 && numero<=90)||(numero>=97 && numero<=122)){
			while((numero>=65 && numero<=90)||(numero>=97 && numero<=122)||(numero>=48 && numero<=57))	{
				subcadena[j++]=cadena[i++];
				numero=(int)cadena[i];
			}
			if(strcmp(subcadena,"ABRIR")==0){
				save(subcadena,5);
			}else if(strcmp(subcadena,"CERRAR")==0){
				save(subcadena,6);
			}else{
				save(subcadena,1);
			}
		}
		//si es un operador o asignacion
		else if(numero==61){	
			subcadena[j++]=cadena[i++];
			numero=(int)cadena[i];
			if(numero==61){
				subcadena[j]=cadena[i++];
				numero=(int)cadena[i];
				save(subcadena,3);
			}else{
				save(subcadena,4);
			}	
		}
		//operador !=
		else if(numero==33){
			subcadena[j++]=cadena[i++];
			numero=(int)cadena[i];
			if(numero==61){
				subcadena[j]=cadena[i++];
				numero=(int)cadena[i];
				save(subcadena,3);
			}else{
				printf("\n%s es un error... no pertenece al alfabeto", subcadena);
				i--;
				break;
			}
		}
		//operadores <>
		else if(numero==60||numero==62){
			subcadena[j]=cadena[i++];
			numero=(int)cadena[i];
			save(subcadena,3);
		}
		//omitimos los espacios
		else if(numero==32)
			numero=(int)cadena[++i];
		//mensaje de error indicando que el caracter no puede ser reconocido por el analizador
		else{
			printf("\n%c es un error... no pertenece al alfabeto", cadena[i]);
			break;
		}	
		j=0;	
		memset(subcadena,'\0', 20);
	} while(i<strlen(cadena));
	i=(i==strlen(cadena))? i:-1;
}

int analizador_sintactico(int *tipo, int a){
	int tamano=a-1;
	char validar[tamano];
	char cadena_correcta[]="5142326";
	if(tamano<=strlen(cadena_correcta)){
		for(int i=0;i<strlen(cadena_correcta);i++)
			if((tipo[i]+'0')!=cadena_correcta[i])
				return i*-1;	
	}else
		return tamano;
	return 1;
}

void save(char valor[], int tip){
	strncpy(lexema[z],valor,strlen(valor));
	tipo[z++]=tip;
}

int result(){
	int numero_1=atoi(lexema[3]);
	int numero_2=atoi(lexema[5]);
	char operador=lexema[4][0];
	int resultado;
	switch(operador){
		case '<':
			resultado=(numero_1 < numero_2)? 1:0;
			break;
		case '>':
			resultado=(numero_1 > numero_2)? 1:0;
			break;
		case '=':
			resultado=(numero_1 == numero_2)? 1:0;
			break;
		case '!':
			resultado=(numero_1 != numero_2)? 1:0;
			break;	
	}
	return resultado;
}
