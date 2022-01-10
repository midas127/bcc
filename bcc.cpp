#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <thread>
#include <chrono>
#include <bits/stdc++.h>
#include <typeinfo>

using namespace std;
using namespace std::chrono;

vector<int> square(vector<int> b) {
	int d = b.size();
	int m = b[d-1];
	vector<int> s(2*m +1, 0);
	for (int i=0; i<d; i++) {
		for (int j=0; j<i; j++) {
			s[b[i]+b[j]] += 2;
		}
		s[2*b[i]]++;
	}
	return s;
}

vector<int> usquare(vector<int> s, vector<int> & b, int l, bool * bptr, int n) {
	int d = b.size();
	s.resize(2*l +1, 0);
	for (int i=0; i<d; i++) {
		if (s[l+b[i]] > n-2) {
			*bptr = false;
			return s;
		}
		s[l+b[i]] += 2;
	}
	s[l+l]++;
	return s;
}

void rusquare(vector<int> &s, vector<int> & b, int l, bool * bptr, int n) {
	int d = b.size();
	s.resize(2*l +1, 0);
	for (int i=0; i<d; i++) {
		if (s[l+b[i]] > n-2) {
			*bptr = false;
			return;
		}
		s[l+b[i]] += 2;
	}
	s[l+l]++;
}

vector<int> desquare(vector<int> s, vector<int> & b, int l) {
	s[l+l]--;
	int d = b.size();
	for (int i=0; i<d; i++) {
		s[l+b[i]] -= 2;
	}
	int u = b[d-1];
	s.resize(2*u +1, 0);
	return s;
}



void printarray(vector<int> a) {
	int len = a.size();
	if (len==0) {
		printf("{%d}\n", a[0]);
		return;
	}
	printf("{");
	printf("%d", a[0]);
	for (int i=1; i<len; i++) {
		printf(",%d", a[i]);
	}
	printf("}\n");
	return;
}

void printall(vector<vector<int>> a) {
	for (int i=0; i<(int)a.size(); i++){
		printf("%d : ", i+1);
		printarray(a[i]);
	}
	return;
}

int indiceoffirstzero(vector<int> & s, int m) { //a ameliorer
	int c = (int)s.size();
	for(int i=m; i<(int)s.size(); i++) {
		if (s[i] == 0) {
			c = i;
			break;
		}
	}
	return c;
}

void rwprint(vector<long int> rw) {
	long int total = 0;
	for(int i=0; i<3; i++) {
		printf("%li  ", rw[i]);
		total += rw[i];
	}
	printf("\n");
	vector<double> d(3, 0);
	for(int i=0; i<3; i++) {
		d[i] = 100*((double)rw[i])/((double)total);
	}
	int e = (int)d[0];
	printf("Efficacité de Cerbère : %d %% \n", e);
}

void displaytimevector(vector<long int> t) {
	long int total = 0;
	for(int i=0; i<9; i++) {
		total += t[i];
	}
	vector<double> d(9, 0);
	for(int i=0; i<9; i++) {
		d[i] = ((double)t[i])/((double)total);
	}
	printf("size : %lf\n", 100*d[0]);
	printf("pointer update : %lf\n", 100*d[1]);
	printf("square : %lf\n", 100*d[2]);
	printf("list of next indices : %lf\n", 100*d[3]);
	printf("copy : %lf\n", 100*d[4]);
	printf("pushback : %lf\n", 100*d[5]);
	printf("new square : %lf\n", 100*d[6]);
	printf("max coeff : %lf\n", 100*d[7]);
	printf("mu : %lf\n", 100*d[8]);
}

bool prunecondition(vector<int> & b, vector<int> & s, int n, int i, int k, int c, int mu) { // k = b.size(), c = b[k-1] + 1
	bool cerbere = true; //meilleur videur du monde
	int ssize = (int)s.size();
	if (i>=ssize) {
		return cerbere;
	}
	if (s[i] > n-2) {
		cerbere = false;
	}
	if (i+1 < ssize && s[i+1] > n-2) {
		cerbere = false;
	}
	if (k > 2) {
		int b2 = b[2];
		if (b2 == 2 && i+2 < ssize) {
			if (s[i+2] > n-2) {
				cerbere = false;
			}
		}
		if (b2 == 3 && i+3 < ssize) {
			if (s[i+3] > n-2) {
				cerbere = false;
			}
		}
	}
	if (k > 3) {
		int b3 = b[3];
		if (b3 == 3 && i+3 < ssize) {
			if (s[i+3] > n-2) {
				cerbere = false;
			}
		}
		if (b3 == 4 && i+4 < ssize) {
			if (s[i+4] > n-2) {
				cerbere = false;
			}
		}
		if (b3 == 5 && i+5 < ssize) {
			if (s[i+5] > n-2) {
				cerbere = false;
			}
		}
	}
	return cerbere;
}

