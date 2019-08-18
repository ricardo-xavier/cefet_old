package xavier.ricardo.otimize;

import java.util.List;

import android.content.Intent;
import android.os.Bundle;
import android.support.v7.app.ActionBarActivity;
import android.widget.ArrayAdapter;
import android.widget.ListView;

@SuppressWarnings("deprecation")
public class DadosActivity extends ActionBarActivity {

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_dados);
		
		Intent intent = getIntent();
		String problema = intent.getStringExtra("problema");
		String instancia = intent.getStringExtra("instancia");
		new DadosTask(this, problema, instancia).execute();

	}
	
	public void mostraDados(RespostaListagem result) {
		if (result == null) {
			return;
		}
		ListView lvDados = (ListView) findViewById(R.id.lvDados);
		final List<String> dados = result.getItens();
		ArrayAdapter<String> adapter = new ArrayAdapter<String>(this, android.R.layout.simple_list_item_1, dados);
		lvDados.setAdapter(adapter);
	}

}
