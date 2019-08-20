package xavier.ricardo.otimize;

import java.io.BufferedReader;
import java.io.InputStream;
import java.io.InputStreamReader;

import org.apache.http.HttpResponse;
import org.apache.http.client.HttpClient;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.impl.client.DefaultHttpClient;

import com.google.gson.Gson;

import android.app.ProgressDialog;
import android.content.Context;
import android.os.AsyncTask;
import android.util.Log;

@SuppressWarnings("deprecation")
public class DadosTask extends AsyncTask<String, Void, RespostaListagem> {
	
	private DadosActivity contexto;
	private String problema;
	private String instancia;
	private ProgressDialog progress;
	
	public DadosTask(DadosActivity contexto, String problema, String instancia) {
		this.contexto = contexto;
		this.problema = problema;
		this.instancia = instancia;
	}
	
	@Override
	protected void onPreExecute() {
		progress = new ProgressDialog((Context) contexto);
		progress.setMessage("Aguarde...");
		progress.show();		
		super.onPreExecute();		
	}
	
	@Override
	protected void onPostExecute(RespostaListagem result) {
		super.onPostExecute(result);
		progress.dismiss();
		contexto.mostraDados(result);
	}
	
	@Override
	protected void onProgressUpdate(Void... values) {
		super.onProgressUpdate(values);
	}

	@Override
	protected RespostaListagem doInBackground(String... params) {
		
		try {
			
			String url = "http://ricardoxavier.no-ip.org/OtimizeService/rest/dados"
					+ "?problema=" + problema
					+ "&instancia=" + instancia;
			Log.i("OTIMIZE", url);
			
			HttpClient httpClient = new DefaultHttpClient();
			
			HttpGet httpGet = new HttpGet(url);

			HttpResponse httpResponse = httpClient.execute(httpGet);
			
			InputStream inputStream = httpResponse.getEntity().getContent();
			BufferedReader bufferedReader = new BufferedReader(new InputStreamReader(inputStream));
			String line = "";
			StringBuilder resultStr = new StringBuilder();
			while ((line = bufferedReader.readLine()) != null) {
				resultStr.append(line);
			}
			inputStream.close();	
			Log.i("OTIMIZE", resultStr.toString());
			
			Gson gson = new Gson();
			RespostaListagem result = gson.fromJson(resultStr.toString(), RespostaListagem.class);
			
			return result;
			
		} catch (Exception e) {
			e.printStackTrace();
			return null;
		}
	}

}
