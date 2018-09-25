/*=========================================================================*/
/* Program: HistMInfo                                                      */
/* Decription: Automutual Information calculation in the form of histogram */
/* Author: Aleksander Dawid                                                */
/*=========================================================================*/

#include <iostream>
#include <fstream>
#include <string>
#define HISTSIZE 100
#define SAMPSIZE 512
using namespace std;

double EEG[SAMPSIZE];
double TT[SAMPSIZE];
double HIST[HISTSIZE];
int X,noFile;


int ReadFile(string FileName) {
	
	X = 0;

	ifstream inFile(FileName);
	if (!inFile.good()) 
	 {
		return 1;
	 }

	while (!inFile.eof()) {
		inFile >> TT[X];
		inFile >> EEG[X++];
	}
	X--;
	noFile++;
	inFile.close();
	return 0;
}

double AutoMutInfo(int T) {
	
	double MI = 0.0;
	double digi = 0, digiT = 0;
	int count_digi = 0, count_digiT = 0;
	int count_pair = 0;
	double p, pT, pp;

	for (int i = 0; i < X-T; i++) {
		digi = EEG[i];
		digiT = EEG[i + T];
		count_digi = 1;
		count_digiT = 1;
		count_pair = 1;
		
		for (int j = 0; j < X; j++) {
			if (j != i) {
				if (digi == EEG[j]) {
					count_digi++;
				}
			}
		}

		for (int j = 0; j < X; j++) {
			if (j != i+T) {
				if (digiT == EEG[j]) {
					count_digiT++;
				}
			}
		}

		for (int j = 0; j < X-T; j++) {
			if (j != i && j != i + T) {
				if (digi == EEG[j] && digiT == EEG[j+T]) {
					count_pair++;
				}
			}
		}

		p = (double)count_digi / (double)(X);
		pT = (double)count_digiT / (double)(X);
		pp = (double)count_pair / (double)(X-T);
			   
	   MI += pp * log10(pp / (p*pT));
	}
	return MI;
}

int main(int argc, char *argv[]) {
	double MIU = 0.0;
	double minMI;
	int idxT = 0;

	char Buf[256];

	if (argc < 2) {
		cout << "Program: Automutual Information calculation in the form of histogram" << endl;
		cout << "Author: Aleksander Dawid" << endl;
		cout << "Organization: WSB Academy (www.wsb.edu.pl)" << endl<<endl;
		cout << "Usage: HistMInfo GroupFileName" << endl;
		cout << "Ex. HistMinfo Signal " << endl;
		cout << "It means Signal000.txt Signal001.txt e.t.c " << endl;
		return 0;
	}
	string FileName;
	
	for (int i = 0; i < HISTSIZE; i++) {
		HIST[i] = 0.0;
	}

	for (int t = 0;;t++) {
		minMI = 1000.0;
		idxT = 0;
		sprintf_s(Buf, "%s%03d.txt", argv[1], t);
		
		FileName = Buf;
		if (ReadFile(FileName) == 1) {
			break;
		}

		for (int dT = 1; dT < SAMPSIZE/2; dT++) {
			MIU = AutoMutInfo(dT);
			if (MIU < minMI) { minMI = MIU; idxT = dT; } //Find minimum of MI		
		}
		if (idxT < 100) {
			HIST[idxT] += 1.0;
		}
		cout << FileName << " t=" << t << " Minimum=" << minMI << " at T=" << idxT << endl;
	}
	
	ofstream OutFile("Histogram.txt");
	for(int i=0;i< HISTSIZE;i++){
		OutFile << i << " " << HIST[i] << endl;
	}
	OutFile.close();
	cout << "Histogram saved" << endl;
	
	return 0;
}