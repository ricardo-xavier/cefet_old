package xavier.ricardo.otimize;

import java.io.File;
import java.util.ArrayList;
import java.util.List;

public class InstanciaDao {
	
	public static List<Instancia> lista(String problema) {
		
		File dir = new File("/cefet/cefet/otimizacao/" + problema);
		List<Instancia> instancias = new ArrayList<Instancia>();
		
		for (String nome : dir.list()) {
			if (!nome.endsWith(".txt")) {
				continue;
			}
			Instancia instancia = new Instancia();
			instancia.setNome(nome.substring(0, nome.length()-4));
			instancias.add(instancia);
		}
		
		return instancias;
	}

}
