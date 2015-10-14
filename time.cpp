#include "doocore/io/EasyTuple.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TH1D.h"
#include "TLegend.h"
#include <string>
#include <vector>
#include <TString.h>
#include <TLine.h>
#include <TAxis.h>
#include <fstream>
#include <iostream>
#include <TF1.h>
#include <TMath.h>

using namespace std;

main(){
  doocore::io::EasyTuple et("/fhgfs/groups/e5/lhcb/NTuples/Bd2JpsiKS/MC/MC11a_Bd2JpsiKS_Detached_Stripping17_DVv33r4_FTfromDV_v6_20130529_rniet_combined_FitTuple.root", "Bd2JpsiKS");
  TTree& tree = et.tree();

  TCanvas canvas("c", "c", 800, 600);
  TH1D hist("hist", "hist", 100, 5350,5380);
  TH1D variable_side ("variable_side", "propertime_side", 100, 0, 15);
  TH1D variable_full ("variable_full", "propertime_full", 100, 0, 15);

  tree.Draw("obsMass>>hist", "(obsMass>=5350)&&(obsMass<=5380)");
  tree.Draw("obsPropertime>>variable_side", "(obsMass>=5350)&&(obsMass<=5380)");
  tree.Draw("obsPropertime>>variable_full", "(obsMass>=5180)&&(obsMass<=5380)");

  TF1 fit_function("fit_function", "[0]*(exp([1]*(x-5280)))", 5180,5380);

  hist.Fit("fit_function");
  hist.Draw();

  variable_side.Draw(); 
  variable_full.Draw();

  TH1D *variable_sig = (TH1D*) variable_full.Clone("propertime_sig");
  double background = (fit_function.Integral(5180,5380))/(fit_function.Integral(5350, 5380));
  variable_sig->Add(&variable_side, -background);
  variable_sig->Draw();

  variable_side.SetLineColor(kRed);
  variable_full.SetLineColor(kBlack);
  variable_sig->SetLineColor(kBlue);

  variable_full.Draw();
  variable_sig->Draw("same");
  variable_side.Draw("same");

  canvas.SaveAs("propertime.pdf");

  variable_sig->DrawNormalized();
  variable_side.DrawNormalized("same");
  canvas.SaveAs("propertime_normalized.pdf");


  TH1D bg0_w("bg0_w", "bg0_w", 100, 0,15);
  TH1D bg1_w("bg1_w", "bg1_w", 100, 0,15);

  tree.Draw("obsPropertime>>bg0_w", "(obsMass>=5180)&&(obsMass<=5380)&&(catBkg==0)");
  tree.Draw("obsPropertime>>bg1_w", "(obsMass>=5180)&&(obsMass<=5380)&&(catBkg>0)");

  variable_sig->DrawNormalized();
  bg0_w.SetLineColor(kRed);  
  bg0_w.DrawNormalized("same");
  canvas.SaveAs("bg0_w.pdf");

  variable_side.DrawNormalized();
  bg1_w.SetLineColor(kGreen+4);
  bg1_w.DrawNormalized("same");
  canvas.SaveAs("bg1_w.pdf");

  return 0;
}
