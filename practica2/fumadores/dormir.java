import java.util.*;
/*************************************************************/
class dormir{
	static Random genAlea = new Random();

	static void dormir_max(int milis){
		try{
			Thread.sleep(genAlea.nextInt(milis));
		}
		catch(Exception e){
			System.err.println("Sleep interrumpido");
		}
	}
}