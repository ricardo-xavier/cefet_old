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
import android.widget.ArrayAdapter;
import android.widget.ListView;

@SuppressWarnings("deprecation")
public class MainActivity extends ActionBarActivity {

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		setTitle(R.string.selecione_problema);

		new ProblemasTask(this).execute();
	}

	public void mostraResposta(RespostaListagem result) {

		if (result == null) {
			return;
		}
		ListView lvProblemas = (ListView) findViewById(R.id.lvProblemas);
		final List<String> problemas = result.getItens();
		ArrayAdapter<String> adapter = new ArrayAdapter<String>(this, android.R.layout.simple_list_item_1, problemas);
		lvProblemas.setAdapter(adapter);
		
		final Context contexto = this;
		
		lvProblemas.setOnItemClickListener(new OnItemClickListener() {

			@Override
			public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
				
				String problema = problemas.get(position);
				Log.i("OTIMIZE", problema);
				
				Intent intent = new Intent(contexto, InstanciaActivity.class);
				intent.putExtra("problema", problema);
				startActivity(intent);
				
			}
		});

	}

}
