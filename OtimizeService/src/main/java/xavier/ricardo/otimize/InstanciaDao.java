package xavier.ricardo.otimize;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;
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
			
			String descricao = "";
			String des = nome.replace(".txt", ".des");
			File fDes = new File(dir + "/" + des);
			if (fDes.exists()) {
				try {
					BufferedReader reader = new BufferedReader(new FileReader(fDes));
					descricao = reader.readLine();
					System.out.println("descricao=" + descricao);
					reader.close();
				} catch (IOException e) {
				}
			}
			
			Instancia instancia = new Instancia();
			instancia.setNome(nome.substring(0, nome.length()-4));
			instancia.setDescricao(descricao);
			instancias.add(instancia);
		}
		
		return instancias;
	}

	public static List<String> dados(String problema, String instancia) {
		
		List<String> dados = new ArrayList<String>();

		File txt = new File("/cefet/cefet/otimizacao/" + problema
				+ "/" + instancia + ".txt");
		
		try {
			BufferedReader reader = new BufferedReader(new FileReader(txt));
			String linha;
			while ((linha = reader.readLine()) != null) {
				dados.add(linha);
			}
			reader.close();
		} catch (IOException e) {
		}
		
		return dados;
	}

}
