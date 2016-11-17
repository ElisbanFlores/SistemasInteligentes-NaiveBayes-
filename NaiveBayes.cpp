// NaiveBayes.cpp: define el punto de entrada de la aplicación de consola.
//

#pragma once

#include "stdafx.h"


#include <string>
#include <cstring>
#include <fstream>
#include <map>
#include <set>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <vector>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <time.h>
#include <cstdlib>

typedef char T_CHAR;
typedef long T_LONG;
typedef float T_FLOAT;
typedef int T_INT;

const T_LONG BLOQUELINEA = 1024;
const T_LONG NATTRIBUTES = 6;
const T_LONG NATTRIBUTESVALUES = 4 + 1;
const T_LONG NCLASS = 4;
const T_LONG LIDENTIFICADOR = 32;

T_CHAR tablaAttributes[NATTRIBUTES][NATTRIBUTESVALUES][LIDENTIFICADOR]; //10 atributos, 10 valores, de ocho caracteres
T_FLOAT valoresAttributes[NATTRIBUTES][NATTRIBUTESVALUES][NCLASS];
T_CHAR tablaClass[NCLASS][LIDENTIFICADOR]; //4 Clases, de ocho caracteres;
T_FLOAT valoresClass[NCLASS];
T_CHAR data[LIDENTIFICADOR];
T_LONG nroData;


void Inicializar()
{
	for (int i = 0;i<NATTRIBUTES;i++)
		for (int j = 0;j < NATTRIBUTESVALUES;j++)
			for (int k=0;k<NCLASS;k++)
			{
				strcpy_s(tablaAttributes[i][j], "");
				valoresAttributes[i][j][k] = 0;
			}
	for (int i = 0;i < NCLASS;i++)
	{
		strcpy_s(tablaClass[i], "");
		valoresClass[i] = 0;
	}
	nroData = 0;
}

void Mostrar()
{
	for (int i = 0;i < NATTRIBUTES;i++)
	{
		for (int j = 0;j < NATTRIBUTESVALUES;j++)
			std::cout <<"*"<< tablaAttributes[i][j] <<"*"<< "\t";
		std::cout << std::endl;
	}

	for (int i = 0;i < NCLASS;i++)
		std::cout << tablaClass[i] << "\t";

	std::cout << std::endl << data<<std::endl;
}

