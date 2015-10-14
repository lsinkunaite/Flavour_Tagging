#include "doocore/io/EasyTuple.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TH1D.h"
#include "TLegend.h"
#include <string>
#include <vector>
#include <TString.h>
#include <TAxis.h>
#include <fstream>
#include <iostream>
#include <TF1.h>
#include <TMath.h>
#include "background.h"

using namespace std;

main(){

  //string file_name = "/fhgfs/groups/e5/lhcb/NTuples/Bd2JpsiKS/MC/MC11a_Bd2JpsiKS_Detached_Stripping17_DVv33r4_FTfromDV_v6_20130529_rniet_combined_FitTuple.root";
  //string file_name = "signal_tree.root";
  string file_name ="/localdata/NTuples/Bd2JpsiKS/Data/2012/DT2012_Bd2JpsiKS_Detached_Stripping20r0_DVv33r5_FTfromDV_v1_20130701_rniet_combined_bestPV_preselected_20130715_ccauet_FitTuple.root";
  //string tree_name = "Bd2JpsiKS";
  string tree_name = "dataset";
  string const_var_name = "obsMass";
  double const_var_lower_range = 5180;
  double const_var_upper_range = 5330;
  double const_var_excluded_lower = 5240;
  double const_var_excluded_upper = 5330;
  double const_var_peak = 5280;
  string var_name = "obsEtaAll";
  double lower_range = 0;
  double upper_range = 0.5; 
  int bins = 65; 
/*
  file_name = name/ location of the tree file
  tree_name = name of the branch
  const_var_name = name of the discrete variable
  const_var_lower_range = lower limit of the discrete variable
  const_var_upper_range = upper limit of the discrete variable
  const_var_excluded_lower = lower value of the excluded peak
  const_var_excluded_upper = upper value of the excluded peak
  const_var_peak = value subtracted from the discrete variable
  var_name = name of the selected variable
  lower_range = lower limit for the selected variable
  upper_range = upper limit for the selected variable
  bins = # of bins
*/
  background (file_name, tree_name, const_var_name, var_name, const_var_lower_range, const_var_excluded_lower, const_var_excluded_upper, const_var_upper_range, const_var_peak, bins, lower_range, upper_range);  

  return 0;
}
