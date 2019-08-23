#include <iostream>
#include <fstream>
#include <vector>
#include <bits/stdc++.h>

using namespace std;

int dominancia2(int n, vector<int> v, int V, vector<int> v2, vector<int> b2) {

	int n2 = 0;
	int b = 0;

	for (int i=0, j=n-1; i<j; ) {
		
		int x = v[i] + v[j];

		if (x == V) {
			fprintf(stderr, "dominancia %d [%d] %d [%d]\n", i, v[i], j, v[j]);
			i++;
			j--;
			b2.insert(b2.end(), b);
			b2.insert(b2.end(), b++);
			v2.insert(v2.end(), v[i]);
			v2.insert(v2.end(), v[j]);
			n2+=2;

		} else if (x < V) {
			j--;
		} else {
			i++;
		}
		
	}

	for (int i=0; i<n2; i+=2) {
		fprintf(stderr, "i=%d %d\n", i, b2[i]);
	}
	for (int i=0; i<n2; i+=2) {
		fprintf(stderr, "i=%d %d %d\n", i, v2[i], v2[i+1]);
	}
	return n2;
}

// empacotamento_cplex <datafile>
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
	vector<int> b2;
	dominancia2(n, a, V, a2, b2);

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
	int result = 0;
	for (int j = 0; j < m; j++) {
		if (q[j] > 0) {
			result++;
		}
	}
	cout << result << endl;

	// mostra a solucao
	cout << "j\tn\tv\t-" << endl;
	for (int j = 0; j < m; j++) {
		if (q[j] > 0) {
			cout << j+1 << "\t" << q[j] << "\t" << v[j] << "\t" << " " << endl;
		}
	}
	cout << "i\ta\tj\t-" << endl;
	for (int i = 0; i < n; i++) {
		cout << i+1 << "\t" << a[i] << "\t" << b[i]+1 << "\t" << " " << endl;
	}

	return 0;
}
