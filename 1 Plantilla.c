#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdbool.h>

#define n 4
#define filas n
#define columnas n
#define condicion 1 //acabar el mapa
#define bateria_max 100
#define p 'p'
#define arriba 0
#define derecha 1
#define abajo 2
#define izquierda 3

typedef double db;
typedef long long ll;
typedef float f;
typedef char c;

typedef struct {
    int x;
    int y;
} Posicion;

void imprimirTablero(int tablero[filas][columnas]) {
    for (int i = 0; i < filas; i++) {
        for (int j = 0; j < columnas; j++) {
            printf("%2d ", tablero[i][j]);
        }
        printf("\n");
    }
}

void giro(ll *direccion,ll *atascado){//gira a la derecha
	if (*direccion==arriba) *direccion=derecha;
	else if (*direccion==derecha) *direccion=abajo;
	else if (*direccion==abajo) *direccion=izquierda;
	else if (*direccion==izquierda) *direccion=arriba;
	*atascado++;
}//direccion++; if (direccion==4) direccion=0;

int main(int argc, char *argv[])
{
	c mapa_principal [filas+2][columnas+2]={{p,p,p,p,p,p},
	{p,'0','0','0','0',p},
	{p,'0','0','0','0',p},
	{p,'0','0','0','0',p},
	{p,'0','0','0','0',p},
	{p,p,p,p,p,p}};//mapa que te dan

	/*for(int i=0;i<n;i++){
		for(int j=0;j<n;j++){
			scanf("%c", mapa_principal[i][j]);
		}
	}*/

	c miguitas_de_pan [filas+2][columnas+2]={{p,p,p,p,p,p},
	{p,'0','0','0','0',p},
	{p,'0','0','0','0',p},
	{p,'0','0','0','0',p},
	{p,'0','0','0','0',p},
	{p,p,p,p,p,p}};
	//mapa que vas a usar

	Posicion pos;//coordenadas del robot
	ll bateria=bateria_max;

	ll hx=pos.x,hy=pos.y;//nos guardamos la base de carga

	ll direccion=0;//0 derecha, 1 abajo, 2 izquierda, 3 arriba

	ll atascado=0;//si llega a 4 es que esta atascado
	
	miguitas_de_pan[pos.x][pos.y]='1';//por aqui ya ha pasado

	while (atascado<4&&bateria>0){

		
		if(direccion==arriba){//si esta mirando a la derecha
			if (mapa_principal[pos.x+1][pos.y]!=p && bateria>=bateria_max/2+1 && miguitas_de_pan[pos.x+1][pos.y]!='1'){//si no hay pared, si tiene bateria y si no ha pasado por ahi
				mapa_principal[pos.x][pos.y]='0';//recoge basura haiga o no haiga
				miguitas_de_pan[pos.x+1][pos.y]='1';//por aqui ya ha pasado
				pos.x++;//se mueve
				bateria--;//gasta bateria
				atascado=0;//resetea el contador de atascado
			}
			else{
				if(mapa_principal[pos.x+1][pos.y]==p){//si hay pared
				miguitas_de_pan[pos.x+1][pos.y]=p;//nos guardamos que hay una pared
				}
				giro(&direccion,&atascado);//si no puede avanzar gira
			} 

		}
		
		
		if(direccion==derecha){//si esta mirando abajo
			if (mapa_principal[pos.x][pos.y+1]!=p&&bateria>=bateria_max/2+1){//si no hay pared, si tiene bateria y si no ha pasado por ahi
				mapa_principal[pos.x][pos.y]='0';//recoge basura haiga o no haiga
				miguitas_de_pan[pos.x][pos.y+1]='1';//por aqui ya ha pasado
				pos.y++;//se mueve
				bateria--;//gasta bateria
				atascado=0;//resetea el contador de atascado
			}
			else {
				if(mapa_principal[pos.x+1][pos.y]==p){//si hay pared
				miguitas_de_pan[pos.x+1][pos.y]=p;//nos guardamos que hay una pared
				}
				giro(&direccion,&atascado);//si no puede avanzar gira
			} 
		}
		
		
		if(direccion==abajo){//si esta mirando a la izquierda
			if (mapa_principal[pos.x-1][pos.y]!=p&&bateria>=bateria_max/2+1){//si no hay pared, si tiene bateria y si no ha pasado por ahi
				mapa_principal[pos.x][pos.y]='0';//recoge basura haiga o no haiga
				miguitas_de_pan[pos.x-1][pos.y]='1';//por aqui ya ha pasado
				pos.x--;//se mueve
				bateria--;//gasta bateria
				atascado=0;//resetea el contador de atascado
			}
			else {
				if(mapa_principal[pos.x+1][pos.y]==p){//si hay pared
				miguitas_de_pan[pos.x+1][pos.y]=p;//nos guardamos que hay una pared
				}
				giro(&direccion,&atascado);//si no puede avanzar gira
			} 
		}
		
		
		if(direccion==izquierda){//si esta mirando arriba
			if (mapa_principal[pos.x][pos.y-1]!=p&&bateria>=bateria_max/2+1){//si no hay pared, si tiene bateria y si no ha pasado por ahi
				mapa_principal[pos.x][pos.y]='0';//recoge basura haiga o no haiga
				miguitas_de_pan[pos.x][pos.y-1]='1';//por aqui ya ha pasado
				pos.y--;//se mueve
				bateria--;//gasta bateria
				atascado=0;//resetea el contador de atascado
			}
			else {
				if(mapa_principal[pos.x+1][pos.y]==p){//si hay pared
				miguitas_de_pan[pos.x+1][pos.y]=p;//nos guardamos que hay una pared
				}
				giro(&direccion,&atascado);//si no puede avanzar gira
			} 
		}

	}


	for(int i=0;i<n+2;i++){
		for(int j=0;j<n+2;j++){
			printf("%c", miguitas_de_pan[i][j]);
		}
		printf("\n");
	}





	//printf("");
	//scanf("");
	return 0;
}

