/*
 * Solucao do bin-packing de uma dimensao com RENS
 * Autor: Ricardo Costa Xavier
 * Data : 04/11/2019
 */
#include <iostream>
#include <fstream>
#include <vector>
#include <bits/stdc++.h>
#include <ilcplex/ilocplex.h>

using namespace std;

#define is_zero(f) ((f > -0.0001) && (f < 0.0001))
#define is_one(f) ((f > 0.9999) && (f < 1.0001))
#define is_less_equal(a, b) ((a-b) < 0.0001)

vector<float> tmp;

class Index {
    public:
    int i, j;
};

class Constraint {
    public:
    vector<Index> idx;
    bool w;
    char op;
    int b;
    int y; // -1 se o b nao depender de y

    bool ok(vector<vector<float>> bins, vector<int> _w, int n) {
        float v = 0;
        for (int _i=0; _i<idx.size(); _i++) {
            int i = idx[_i].i;
            int j = idx[_i].j;
            if (w) {
                v += _w[i] * bins[i][j];
            } else {
                v += bins[i][j];
            }
        }
        int _b = b;
        if (y != -1) {
            _b *= bins[n][y];
        }
        switch (op) {
            case '=':
                return is_zero(v - _b);
            case '[':
                return is_less_equal(v, _b);
        }
        return false;
    }

    void print(vector<int> _w) {
        /*
        for (int i=0; i<idx.size(); i++) {
            if (w) {
                fprintf(stderr, "%dx%d%d ", _w[i], idx[i].i+1, idx[i].j+1);
            } else {
                fprintf(stderr, "x%d%d ", idx[i].i+1, idx[i].j+1);
            }
        }
        fprintf(stderr, "%c %d", op, b);
        if (y != -1) {
            fprintf(stderr, "y%d", y+1);
        }
        fprintf(stderr, "\n");
        */
    }
};

