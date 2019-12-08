/*
 * Solucao do bin-packing de uma dimensao com relaxacao lagrangeana
 * Autor: Ricardo Costa Xavier
 */
#include <iostream>
#include <fstream>
#include <vector>
#include <ilcplex/ilocplex.h>

#define M 10e13
#define TimeLimit 240

using namespace std;

// empacotamento_lagrangeana <datafile>
int main(int argc, char *argv[]) {

	// carrega o arquivo com os dados da instancia do problema
    // =======================================================

	ifstream datafile(argc > 1 ? argv[1] : "empacotamento_3_7.txt");

	int num_itens; 
    // tamanho do bin
	int S; 
	datafile >> num_itens >> S;
	// no pior caso cada item vai ser empacotado em um pacote diferente
	int num_bins = num_itens; 

    // w = vetor com o tamanho dos itens
	vector<int> w(num_itens); 
	for (int i = 0; i < num_itens; i++) {
		datafile >> w[i];
	}

    // W - vetor de utilizacao do bin
	vector<int> W(num_bins); 

	datafile.close();
    
    IloEnv env;
    IloModel mod_sp(env);
    IloCplex cplex_sp(mod_sp);

    // variaveis de decisao
    IloNumVarArray vy(env, num_bins, 0, 1, ILOFLOAT);
    IloArray<IloNumVarArray> vx(env, num_itens);
    for (int i = 0; i < num_itens; i++) {
        vx[i] = IloNumVarArray(env, num_bins, 0, 1, ILOFLOAT);
    }

    // variaveis auxiliares
	IloNumArray y(env, num_bins); 
	IloArray<IloNumArray> x(env, num_itens);  
	for (int i = 0; i < num_itens; i++) {
		x[i] = IloNumArray(env, num_bins);	
    }

    // coeficientes
	IloArray<IloNumArray> coef_x(env, num_itens); 
	for(int i = 0; i < num_itens; i++) {
		coef_x[i] = IloNumArray(env, num_bins);
    }

    // funcao objetivo do sub-problema
    // min cx + pi ( d - Dx )
    // restricao adicionada ao objetivo: sum(xij) = 1
    // cx = yj
    // d = 1
    // Dx = xij
    IloExpr expfo_sp(env);
    char name[257];
    for (int j = 0; j < num_bins; j++) {
        sprintf(name, "y%d", j+1);
        expfo_sp += vy[j];
        vy[j].setName(name);
        for (int i = 0; i < num_itens; i++) {
            expfo_sp += vx[i][j];
            sprintf(name, "x%d%d", i+1, j+1);
            vx[i][j].setName(name);
        }
    }
    IloObjective obj_sp = IloAdd(mod_sp, IloMinimize(env, expfo_sp));
    expfo_sp.end();    

    // restricao mantida
	for (int j = 0; j < num_bins; j++) {
		IloExpr r1(env);
		for (int i = 0; i < num_itens; i++) {
			r1 += w[i] * vx[i][j];
		}
		r1 -= S * vy[j];
		mod_sp.add(r1 <= 0);
		r1.end();
	}

    // restricao relaxada
    /*
	for (int i = 0; i < n; i++) {
		IloExpr r2(env);
		for (int j = 0; j < m; j++) {
			r2 += vx[i][j];
		}
		mod_sp.add(r2 == 1);
		r2.end();
	}
    */

    cplex_sp.exportModel("model.lp");
	cplex_sp.setWarning(env.getNullStream());
	cplex_sp.setOut(env.getNullStream());
	IloTimer crono(env);
	crono.start();
	float UB = M;
	float LB = -M;
	int t = 1;
	IloNumArray pi(env, num_itens);
	IloNumArray subGradiente(env, num_itens);

    // inicializa os pis com o indice do item
	for (int i = 0; i < num_itens; i++) {
	    pi[i] = -w[i];
	}
    float C = num_itens * num_bins;

    // variavel utilizada na heuristica lagrangeana
    vector<vector<float>> xh(num_itens); 
	for (int i = 0; i < num_itens; i++) {  
        xh[i] = vector<float>(num_bins); 
        for (int j = 0; j < num_bins; j++) {
            xh[i][j] = x[i][j];
        }
    }

	//while ((1 - LB/UB) > 0.001  and crono.getTime() < TimeLimit) {
	while (true) {
        fprintf(stderr, "=============================== %d\n", t);

        // seta os coeficientes
	    for (int i = 0; i < num_itens; i++) {
		    for (int j = 0; j < num_bins; j++) {
			    coef_x[i][j] = pi[i]; 
		    }
            fprintf(stderr, "pi %d = %f\n", i, pi[i]);
	    }
	    for (int i = 0; i < num_itens; i++) {
		    obj_sp.setLinearCoefs(vx[i], coef_x[i]);
	    }

	    cplex_sp.solve();

        float obj = cplex_sp.getObjValue();
        float pis = IloSum(pi);
	    //LB = max(LB, (float) (cplex_sp.getObjValue() + IloSum(pi)));
	    LB = cplex_sp.getObjValue() + IloSum(pi);
        fprintf(stderr, "OBJ = %f\n", obj);
        fprintf(stderr, "PIS = %f\n", pis);
        fprintf(stderr, "LB  = %f\n", LB);

        // mostra os valores das variaveis
		cplex_sp.getValues(y, vy);
		for (int j = 0; j < num_bins; j++) {
            fprintf(stderr, "y %d = %f\n", j, y[j]);
        }
		for(int i = 0; i < num_itens; i++) {  
			cplex_sp.getValues(x[i], vx[i]);
            for (int j = 0; j < num_bins; j++) {
                fprintf(stderr, "x %d %d = %f\n", i, j, x[i][j]);
			}
        }

        // heuristica lagrangeana
	    for (int j = 0; j < num_bins; j++) {
            W[j] = 0;
        }

		for (int i = 0; i < num_itens; i++) {  
            for (int j = 0; j < num_bins; j++) {
                xh[i][j] = x[i][j];
            }
        }

        // corrige itens que foram alocados a mais de um bin
		for (int i = 0; i < num_itens; i++) {  
            bool alocado = false;
            bool alocar = false;
            for (int j = 0; j < num_bins; j++) {
                if (xh[i][j] <= 0.001) {
                    continue;
                }
                if (alocado) {
                    // o item foi alocado a mais de um bin
                    // verificar se tem que alocar(caso tenha sido desalocado do bin original)
                    if (alocar && ((S - W[j]) >= w[i])) {
                        xh[i][j] = 1;
                        W[j] += w[i];
                        alocar = false;
                        fprintf(stderr, "B %d %d %d\n", i, j, W[j]);
                    } else {
                        xh[i][j] = 0;
                    }
                } else {
                    // o item vai ficar no primeiro bin em que foi alocado(se couber)
                    if ((S - W[j]) >= w[i]) {
                        xh[i][j] = 1;
                        W[j] += w[i];
                        fprintf(stderr, "A %d %d %d\n", i, j, W[j]);
                    } else {
                        // nao cabe, alocar a outro bin
                        xh[i][j] = 0;
                        alocar = true;
                    }
                    alocado = true;
                }
            }
        }

        // corrige itens que nao foram alocados
		for (int i = 0; i < num_itens; i++) {  
            bool alocado = false;
            for (int j = 0; j < num_bins; j++) {
                if (xh[i][j] > 0.001) {
                    alocado = true;
                    break;
                }
            }
            if (alocado) {
                continue;
            }
            // aloca o item ao primeiro bin com espaco
            for (int j = 0; j < num_bins; j++) {
                if ((S - W[j]) >= w[i]) {
                    xh[i][j] = 1;
                    W[j] += w[i];
                    fprintf(stderr, "C %d %d %d\n", i, j, W[j]);
                    break;
                }
            }
        }

        // calcula o upper bound
        UB = 0;
        for (int j = 0; j < num_bins; j++) {
            fprintf(stderr, "W %d = %d\n", j, W[j]);
            if (W[j] > 0) {
                UB++;
            }
        }
        fprintf(stderr, "UB = %f\n", UB);

        // calcula o subgradiente
        for (int i = 0; i < num_itens; i++) {
			subGradiente[i] = 1;
			for (int j = 0; j < num_bins; j++){  
				subGradiente[i] -= x[i][j];
			}
            fprintf(stderr, "subgradiente %d = %f\n", i, subGradiente[i]);
		}
		float somaQuadrados = 0.0;
		for (int i = 0; i < num_itens; i++) {
			somaQuadrados += IloPower(subGradiente[i], 2);
		}
		double normaSubGradiente = sqrt(somaQuadrados);
        fprintf(stderr, "norma = %f\n", normaSubGradiente);

        // atualiza os multiplicadores
		float alfa = (float) C * pow(0.5, (float) t);
        float mi = alfa / normaSubGradiente;
		for (int i = 0; i < num_itens; i++) {  
			//pi[i] = (double) (pi[i] - mi * subGradiente[i]);
			pi[i] = (double) (pi[i] + mi * subGradiente[i]);
			//min(0.00, pi[i] = (double) (pi[i] + mi * subGradiente[i]));
		}

        fprintf(stdout, "%d\t%f\t%f\t%f\n", t, LB, UB, (double) crono.getTime());

		t++;
        if (t > 20) break;
    }

    return 0;
}
