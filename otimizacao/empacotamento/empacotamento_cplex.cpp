#include <iostream>
#include <fstream>
#include <vector>
#include <ilcplex/ilocplex.h>

using namespace std;

// empacotamento_cplex <datafile>
int main(int argc, char *argv[]) {

	// load datafile

	ifstream datafile(argv[1]);

	int n; // numero de itens
	int v; // capacidade do pacote
	datafile >> n >> v;
	// no pior caso cada item vai ser empacotado em um pacote diferente
	int m = n; // numero de pacotes

	vector<int> a(n); // tamanho dos itens
	for (int i = 0; i < n; i++) {
		datafile >> a[i];
	}

	datafile.close();

	// inicializa o cplex
	IloEnv env;
	IloModel mod(env);
	IloCplex cplex(mod);
	cplex.setOut(env.getNullStream());

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
			r1 += (a[i] * x[i][j]);
		}
		mod.add(r1 * y[j] <= v);
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
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			mod.add(x[i][j] <= y[j]);
		}
	}

	// resolve o problema
	cplex.solve();

	// mostra o resultado
	double result = cplex.getObjValue();
	cout << result << endl;

	// mostra a solucao
	cout << "j\tn\tv\t*" << endl;
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
			cout << j+1 << "\t" << num_itens << "\t" << vol_itens << "\t" << cplex.getValue(y[j]) << endl;
		}
	}
	cout << endl;
	cout << "i\ta\tj\t*" << endl;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			if (cplex.getValue(x[i][j]) > 0) {
				cout << i+1 << "\t" << a[i] << "\t" << j+1 << "\t" << cplex.getValue(x[i][j]) << endl;
			}
		}
	}

	return 0;
}
