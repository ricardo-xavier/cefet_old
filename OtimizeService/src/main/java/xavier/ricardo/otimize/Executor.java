package xavier.ricardo.otimize;

import java.io.BufferedReader;
import java.io.File;
import java.io.InputStreamReader;
import java.util.ArrayList;

public class Executor {

	public static RespostaOtimizacao executa(String problema, String instancia, String implementacao) throws Exception {
		
		RespostaOtimizacao resposta = new RespostaOtimizacao();
		
		Runtime rt = Runtime.getRuntime();
		String[] cmd = { "/cefet/cefet/executa.sh", implementacao, instancia + ".txt" };
		Process proc = rt.exec(cmd, null, new File("/cefet/cefet/otimizacao/" + problema));
		BufferedReader reader = new BufferedReader(new InputStreamReader(proc.getInputStream()));
		String linha = reader.readLine();
		if (linha == null) {
			throw new Exception("Tempo limite excedido");
		}
		resposta.setResultado(Integer.parseInt(linha));
		resposta.setSolucao(new ArrayList<String>());

		while ((linha = reader.readLine()) != null) {
			resposta.getSolucao().add(linha);
		}
		
		reader.close();
		return resposta;
	}

}
