/*PROBLEMA DE LOS FUMADORES
Consideraciones:
	-> Habrá una clase hebra Estanquero y otra Fumador. 
	-> Fumador 3 instancias. Numero de ingrediente que necesita == numero de fumador
	-> Monitor Estanco
*/

import monitor.*;
/*********************************************************/

class Estanco extends AbstractMonitor{
	private int ingrediente;
	//podrá producir si el mostrador está vacío
	private Condition puede_producir = makeCondition();	
	//Podrá fumar si tiene los ingredientes que necesita
	//El ingrediente de cada fumador es igual al numero de identificacion de la hebra
	private Condition [] puede_fumar = new Condition[3];

	public Estanco(){
		for(int i = 0; i < 3;i++)
			puede_fumar[i] = makeCondition();
		ingrediente = -1;
	}
	// invocado por cada fumador, indicando su ingrediente o numero
	public void obtenerIngrediente( int miIngrediente ){
		enter();
			if(ingrediente != miIngrediente)
				puede_fumar[miIngrediente].await();
			System.out.println("Fumador "+miIngrediente+" esta fumando");
			ingrediente = -1;
			puede_producir.signal();
		leave();
	}
	// invocado por el estanquero, indicando el ingrediente que pone
	public void ponerIngrediente( int ingrediente )	{
		enter();
			this.ingrediente = ingrediente;
			System.out.println("Estanquero reparte "+ingrediente);
			puede_fumar[this.ingrediente].signal();
		leave();
	}
	// invocado por el estanquero
	public void esperarRecogidaIngrediente(){
		enter();
			if(ingrediente != -1)
				puede_producir.await();
		leave();
	}
}	

class Fumador implements Runnable
{
	int miIngrediente;
	public Thread thr ;
	public Estanco estanco;

	public Fumador(Estanco miEstanco, int p_miIngrediente, String nombre){ 
		estanco = miEstanco;
		miIngrediente = p_miIngrediente;
		thr = new Thread(this,nombre);
	}

	public void run(){
		while ( true ){ 
			estanco.obtenerIngrediente(miIngrediente);
			aux.dormir_max(2000);
		}
	}
}

class Estanquero implements Runnable{ 
	public Thread thr ;
	public Estanco miEstanco;

	public Estanquero(Estanco estanco, String nombre){
		miEstanco = estanco;
		thr = new Thread(this, nombre);
	}
	
	public void run(){ 
		int ingrediente ;
		while (true){
			ingrediente = (int)(Math.random () * 3.0); // 0,1 o 2
			miEstanco.ponerIngrediente( ingrediente );
			miEstanco.esperarRecogidaIngrediente() ;
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