#include <stdio.h>  
#include <stdlib.h> 
#include <iostream>
#include <fstream>
#include <cmath>
#include <time.h>

using namespace std; 

int NNheuristic(int n, int *sp, int **s, int *p);
int CalcDur(int no, int *sp, int **s, int *p);
int InterchangeLS(int n, int *s, int dur, int **t);
int Disturb(int n, int *sec, int dur, int **s, int *p);

int main(int argc, char *argv[])
{
	char comment[100];
	clock_t itime, ftime;
	double gtime;
	int n, *p, **s;
	int i, j, cnt;
	int *sp, *sa, *sec; //secuencia parcial, conjunto de tareas disponibles, conjunto de todas las tareas, secuencia de todas las tareas
	int dur0, dur, bdur, iter, MaxIter;
	if (argc != 4) {
		cout << "usage: IterLocSearchCmax <input datafile>, <output solution>, <output table>, <output datafile>" << endl;
		exit(1);
	}
	ifstream indata; // para lectura                          
	indata.open(argv[1]);
	if (!indata) {
		cerr << "Error: Unable to open input file." << endl;
		exit(1);
	}
	indata >> comment >> n;
	indata >> comment;
	p = new int[n + 1];
	for (i = 0; i < n + 1; i++) indata >> p[i];
	indata >> comment;
	s = new int *[n + 1];
	for (i = 0; i < n + 1; i++) s[i] = new int[n + 1];
	for (i = 0; i < n + 1; i++)
		for (j = 0; j < n + 1; j++) indata >> s[i][j];
	indata.close();
	return 0;
}

int CalcDur(int no, int *sp, int **s, int *p)
{
	int dur, i;
	dur = s[sp[0]][sp[1]] + p[sp[1]];
	for (i = 1; i < no; i++) dur += s[sp[i]][sp[i + 1]] + p[sp[i + 1]];
	return dur;
}
int NNheuristic(int n, int *sp, int **s, int *p)
{
	int i, j, k, h, cnt, *sa, m1, m2;
	int dur;
	sa = new int[n + 1];
	for (i = 0; i < n + 1; i++) sa[i] = i;
	cnt = n; k = 0; dur = 0;
	while (cnt) {
		m1 = s[sp[k]][sa[1]] + p[sa[1]];
		j = sa[1]; h = 1;
		for (i = 2; i < cnt + 1; i++) {
			m2 = s[sp[k]][sa[i]] + p[sa[i]];
			if (m1 > m2) {
				m1 = m2; j = sa[i]; h = i;

			}
		}
		sp[k + 1] = j; dur += m1;
		for (i = h; i < cnt; i++) sa[i] = sa[i + 1];
		k++; cnt--;
	}
	return dur;
}
int InterchangeLS(int n, int *s, int dur, int **t)
{
	//Trata de mejorar una soluci�n (S) a traves de intercambios entre dos nodos en una misma secuencia
	int i, j, mejor_i, mejor_j, k1;
	int delta, mejor_delta, dur1, dif;
	dur1 = dur;
	dif = -1;
	while (dif < 0) {
		dif = 1;
		for (i = 1; i <= n - 1; ++i) {
			mejor_i = i;
			j = i + 1;
			if (j < n) mejor_delta = (t[s[i - 1]][s[j]] - t[s[i - 1]][s[i]]) + (t[s[j]][s[i]] - t[s[i]][s[j]]) + (t[s[i]][s[j + 1]] - t[s[j]][s[j + 1]]);
			else mejor_delta = (t[s[i - 1]][s[j]] - t[s[i - 1]][s[i]]) + (t[s[j]][s[i]] - t[s[i]][s[j]]);
			mejor_j = j;
			for (j = i + 2; j <= n; ++j) {
				if (j < n) delta = (t[s[i - 1]][s[j]] - t[s[i - 1]][s[i]]) + (t[s[j]][s[i + 1]] - t[s[i]][s[i + 1]]) + (t[s[j - 1]][s[i]] - t[s[j - 1]][s[j]]) + (t[s[i]][s[j + 1]] - t[s[j]][s[j + 1]]);
				else delta = (t[s[i - 1]][s[j]] - t[s[i - 1]][s[i]]) + (t[s[j]][s[i + 1]] - t[s[i]][s[i + 1]]) + (t[s[j - 1]][s[i]] - t[s[j - 1]][s[j]]);
				if (delta < mejor_delta) {
					mejor_delta = delta;
					mejor_j = j;
				}
			}
			if (mejor_delta < 0) {
				k1 = s[mejor_i];
				s[mejor_i] = s[mejor_j];
				s[mejor_j] = k1;
				dur1 += mejor_delta;
				dif = mejor_delta;

			}
		}
	}
	return dur1;
}
int Disturb(int n, int *sec, int dur, int **s, int *p)
{
	int i, j, m, pos1, pos2, k;
	pos1 = rand() % n/2 + 1;
	pos2 = n / 2 + rand() % n / 2 + 1;
	m = (pos1 + pos2 - 1) / 2;
	for (i = 0; i < m; i++) {
		k = sec[pos1 + i];
		sec[pos1 + i] = sec[pos2 - i-1];
		sec[pos2 - i-1] = k;
	}
	pos1 = rand() % n / 4 + 1;
	pos2 = n / 4 + rand() % n / 4 + 1;
	m = (pos1 + pos2 - 1) / 2;
	for (i = 0; i < m; i++) {
		k = sec[pos1 + i];
		sec[pos1 + i] = sec[pos2 - i-1];
		sec[pos2 - i-1] = k;
	}
	pos1 = n / 2 + rand() % n / 4 + 1;
	pos2 = 3*n / 4 + rand() % n / 4 + 1;
	m = (pos1 + pos2 - 1) / 2;
	for (i = 0; i < m; i++) {
		k = sec[pos1 + i];
		sec[pos1 + i] = sec[pos2 - i-1];
		sec[pos2 - i-1] = k;
	}
	dur = CalcDur(n, sec, s, p);
	return dur;
}