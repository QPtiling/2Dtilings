#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

// src.cpp //
void read_data(char *,vector<vector<double> > &,vector<vector<int> > &);
void cut_data(vector<vector<double> > &,vector<vector<int> > &,double);

int main(int argc,char **argv)
{
  if(argc!=2){
    cerr << "./GRAPH [binary file]" << endl;
    exit(0);
  }

  vector<vector<double> > site;
  vector<vector<int> > bond;
  read_data(argv[1],site,bond);

  // only consider the circular region with a radius Rmax //
  double Rmax(25.0);
  cut_data(site,bond,Rmax);
  
  ofstream file1("sites");
  for(int i=0;i<site.size();i++)
    file1 << site[i][0] << " " << site[i][1] <<endl;

  ofstream file2("bonds");
  for(int i=0;i<bond.size();i++)
    for(int j=0;j<bond[i].size();j++){
      file2 << site[i][0] << " " << site[i][1] << endl;
      int k=bond[i][j];
      file2 << site[k][0] << " " << site[k][1] << endl << endl;
    }

  return 1;
}
