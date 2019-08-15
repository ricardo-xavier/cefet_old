package xavier.ricardo.otimize;

import java.util.ArrayList;
import java.util.List;

import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.RestController;

// https://dzone.com/articles/build-rest-web-service-using-spring

@RestController
@RequestMapping("/")
public class Controlador {
	
	@RequestMapping(value = "/hello", method = RequestMethod.GET)
	public String hello() {
		return "OtimizeService v1.0.0 (15/08/2019)";
	}

	@RequestMapping(value = "/problemas", method = RequestMethod.GET)
	public RespostaListagem getProblemas() {
		RespostaListagem resposta = new RespostaListagem();
		resposta.setItens(new ArrayList<String>());
		List<Problema> problemas = ProblemaDao.lista();
		for (Problema problema : problemas) {
			resposta.getItens().add(problema.getNome());
		}
		return resposta;
	}

	@RequestMapping(value = "/instancias", method = RequestMethod.GET)
	public RespostaListagem getInstancias(@RequestParam(value = "problema") String problema) {
		RespostaListagem resposta = new RespostaListagem();
		resposta.setItens(new ArrayList<String>());
		List<Instancia> instancias = InstanciaDao.lista(problema);
		for (Instancia instancia : instancias) {
			resposta.getItens().add(instancia.getNome());
		}
		return resposta;
	}

	@RequestMapping(value = "/implementacoes", method = RequestMethod.GET)
	public RespostaListagem getImplementacoes(@RequestParam(value = "problema") String problema) {
		RespostaListagem resposta = new RespostaListagem();
		resposta.setItens(new ArrayList<String>());
		List<Implementacao> implementacoes = ImplementacaoDao.lista(problema);
		for (Implementacao implementacao : implementacoes) {
			resposta.getItens().add(implementacao.getNome());
		}
		return resposta;
	}	
	
}
