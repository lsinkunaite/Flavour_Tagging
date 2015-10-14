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
#include "etatime.h"
#include <TGraph.h>



using namespace std;

int etatime (string file_name, string tree_name, string var_name, string title_name, string const_var_name, string const_title_name, double var_lower_bound, double var_upper_bound, double const_var_lower_bound, double const_var_upper_bound, int bins) 

{


  doocore::lutils::setStyle();
  doocore::io::EasyTuple et(file_name, tree_name);
 

  TTree& tree = et.tree();
  
  TCanvas canvas("c", "c", 800, 600);
  canvas.SetLogx();
  //TH2D eta_time("eta_time", "eta vs time", bins, const_var_lower_bound, const_var_upper_bound, bins, var_lower_bound, var_upper_bound);
  

  tree.Draw(TString(var_name)+":"+TString(const_var_name), 
	"(obsMass>=5230)&&(obsMass<5240)&&(catTagCompOSvsSSPion!=0)");
  //eta_time.Sumw2();
  //eta_time.Draw();
  //TF1 fit_function("fit_function", "[0]+[1]*x", const_var_lower_bound, const_var_upper_bound);
  //fit_function.SetParameters(0, 1);
  //eta_time.Fit("fit_function");

  //eta_time.Fit("pol1", "EMF");
  
  TGraph *graph = (TGraph*)gPad->GetPrimitive("Graph"); // 2D
  graph->Draw("*");
  

  doocore::lutils::printPlot(&canvas,"TimeFrame_sw","DT2012_Bd2JpsiKS/obsEta/"+TString(var_name)+"/");


  return 0;
}
