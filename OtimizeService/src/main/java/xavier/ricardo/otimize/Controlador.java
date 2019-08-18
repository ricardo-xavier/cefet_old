package xavier.ricardo.otimize;

import java.io.IOException;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;

import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.RestController;

// https://dzone.com/articles/build-rest-web-service-using-spring

// ricardoxavier.no-ip.org/OtimizeService/rest/executa?problema=mochila&instancia=mochila_5_10&implementacao=mochila_cplex

@RestController
@RequestMapping("/")
public class Controlador {
	
	@RequestMapping(value = "/hello", method = RequestMethod.GET)
	public String hello() {
		return "OtimizeService v1.1.0 (17/08/2019)";
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

	@RequestMapping(value = "/instancias2", method = RequestMethod.GET)
	public Instancias getInstancias2(@RequestParam(value = "problema") String problema) {
		Instancias resposta = new Instancias();
		List<Instancia> instancias = InstanciaDao.lista(problema);
		resposta.setInstancias(instancias);
		return resposta;
	}

	@RequestMapping(value = "/dados", method = RequestMethod.GET)
	public RespostaListagem getImplementacoes(@RequestParam(value = "problema") String problema,
			@RequestParam(value = "instancia") String instancia) {
		RespostaListagem resposta = new RespostaListagem();
		List<String> dados = InstanciaDao.dados(problema, instancia);
		resposta.setItens(dados);
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

	@RequestMapping(value = "/executa", method = RequestMethod.GET)
	public RespostaOtimizacao executa(@RequestParam(value = "problema") String problema,
			@RequestParam(value = "instancia") String instancia,
			@RequestParam(value = "implementacao") String implementacao) {
		RespostaOtimizacao resposta;
		try {
			Date t1 = new Date();
			resposta = Executor.executa(problema, instancia, implementacao);
			Date t2 = new Date();
			resposta.setOk(true);
			resposta.setTempo((int) (t2.getTime() - t1.getTime()));
			
		} catch (IOException e) {
			e.printStackTrace();
			resposta = new RespostaOtimizacao();
			resposta.setOk(false);
			resposta.setMensagem(e.getMessage());
		}
		return resposta;
	}	
	
}
