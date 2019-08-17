package xavier.ricardo.otimize;

import java.util.List;

public class RespostaOtimizacao {
	
	private boolean ok;
	private String mensagem;
	private int tempo;
	private int resultado;
	private List<String> solucao;
	public boolean isOk() {
		return ok;
	}
	public void setOk(boolean ok) {
		this.ok = ok;
	}
	public String getMensagem() {
		return mensagem;
	}
	public void setMensagem(String mensagem) {
		this.mensagem = mensagem;
	}
	public int getTempo() {
		return tempo;
	}
	public void setTempo(int tempo) {
		this.tempo = tempo;
	}
	public int getResultado() {
		return resultado;
	}
	public void setResultado(int resultado) {
		this.resultado = resultado;
	}
	public List<String> getSolucao() {
		return solucao;
	}
	public void setSolucao(List<String> solucao) {
		this.solucao = solucao;
	}


}
