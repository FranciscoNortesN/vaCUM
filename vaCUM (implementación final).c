#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>
#include "simula.h"

#define p -1
#define filas 30
#define columnas 50
#define dimension 3
#define bateria_max 1000.00

typedef struct {
	int x;
	int y;
} Posicion;

typedef enum {
	arriba,//0
	esquina_arriba_derecha,//1
	derecha,//2
	esquina_abajo_derecha,//3
	abajo,//4
	esquina_abajo_izquierda,//5
	izquierda,//6
	esquina_arriba_izquierda//7
}direcciones;

typedef long long ll;
typedef float f;
typedef double db;
typedef char c;

//se crean variables globales para que sean accesibles en todo el codigo y no tener que usar punteros en todas partes
direcciones direccion;//direccion del robot
ll atascado=0;//si llega a 8 es que esta atascado
Posicion pos;
Posicion base_de_carga;
ll miguitas_de_pan[filas][columnas][dimension]={0};
/*
dimensión=0: mapa donde con cada paso que se realiza en una casilla nueva, se deja un número mayor
dimensión=1: mapa donde con cada paso que se realiza en una casilla nueva, se deja un 1, con cada casilla ya visitada, se deja un 2 si se ha visitado 2 veces, un 3 si 3...
dimensión=2: mapa que guarda el rastro que deja la función de home_coming para luego regresar
*/
ll miguitas=1;
bool base=false;
ll rastro_home_coming=1;
int tx[8]={0,1,1,1,0,-1,-1,-1},ty[8]={-1,-1,0,1,1,1,0,-1};
int dx, dy; //variables donde se guarda tx y ty
ll ultima_direccion=0;

void giro45 (){
	direccion=(direccion+1)%8;//gira a la derecha 45 grados
	rmb_turn(M_PI/4);
}

void giro90 (){
	direccion=(direccion+2)%8;
	rmb_turn(M_PI/2);
}

void giro135 (){
	direccion=(direccion+3)%8;//gira a la derecha 135 grados
	rmb_turn(3*M_PI/4);
}

void giro180 (){
	direccion=(direccion+4)%8;//gira a la derecha 180 grados
	rmb_turn(M_PI);
}

void giromenos135 (){
	direccion=(direccion+5)%8;//gira a la izquierda 135 grados
	rmb_turn(-3*M_PI/4);
}

void giromenos90 (){
	direccion=(direccion+6)%8;//gira a la izquierda 90 grados
	rmb_turn(-M_PI/2);
}

void giromenos45 (){
	direccion=(direccion+7)%8;//gira a la izquierda 45 grados
	rmb_turn(-M_PI/4);
}

void avance (){
	rmb_forward();
	sensor_t rmb=rmb_state();
	pos.x=rmb.x;
	pos.y=rmb.y;
}

void limpiar_basura(){
	while(rmb_ifr()>0){
		rmb_clean();
	}
}

void girar(int optimo){
	switch(optimo){
		case 1:
			giro45();
			break;
		case 2:
			giro90();
			break;
		case 3:
			giro135();
			break;
		case 4:
			giro180();
			break;
		case 5:
			giromenos135();
			break;
		case 6:
			giromenos90();
			break;
		case 7:
			giromenos45();
			break;
	}
}

void home_coming(){//vuelta a la base a por batería
  
	int menor=-1;
	bool primera_vez=true;
	for (int j = 0; j < 8; j++){
		int i=(direccion+j)%8;
		if(miguitas_de_pan[pos.x+tx[i]][pos.y+ty[i]][0]!=p && primera_vez==true && miguitas_de_pan[pos.x+tx[i]][pos.y+ty[i]][0]!=0){
			menor=i;
			primera_vez=false;
		}
		else if (miguitas_de_pan[pos.x + tx[i]][pos.y + ty[i]][0] < miguitas_de_pan[pos.x + tx[menor]][pos.y + ty[menor]][0] && miguitas_de_pan[pos.x+tx[i]][pos.y+ty[i]][0]!=0){
			menor = i;
		}
	}
	miguitas_de_pan[pos.x][pos.y][2] = rastro_home_coming++;
	girar(menor);
	avance();
}

void far_from_home(){//regresa al sitio donde había entrado en reserva
	bool encontrado= false;
	int i=0, menor;
	while(encontrado==false){
		if(miguitas_de_pan[pos.x+tx[i]][pos.y+ty[i]][2]!=0 && miguitas_de_pan[pos.x+tx[i]][pos.y+ty[i]][0]!=p){
			menor=miguitas_de_pan[pos.x+tx[i]][pos.y+ty[i]][2];
			encontrado=true;
						break;//así no se incrementa cuando ya has encontrado la direccion óptima
		}
		i++;
	}    
	girar(i);//se orienta hacia la casilla menor 
}

void desatasco(){
	bool primera=true;
	int menor;
	int d;
	for(int j=7;j>=0;j--){
		int i=(direccion+j)%8;//es más facil que avance el for con la j, que cambiar todas las i por j
		if(miguitas_de_pan[pos.x+tx[i]][pos.y+ty[i]][1]>=0 && miguitas_de_pan[pos.x+tx[i]][pos.y+ty[i]][1]!=p && primera==true){
			menor=miguitas_de_pan[pos.x+tx[i]][pos.y+ty[i]][1];
			primera=false;
		}
		else if(miguitas_de_pan[pos.x+tx[i]][pos.y+ty[i]][1]<menor && miguitas_de_pan[pos.x+tx[i]][pos.y+ty[i]][1]!=p){
			menor=miguitas_de_pan[pos.x+tx[i]][pos.y+ty[i]][1];
			d=i;
		}
	}
	girar(d);//se orienta hacia la casilla menor
}

