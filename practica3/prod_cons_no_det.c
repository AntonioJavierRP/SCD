/*Productor consumidor con selección no determinista*/

#include <mpi.h>
#include <iostream>
#include <math.h>
#include <time.h>      // incluye "time"
#include <unistd.h>    // incluye "usleep"
#include <stdlib.h>    // incluye "rand" y "srand"

// ---------------------------------------------------------------------

#define TAM 			    5
#define PRODUCTORES 	4
#define CONSUMIDORES 	5

using namespace std;

// ---------------------------------------------------------------------

void productor(int tag){ 
 for ( unsigned int i= 0 ; i < 20/5 ; i++ ){ 
  cout << "Productor" << tag << " produce valor " << i << endl << flush;
  // espera bloqueado durante un intervalo de tiempo aleatorio 
  // (entre una décima de segundo y un segundo)
  usleep( 1000U * (100U+(rand()%900U)) ); 
  
  // enviar valor
  MPI_Ssend( &i, 1, MPI_INT, TAM, 0, MPI_COMM_WORLD );
 }
}
// ---------------------------------------------------------------------

void buffer(){
 int value[TAM] ,peticion, pos = 0, rama; 
 MPI_Status  status;
 
 for (unsigned int i = 0 ; i < 20*2 ; i++ ){ 
 	if(pos == 0)
 		rama = 0;	//el consumidor no consume
 	else{
 		if (pos == TAM)
 		{
 			rama = 1;	//el productor no puede producir
 		}
 		else{
 			//Se puede consumir y producir
 			MPI_Probe(MPI_ANY_SOURCE,MPI_ANY_TAG,MPI_COMM_WORLD, &status);
 			if(status.MPI_SOURCE <= 4)
 				rama = 0;
 			else
 				rama = 1;
 		}
 	}
 	switch (rama){
 		case 0:
 			MPI_Recv(&value[pos], 1, MPI_INT,MPI_ANY_SOURCE, 0 , MPI_COMM_WORLD,&status);
 			cout << "Buffer recibe " << value[pos] << " del Productor " << status.MPI_TAG << endl << flush;
 			pos++;
 			break;
 		case 1:
 			MPI_Recv(&peticion,1,MPI_INT,MPI_ANY_SOURCE,1,MPI_COMM_WORLD,&status);
 			MPI_Ssend(&value[pos-1],1,MPI_INT,status.MPI_SOURCE,0,MPI_COMM_WORLD);
 			cout << "Buffer envía " << value[pos - 1] << " al Consumidor " << status.MPI_TAG << endl << flush;
      pos--;
      break;
 	}
 }
}  
// ---------------------------------------------------------------------  

void consumidor(int tag){
 int         value,
             peticion = 1 ; 
 float       raiz ;
 MPI_Status  status ;
 
 for (unsigned int i=0;i < 20/4;i++){
  MPI_Ssend(&peticion, 1, MPI_INT, TAM, 1, MPI_COMM_WORLD); 
  MPI_Recv(&value, 1,     MPI_INT, TAM, 0, MPI_COMM_WORLD,&status );
  
  cout << "Consumidor" << tag <<" recibe valor " << value << " de Buffer " << endl << flush ;
  
  // espera bloqueado durante un intervalo de tiempo aleatorio 
  // (entre una décima de segundo y un segundo)
  usleep( 1000U * (100U+(rand()%900U)) ); 
  
  // calcular raíz del valor recibido
  raiz = sqrt( value);
  cout << "Consumidor -> raiz cuadrada de " << value << " = " << raiz << endl << flush;
 }
}
// ---------------------------------------------------------------------

int main( int argc, char *argv[] ) {
 int rank ,  // identificador de proceso (comienza en 0)
     size ;  // numero de procesos
    
 // inicializar MPI y leer identificador de proceso y número de procesos
 MPI_Init( &argc, &argv );
 MPI_Comm_rank( MPI_COMM_WORLD, &rank );
 MPI_Comm_size( MPI_COMM_WORLD, &size );

 // inicializa la semilla aleatoria:
 srand ( time(NULL) );
 if (size != 10) {
  cout << "El numero de procesos debe ser 10 " << endl;
  return 0;
 }
 // verificar el identificador de proceso (rank), y ejecutar la
 // operación apropiada a dicho identificador
 switch(rank){
 	case 0:
 		productor(0);
 		break;
 	case 1:
 		productor(1);
 		break;
 	case 2:
 		productor(2);
 		break;
 	case 3:
 		productor(3);
 		break;
 	case 4:
 		productor(4);
 		break;
 	case 5:
 		buffer();
 		break;
 	case 6:
 		consumidor(6);
 		break;
 	case 7:
 		consumidor(7);
 		break;
 	case 8:
 		consumidor(8);
 		break;
 	case 9:
 		consumidor(9);
 		break;
 }
 
 // al terminar el proceso, finalizar MPI
 MPI_Finalize( );

 return 0;
}
// ---------------------------------------------------------------------
