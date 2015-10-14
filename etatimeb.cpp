#include "doocore/io/EasyTuple.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TLegend.h"
#include <string>
#include <vector>
#include <TString.h>
#include <TAxis.h>
#include <fstream>
#include <iostream>
#include <TF1.h>
#include <TF2.h>
#include <TMath.h>
#include <TLine.h>
#include <TAxis.h>
#include <RooDataSet.h>
#include <RooRealVar.h>
#include <RooArgSet.h>
#include <RooDataHist.h>
#include <RooPlot.h>
#include <sstream>
#include "doocore/lutils/lutils.h"
#include "etatimeb.h"
#include <TGraph.h>



using namespace std;

int etatime (string file_name, string tree_name, string var_name, string title_name, string const_var_name, string const_title_name, double var_lower_bound, double var_upper_bound, double const_var_lower_bound, double const_var_upper_bound, int bins) 

{

  doocore::lutils::setStyle();
  doocore::io::EasyTuple et(file_name, tree_name);
 
/*
// Plots with intervals of 1.
  double i;
  double j = const_var_lower_bound+1.0;
  for (i=const_var_lower_bound;j<=const_var_upper_bound;i=i) {

  std::stringstream i_s; i_s << i;
  std::stringstream j_s; j_s << j;
  std::stringstream const_var_upper_bound_s; const_var_upper_bound_s << const_var_upper_bound;

  TTree& tree = et.tree();
  TCanvas canvas("c", "c", 800, 600);
  
  tree.Draw(TString(var_name)+":"+TString(const_var_name),"(catTagCompOSvsSSPion!=0)&&"+TString(const_var_name)+">="+TString(i_s.str())+"&&"+TString(const_var_name)+"<="+TString(j_s.str()));

  TGraph *graph = (TGraph*)gPad->GetPrimitive("Graph");
  graph->Draw("*");
  
  doocore::lutils::printPlot(&canvas,"TimeFrame_"+TString(i_s.str())+"_"+TString(j_s.str()),"DT2012_Bd2JpsiKS/obsEta/"+TString(var_name)+"/bins/");
  j+=.5;
  i+=.5;
  }
*/


    TTree& tree = et.tree();

    ifstream file;
    file.open("/home/lsinkunaite/sandbox/main/bins.txt");
    string line;
    getline(file,line);
    string lower_bound = line;
    string upper_bound = lower_bound;
    if (file.is_open()) {
    while(!file.eof()) {
	lower_bound = upper_bound;
	if (lower_bound=="18.3") {break;}
        getline(file,line);
	upper_bound = line;


  cout << "("+TString(const_var_name)+">="+TString(lower_bound)+")&&("+TString(const_var_name)+"<"+TString(upper_bound)+")" << endl;
  TCanvas canvas("c", "c", 800, 600);
  
  tree.Draw(TString(var_name)+":"+TString(const_var_name),"("+TString(const_var_name)+">="+TString(lower_bound)+")&&("+TString(const_var_name)+"<"+TString(upper_bound)+")");

  TGraph *graph = (TGraph*)gPad->GetPrimitive("Graph");
  graph->Draw("*");
  
  doocore::lutils::printPlot(&canvas,"TimeFrame_"+TString(lower_bound)+"_"+TString(upper_bound),"DT2012_Bd2JpsiKS/obsEta/"+TString(var_name)+"/bins/");

  delete graph, gPad;
    }
    }
    file.close();



  return 0;
}
