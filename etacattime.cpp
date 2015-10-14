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
#include <RooCategory.h>
#include <TLine.h>


using namespace std;

main()
{

  doocore::lutils::setStyle();
  doocore::io::EasyTuple et("/localdata/NTuples/Bd2JpsiKS/Data/2012/DT2012_Bd2JpsiKS_Detached_Stripping20r0_DVv33r5_FTfromDV_v1_20130701_rniet_combined_bestPV_preselected_20130715_ccauet_FitTuple.root", "Bd2JpsiKS");

  TTree& tree = et.tree();
  TCanvas canvas("c", "c", 800, 600);
  string var_name = "obsEtaAll";


  std::vector<TH1D>histograms;

  for (int i=1;i<=20;i++) {
  std::stringstream i_s;i_s<<i;
  TH1D eta_time("eta_time_"+TString(i_s.str()), "eta_time", 50, 0, 0.5);
  histograms.push_back(eta_time);
}

  int j = 1;
  std::vector<TH1D>::iterator ii;
  for(ii=histograms.begin(); ii !=histograms.end(); ii++) {
  std::stringstream j_s;j_s<<j;
  tree.Draw(TString(var_name)+">>eta_time_"+TString(j_s.str()), "catVar_obsTime=="+TString(j_s.str()));
  j++;}

  for (int i=1;i<=20;i++) {
  std::stringstream i_s;i_s<<i;
  if (i==1) {histograms[i-1].SetLineColor(kOrange+(i)); histograms[i-1].GetXaxis()->SetTitle("obsEtaAll");
		    histograms[i-1].GetXaxis()->CenterTitle();histograms[i-1].DrawNormalized();}
  else if (i<6) {histograms[i-1].SetLineColor(kOrange+(i)); histograms[i-1].DrawNormalized("same");}
  else if (i<11) {histograms[i-1].SetLineColor(kRed+(i-5));histograms[i-1].DrawNormalized("same");}
  else if (i<16) {histograms[i-1].SetLineColor(kSpring+(i-10));histograms[i-1].DrawNormalized("same");}
  else if (i<21) {histograms[i-1].SetLineColor(kViolet+(i-15));histograms[i-1].DrawNormalized("same");}
  }

 
  doocore::lutils::printPlot(&canvas, TString(var_name)+"_1-20", "DT2012_Bd2JpsiKS/obsEta/"+TString(var_name)+"/bins/");


  return 0;
}
