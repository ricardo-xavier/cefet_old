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
public class InstanciaActivity extends ActionBarActivity {
	
	private String problema;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_instancia);
		
		Intent intent = getIntent();
		problema = intent.getStringExtra("problema");
		new InstanciasTask(this, problema).execute();
		
	}

	public void mostraResposta(RespostaListagem result) {
		if (result == null) {
			return;
		}
		ListView lvInstancias = (ListView) findViewById(R.id.lvInstancias);
		final List<String> instancias = result.getItens();
		ArrayAdapter<String> adapter = new ArrayAdapter<String>(this, android.R.layout.simple_list_item_1, instancias);
		lvInstancias.setAdapter(adapter);
		
		final Context contexto = this;
		
		lvInstancias.setOnItemClickListener(new OnItemClickListener() {

			@Override
			public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
				
				String instancia = instancias.get(position);
				Log.i("OTIMIZE", instancia);
				
				Intent intent = new Intent(contexto, ImplementacoesActivity.class);
				intent.putExtra("problema", problema);
				intent.putExtra("instancia", instancia);
				startActivity(intent);
				
			}
		});
		
	}

}
