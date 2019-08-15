package xavier.ricardo.otimize;

import java.io.File;
import java.util.ArrayList;
import java.util.List;

public class ImplementacaoDao {
	
	public static List<Implementacao> lista(String problema) {
		
		File dir = new File("/cefet/cefet/otimizacao/" + problema);
		List<Implementacao> implementacoes = new ArrayList<Implementacao>();
		
		for (String nome : dir.list()) {
			if (!nome.endsWith(".cpp")) {
				continue;
			}
			Implementacao implementacao = new Implementacao();
			implementacao.setNome(nome.substring(0, nome.length()-4));
			implementacoes.add(implementacao);
		}
		
		return implementacoes;
	}

}