void bfs(vector<int> start, vector<int> sq, int n, int m, vector<vector<int>> * ev, int k1) {
	int k = (int)start.size();	
	if (k == k1) {
		(*ev).push_back(start);
	}
	
	int mu = indiceoffirstzero(sq, m);
	int c = start[k-1]+1;
	
	for (int i=c; i<=mu; i++) {
		if (prunecondition(start, sq, n, i, k, c, mu)) {
			bool smallerthann = true;
			vector<int> st = usquare(sq, start, i, &smallerthann, n);
			if(smallerthann) {
				if (k < k1) {
					start.push_back(i);
					bfs(start, st, n, mu, ev, k1);
					start.pop_back();
				}
			}
		}
	}
	return;
}

void e(int k, int n, vector<vector<int>> * ev) {
	vector<int> b = {0};
	vector<int> s = square(b);
	int m = 0;
	bfs(b, s, n, m, ev, k);
	return;
}

void is_tree_finite(vector<int> & start, vector<int> & sq, int n, vector<int> * ptr, vector<long int> * rwt, vector<long int> * timev, int m, int kmax) { //start est sous forme de base
	int k = (int)start.size();
	(*ptr)[k]++;
	//printarray(start);
	//printarray(*ptr);
	//printarray(*rwt);
	
	if (k >= kmax) {
		return;
	}
	
	int mu = indiceoffirstzero(sq, m); //optimisation possible
	int c = start[k-1]+1;
	
	for (int i=c; i<=mu; i++) {
		//condition de prunage
		if (prunecondition(start, sq, n, i, k, c, mu)) { //optimisation tres possible
			bool smallerthann = true;
			vector<int> st = usquare(sq, start, i, &smallerthann, n);
			//rusquare(sq, start, i, &smallerthann, n);
			if(smallerthann) {
				if (k < kmax) {
					start.push_back(i);
					is_tree_finite(start, st, n, ptr, rwt, timev, mu, kmax);
					start.pop_back();
					(*rwt)[0]++;
				}
				else {
					(*rwt)[2]++;
				}
			}
			else {
				(*rwt)[1]++;
			}
			/*vector<int> a;
			a = desquare(st, start, i);
			if (a != sq) {
				printf("wrong desquare \n");
				printf("start ");
				printarray(start);
				printf("sq ");
				printarray(sq);
				printf("i : %d\n", i);
				printf("st ");
				printarray(st);
				printf("a: ");
				printarray(a);
			}*/
		}
	}
	return;
}

void searchwithonethread(int depth, int n, int countersize, vector<int> b) {
	vector<int> s = square(b);
	
	vector<int> counter(countersize, 0);
	vector<int> * ptr = &counter;
	
	vector<long int> rw = {0, 0, 0};
	vector<long int> * rwptr = &rw;
	
	vector<long int> tv(9, 0);
	vector<long int> * timev = &tv;
	
	is_tree_finite(b, s, n, ptr, rwptr, timev, 0, depth);
	printarray(b);
	printarray(*ptr);
	rwprint(*rwptr);
}

/*void proxy(int depth, int n, int countersize, vector<vector<int>> worklist) {
	for (int i=0; i<(int)worklist.size(); i++) {
		searchwithonethread(depth, n, countersize, worklist[i]);
	}
}

vector<vector<int>> createworklist(vector<vector<int>> ev, int nc, int id) {
	int a = (int)ev.size();
	vector<vector<int>> wl = {{}};
	int l = a/nc;
	for (int i=0; i<a; i++) {
		if ((i >= l*id && i < l*(id+1) && id != nc-1) || (id == nc-1 && l>=l*id))
		wl.push_back(ev[i]);
	}
	return wl;
}*/

void threadedsearch(int depth, int n, int countersize) {
	int nthreads = (int)thread::hardware_concurrency();
	printf("threads %d\n", nthreads);
	
	vector<vector<int>> ev = {};
	vector<vector<int>> * evptr = &ev;
	e(4, n, evptr);
	printall(ev);
	
	
	std::vector<thread> threads(nthreads);
	for (int i = 0; i < min(nthreads, (int)ev.size()); i++) {
		vector<int> ei = ev[i];
		threads[i] = thread(searchwithonethread, depth, n, countersize, ei);
	}
	for (auto& th : threads) {
		th.join();
		printf("thread joined \n");
	}
	printf("out of the loop");
	return;
}

int main() {	
	vector<int> b = {0};	
	searchwithonethread(17, 6, 20, b);
	//threadedsearch(17, 6, 50);	
	return 0;
}