/*void explorar(){
	if(miguitas_de_pan[pos.x+tx[(direccion+2)%8]][pos.y+ty[(direccion+2)%8]][0]==0){//gira a la derecha, si no, a la izquierda
		giro90();
		atascado+=2;
	}
	else{
		giromenos90();
		atascado+=6;
	}  
}*/

void empareda2(){//rellena los tres mapas con las paredes
	for(int i=0; i<filas; i++){
		for(int j=0; j<dimension; j++){
			miguitas_de_pan[i][0][j]=p;
			miguitas_de_pan[i][49][j]=p;
		}
	}
	for(int i=0; i<columnas; i++){
		for(int j=0; j<dimension; j++){
			miguitas_de_pan[0][i][j]=p;
			miguitas_de_pan[29][i][j]=p;
		}
	}
}

void on_start(){
	rmb_awake(&base_de_carga.x, &base_de_carga.y);
	empareda2();
	sensor_t rob_state = rmb_state();
	int d1 = rob_state.head/(M_PI/4);
	int d=d1+8;
	direccion=(d)%8;
    pos.x=rob_state.x;
    pos.y=rob_state.y;
    miguitas_de_pan[pos.x][pos.y][0]=miguitas++;//indica que por aqui ya ha pasado y en que momento
	miguitas_de_pan[pos.x][pos.y][1]++;//se guarda el número de veces que se ha pasado por ahi
	ultima_direccion=direccion;
	giro90();
	//info sobre las variables locales para tener de forma mas sencilla el encontrar errores
	direcciones local_direccion=direccion;
	direcciones local_ultima_direccion=ultima_direccion;
	Posicion local_pos=pos;
	Posicion local_base_de_carga=base_de_carga;
	int local_atascado=atascado;
	bool local_base=base;
	ll local_miguitas=miguitas;
	ll local_rastro_home_coming=rastro_home_coming;
	int local_dx=dx;
	int local_dy=dy;
	int local_at_base=rmb_at_base();
	ll local_bateria=rmb_battery();
	int local_bumper=rmb_bumper();
	sensor_t local_rob_state=rmb_state();
}


void cyclic_behav(){
	if(rmb_battery()>bateria_max/2 && base==false ){//si la bateria es mayor a la mitad, y si no está en el algoritmo de volver a la base y devuelta
	//la condición se puede optimizar calculando cuendo realmente es necesario volver
		if(atascado<8){//como con la roomba normal, se puede quedar atascado, hay que comprobar que no se atasque
			dx=tx[direccion];
			dy=ty[direccion];
			if (rmb_bumper()==0 && miguitas_de_pan[pos.x+dx][pos.y+dy][0]==0){//si no hay pared y si no ha pasado por ahi
				rmb_forward();//se mueve
				pos.x+=dx;//se guarda a donde se ha movido segun las x
				pos.y+=dy;//se guarda a donde se ha movido segun las y
				limpiar_basura();
				miguitas_de_pan[pos.x][pos.y][0]=miguitas++;//indica que por aqui ya ha pasado y en que momento
				miguitas_de_pan[pos.x][pos.y][1]++;//se guarda el número de veces que se ha pasado por ahi
				atascado=0;//resetea el contador de atascado porque se ha podido mover
			}
			else{
				if(rmb_bumper()==1){
					for(int i=0; i<dimension; i++){
						miguitas_de_pan[pos.x][pos.y][i]=p;//se guarda que hay una pared en la casilla donde se ha pasado
					}
				}
				//explorar();//gira hacia la primera casilla donde ha encontrado un cero en sentido horario es decir, gira a una casilla libre
				giro90();
				atascado+=2;
			}
		}
		else{
			desatasco();
			avance();
			if(miguitas_de_pan[pos.x][pos.y][0]==0){//si ha llegado a un lugar donde no ha pasado anterioremente, es que se ha desatascado
				atascado=0;
			}
		}
	}
	else if(rmb_battery()<bateria_max/2 && !rmb_at_base()){
		//aquí habría que guardarse la direccion con la que se ha quedado
		home_coming();
		base=true;
	}
	else if(rmb_at_base() && rmb_battery()<bateria_max-10){
		base=true;//por si no se ha puesto a true cuando estaba de camino a la base
		while(rmb_battery()<bateria_max-10)//menos 10, para no rellenar de más
			rmb_load();
	}
	else if (base==true){//batería recargada y pendiente de volver donde lo había dejado
		if(miguitas_de_pan[pos.x][pos.y][2]!=1){//si no ha llegado todavía
			far_from_home();//sigue el rastro
			rmb_forward();
		}
				else{
			base=false;//esto permite que se inicie el algoritmo de busqueda principal
		}
	}
	if (base==false){
		ultima_direccion=direccion;
	}
	//información sobre las variables útil para encontrar errores
	direcciones local_direccion=direccion;
	direcciones local_ultima_direccion=ultima_direccion;
	Posicion local_pos=pos;
	Posicion local_base_de_carga=base_de_carga;
	int local_atascado=atascado;
	bool local_base=base;
	ll local_miguitas=miguitas;
	ll local_rastro_home_coming=rastro_home_coming;
	int local_dx=dx;
	int local_dy=dy;
	int local_at_base=rmb_at_base();
	ll local_bateria=rmb_battery();
	int local_bumper=rmb_bumper();
	sensor_t local_rob_state=rmb_state();
}

void on_stop(){
	visualize();
}


int main(){
	configure(on_start, cyclic_behav, on_stop, 500);
	run();
	return 0;
}
//que haces?

//yo?(fran)
//si te refieres a mi, copio el código para poder ejecutarlo
