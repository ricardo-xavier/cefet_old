#include <iostream>
#include <vector>

using namespace std;

vector<int> dominancia2(int n, vector<int> v, int V, vector<int> &v2) {

	int b = 0;

	for (int i=0, j=n-1; i<j; ) {
		
		int x = v[i] + v[j];

		if (x == V) {
			//fprintf(stderr, "dominancia %d [%d] %d [%d]\n", i, v[i], j, v[j]);
			v2.insert(v2.end(), v[i]);
			v2.insert(v2.end(), v[j]);
            v[i] = v[j] = -1;
			i++;
			j--;

		} else if (x < V) {
			j--;
		} else {
			i++;
		}
		
	}

    vector<int> result;
	for (int i=0; i<n; i++) {
        if (v[i] != -1) {
			result.insert(result.end(), v[i]);
        }
    }
               
	return result;
}
