import java.util.Random;
import monitor.*;


class Barberia extends AbstractMonitor
{
  private Condition sala_espera = makeCondition();
  private Condition barbero = makeCondition();
  private Condition silla = makeCondition();

  // invocado por los clientes para cortarse el pelo
  public void cortarPelo(){
    enter();
      // Si la silla está ocupada
      if (!silla.isEmpty()){
        //pasamos el cliente a la sala de espera
        System.out.println("Silla ocupada");
        sala_espera.await();
      }
      // Pelamos al cliente (despertamos al barbero y ponemos al cliente en la silla)
      System.out.println("Cliente empieza a afeitarse ");
      barbero.signal();
      silla.await();
    leave();
  }

  public void siguienteCliente(){
    enter();
      // Si la sala y la silla están vacias
      if (sala_espera.isEmpty() && silla.isEmpty()){
        System.out.println("Barbero se pone a dormir ");
        barbero.await();
      }
      // Sacamos al siguiente cliente de la sala de espera
      System.out.println("Barbero coge otro cliente ");
      sala_espera.signal();
  	leave();
  }

  public void finCliente(){
    enter();
      // Sacamos al cliente de la silla
      System.out.println("Barbero termina de afeitar ");
      silla.signal();
    leave();
  }
}

class Cliente implements Runnable
{
  private Barberia barberia;
  public Thread thr;
  public Cliente(Barberia bar,String nombre){
    barberia = bar;
    thr   = new Thread(this,nombre);
  }

  public void run(){
    while (true){
      try{
        barberia.cortarPelo();  // el cliente espera (si procede) y se corta el pelo
        dormir.dormir_max( 2000 ); // el cliente esta fuera de la barberia un tiempo
      }
      catch(Exception e){
        System.err.println("Excepcion en main: " + e);
      }
    }
  }
}

class Barbero implements Runnable
{
  private Barberia barberia;
  public Thread thr;

  public Barbero(Barberia mon){
    barberia = mon;
    thr   = new Thread(this,"barbero");
  }

  public void run(){
    while (true){
      try{
        barberia.siguienteCliente();
        dormir.dormir_max( 2500 ); // el barbero esta cortando el pelo
        barberia.finCliente();
     	}
      catch(Exception e){
        System.err.println("Excepcion en main: " + e);
      }
    }
  }
}

class MainBArberia{
  public static void main(String[] args){

    // leer parametros, crear vectores y buffer intermedio
    Barberia barberia = new Barberia();

    // crear hebras
    Barbero barbero = new Barbero(barberia);
    Cliente[] clientes = new Cliente[4];

    for (int i=0; i < 4; i++)
      clientes[i] = new Cliente(barberia,"cliente"+i);

    // poner en marcha las hebras
    barbero.thr.start();

    for (int i=0; i < 4; i++)
      clientes[i].thr.start();
  }
}