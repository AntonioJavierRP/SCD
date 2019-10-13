/*PROBLEMA DE LOS FUMADORES LIFO*/

import monitor.*;
/**************************************************/

class Estanco extends AbstractMonitor{
	private int ingrediente;
	private int primera_libre;
	private int tam;
	private int [] mostrador;

	private Condition puede_producir = makeCondition();
 	private Condition [] puede_fumar = new Condition[3];

 	public Estanco(){
 		for(int i = 0; i < 3; i++)
 			puede_fumar[i] = makeCondition();
 		
 		ingrediente = -1;
 		primera_libre = 0;
 		tam = 10;
 		mostrador = new int [tam];
 	}

 	public void obtenerIngrediente(int miIngrediente){
 		enter();
 			//si primera libre es 0 y el estanco esta lleno
 			if(ingrediente != miIngrediente || primera_libre == 0)
 				puede_fumar[miIngrediente].await();
 			System.out.println("Fumador "+miIngrediente+" esta fumando");
 			primera_libre--;
 			puede_producir.signal();
 		leave();
 	}

 	public void ponerIngrediente(int ingrediente){
 		enter();
 			if(primera_libre == tam)
 				puede_producir.await();

 			mostrador[primera_libre] = ingrediente;
 			primera_libre++;
 			this.ingrediente = ingrediente;
 			System.out.println("ingrediente producido "+ingrediente);
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