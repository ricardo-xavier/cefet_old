#include <iostream>
#include <fstream>
#include <vector>
#include <ilcplex/ilocplex.h>

using namespace std;

// localizacao_cplex <datafile>
int main(int argc, char *argv[]) {

	// load datafile

	ifstream datafile(argv[1]);

	int n; // numero de facilidades
	int m; // numero de clientes
	datafile >> n >> m;

	vector<int> f(n); // custo de instalacao da facilidade
	for (int j = 0; j < n; j++) {
		datafile >> f[j];
	}

	vector< vector<int> > c(m, vector<int>(n)); // distancia entre o cliente i e a facilidade j (mxn)
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			datafile >> c[i][j];
		}
	}

	datafile.close();

	// inicializa o cplex
	IloEnv env;
	IloModel mod(env);
	IloCplex cplex(mod);
	cplex.setOut(env.getNullStream());

	// variavel booleana indicando se a facilidade vai ser alocada
	IloNumVarArray y(env, n, 0, 1, ILOBOOL);

	// indica se o cliente i sera atendido pela facilidade j
	IloArray<IloNumVarArray> x(env, m);
	for (int i = 0; i < m; i++) {
		x[i] = IloNumVarArray(env, n, 0, 1, ILOBOOL);
	}

	// funcao objetivo - minimizar o custo
	IloExpr objective(env);
	for (int j = 0; j < n; j++) {
		objective += f[j] * y[j];
		for (int i = 0; i < m; i++) {
			objective += c[i][j] * x[i][j];
		}
	}
	IloAdd(mod, IloMinimize(env, objective));
	objective.end();

	// restricoes
	// a conexao só existe se a facilidade estiver alocada
	for (int j = 0; j < n; j++) {
		for (int i = 0; i < m; i++) {
			mod.add(x[i][j] <= y[j]);
		}
	}

	// un cliente só pode ser alocado a uma facilidade
	for (int i = 0; i < m; i++) {
		IloExpr r2(env);
		for (int j = 0; j < n; j++) {
			r2 += x[i][j];
		}
		mod.add(r2 == 1);
		r2.end();
	}

	// resolve o problema
	cplex.solve();

	// mostra o resultado
	double result = cplex.getObjValue();
	cout << result << endl;

	// mostra a solucao
	cout << "j\tfj\t\t*" << endl;
	for (int j = 0; j < n; j++) {
		cout << j+1 << "\t" << f[j] << "\t" << "" << "\t" << cplex.getValue(y[j]) << endl;
	}
	cout << "\t\t\t\t" << endl;
	cout << "i\tj\tcij\t*" << endl;
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			cout << i+1 << "\t" << j+1  << "\t" << c[i][j] << "\t" << cplex.getValue(x[i][j]) << endl;
		}
	}

	return 0;
}
