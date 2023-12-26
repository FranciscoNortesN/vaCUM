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

typedef enum : long long {
//es totalmente inutil especificarlo pero lo pongo para que conozcais que con los dos puntos puedes especificar el tipo de datos
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
bool full=true;
ll rastro_home_coming=1;
int tx[8]={0,1,1,1,0,-1,-1,-1},ty[8]={1,1,0,-1,-1,-1,0,1};
int dx, dy; //variables donde se guarda tx y ty
ll ultima_direccion=0;

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
    int menor = 2, j;
    if (rmb_battery() < (bateria_max/2)){
        for (int i = 0; i < 8; i++){
            if (miguitas_de_pan[pos.x + tx[i]][pos.y + ty[i]][0] < miguitas_de_pan[pos.x + tx[menor]][pos.y + ty[menor]][0]){   
                menor = i;
            }
        }
        miguitas_de_pan[pos.x][pos.y][2] = rastro_home_coming++;
    }  
    girar(menor);
    rmb_forward();
}

void far_from_home(){//regresa al sitio donde había entrado en reserva
    bool encontrado= false;
    int i=0, menor;
    while(encontrado==false){
        if(miguitas_de_pan[pos.x+tx[i]][pos.y+ty[i]][2]!=0 && miguitas_de_pan[pos.x+tx[i]][pos.y+ty[i]][0]!=p){
            menor=miguitas_de_pan[pos.x+tx[i]][pos.y+ty[i]][2];
            encontrado=true;
        }
        i++;
    }    
    girar(i);//se orienta hacia la casilla menor 
}

void desatasco(){
    bool primera=true;
    int menor;
    int d;
    for(int i=7;i>=0;i--){
        i=(direccion+i)%8;
        if(miguitas_de_pan[pos.x+tx[i]][pos.y+ty[i]][1]>=0 && miguitas_de_pan[pos.x+tx[i]][pos.y+ty[i]][1]!=p && primera==true){
            menor=miguitas_de_pan[pos.x+tx[i]][pos.y+ty[i]][1];
            primera=false;
        }
        else if(miguitas_de_pan[pos.x+tx[i]][pos.y+ty[i]][1]<menor){
            menor=miguitas_de_pan[pos.x+tx[i]][pos.y+ty[i]][1];
            d=i;
        }
    }
    girar(d);//se orienta hacia la casilla menor
}

void explorar(){
    if(miguitas_de_pan[pos.x+tx[(direccion+2)%8]][pos.y+ty[(direccion+2)%8]][0]==0){//gira a la derecha, si no, a la izquierda
        giro90();
        atascado+=2;
    }
    else{
        giromenos90();
        atascado+=6;
    }  
}

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
    rmb_awake(&pos.x, &pos.y);
    rmb_awake(&base_de_carga.x, &base_de_carga.y);
    empareda2();
    sensor_t rob_state = rmb_state();
    int d = rob_state.head/90+6;
    direccion=d%8;
    giro90();
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
                explorar();//gira hacia la primera casilla donde ha encontrado un cero en sentido horario es decir, gira a una casilla libre
            }
        }
        else{
            desatasco();
            dx=tx[direccion];//desde aquí hasta rmb_forward() lo que se hace es avanzar y quedarse con la nueva dirección
            dy=ty[direccion];
            pos.x+=dx;
            pos.y+=dy;
            rmb_forward();
            if(miguitas_de_pan[pos.x][pos.y][1]==0){//si ha llegado a un lugar donde no ha pasado anterioremente, es que se ha desatascado
                atascado=0;
            }
        }
    }
    else if(rmb_battery()<bateria_max/2 && !rmb_at_base()){
        //aquí habría que guardarse la direccion con la que se ha quedado
        home_coming();
        base=true;
    }
    else if(rmb_at_base() && full==false){
        while(rmb_battery() < 250)
            rmb_load();
            base=true;
            if(rmb_battery()>=bateria_max-10)
                full=true;
    }
    else if (full==true){//batería recargada y pendiente de volver donde lo había dejado
        if(miguitas_de_pan[pos.x][pos.y][2]!=1){//si no ha llegado todavía
            far_from_home();//sigue el rastro
            rmb_forward();
        }
       else{
            base=false;//esto permite que se inicie el algoritmo de busqueda principal
            full=false;//restauramos también este valor
        }
    }
    if (base==false){
        ultima_direccion=direccion;
    }
}

void on_stop(){
    visualize();
}


int main(){
    configure(on_start, cyclic_behav, on_stop, 2500);
    run();
    return 0;
}
