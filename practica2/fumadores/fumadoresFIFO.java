/*PROBLEMA DE LOS FUMADORES FIFO*/

import monitor.*;
/******************************************/

class Estanco extends AbstractMonitor{
	private int ingrediente;
	private int primera_libre;
	private int primera_ocupada;
	private int tam;
	private int [] mostrador;

	private Condition puede_producir = makeCondition();
	private Condition [] puede_fumar = new Condition[3];

	public Estanco(){
		for(int i = 0; i < 3;i++)
			puede_fumar[i] = makeCondition();
		
		primera_libre = 0;
		primera_ocupada = 0;
		tam = 10;
		mostrador = new int [10];
	}

	public void obtenerIngrediente(int miIngrediente){
		enter();	
			//si la primera libre es 0 entonces aun no hay ningun ingrediente en el mostrador
			if(ingrediente != miIngrediente || primera_libre > 0)
				puede_fumar[miIngrediente].await();
			System.out.println("Fumador "+miIngrediente+"fumando");
			//si coge el ingrediente de una posicion entonces el siguiente ingrediente esta en la pos++
			primera_ocupada++;
			//si la primera ocupada es al final del vector y la primera libre es distinta de 0, entonces sabemos
			//qie 
			if(primera_ocupada == tam && primera_libre != 0)
				primera_ocupada = 0;
		leave();
	}
	public void ponerIngrediente(int ingrediente){
		enter();
			//el mostrador esta lleno
			if(primera_libre == tam)
				puede_producir.await();

			mostrador[primera_libre] = ingrediente;
			primera_libre++;
			this.ingrediente = ingrediente;
			System.out.println("ingrediente producido "+ingrediente);
			//si la primera libre es al final del mostrador y la primera ocupada no esta al principio
			//entonces el principio está libre y el estanquero puede dejar el ingrediente al principio
			//no hace el wait
			if(primera_ocupada > 0 && primera_libre == tam)
				primera_libre = 0;
			//le da permiso al fumador para fumar
			puede_fumar[this.ingrediente].signal();			
		leave();
	}
	public void esperarRecogidaIngrediente(){
 		enter();
 			dormir.dormir_max(2000);
 		leave();
 	}
}

class Fumador implements Runnable{
	int miIngrediente;
	public Thread thr;
	public Estanco estanco;

	public Fumador(Estanco miEstanco, int p_miIngrediente, String nombre){
		estanco = miEstanco;
		miIngrediente = p_miIngrediente;
		thr = new Thread(this,nombre);
	}

	public void run(){
		while(true){
			estanco.obtenerIngrediente(miIngrediente);
			dormir.dormir_max(2000);
		}
	}
}

class Estanquero implements Runnable{
	public Thread thr;
	public Estanco miEstanco;

	public Estanquero(Estanco estanco, String nombre){
		miEstanco = estanco;
		thr = new Thread(this,nombre);
	}

	public void run(){
		int ingrediente;
		while(true){
			ingrediente = (int)(Math.random()*3.0);
			miEstanco.ponerIngrediente(ingrediente);
			miEstanco.esperarRecogidaIngrediente();
		}
	}
}


class MainFumadores { 
  public static void main(String[] args) { 
    // crear monitor
    Estanco estan = new Estanco();
    
    // crear los dos vectores de hebras:
    Fumador [] fumadores = new Fumador [3];
    Estanquero estanq = new Estanquero(estan,"estanquero");

    // crear hebras
    for( int i = 0; i < 3; i++) 
      fumadores[i] = new Fumador(estan,i,"fumador"+(i));
    
    estanq.thr.start();
    // lanzar hebras
    for( int i = 0; i < 3; i++) 
    	fumadores[i].thr.start();
  }
}
