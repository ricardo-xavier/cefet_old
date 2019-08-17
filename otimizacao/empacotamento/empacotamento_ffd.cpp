#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

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

	datafile.close();

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
