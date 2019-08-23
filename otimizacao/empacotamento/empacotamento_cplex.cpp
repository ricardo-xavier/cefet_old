#include <iostream>
#include <fstream>
#include <vector>
#include <ilcplex/ilocplex.h>
#include <bits/stdc++.h>

using namespace std;

int ffd(int n, int V, vector<int> a);
vector<int> dominancia2(int n, vector<int> v, int V, vector<int> &v2);

// empacotamento_cplex <datafile>
int main(int argc, char *argv[]) {

	// load datafile

	ifstream datafile(argv[1]);

	int n; // numero de itens
	int v; // capacidade do pacote
	datafile >> n >> v;
	// no pior caso cada item vai ser empacotado em um pacote diferente

	vector<int> a(n); // tamanho dos itens
	for (int i = 0; i < n; i++) {
		datafile >> a[i];
	}
	sort(a.begin(), a.end(), greater<int>());

	datafile.close();

	vector<int> a2;
	a = dominancia2(n, a, v, a2);
    n = a.size();

	int m = ffd(n, v, a); // numero de pacotes

	// inicializa o cplex
	IloEnv env;
	IloModel mod(env);
	IloCplex cplex(mod);
	//cplex.setOut(env.getNullStream());

	// variavel booleana indicando se um pacote foi usado
	IloNumVarArray y(env, m, 0, 1, ILOBOOL);

	// indica se o item i sera empacotado no pacote j
	IloArray<IloNumVarArray> x(env, n);
	for (int i = 0; i < n; i++) {
		x[i] = IloNumVarArray(env, m, 0, 1, ILOBOOL);
	}

	// funcao objetivo - minimizar o numero de pacotes
	IloExpr objective(env);
	for (int j = 0; j < m; j++) {
		objective += y[j];
	}
	IloAdd(mod, IloMinimize(env, objective));
	objective.end();

	// restricoes
	// a soma dos itens de um pacote tem que ser menor que a capacidade do pacote
	for (int j = 0; j < m; j++) {
		IloExpr r1(env);
		for (int i = 0; i < n; i++) {
			r1 += a[i] * x[i][j];
		}
		mod.add(r1 <= v * y[j]);
		r1.end();
	}

	// todo item tem que ser empacotado em um pacote
	for (int i = 0; i < n; i++) {
		IloExpr r2(env);
		for (int j = 0; j < m; j++) {
			r2 += x[i][j];
		}
		mod.add(r2 == 1);
		r2.end();
	}

	// um item só pode ser empacotado em um pacote alocado
/*
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			mod.add(x[i][j] <= y[j]);
		}
	}
*/


	// resolve o problema
	cplex.exportModel("model.lp");
	cplex.solve();

	// mostra o resultado
	double result = cplex.getObjValue();
	cout << result << endl;

	// mostra a solucao
	cout << "j\tn\tv\t-" << endl;
    int k = a2.size() / 2;
	for (int j = 0; j < k; j++) {
		cout << j+1 << "\t" << 2 << "\t" << v << "\t" << " " << endl;
	}
	for (int j = 0; j < m; j++) {
		int num_itens = 0;
		int vol_itens = 0;
		for (int i = 0; i < n; i++) {
			if (cplex.getValue(x[i][j]) > 0) {
				num_itens++;
				vol_itens += a[i];
			}
		}
		if (num_itens > 0) {
			cout << j+1+k << "\t" << num_itens << "\t" << vol_itens << "\t" << " " << endl;
		}
	}
	cout << "\t\t\t\t" << endl;
	cout << "i\ta\tj\t-" << endl;
	for (int i = 0; i < k; i++) {
		cout << i*2+1 << "\t" << a2[i*2] << "\t" << i+1 << "\t" << " " << endl;
		cout << i*2+2 << "\t" << a2[i*2+1] << "\t" << i+1 << "\t" << " " << endl;
    }
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			if (cplex.getValue(x[i][j]) > 0) {
				cout << i+1+a2.size() << "\t" << a[i] << "\t" << j+1+k << "\t" << " " << endl;
			}
		}
 	}

	return 0;
}

int ffd(int n, int V, vector<int> a) {

	vector<int> b(n); // indica em qual pacote o item foi empacotado
	vector<int> v(n); // volume preenchido

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if ((v[j] + a[i]) <= V) {
				b[i] = j;
				v[j] += a[i];
				break;
			}	
		}
	}

	int m = 0;
	for (int j = 0; j < n; j++) {
		if (v[j] > 0) {
			m++;
		}
	}
	return m;
}
