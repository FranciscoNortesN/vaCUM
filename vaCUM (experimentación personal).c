#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdbool.h>

//constantes
#define filas 6
#define columnas 6
#define condicion 1 //acabar el mapa
#define bateria_max 1000000000 //para que sobre bateria
#define p -1 //pared

//direcciones
#define arriba 0
#define esquina_arriba_derecha 1
#define derecha 2
#define esquina_abajo_derecha 3
#define abajo 4
#define esquina_abajo_izquierda 5
#define izquierda 6
#define esquina_arriba_izquierda 7
/*si la posicion del robot es x, con lo que viene ahora representamos las posiciones relativas a x
701
6x2
543
*/

//macros
#define giro direccion++; if (direccion==8) direccion=0; atascado++;//gira a la derecha 45 grados y suma 1 al contador de atascado (si llega a 8 es que esta atascado)
/*
	se puede poner de esta forma:
	if (*direccion==arriba) *direccion=esquina_arriba_derecha;
	else if (*direccion==esquina_arriba_derecha) *direccion=derecha;
	else if (*direccion==derecha) *direccion=esquina_abajo_derecha;
	else if (*direccion==esquina_abajo_derecha) *direccion=abajo;
	else if (*direccion==abajo) *direccion=esquina_abajo_izquierda;
	else if (*direccion==esquina_abajo_izquierda) *direccion=izquierda;
	else if (*direccion==izquierda) *direccion=esquina_arriba_izquierda;
	else if (*direccion==esquina_arriba_izquierda) *direccion=arriba;
*/
#define printf_pos_map printf("x%d, y%d, %lld\n", pos.x, pos.y, (long long int)mapa_principal[pos.x][pos.y])//imprime la posicion del robot y el estado de la casilla del mapa principal
//esta macro está, porque la vamos a usar mucho y es mas facil de escribir
#define printf_pos_pan printf("x%d, y%d, %lld\n", pos.x, pos.y, (long long int)miguitas_de_pan[pos.x][pos.y])//imprime la posicion del robot y el estado de la casilla de las miguitas de pan
//lo mismo que la anterior macro pero con las miguitas de pan


typedef long long ll;
typedef float f;
typedef double db;
typedef char c;

typedef struct {
    int x;
    int y;
} Posicion;

void imprimirTablero(int tablero[filas][columnas]) {//imprime el tablero
    for (int i = 0; i < filas; i++) {
        for (int j = 0; j < columnas; j++) {
            printf("%2d ", tablero[i][j]);
        }
        printf("\n");
    }
}

// Función para verificar si una posición está dentro de los límites del tablero
bool dentroDeLimites(int x, int y) {
    return (x >= 0 && x < filas && y >= 0 && y < columnas);
}

void limpiar_basura (int mapa_principal[filas][columnas],Posicion pos/*,ll miguitas_de_pan[filas][columnas]*/){
	while(mapa_principal[pos.x][pos.y]>0){
		mapa_principal[pos.x][pos.y]--;
		printf_pos_map;
	}
}


int main(int argc, char *argv[])
{
	int mapa_principal [filas][columnas]={{p,p,p,p,p,p},
	{p,0,0,0,0,p},
	{p,0,0,3,0,p},
	{p,0,p,0,0,p},
	{p,0,0,0,0,p},
	{p,p,p,p,p,p}};
	//mapa que te dan

	int miguitas_de_pan [filas][columnas]={{p,p,p,p,p,p},
	{p,0,0,0,0,p},
	{p,0,0,0,0,p},
	{p,0,0,0,0,p},
	{p,0,0,0,0,p},
	{p,p,p,p,p,p}};
	//mapa que vas a usar

	//en el caso de que te den el mapa se usa esto para leerlo
	/*for(int i=0;i<n;i++){
		for(int j=0;j<n;j++){
			scanf("%c", mapa_principal[i][j]);
		}
	}*/

	Posicion pos={1,1};//coordenadas del robot
	ll bateria=bateria_max;//bateria del robot
	ll hx=pos.x,hy=pos.y;//nos guardamos la base de carga
	ll direccion=0;//0 arriba, 1 esquina_arriba_derecha, 2 derecha, 3 esquina_abajo_derecha, 4 abajo, 5 esquina_abajo_izquierda, 6 izquierda, 7 esquina_arriba_izquierda
	ll atascado=0;//si llega a 8 es que esta atascado
	miguitas_de_pan[pos.x][pos.y]='1';//por aqui ya ha pasado


	while(atascado<=8/*&&bateria>bateria_max/2+1*/){
		int tx[8]={0,1,1,1,0,-1,-1,-1},ty[8]={1,1,0,-1,-1,-1,0,1};
		int dx=tx[direccion];
		int dy=ty[direccion];
		if (mapa_principal[pos.x+dx][pos.y+dy]!=p && miguitas_de_pan[pos.x+dx][pos.y+dy]!=1){//si no hay pared, si tiene bateria y si no ha pasado por ahi
			miguitas_de_pan[pos.x+dx][pos.y+dy]=1;//por aqui ya ha pasado
			pos.x+=dx;//se mueve
			pos.y+=dy;
			bateria--;//gasta bateria
			atascado=0;//resetea el contador de atascado
			limpiar_basura(mapa_principal,pos);
		}
		else{
			if(mapa_principal[pos.x+dx][pos.y+dy]==p){//si hay pared
				miguitas_de_pan[pos.x+dx][pos.y+dy]=p;//nos guardamos que hay una pared
			}
			giro;//si no puede avanzar gira
		}
	}

	imprimirTablero(mapa_principal);
	imprimirTablero(miguitas_de_pan);




	//printf("");
	//scanf("");
	return 0;
}

