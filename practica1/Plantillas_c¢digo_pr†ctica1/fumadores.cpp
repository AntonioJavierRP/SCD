// *****************************************************************************
//
// Prácticas de SCD. Práctica 1.
// Plantilla de código para el ejercicio de los fumadores
//
// *****************************************************************************

#include <iostream>
#include <cassert>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>      // incluye "time(....)"
#include <unistd.h>    // incluye "usleep(...)"
#include <stdlib.h>    // incluye "rand(...)" y "srand"

using namespace std ;

sem_t puede_producir; //cada semáforo se corresponde con un fumador y el productor
sem_t puede_fumar[3];
sem_t mutex;

// ---------------------------------------------------------------------
// introduce un retraso aleatorio de duración comprendida entre
// 'smin' y 'smax' (dados en segundos)

void retraso_aleatorio( const float smin, const float smax ){
  static bool primera = true ;
  if ( primera ){        // si es la primera vez: 
    srand(time(NULL)); //   inicializar la semilla del generador
    primera = false ;  //   no repetir la inicialización
  }
  // calcular un número de segundos aleatorio, entre {\ttbf smin} y {\ttbf smax}
  const float tsec = smin+(smax-smin)*((float)random()/(float)RAND_MAX);
  // dormir la hebra (los segundos se pasan a microsegundos, multiplicándos por 1 millón)
  usleep( (useconds_t) (tsec*1000000.0)  );
}

// ----------------------------------------------------------------------------
// función que simula la acción de fumar, como un retardo aleatorio de la hebra.
// recibe como parámetro el numero de fumador
// el tiempo que tarda en fumar está entre dos y ocho décimas de segundo.

void * fumar( unsigned long num_fumador){
  
  sem_wait(&mutex);
  cout << "Fumador número " << num_fumador << ": comienza a fumar." << endl << flush ;
  sem_post(&mutex);
  
  retraso_aleatorio( 0.2, 0.8 );
  
  sem_wait(&mutex);
  cout << "Fumador número " << num_fumador << ": termina de fumar." << endl << flush ;
  sem_post(&mutex);

}
// ----------------------------------------------------------------------------

// falta: resto de funciones
// función para producir un dato aleatorio en el rango de 0 a 2
unsigned int producir_dato(){
  unsigned int dato = (rand() % 3U);  //producir un numero aleatorio de 0 a 2

  retraso_aleatorio( 0.1, 0.5 );

  return dato;
}

void * funcion_estanquero(void *){
  while(true){

    sem_wait(&puede_producir);
    
    unsigned int dato = producir_dato();
    
    sem_wait(&mutex);
    cout << "Producido el ingrediente: " << dato << endl;
    sem_post(&mutex);
    
    sem_post(&puede_fumar[dato]);

  }

  return NULL;
}

void * funcion_fumador(void * num_fumador){
  unsigned long num_fum = (unsigned long) num_fumador;

  while(true){

    sem_wait(&puede_fumar[num_fum]);

    sem_post(&puede_producir);
    
    fumar(num_fum);   

  }
  return NULL;
}
// ----------------------------------------------------------------------------

int main(){
  srand( time(NULL) ); // inicializa semilla aleatoria para selección aleatoria de fumador
  
  pthread_t fumador_1, fumador_2, fumador_3, estanquero; // vector de hebras, cada hebra es un fumador
   
  pthread_create( &fumador_1, NULL, funcion_fumador, (void *)0);
  pthread_create( &fumador_2, NULL, funcion_fumador, (void *)1);
  pthread_create( &fumador_3, NULL, funcion_fumador, (void *)2);
  pthread_create( &estanquero, NULL, funcion_estanquero, NULL);

  //inicializo los semáforos
  //primer semáforo, fumador0 puede_fumar0, no puede fumar porque le falta elemento 2
  sem_init(&puede_fumar[0],0,0);
  //segundo semaforo, fumador1 puede_fumar1, no puede fumar porque le falta elemento 0
  sem_init(&puede_fumar[1],0,0);
  //tercer semaforo, fumador2 puede_fumar2, no puede fumar porque le falta elemento 1
  sem_init(&puede_fumar[2],0,0);
  //cuarto semaforo, puede_producir, produce un elemento aleatorio
  sem_init(&puede_producir,0,1);
  sem_init(&mutex,0,1);

  pthread_join(fumador_1,NULL);
  pthread_join(fumador_2,NULL);
  pthread_join(fumador_3,NULL);
  pthread_join(estanquero,NULL);

  sem_destroy(&puede_producir);
  sem_destroy(&puede_fumar[0]);
  sem_destroy(&puede_fumar[1]);
  sem_destroy(&puede_fumar[2]);

  return 0 ;
}
