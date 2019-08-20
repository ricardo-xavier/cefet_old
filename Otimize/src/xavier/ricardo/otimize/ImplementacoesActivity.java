package xavier.ricardo.otimize;

import java.util.List;

import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.support.v7.app.ActionBarActivity;
import android.util.Log;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.ListView;
import android.widget.AdapterView.OnItemClickListener;

@SuppressWarnings("deprecation")
public class ImplementacoesActivity extends ActionBarActivity {
	
	private String problema;
	private String instancia;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_implementacoes);
		
		Intent intent = getIntent();
		problema = intent.getStringExtra("problema");
		instancia = intent.getStringExtra("instancia");
		new ImplementacoesTask(this, problema).execute();
		
	}

	public void mostraResposta(RespostaListagem result) {
		if (result == null) {
			return;
		}
		ListView lvImplementacoes = (ListView) findViewById(R.id.lvImplementacoes);
		final List<String> implementacoes = result.getItens();
		ArrayAdapter<String> adapter = new ArrayAdapter<String>(this, android.R.layout.simple_list_item_1, implementacoes);
		lvImplementacoes.setAdapter(adapter);
		
		final Context contexto = this;
		
		lvImplementacoes.setOnItemClickListener(new OnItemClickListener() {

			@Override
			public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
				
				String implementacao = implementacoes.get(position);
				Log.i("OTIMIZE", implementacao);
				
				Intent intent = new Intent(contexto, ResultadoActivity.class);
				intent.putExtra("problema", problema);
				intent.putExtra("instancia", instancia);
				intent.putExtra("implementacao", implementacao);
				startActivity(intent);
				
			}
		});

	}
}
