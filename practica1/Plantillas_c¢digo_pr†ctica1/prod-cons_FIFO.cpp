// *****************************************************************************
//
// Prácticas de SCD. Práctica 1.
// 
//  Alba Moreno Ontiveros B3
//
// *****************************************************************************

#include <iostream>
#include <cassert>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h> // necesario para {\ttbf usleep()}
#include <stdlib.h> // necesario para {\ttbf random()}, {\ttbf srandom()}
#include <time.h>   // necesario para {\ttbf time()}


using namespace std ;

// ---------------------------------------------------------------------
// constantes configurables:

const unsigned
  num_items  = 15 ,    // numero total de items que se producen o consumen
  tam_vector = 10 ;    // tamaño del vector, debe ser menor que el número de items

int primera_libre = 0;	//primera casilla libre del buffer donde el productor almacena los valores
int primera_ocupada = 0; //primera casilla ocupada del buffer, valor que coge el consumidor

int buf[tam_vector];

sem_t puede_leer;
sem_t puede_escribir;


// ---------------------------------------------------------------------
// introduce un retraso aleatorio de duración comprendida entre
// 'smin' y 'smax' (dados en segundos)
void retraso_aleatorio( const float smin, const float smax )
{
  static bool primera = true ;
  if ( primera )        // si es la primera vez:
  {  srand(time(NULL)); //   inicializar la semilla del generador
     primera = false ;  //   no repetir la inicialización
  }
  // calcular un número de segundos aleatorio, entre {\ttbf smin} y {\ttbf smax}
  const float tsec = smin+(smax-smin)*((float)random()/(float)RAND_MAX);
  // dormir la hebra (los segundos se pasan a microsegundos, multiplicándos por 1 millón)
  usleep( (useconds_t) (tsec*1000000.0)  );
}

// ---------------------------------------------------------------------
// función que simula la producción de un dato

unsigned producir_dato()
{
  static int contador = 0 ;
  contador = contador + 1 ;
  retraso_aleatorio( 0.1, 0.5 );
  cout << "Productor : dato producido: " << contador << endl << flush ;
  return contador ;
}
// ---------------------------------------------------------------------
// función que simula la consumición de un dato

void consumir_dato( int dato )
{
   retraso_aleatorio( 0.1, 1.5 );
   cout << "Consumidor: dato consumido: " << dato << endl << flush ;
}
// ---------------------------------------------------------------------
// función que ejecuta la hebra del productor

void * funcion_productor( void * ){
  for( unsigned i = 0 ; i < num_items ; i++ ){
    int dato = producir_dato() ;

    //insertar "dato" en el vector o buffer
    sem_wait(&puede_leer);
    buf[primera_libre] = dato;
    primera_libre++;
    //llego al final del vector para poder escribir otra vez al principio tengo que comprobar primero que la casilla inicial esta vacía
    if(primera_libre >= tam_vector && primera_ocupada > 0)
      primera_libre = 0;
    sem_post(&puede_escribir);
    
    cout << "Productor : 			dato insertado: " << dato << endl << flush ;
  }

  return NULL ;
}
// ---------------------------------------------------------------------
// función que ejecuta la hebra del consumidor

void * funcion_consumidor( void * )
{
  for( unsigned i = 0 ; i < num_items ; i++ )
  {
    int dato ;

    // falta aquí: leer "dato" desde el vector intermedio
    sem_wait(&puede_escribir);
    dato = buf[primera_ocupada];
    primera_ocupada++;
    //para poder leer tengo que comprobar que al princpio hay datos escritos
    if(primera_ocupada >= tam_vector && primera_libre > 0)
      primera_ocupada = 0;
    sem_post(&puede_leer);

    cout << "Consumidor: 		dato extraído : " << dato << endl << flush ;
    consumir_dato( dato ) ;
  }
  return NULL ;
}
//----------------------------------------------------------------------

int main(){

	// falta: crear y poner en marcha las hebras, esperar que terminen
	pthread_t productor;
	pthread_t consumidor;

	pthread_create(&productor, NULL, funcion_productor, NULL);
	pthread_create(&consumidor, NULL, funcion_consumidor, NULL);

	sem_init(&puede_leer, 0,tam_vector);
	sem_init(&puede_escribir,0,0);

	pthread_join(productor,NULL);
	pthread_join(consumidor,NULL);

	return 0 ;
}
