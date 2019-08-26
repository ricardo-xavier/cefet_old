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
	int V; // volume total
	int P; // peso total

	datafile >> n >> V >> P;

	vector<int> v(n); // vetor de volumes
	for (int j = 0; j < n; j++) {
		datafile >> v[j];
	}

	vector<int> p(n); // vetor de pesos
	for (int j = 0; j < n; j++) {
		datafile >> p[j];
	}

	vector<int> c(n); // vetor de custos
	for (int j = 0; j < n; j++) {
		datafile >> c[j];
	}

	datafile.close();

	// inicializa o cplex
	IloEnv env;
	IloModel mod(env);
	IloCplex cplex(mod);
	//cplex.setOut(env.getNullStream());

	// variavel booleana indicando se o item vai ser colocado na mochila
	IloNumVarArray x(env, n, 0, 1, ILOBOOL);

	// funcao objetivo - maximizar o valor
	IloExpr objective(env);
	for (int j = 0; j < n; j++) {
		objective += c[j] * x[j];
	}
	IloAdd(mod, IloMaximize(env, objective));
	objective.end();

	// restricao - volume da mochila
	IloExpr constraint_vol(env);
	for (int j = 0; j < n; j++) {
		constraint_vol += v[j] * x[j];
	}
	mod.add(constraint_vol <= V);
	constraint_vol.end();

	// restricao - peso suportado pela mochila
	IloExpr constraint_peso(env);
	for (int j = 0; j < n; j++) {
		constraint_peso += p[j] * x[j];
	}
	mod.add(constraint_peso <= P);
	constraint_peso.end();

	// resolve o problema
	cplex.solve();

	// mostra o resultado
	double result = cplex.getObjValue();
	cout << result << endl;

	// mostra a solucao
	cout << "#\tv\tc\t*" << endl;
	for (int j = 0; j < n; j++){
		cout << j+1 << "\t" << v[j] << "\t" << c[j] << "\t" << cplex.getValue(x[j]) << endl;
	}
	cout << "#\tp\tc\t*" << endl;
	for (int j = 0; j < n; j++){
		cout << j+1 << "\t" << p[j] << "\t" << c[j] << "\t" << cplex.getValue(x[j]) << endl;
	}

	return 0;
}
