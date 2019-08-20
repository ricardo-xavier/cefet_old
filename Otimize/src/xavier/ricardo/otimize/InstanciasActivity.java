package xavier.ricardo.otimize;

import java.util.List;

import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.support.v7.app.ActionBarActivity;
import android.util.Log;
import android.view.View;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.ImageButton;
import android.widget.ListView;

@SuppressWarnings("deprecation")
public class InstanciasActivity extends ActionBarActivity {
	
	private String problema;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_instancia);
		
		Intent intent = getIntent();
		problema = intent.getStringExtra("problema");
		new InstanciasTask(this, problema).execute();
		
	}

	public void mostraResposta(Instancias result) {
		if (result == null) {
			return;
		}
		ListView lvInstancias = (ListView) findViewById(R.id.lvInstancias);
		final List<Instancia> instancias = result.getInstancias();
		InstanciasAdapter adapter = new InstanciasAdapter(this, instancias);
		lvInstancias.setAdapter(adapter);
		
		final Context contexto = this;
		
		lvInstancias.setOnItemClickListener(new OnItemClickListener() {

			@Override
			public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
				
				Instancia instancia = instancias.get(position);
				Log.i("OTIMIZE", instancia.getNome());
				
				Intent intent = new Intent(contexto, ImplementacoesActivity.class);
				intent.putExtra("problema", problema);
				intent.putExtra("instancia", instancia.getNome());
				startActivity(intent);
				
			}
		});
		
	}
	
	public void dados(View v) {
		ImageButton b = (ImageButton) v;
		String instancia = (String) b.getTag();
		Log.i("OTIMIZE", instancia);
		
		Intent intent = new Intent(this, DadosActivity.class);
		intent.putExtra("problema", problema);
		intent.putExtra("instancia", instancia);
		startActivity(intent);
		
	}

}
