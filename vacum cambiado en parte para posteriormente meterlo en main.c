#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdbool.h>
#include "simula.h"

//constantes
#define filas 30
#define columnas 50
#define dimension 3
#define condicion 1 //acabar el mapa
#define bateria_max 1000000000 //para que sobre bateria
#define p -1 //pared
#define posicion_inicial_x 1
#define posicion_inicial_y 1
#define M_PI 3.14159265358979323846264338327950288419716939937510

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
ll miguitas=1;

ll miguitas_de_pan [filas][columnas][dimension];
//mapa que vas a usar

void giro45 (){
	direccion=(direccion+1)%8;//gira a la derecha 45 grados
	rmb_turn(M_PI/4);
	atascado++;//suma 1 al contador de atascado
}

void giro90 (){
	direccion=(direccion+2)%8;//gira a la derecha 90 grados
	rmb_turn(M_PI/2);
	atascado+=2;//suma 2 al contador de atascado
	//porque ha girado 45 grados 2 veces
}

void giro135 (){
	direccion=(direccion+3)%8;//gira a la derecha 135 grados
	rmb_turn(3*M_PI/4);
	atascado+=3;//suma 3 al contador de atascado
	//porque ha girado 45 grados 3 veces
}

void giro180 (){
	direccion=(direccion+4)%8;//gira a la derecha 180 grados
	rmb_turn(M_PI);
	atascado+=4;//suma 4 al contador de atascado
	//porque ha girado 90 grados 2 veces
}

void giromenos135 (){
	direccion=(direccion+5)%8;//gira a la derecha 225 grados
	rmb_turn(5*M_PI/4);
	atascado+=5;//suma 5 al contador de atascado
	//porque ha girado 45 grados 5 veces
}


void giromenos90 (){
	direccion=(direccion+6)%8;//gira a la derecha 270 grados
	rmb_turn(3*M_PI/2);
	atascado+=6;//suma 6 al contador de atascado
	//porque ha girado 90 grados 3 veces
}

void giromenos45 (){
	direccion=(direccion+7)%8;//gira 45 grados a la izquierda
	rmb_turn(-M_PI/4);
	atascado+=7;//suma 7 al contador de atascado
	//porque ha girado 45 grados 7 veces
}

void limpiar_basura(){
	while(rmb_ifr>0){
		rmb_clean();
	}
}

/*void movimiento(int dx, int dy){
	miguitas_de_pan[pos.x+dx][pos.y+dy]=1;//por aqui ya ha pasado
	pos.x+=dx;//se mueve
	pos.y+=dy;
	bateria--;//gasta bateria
	atascado=0;//resetea el contador de atascado
	printf_pos_map();
	casillas_recorridas++;
	limpiar_basura();
}*/

/*void comprobar_pared(int dx, int dy){
	if(mapa_principal[pos.x+dx][pos.y+dy]==p){//si hay pared
		if(miguitas_de_pan[pos.x+dx][pos.y+dy]!=p){//si de antes no se sabia que habia una pared
			casillas_por_recorrer--;
			printf("x%d, y%d, %lld\n", pos.x+dx, pos.y+dy, (long long int)mapa_principal[pos.x+dx][pos.y+dy]);
		}
		miguitas_de_pan[pos.x+dx][pos.y+dy]=p;//nos guardamos que hay una pared
	}
}*/

// Función para verificar si una posición está dentro de los límites del tablero
bool dentroDeLimites(int x, int y) {
    return (x >= 0 && x < filas && y >= 0 && y < columnas);
}

void movimiento_normal(int dx,int dy){
	if (rmb_bumper()==0 && miguitas_de_pan[pos.x+dx][pos.y+dy][0]==0){//si no hay pared, si tiene bateria y si no ha pasado por ahi
		rmb_forward();//se mueve
		limpiar_basura();
		int dx=tx[direccion];
		int dy=ty[direccion];
		pos.x+=dx;
		pos.y+=dy;
		miguitas_de_pan[pos.x][pos.y][0]=miguitas++;//por aqui ya ha pasado
		atascado=0;//resetea el contador de atascado
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
	
}

int main(int argc, char *argv[])
{
	srand(time(NULL));

	miguitas_de_pan[pos.x][pos.y][0]=1;//por aqui ya ha pasado
	
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

	imprimirTablero(miguitas_de_pan);




	//printf("");
	//scanf("");
	return 0;
}