// empacotamento_rens <datafile>
int main(int argc, char *argv[]) {

	// carrega o arquivo com os dados da instancia do problema
    // =======================================================

	ifstream datafile(argc > 1 ? argv[1] : "empacotamento_6_7.txt");

    // n = numero de itens para empacotar
	int n; 
    // C = capacidade do pacote - todos os pacotes tem a mesma capacidade
	int C; 
	datafile >> n >> C;
	// no pior caso cada item vai ser empacotado em um pacote diferente
    // m = numero de pacotes
	int m = n; 

    // w = vetor com o tamanho dos itens
	vector<int> w(n); 
	for (int i = 0; i < n; i++) {
		datafile >> w[i];
	}

	datafile.close();

    IloEnv env;
	IloTimer crono(env);
	crono.start();

	// resolve o problema relaxado usando o ffd
    // ========================================

    // ordena em ordem descrescente
	sort(w.begin(), w.end(), greater<int>());

    // guarda o percentual utilizado do bin em +1
    vector<float> items(m); 
    vector<vector<float>> bins(n+1, items);

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {

            // espaco utilizado do bin
            float u = bins[n][j] * C;
            // espaco livre no bin
            float f = C - u;

            // verifica se o bin esta cheio
            if (is_zero(f)) {
                continue;
            }

            // verifica se o item cabe inteiro no bin
            if (f >= w[i]) {
                // armazena o item inteiro no bin
                bins[i][j] = 1;
                u += w[i];
                bins[n][j] = u / C; 

            } else {
                // armazena percentual do item no bin atual
                float p = f / w[i];

                // parte do item a ser armazenada no bin atual
                float w1 = w[i] * p;
                bins[i][j] = p; 
                u += w1;
                bins[n][j] = u / C; 

                // parte do item a ser armazenada no proximo bin
                float w2 = w[i] - w1;
                bins[i][j+1] = 1 - p; 
                u = bins[n][j+1] * C + w2;
                bins[n][j+1] = u / C; 
                
            }
            break;

        }
    }

    // mostra a solucao relaxada
    // =========================
    fprintf(stderr, "relaxed solution\n");
    fprintf(stderr, "================\n");
	for (int i = 0; i <= n; i++) {
	    for (int j = 0; j < m; j++) {
            fprintf(stderr, "%.2f\t", bins[i][j]);
        }
        fprintf(stderr, "\n");
    }
    int result = 0;
    for (int j = 0; j < m; j++) {
        if (is_zero(bins[n][j])) {
            continue;
        }
        result++;
        //fprintf(stderr, "y%d = %.2f\n", j+1, bins[n][j]);
    }
    /*
	for (int i = 0; i < n; i++) {
	    for (int j = 0; j < m; j++) {
            if (is_zero(bins[i][j])) {
                continue;
            }
            fprintf(stderr, "x%d%d = %.2f\n", i+1, j+1, bins[i][j]);
        }
    }
    fprintf(stderr, "\n");
    */
    fprintf(stderr, "relaxada %d\n", result);

    // mostra a formulacao do sub-problema
    // ===================================
    /*
    fprintf(stderr, "sub-problem\n");
    fprintf(stderr, "===========\n");
    */

    vector<Constraint> constraints;
    Constraint constraint;
    // conjunto dos itens fracionarios
    vector<Index> F;

    // funcao objetivo
    /*
    fprintf(stderr, "objective:\n");
    fprintf(stderr, "    min ");
	for (int j = 0; j < m; j++) {
        bool fix = false;
	    for (int i = 0; i < n; i++) {
            if (is_one(bins[i][j])) {
                fix = true;
                break;
            }
        }
        if (j > 0) {
            fprintf(stderr, " + ");
        }
        if (fix) {
            fprintf(stderr, "1");
        } else {
            fprintf(stderr, "y%d", j+1);
        }
    }
    fprintf(stderr, "\n");

    fprintf(stderr, "subject to:\n");

    // variaveis com valor inteiro na solucao relaxada
    fprintf(stderr, "    ");
	for (int i = 0; i < n; i++) {
	    for (int j = 0; j < m; j++) {
            if (is_one(bins[i][j])) {
                fprintf(stderr, "x%d%d = ", i+1, j+1);
            }
        }
    }
    fprintf(stderr, "1\n");
    */

    // constraints para variaveis com valor fracionado: sum(xij) = 1 para todo j
	for (int i = 0; i < n; i++) {
        constraint.idx = {};
        bool first = true;
	    for (int j = 0; j < m; j++) {
            if (!is_zero(bins[i][j]) && !is_one(bins[i][j])) {
                Index idx;
                idx.i = i;
                idx.j = j;
                F.push_back(idx);
                if (first) {
                    first = false;
                    //fprintf(stderr, "    ");
                } else {
                    //fprintf(stderr, " + ");
                }
                //fprintf(stderr, "x%d%d", i+1, j+1);
                constraint.idx.push_back(idx);
            }
        }
        if (!first) {
	        for (int j = 0; j < m; j++) {
                if (is_zero(bins[n][j])) {
                    Index idx;
                    idx.i = i;
                    idx.j = j;
                    //fprintf(stderr, " + x%d%d", i+1, j+1);
                    constraint.idx.push_back(idx);
                    F.push_back(idx);
                }
            }
            //fprintf(stderr, " = 1\n");
            constraint.op = '=';
            constraint.b = 1;
            constraint.y = -1;
            constraint.w = false;
            constraints.push_back(constraint);
        }
    }

    // constraints de capacidade para variaveis com valor fracionado
	for (int j = 0; j < m; j++) {
        //bool first = true;
        bool fix = false;
        float u_one = 0;
        float u_all = 0;
	    for (int i = 0; i < n; i++) {
            if (!is_zero(bins[i][j])) {
                u_all += w[i];
            }
            if (is_one(bins[i][j])) {
                u_one += w[i];
                fix = true;
            }
        }

        // bin vazio
        if (is_zero(u_all)) {
            constraint.idx = {};
	        for (int i = 0; i < n; i++) {
                // verifica se o item tem fracao em um bin anterior
                bool f = false;
	            for (int k = 0; k < j; k++) {
                    if (!is_zero(bins[i][k]) && !is_one(bins[i][k])) {
                        f = true;
                        break;
                    }
                }
                if (f) {
                    /*
                    if (first) {
                        first = false;
                        fprintf(stderr, "    ");
                    } else {
                        fprintf(stderr, " + ");
                    }
                    fprintf(stderr, "%dx%d%d", w[i], i+1, j+1);
                    */
                    Index idx;
                    idx.i = i;
                    idx.j = j;
                    constraint.idx.push_back(idx);
                }
            }
            constraint.op = '[';
            constraint.b = C-(int) u_one;
            /*
            fprintf(stderr, " <= %d", C-(int) u_one);
            fprintf(stderr, "y%d", j+1);
            */
            constraint.y = j;
            constraint.w = true;
            constraints.push_back(constraint);
            //fprintf(stderr, "\n");
        }

        // somente itens inteiros
        if (u_one == u_all) {
            continue;
        }
        constraint.idx = {};
	    for (int i = 0; i < n; i++) {
            if (!is_zero(bins[i][j]) && !is_one(bins[i][j])) {
                // fracionado
                /*
                if (first) {
                    first = false;
                    fprintf(stderr, "    ");
                } else {
                    fprintf(stderr, " + ");
                }
                fprintf(stderr, "%dx%d%d", w[i], i+1, j+1);
                */
                Index idx;
                idx.i = i;
                idx.j = j;
                constraint.idx.push_back(idx);
            }
        }
        constraint.op = '[';
        constraint.b = C-(int) u_one;
        //fprintf(stderr, " <= %d", C-(int) u_one);
        if (!fix) {
            //fprintf(stderr, "y%d", j+1);
            constraint.y = j;
        }
        constraint.w = true;
        constraints.push_back(constraint);
        //fprintf(stderr, "\n");
    }

    // executa o RENS

    int iOk = -1;
    for (int f=0; f<F.size(); f++) {

        int i = F[f].i;
        int j = F[f].j;
        //fprintf(stderr, "x%d%d %.2f\n", i+1, j+1, bins[i][j]);
        if (i == iOk) {
            continue;
        }

        bool ok = true;
        // arredonda para cima e zera o restante
        for (int k=0; k<m; k++) {
            tmp.push_back(bins[i][k]);
            if (k == j) {
                float u = bins[n][j] * C;
                float wi = w[i] * bins[i][j];
                if ((u - wi + w[i] - 0.0001) > C) {
                    ok = false;
                    break;
                }
                u = u - wi + w[i];
                bins[n][j] = u / C; 
                bins[i][k] = 1;
            } else {
                bins[i][k] = 0;
            }
        }
        if (!ok) {
            for (int k=0; k<tmp.size(); k++) {
                bins[i][k] = tmp[k];
            }
            tmp.clear();
            continue;
        }

        for (int c=0; c<constraints.size(); c++) {
            constraint = constraints[c];
            constraint.print(w);
            if (!constraint.ok(bins, w, n)) {
                ok = false;
                break;
            }
        } 

        //fprintf(stderr, "ok=%d\n", ok);
        if (!ok) {
            // restaura os valores anteriores
            for (int k=0; k<m; k++) {
                bins[i][k] = tmp[k];
                if (k == j) {
                    float u = bins[n][j] * C;
                    float wi = w[i] * bins[i][j];
                    u = u + wi - w[i];
                    bins[n][j] = u / C; 
                }
            }
        } else {
            iOk = i;
            // ajusta a alocacao dos bins
            for (int k=0; k<m; k++) {
                if ((k != j) && (bins[i][k] != tmp[k])) {
                    float u = bins[n][j] * C;
                    float wi = w[i] * tmp[k];
                    u -= wi;
                    bins[n][k] = u / C; 
                }
            }
            /*
            fprintf(stderr, "partial\n");
            fprintf(stderr, "=======\n");
	        for (int i = 0; i <= n; i++) {
	            for (int j = 0; j < m; j++) {
                    fprintf(stderr, "%.2f\t", bins[i][j]);
                }
                fprintf(stderr, "\n");
            }
            */
        }
        tmp.clear();
    }

    fprintf(stderr, "result\n");
    fprintf(stderr, "======\n");
	for (int i = 0; i <= n; i++) {
	    for (int j = 0; j < m; j++) {
            fprintf(stderr, "%.2f\t", bins[i][j]);
        }
        fprintf(stderr, "\n");
    }

    result = 0;
    for (int j = 0; j < m; j++) {
        if (is_zero(bins[n][j])) {
            continue;
        }
        result++;
    }
    fprintf(stdout, "%d %f\n", result, (double) crono.getTime());

	return 0;
}
