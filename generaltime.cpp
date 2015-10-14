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

using namespace std;

main(){

  //string file_name = "signal_tree.root";
  //string tree_name = "dataset";
  string file_name ="/localdata/NTuples/Bd2JpsiKS/Data/2012/DT2012_Bd2JpsiKS_Detached_Stripping20r0_DVv33r5_FTfromDV_v1_20130701_rniet_combined_bestPV_preselected_20130715_ccauet_FitTuple.root";
  string tree_name = "Bd2JpsiKS";
  string var_name = "obsEtaSSPion";
  string title_name = var_name;
  string const_var_name = "obsTime";
  string const_title_name = const_var_name;
  double var_lower_bound = 0;
  double var_upper_bound = 0.55;
  double const_var_lower_bound = 0;
  double const_var_upper_bound = 5;
  int bins = 1000;

/*
  file_name = name/ location of the tree file
  tree_name = name of the branch
  var_name = y-axis variable
  title_name = y-axis variable
  const_var_name = name of the discrete variable (x-axis variable)
  const_title_name = name of the discrete variable (x-axis variable)
  var_lower_bound = lower limit for y-axis variable
  var_upper_bound = upper limit for y-axis variable
  const_var_lower_bound = lower limit for x-axis variable
  const_var_upper_bound = upper limit for x-axis variable
  bins = # of bins
*/


  etatime (file_name, tree_name, var_name, title_name, const_var_name, const_title_name, var_lower_bound, var_upper_bound, const_var_lower_bound, const_var_upper_bound, bins);  

  return 0;
}
