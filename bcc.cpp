#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <thread>
#include <chrono>
#include <bits/stdc++.h>

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

vector<int> usquare(vector<int> s, vector<int> b, int l, bool * bptr, int n) { //inclure calcul du max ?
	int d = b.size();
	//vector<int> r = s;
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
}

void printarrayarray(vector<vector<int>> a) {
	int len = a.size();
	if (len==0) {
		printf("empty\n");
		return;
	}
	for (int i=1; i<len; i++) {
		printarray(a[i]);
	}
}

int indiceoffirstzero(vector<int> s, int m) { //a ameliorer
	int c = (int)s.size();
	for(int i=m; i<(int)s.size(); i++) {
		if (s[i] == 0) {
			c = i;
			break;
		}
	}
	return c;
}

bool isadmissible(vector<int> b, vector<int> s, int n, int i) {
	bool r = false;
	if (s[i] != n && s[i] != n-1 && s[i+1] != n && s[i+1] != n-1) {
		r = true;
	}
	return r;
}

vector<int> listofnextindices(vector<int> b, vector<int> s, int n, int mu) { //on recalcule pas s
	int d = (int)b.size();
	//int mu = indiceoffirstzero(s);
	vector<int> l;
	int c = b[d-1]+1;
	for (int i=c; i<=mu; i++) {
		if (s[i] != n && s[i] != n-1 && s[i+1] != n && s[i+1] != n-1 && (s[i+2] != n || s[i+3] != n) && (s[i+2] != n-1 || s[i+3] != n-1) /*isadmissible(b, s, n, i)*/) {//pruner efficacement ici peut sauver beaucoup de temps, au moins 80%.
			l.push_back(i);
		}
	}
	return l;
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

void bfs(vector<int> start, vector<int> sq, int n, int m, vector<vector<int>> * e, int k1) {
	int k = (int)start.size();
	if (k==k1) {
		(*e).push_back(start);
	}
	int mu = indiceoffirstzero(sq, m);
	vector<int> l = listofnextindices(start, sq, n, mu);
	for (int i=0; i<(int)l.size(); i++) {
		bool smallerthann = true;
		bool * bptr = &smallerthann;
		vector<int> st = usquare(sq, start, l[i], bptr, n);		
		if(*bptr) {
			if (k < k1) {
				vector<int> t = start;
				t.push_back(l[i]);
				bfs(t, st, n, mu, e, k1);
			}
		}
	}
	return;
}

void e(int k, int n, vector<vector<int>> * eptr) {
	vector<int> b = {0};
	vector<int> s = square(b);
	int m = 0;
	bfs(b, s, n, m, eptr, k);
	return;
}

void is_tree_finite(vector<int> start, vector<int> sq, int n, vector<int> * ptr, vector<int> * rw, vector<long int> * timev, int m) { //start est sous forme de base
	int k = (int)start.size();
	(*ptr)[k]++;
	//printarray(start);
	//printarray(*ptr);
	//printarray(*rw);
	
	int mu = indiceoffirstzero(sq, m);
	vector<int> l = listofnextindices(start, sq, n, mu);
	//int c = start[k-1]+1;
	
	for (int i=0; i<(int)l.size(); i++) {
		bool smallerthann = true;
		bool * bptr = &smallerthann;
		vector<int> st = usquare(sq, start, l[i], bptr, n);
		
		//auto t11 = high_resolution_clock::now();
		/*auto timev4 = duration_cast<nanoseconds>(t8 - t7);
		(*timev)[4] += timev4.count();*/
		
		if(*bptr) {
			if (k < 14) {
				vector<int> t = start;
				t.push_back(l[i]);
				is_tree_finite(t, st, n, ptr, rw, timev, mu);
				(*rw)[0]++;
			}
		}
		else {
			(*rw)[1]++;
		}
	}
	return;
}

int main() {
	vector<int> b = {0};
	vector<int> s = square(b);
	
	vector<int> counter(20, 0);
	vector<int> * ptr = &counter;
	
	vector<int> rw = {0, 0};
	vector<int> * rwptr = &rw;
	
	vector<long int> tv(9, 0);
	vector<long int> * timev = &tv;
	
	/*vector<vector<int>> estart;
	vector<vector<int>> * elist = &estart;
	e(6, 6, elist);
	printarrayarray(*elist);*/
	
	is_tree_finite(b, s, 6, ptr, rwptr, timev, 0);
	printarray(*ptr);
	printarray(*rwptr);
	
	//displaytimevector(*timev);
	
	return 0;
}
