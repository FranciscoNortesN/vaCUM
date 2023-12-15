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


enum direcciones {
	arriba,
	esquina_arriba_derecha,
	derecha,
	esquina_abajo_derecha,
	abajo,
	esquina_abajo_izquierda,
	izquierda,
	esquina_arriba_izquierda
};
/*si la posicion del robot es x, con lo que viene ahora representamos las posiciones relativas a x
701
6x2
543
*/

//macros
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

void giro (enum direcciones *direccion){//gira a la derecha 45 grados y suma 1 al contador de atascado (si llega a 8 es que esta atascado)
	if (*direccion==arriba) *direccion=esquina_arriba_derecha;
	else if (*direccion==esquina_arriba_derecha) *direccion=derecha;
	else if (*direccion==derecha) *direccion=esquina_abajo_derecha;
	else if (*direccion==esquina_abajo_derecha) *direccion=abajo;
	else if (*direccion==abajo) *direccion=esquina_abajo_izquierda;
	else if (*direccion==esquina_abajo_izquierda) *direccion=izquierda;
	else if (*direccion==izquierda) *direccion=esquina_arriba_izquierda;
	else if (*direccion==esquina_arriba_izquierda) *direccion=arriba;
}

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
	{p,0,0,0,0,p},
	{p,0,p,0,0,p},
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
	enum direcciones direccion=0;//0 arriba, 1 esquina_arriba_derecha, 2 derecha, 3 esquina_abajo_derecha, 4 abajo, 5 esquina_abajo_izquierda, 6 izquierda, 7 esquina_arriba_izquierda
	ll atascado=0;//si llega a 8 es que esta atascado
	miguitas_de_pan[pos.x][pos.y]=1;//por aqui ya ha pasado
	ll casillas_recorridas=0;//casillas recorridas
	ll casillas_por_recorrer=(filas-2)*(columnas-2);//casillas por recorrer

	while(casillas_recorridas<=casillas_por_recorrer){
		int tx[8]={0,1,1,1,0,-1,-1,-1},ty[8]={1,1,0,-1,-1,-1,0,1};
		int dx=tx[direccion];
		int dy=ty[direccion];
		if (mapa_principal[pos.x+dx][pos.y+dy]!=p && miguitas_de_pan[pos.x+dx][pos.y+dy]!=1){//si no hay pared, si tiene bateria y si no ha pasado por ahi
			miguitas_de_pan[pos.x+dx][pos.y+dy]=1;//por aqui ya ha pasado
			pos.x+=dx;//se mueve
			pos.y+=dy;
			bateria--;//gasta bateria
			atascado=0;//resetea el contador de atascado
			printf_pos_map;
			casillas_recorridas++;
			limpiar_basura(mapa_principal,pos);
		}
		else{
			if(mapa_principal[pos.x+dx][pos.y+dy]==p){//si hay pared
				if(miguitas_de_pan[pos.x+dx][pos.y+dy]!=p)
					casillas_por_recorrer--;
				miguitas_de_pan[pos.x+dx][pos.y+dy]=p;//nos guardamos que hay una pared
			}
			giro(&direccion);//gira a la derecha
			atascado++;//suma 1 al contador de atascado
			giro(&direccion);//gira a la derecha
		}
	}

	imprimirTablero(mapa_principal);
	imprimirTablero(miguitas_de_pan);




	//printf("");
	//scanf("");
	return 0;
}

