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
public class InstanciasTask extends AsyncTask<String, Void, Instancias> {
	
	private InstanciasActivity contexto;
	private String problema;
	private ProgressDialog progress;
	
	public InstanciasTask(InstanciasActivity contexto, String problema) {
		this.contexto = contexto;
		this.problema = problema;
	}
	
	@Override
	protected void onPreExecute() {
		progress = new ProgressDialog((Context) contexto);
		progress.setMessage("Aguarde...");
		progress.show();		
		super.onPreExecute();		
	}
	
	@Override
	protected void onPostExecute(Instancias result) {
		super.onPostExecute(result);
		progress.dismiss();
		contexto.mostraResposta(result);
	}
	
	@Override
	protected void onProgressUpdate(Void... values) {
		super.onProgressUpdate(values);
	}

	@Override
	protected Instancias doInBackground(String... params) {
		
		try {
			
			String url = "http://ricardoxavier.no-ip.org/OtimizeService/rest/instancias2"
					+ "?problema=" + problema;
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
			Instancias result = gson.fromJson(resultStr.toString(), Instancias.class);
			
			return result;
			
		} catch (Exception e) {
			e.printStackTrace();
			return null;
		}
	}

}
