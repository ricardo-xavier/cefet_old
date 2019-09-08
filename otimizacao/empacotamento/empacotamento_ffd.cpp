#include <iostream>
#include <fstream>
#include <vector>
#include <bits/stdc++.h>

using namespace std;

vector<int> dominancia2(int n, vector<int> v, int V, vector<int> &v2);

// empacotamento_ffd <datafile>
int main(int argc, char *argv[]) {

	// load datafile

	ifstream datafile(argv[1]);

	int n; // numero de itens
	int V; // capacidade do pacote
	datafile >> n >> V;
	// no pior caso cada item vai ser empacotado em um pacote diferente
	int m = n; // numero de pacotes

	vector<int> a(n); // tamanho dos itens
	for (int i = 0; i < n; i++) {
		datafile >> a[i];
	}
	sort(a.begin(), a.end(), greater<int>());

	datafile.close();

	vector<int> a2;
	a = dominancia2(n, a, V, a2);
    n = a.size();

	// resolve o problema
	vector<int> b(n); // indica em qual pacote o item foi empacotado
	vector<int> v(m); // volume preenchido
	vector<int> q(m); // quantidade de itens

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			if ((v[j] + a[i]) <= V) {
				b[i] = j;
				v[j] += a[i];
				q[j]++;
				break;
			}	
		}
	}

	// mostra o resultado
	int result = a2.size() / 2;
	for (int j = 0; j < m; j++) {
		if (q[j] > 0) {
			result++;
		}
	}
	cout << result << endl;

	// mostra a solucao
	cout << "j\tn\tv\t-" << endl;
    int k = a2.size() / 2;
	for (int j = 0; j < k; j++) {
		cout << j+1 << "\t" << 2 << "\t" << V << "\t" << " " << endl;
	}
	for (int j = 0; j < m; j++) {
		if (q[j] > 0) {
			cout << j+1+k << "\t" << q[j] << "\t" << v[j] << "\t" << " " << endl;
		}
	}
	cout << "i\ta\tj\t-" << endl;
	for (int i = 0; i < k; i++) {
		cout << i*2+1 << "\t" << a2[i*2] << "\t" << i+1 << "\t" << " " << endl;
		cout << i*2+2 << "\t" << a2[i*2+1] << "\t" << i+1 << "\t" << " " << endl;
    }
	for (int i = 0; i < n; i++) {
		cout << i+1+a2.size() << "\t" << a[i] << "\t" << b[i]+1+k << "\t" << " " << endl;
	}

	return 0;
}
