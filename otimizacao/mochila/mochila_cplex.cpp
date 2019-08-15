#include <iostream>
#include <fstream>
#include <vector>
#include <ilcplex/ilocplex.h>

using namespace std;

// mochila_cplex <datafile>
int main(int argc, char *argv[]) {

	// load datafile

	ifstream datafile(argv[1]);

	int n; // numero de itens
	int W; // capacidade
	datafile >> n >> W;

	vector<int> w(n); // vetor de pesos
	for (int j = 0; j < n; j++) {
		datafile >> w[j];
	}

	vector<int> v(n); // vetor de valores
	for (int j = 0; j < n; j++) {
		datafile >> v[j];
	}

	datafile.close();

	// inicializa o cplex
	IloEnv env;
	IloModel mod(env);
	IloCplex cplex(mod);
	cplex.setOut(env.getNullStream());

	// variavel booleana indicando se o item vai ser colocado na mochila
	IloNumVarArray x(env, n, 0, 1, ILOBOOL);

	// funcao objetivo - maximizar o valor
	IloExpr objective(env);
	for (int j = 0; j < n; j++) {
		objective += v[j] * x[j];
	}
	IloAdd(mod, IloMaximize(env, objective));
	objective.end();

	// restricoes - capacidade da mochila
	IloExpr constraints(env);
	for (int j = 0; j < n; j++) {
		constraints += w[j] * x[j];
	}
	mod.add(constraints <= W);
	constraints.end();

	// resolve o problema
	cplex.solve();

	// mostra o resultado
	double result = cplex.getObjValue();
	cout << result << endl;

	// mostra a solucao
	for (int j = 0; j < n; j++){
		cout << cplex.getValue(x[j]) << " ";
	}
	cout << endl;

	return 0;
}
