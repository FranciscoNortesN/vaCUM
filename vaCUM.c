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
#define posicion_inicial_x 1
#define posicion_inicial_y 1

typedef long long ll;
typedef float f;
typedef double db;
typedef char c;

typedef struct {
    int x;
    int y;
} Posicion;

typedef enum {
	arriba,
	esquina_arriba_derecha,
	derecha,
	esquina_abajo_derecha,
	abajo,
	esquina_abajo_izquierda,
	izquierda,
	esquina_arriba_izquierda
}direcciones;
/*si la posicion del robot es x, con lo que viene ahora representamos las posiciones relativas a x
701
6x2
543
*/

ll casillas_recorridas=0;//casillas recorridas
ll casillas_por_recorrer=(filas-2)*(columnas-2);//casillas por recorrer
direcciones direccion=arriba;//direccion del robot
Posicion pos={posicion_inicial_x,posicion_inicial_y};//coordenadas del robot
int tx[8]={0,1,1,1,0,-1,-1,-1},ty[8]={1,1,0,-1,-1,-1,0,1};//para moverse en las 8 direcciones
ll bateria=bateria_max;//bateria del robot
ll hx=posicion_inicial_x, hy=posicion_inicial_y; // nos guardamos la base de carga
ll atascado=0;//si llega a 8 es que esta atascado

ll mapa_principal [filas][columnas]={{p,p,p,p,p,p},
{p,0,0,0,0,p},
{p,0,0,3,0,p},
{p,0,0,0,0,p},
{p,0,p,0,0,p},
{p,p,p,p,p,p}};
//mapa que te dan

ll miguitas_de_pan [filas][columnas]={{p,p,p,p,p,p},
{p,0,0,0,0,p},
{p,0,0,0,0,p},
{p,0,0,0,0,p},
{p,0,0,0,0,p},
{p,p,p,p,p,p}};
//mapa que vas a usar

void coger_mapa_principal (){
	for(int i=0;i<filas;i++){
		for(int j=0;j<columnas;j++){
			scanf("%lld", &mapa_principal[i][j]);
		}
	}
}

void imprimirTablero(ll tablero[filas][columnas]) {//imprime el tablero
    for (int i = 0; i < filas; i++) {
        for (int j = 0; j < columnas; j++) {
            printf("%2lld ", tablero[i][j]);
        }
        printf("\n");
    }
}

void printf_pos_map (){
	printf("x%d, y%d, %lld\n", pos.x, pos.y, (long long int)mapa_principal[pos.x][pos.y]);
}

void printf_pos_mig (){
	printf("x%d, y%d, %lld\n", pos.x, pos.y, (long long int)miguitas_de_pan[pos.x][pos.y]);
}

void giro45 (){
	direccion=(direccion+1)%8;//gira a la derecha 45 grados
}

void giro90 (){
	direccion=(direccion+2)%8;//gira a la derecha 90 grados
}

void giro180 (){
	direccion=(direccion+4)%8;//gira a la derecha 180 grados
}

void limpiar_basura(){
	while(mapa_principal[pos.x][pos.y]>0){
		mapa_principal[pos.x][pos.y]--;
		printf_pos_map;
	}
}

// Función para verificar si una posición está dentro de los límites del tablero
bool dentroDeLimites(int x, int y) {
    return (x >= 0 && x < filas && y >= 0 && y < columnas);
}

int main(int argc, char *argv[])
{
	srand(time(NULL));

	miguitas_de_pan[pos.x][pos.y]=1;//por aqui ya ha pasado
	
	while(casillas_recorridas<=casillas_por_recorrer){
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
			limpiar_basura();
		}
		else{
			if(mapa_principal[pos.x+dx][pos.y+dy]==p){//si hay pared
				//if(miguitas_de_pan[pos.x+dx][pos.y+dy]!=p)
					casillas_por_recorrer--;
				miguitas_de_pan[pos.x+dx][pos.y+dy]=p;//nos guardamos que hay una pared
			}
			giro90();//gira 90 grados a la derecha
			atascado++;//suma 1 al contador de atascado
		}
	}

	imprimirTablero(mapa_principal);
	imprimirTablero(miguitas_de_pan);




	//printf("");
	//scanf("");
	return 0;
}
