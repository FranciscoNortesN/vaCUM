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
{p,0,p,0,0,p},
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

void giromenos90 (){
	direccion=(direccion+6)%8;//gira a la derecha 270 grados
}

void giromenos45 (){
	direccion=(direccion+7)%8;//gira 45 grados a la izquierda
}

void limpiar_basura(){
	while(mapa_principal[pos.x][pos.y]>0){
		mapa_principal[pos.x][pos.y]--;
		printf_pos_map();
	}
}

void movimiento(int dx, int dy){
	miguitas_de_pan[pos.x+dx][pos.y+dy]=1;//por aqui ya ha pasado
	pos.x+=dx;//se mueve
	pos.y+=dy;
	bateria--;//gasta bateria
	atascado=0;//resetea el contador de atascado
	printf_pos_map();
	casillas_recorridas++;
	limpiar_basura();
}

void comprobar_pared(int dx, int dy){
	if(mapa_principal[pos.x+dx][pos.y+dy]==p){//si hay pared
		if(miguitas_de_pan[pos.x+dx][pos.y+dy]!=p){//si de antes no se sabia que habia una pared
			casillas_por_recorrer--;
			printf("x%d, y%d, %lld\n", pos.x+dx, pos.y+dy, (long long int)mapa_principal[pos.x+dx][pos.y+dy]);
		}
		miguitas_de_pan[pos.x+dx][pos.y+dy]=p;//nos guardamos que hay una pared
	}
}

// Función para verificar si una posición está dentro de los límites del tablero
bool dentroDeLimites(int x, int y) {
    return (x >= 0 && x < filas && y >= 0 && y < columnas);
}

void movimiento_normal(int dx,int dy){
	if (mapa_principal[pos.x+dx][pos.y+dy]!=p && miguitas_de_pan[pos.x+dx][pos.y+dy]!=1){//si no hay pared, si tiene bateria y si no ha pasado por ahi
		movimiento(dx,dy);
	}
	else{
		comprobar_pared(dx,dy);
		giro90();//gira 90 grados a la derecha
		atascado++;//suma 1 al contador de atascado
	}
}

ll buscar(){//busca una casilla por la que no haya pasado
	for(int i=0;i<8;i++){
		int dx=tx[i];
		int dy=ty[i];
		if(miguitas_de_pan[pos.x+dx][pos.y+dy]==0){
			return i;
		}
	}
	return -1;
}

void desatasco(){
	giro180();//gira 180 grados
	while(miguitas_de_pan[pos.x][pos.y]!=0&&casillas_recorridas<casillas_por_recorrer){//mientras no haya pasado por ahi
		int dx=tx[buscar()];
		int dy=ty[buscar()];
		comprobar_pared(dx,dy);
		if(buscar()!=-1){
			movimiento(dx,dy);
			break;
		}
		else{
			dx=tx[direccion];
			dy=ty[direccion];
			if(mapa_principal[pos.x+dx][pos.y+dy]!=p&&miguitas_de_pan[pos.x+dx][pos.y+dy]!=0){
				movimiento(dx,dy);
			}
			else{
				comprobar_pared(dx,dy);
				giromenos90();//gira 90 grados a la izquierda
			}
		}
	}
}

int main(int argc, char *argv[])
{
	srand(time(NULL));

	miguitas_de_pan[pos.x][pos.y]=1;//por aqui ya ha pasado
	
	while(casillas_recorridas<=casillas_por_recorrer){
		int dx=tx[direccion];
		int dy=ty[direccion];
		if(atascado<=8){//como el robot se movería cuando va por nuevas zonas
			movimiento_normal(dx,dy);
		}
		else{
			desatasco();
			casillas_recorridas++;
		}
	}

	imprimirTablero(mapa_principal);
	imprimirTablero(miguitas_de_pan);




	//printf("");
	//scanf("");
	return 0;
}
