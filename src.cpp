#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

void read_data(char *name,vector<vector<double> > &xy,vector<vector<int> > &bond)
{
  cerr << "Reading " << name;
  ifstream file(name,ios::binary);
  int num;
  file.read((char*)&num,sizeof(int));

  vector<vector<double> > buf1(num,vector<double> (2));
  for(int i=0;i<num;i++)
    file.read((char*)&buf1[i][0],sizeof(double)*2);

  int nbd(0);
  vector<vector<int> > buf2(num);
  for(int i=0;i<num;i++){
    int sz;
    file.read((char*)&sz,sizeof(int));

    if(sz!=0){
      vector<int> buf3(sz);
      file.read((char*)&buf3[0],sizeof(int)*sz);
      swap(buf3,buf2[i]);
    }
    nbd+=sz;
  }
  cerr << endl;
  cerr << " total sites = " << buf1.size() << endl;
  cerr << " total bonds = " << nbd << endl;
  
  swap(xy,buf1);
  swap(bond,buf2);
}

int cut_single_connection(vector<vector<double> > &xy,vector<vector<int> > &bond)
{
  vector<int> ch0(xy.size());

  for(int i=0;i<bond.size();i++){
    for(int j=0;j<bond[i].size();j++){
      ch0[i]++;
      ch0[bond[i][j]]++;
    }
  }

  vector<int> ch,ch2(xy.size(),-1);
  for(int i=0;i<bond.size();i++)
    if(ch0[i]>1){ ch2[i]=ch.size(); ch.push_back(i); }

  vector<vector<double> > buf1;
  for(int i=0;i<ch.size();i++) buf1.push_back( xy[ch[i]] );

  vector<vector<int> > buf2;
  for(int i=0;i<ch.size();i++){
    int i1=ch[i];
    vector<int> buf;
    for(int j=0;j<bond[i1].size();j++)
      if(ch2[bond[i1][j]]!=-1) buf.push_back(ch2[bond[i1][j]]);
    buf2.push_back(buf);
  }

  int ret=xy.size()-buf1.size();
  
  swap(buf1,xy);
  swap(buf2,bond);
  
  return ret;
}

void cut_data(vector<vector<double> > &xy,vector<vector<int> > &bond,double rmx)
{
  vector<int> ch,ch2(xy.size(),-1);
  int num(0);
  for(int i=0;i<xy.size();i++){
    double r2=xy[i][0]*xy[i][0]+xy[i][1]*xy[i][1];
    if(r2<rmx*rmx){ ch2[i]=ch.size(); ch.push_back(i); }
  }

  vector<vector<double> > buf1;
  for(int i=0;i<ch.size();i++) buf1.push_back( xy[ch[i]] );

  vector<vector<int> > buf2;
  for(int i=0;i<ch.size();i++){
    int i1=ch[i];
    vector<int> buf;
    for(int j=0;j<bond[i1].size();j++)
      if(ch2[bond[i1][j]]!=-1) buf.push_back(ch2[bond[i1][j]]);
    buf2.push_back(buf);
  }

  while(cut_single_connection(buf1,buf2)!=0);
  
  cerr << "In the circular region ( R = " << rmx << " ), ";
  cerr << "total number of sites : " << buf1.size() << endl;
  
  
  swap(buf1,xy);
  swap(buf2,bond);
}

void save_data(vector<vector<double> > &xy,vector<vector<int> > &bond)
{
  ofstream file("newdata.bin",ios::binary);

  int num(xy.size());
  file.write((char*)&num,sizeof(int));

  for(int i=0;i<num;i++)
    file.write((char*)&xy[i][0],sizeof(double)*2);

  for(int i=0;i<bond.size();i++){
    int sz(bond[i].size());
    file.write((char*)&sz,sizeof(int));
    file.write((char*)&bond[i][0],sizeof(int)*sz);
  }
  
}
