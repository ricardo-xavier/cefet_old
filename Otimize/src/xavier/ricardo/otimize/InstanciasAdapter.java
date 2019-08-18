package xavier.ricardo.otimize;

import java.util.List;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.ImageButton;
import android.widget.TextView;

public class InstanciasAdapter extends BaseAdapter {
	
	private Context context;
	private List<Instancia> instancias;
	
	public InstanciasAdapter(Context context, List<Instancia> instancias) {
		this.context = context;
		this.instancias = instancias;
	}

	@Override
	public int getCount() {
		return instancias.size();
	}

	@Override
	public Object getItem(int position) {
		return instancias.get(position);
	}

	@Override
	public long getItemId(int position) {
		return 0;
	}

	@Override
	public View getView(int position, View convertView, ViewGroup parent) {

		if ((instancias == null) || (instancias.size() <= position)) {
			return null;
		}
		
		LayoutInflater inflater = (LayoutInflater) context.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
		View v = inflater.inflate(R.layout.instancia, null);
		
		Instancia instancia = instancias.get(position);
		
		TextView tvNome = (TextView) v.findViewById(R.id.tvNome);
		tvNome.setText(instancia.getNome());
		
		TextView tvDescricao = (TextView) v.findViewById(R.id.tvDescricao);
		tvDescricao.setText(instancia.getDescricao());
		
		ImageButton btDados = (ImageButton) v.findViewById(R.id.btDados);
		btDados.setTag(instancia.getNome());

		return v;
	}

}
