package xavier.ricardo.otimize;

import java.io.File;
import java.util.ArrayList;
import java.util.List;

public class ProblemaDao {
	
	public static List<Problema> lista() {
		
		File dir = new File("/cefet/cefet/otimizacao");
		List<Problema> problemas = new ArrayList<Problema>();
		
		for (String nome : dir.list()) {
			Problema problema = new Problema();
			problema.setNome(nome);
			problemas.add(problema);
		}
		
		return problemas;
	}

}