void MostrarValores()
{
	std::cout <<"valores de atributos"<< std::endl;
	for (int i = 0;i < NATTRIBUTES;i++)
	{
		for (int j = 0;j < NATTRIBUTESVALUES;j++)
		{
			for (int k = 0;k < NCLASS;k++)
				std::cout << valoresAttributes[i][j][k] << "\t";
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}

	std::cout <<"valores de clases"<< std::endl;
	for (int i = 0;i < NCLASS;i++)
		std::cout << valoresClass[i] << "\t";	
	std::cout << std::endl << nroData<<std::endl;
}

int Acumular(T_LONG nva, T_CHAR *valor, T_LONG nclass)
{	
	for (int i = 1;i < NATTRIBUTES;i++)
		if (strcmp(tablaAttributes[nva][i], valor) == 0) 
		{
			valoresAttributes[nva][i][nclass]++;
			return 1;
		}
	return 0;	
}

T_FLOAT GetProbabilidadAttribute(T_LONG nva, T_CHAR *valor, T_LONG nclass)
{
	for (int i = 1; i < NATTRIBUTES; i++)
		if (strcmp(tablaAttributes[nva][i], valor) == 0)
			return valoresAttributes[nva][i][nclass];		
	return 0.0;
}

T_FLOAT GetProbabilidadClass(T_LONG nclass)
{
	return valoresClass[nclass];	
}

void CargarConfiguracion(T_CHAR *FileOrigen)
{
	std::ifstream origen(FileOrigen);
	if (origen.fail())
		std::cerr << "Error al abrir el archivo: " << FileOrigen << std::endl;
	else
	{
		T_CHAR *bloque;
		bloque = new T_CHAR[BLOQUELINEA+1];
		
		T_INT caso=0;
		T_LONG na=0, nva=0;// Nro Atributos, Numero de clases
		
		Inicializar();

		while (!origen.eof())
		{
			origen.getline(bloque, BLOQUELINEA, '\n');			
			
			if (bloque[0] == '#')
			{
				if (strcmp(bloque, "#Attributes")==0) caso = 1;
				else if (strcmp(bloque, "#ClassValues") == 0) caso = 2;
				else if (strcmp(bloque, "#DataIn") == 0) caso = 3;
				else caso = 0;
				std::cout << caso << std::endl;
				na = 0;
			}				
			else
			{
				
				T_LONG largo = strlen(bloque);
				T_CHAR valor[BLOQUELINEA];			

				switch (caso)
				{
					case 1:
					{						
						nva = 0; int i ;
						
						//para el valor de los atributos
						for (i=0;i < largo;i++)
						{							
							if (bloque[i] == ':')	
								{ valor[i] = '\0';break;	}
							else							
								valor[i] = bloque[i];														
						}					
						strcpy_s(tablaAttributes[na][nva], valor);// ingreso el nombre del atributo						
						
						// ahora para llenar los atributos
						nva++;
						//i++;
						int j = 0;
						while (++i < largo) 
						{
							if (bloque[i] == ','|| bloque[i] == '.')
							{
								valor[j] = '\0';
								strcpy_s(tablaAttributes[na][nva++], valor); //  ingreso los valores aceptables de cada atributo
								valor[0] = '\0';
								j = 0;
							}
							else
								valor[j++] = bloque[i];							
							//i++;
						}
						na++;
						break;
					}
					case 2:
					{
						int i = 0;
						int j = 0;
						nva = 0;
						while (i < largo)
						{
							if (bloque[i] == ',' || bloque[i] == '.')
							{
								valor[j] = '\0';
								strcpy_s(tablaClass[nva++], valor); //  ingreso los valores aceptables de cada atributo
								valor[0] = '\0';
								j = 0;
							}
							else
								valor[j++] = bloque[i];							
							i++;
						}						
						break;
					}
					case 3:
					{
						strcpy_s(data, bloque);						
						break;
					}
					default:
						break;
				}			
			}		
		}
		Mostrar();		
		origen.close();
	}
}

T_LONG ObtenerClass(T_CHAR *bloque, T_LONG longitud)
{
	T_LONG j = 0;
	T_CHAR valor[BLOQUELINEA];
	while (bloque[--longitud] != ',');
	while (bloque[longitud++] != '\0')
		valor[j++] = bloque[longitud];
	valor[j] = '\0';
	for (j = 0; j < NCLASS; j++)
		if (strcmp(tablaClass[j], valor) == 0)
			return j;
	return -1;
}

void CargarData()
{	
	std::ifstream origen(data);
	if (origen.fail())
		std::cerr << "Error al abrir el archivo: " << data << std::endl;
	else
	{	
		T_CHAR *bloque;
		bloque = new T_CHAR[BLOQUELINEA + 1];
		T_LONG nva, cl;
		T_LONG largo = strlen(bloque);
		T_CHAR valor[BLOQUELINEA];

		bloque = new T_CHAR[BLOQUELINEA + 1];
		int i,j=0;
		while (!origen.eof())
		{
			origen.getline(bloque, BLOQUELINEA, '\n');
			largo = strlen(bloque);
			if (largo > 0)
			{
				j = 0;
				nva = 0;
				i = 0;

				cl = ObtenerClass(bloque, largo);
				valoresClass[cl]++;
				nroData++;
				while (i < largo)
				{
					if (bloque[i] == ',' || bloque[i] == '\0')
					{
						valor[j] = '\0';
						Acumular(nva++, valor, cl); //  ingreso los valores aceptables de cada atributo
						valor[0] = '\0';
						j = 0;
					}
					else
						valor[j++] = bloque[i];
					i++;
				}
			}
		}
		origen.close();
		MostrarValores();
	}
}

void Normalizar() //  si hay ceros en los conteos sumar uno a cada valor, y aplicar la correccion Laplaciana
{
	// pone 1 acada elemento que tiene valor 0
	for (int i = 0; i<NATTRIBUTES; i++)
		for (int j = 1; j < NATTRIBUTESVALUES; j++)
			for (int k = 0; k < NCLASS; k++)
				if (strcmp(tablaAttributes[i][j], "") != 0)
					valoresAttributes[i][j][k]++;
				
	std::cout << "Valores normalizados a 1:"<<std::endl;
	MostrarValores();
}

void CalcularProbabilidades() //  con  la corrreccion 
{
	T_FLOAT suma;
	for (int i = 0; i < NATTRIBUTES; i++)
		for (int k = 0; k < NCLASS; k++)
		{
			suma = 0;
			for (int j = 1; j < NATTRIBUTESVALUES; j++)
				suma += valoresAttributes[i][j][k];

			for (int j = 1; j < NATTRIBUTESVALUES; j++)
				valoresAttributes[i][j][k] /= suma;
		}
	for (int i = 0; i < NCLASS; i++)
		valoresClass[i]/=nroData;		
	MostrarValores();
}

void Entrenar()
{
	CargarConfiguracion("conf.txt");
	CargarData();
	Normalizar();
	CalcularProbabilidades();
}


void Predecir(T_CHAR *FileOrigen)
{
	std::ifstream origen(FileOrigen);
	if (origen.fail())
		std::cerr << "Error al abrir el archivo: " << FileOrigen << std::endl;
	else
	{
		T_CHAR *bloque;
		bloque = new T_CHAR[BLOQUELINEA + 1];
		T_FLOAT prediccion[NCLASS];
		T_FLOAT vp = 0;
		T_LONG nva, cl;
		T_LONG largo;
		T_CHAR valor[BLOQUELINEA];

		bloque = new T_CHAR[BLOQUELINEA + 1];
		int i, j = 0;
		while (!origen.eof())
		{
			origen.getline(bloque, BLOQUELINEA, '\n');
			largo = strlen(bloque);
			if (largo > 0)
			{
				std::cout << std::endl << "prediccion de: " << std::endl;
				std::cout << bloque << std::endl;

				j = 0;
				nva = 0;
								
				for (i = 0; i < NCLASS; i++)
					prediccion[i] = 1;
				i = 0;

				while (i < largo + 1)
				{
					if (bloque[i] == ',' || bloque[i] == '\0')
					{
						valor[j] = '\0';
						std::cout << std::endl << valor << ": ";
						for (int k = 0; k < NCLASS; k++)
						{
							vp = GetProbabilidadAttribute(nva, valor, k);
							prediccion[k] *= vp;
							std::cout <<"\t"<< vp<<", ";
						}
						nva++;
						valor[0] = '\0';
						j = 0;
					}
					else
						valor[j++] = bloque[i];
					i++;
				}
				T_FLOAT suma=0;
				for (i = 0; i < NCLASS; i++)
				{
					prediccion[i] *= GetProbabilidadClass(i);
					suma += prediccion[i];
				}	
				T_FLOAT max = 0;
				T_LONG id = 0;
				for (i = 0; i < NCLASS; i++)
				{
					prediccion[i] /= suma;
					if (prediccion[i] > max) { max = prediccion[i]; id = i; }
						std::cout << std::endl << tablaClass[i] << " * * * " << prediccion[i] ;
				}							
				std::cout <<std::endl<< "Se puede decir por lo tanto que es: "<< tablaClass[id]<< std::endl;
			}
		}
		origen.close();
	}
}

int main()
{
	Entrenar();
	Predecir("car-prueba.data");
	getchar();
	return 0;
}