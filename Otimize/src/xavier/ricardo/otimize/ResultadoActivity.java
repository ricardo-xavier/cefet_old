package xavier.ricardo.otimize;

import java.util.ArrayList;
import java.util.List;

import android.content.Intent;
import android.os.Bundle;
import android.support.v7.app.ActionBarActivity;
import android.widget.ArrayAdapter;
import android.widget.GridView;
import android.widget.TextView;
import android.widget.Toast;

@SuppressWarnings("deprecation")
public class ResultadoActivity extends ActionBarActivity {

	private String problema;
	private String instancia;
	private String implementacao;
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_resultado);
		
		Intent intent = getIntent();
		problema = intent.getStringExtra("problema");
		instancia = intent.getStringExtra("instancia");
		implementacao = intent.getStringExtra("implementacao");
		new ExecutaTask(this, problema, instancia, implementacao).execute();
		
	}

	public void mostraResposta(RespostaExecuta result) {
		
		if (!result.isOk()) {
			Toast.makeText(this, result.getMensagem(), Toast.LENGTH_SHORT).show();
			return;
		}

		TextView tvResultado = (TextView) findViewById(R.id.tvResultado);
		tvResultado.setText(String.valueOf(result.getResultado()));

		TextView tvTempo = (TextView) findViewById(R.id.tvTempo);
		tvTempo.setText(String.valueOf(result.getTempo()));
		
		GridView gvSolucao = (GridView) findViewById(R.id.gvSolucao);
		
		List<String> solucao = result.getSolucao();
		List<String> grid = new ArrayList<String>();
		for (String s : solucao) {
			String[] partes = s.split("\t"); 
			for (String parte : partes) {
				if (parte.equals("-0")) {
					parte = "0";
				}
				grid.add(parte);
			}
		}
		
		ArrayAdapter<String> adapter = new ArrayAdapter<String>(this,
				android.R.layout.simple_list_item_1, grid);
		gvSolucao.setAdapter(adapter);
		
	}
}
