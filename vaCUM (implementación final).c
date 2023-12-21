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
#define BATERIA_MAX 1000

struct coor{
  int x, y;
};

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

direcciones direccion;//direccion del robot
ll atascado=0;//si llega a 8 es que esta atascado
Posicion pos;
ll miguitas_de_pan[filas][columnas][dimension]={0};
int tx[8]={0,1,1,1,0,-1,-1,-1},ty[8]={1,1,0,-1,-1,-1,0,1};

void giro45 (){
  direccion=(direccion+1)%8;//gira a la derecha 45 grados
  rmb_turn(M_PI/4);
}

void giro90 (){
  direccion=(direccion+2)%8;//gira a la derecha 90 grados
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

void limpiar_basura(){
  while(rmb_ifr()>0){
    rmb_clean();
  }
}

void desatasco(){
  bool primera=true;
  int menor;
  for(int i=8;i>=0;i--){
    if(miguitas_de_pan[pos.x+tx[i]][pos.y+ty[i]][0]>=0 && miguitas_de_pan[pos.x+tx[i]][pos.y+ty[i]][0]!=p && primera==true){
      menor=miguitas_de_pan[pos.x+tx[i]][pos.y+ty[i]][0];
      primera=false;
    }
    else if(miguitas_de_pan[pos.x+tx[i]][pos.y+ty[i]][0]<menor){
      menor=miguitas_de_pan[pos.x+tx[i]][pos.y+ty[i]][0];
    }
  }
}

void explorar(){
  if(miguitas_de_pan[pos.x+tx[2]][pos.y+ty[2]][0]==0){//gira a la derecha, si no, a la izquierda
    giro90();
    atascado+=2;
  }
  else{
    giromenos90();
    atascado+=6;
  }  
}

void empareda(){//rellena los tres mapas con las paredes
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
  rmb_awake(&pos.x, &pos.y);
  empareda();
  sensor_t rob_state = rmb_state();
  direccion=rob_state.head/90+6;
  giro90();
}


void cyclic_behav(){
  //da un paso en la dirección actual
  if (rmb_bumper()==0 && miguitas_de_pan[pos.x+dx][pos.y+dy][0]==0 && rmb_battery<BATERIA_MAX/2){//si no hay pared, si tiene bateria y si no ha pasado por ahi
    rmb_forward();//se mueve
    limpiar_basura();
    int dx=tx[direccion];
    int dy=ty[direccion];
    pos.x+=dx;
    pos.y+=dy;
    miguitas_de_pan[pos.x][pos.y][0]=miguitas++;//por aqui ya ha pasado
    atascado=0;//resetea el contador de atascado
  }
  else if(rmb_bumper()==1 && bateria<BATERIA_MAX/2){
    explorar();//gira hacia la primera casilla donde ha encontrado un cero en sentido horario
  }
  //deteccion de suciedad
  limpiar_basura();
  //carga si se encuentra en la base
  if(rmb_at_base())
    while(rmb_battery() < 250)
      rmb_load();
}


void on_stop(){
  visualize();
}


int main(){
  configure(on_start, cyclic_behav, on_stop, 2500);
  run();
  return 0;
}
