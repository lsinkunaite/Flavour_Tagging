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
#include "background.h"
#include <RooDataSet.h>
#include <RooRealVar.h>
#include <RooArgSet.h>
#include <RooDataHist.h>
#include <RooPlot.h>
#include <sstream>
#include "doocore/lutils/lutils.h"
#include <TPaveText.h>


using namespace std;

int background (string file_name, string tree_name, string const_var_name, string var_name, double const_var_lower_range, double const_var_excluded_lower, double const_var_excluded_upper, double const_var_upper_range, double const_var_peak, int bins, double lower_range, double upper_range)

{

  doocore::lutils::setStyle();
  doocore::io::EasyTuple et(file_name, tree_name);
 
  // conversion: double -> string
  std::stringstream const_var_lower_range_s; const_var_lower_range_s << const_var_lower_range;
  std::stringstream const_var_excluded_lower_s; const_var_excluded_lower_s << const_var_excluded_lower;
  std::stringstream const_var_excluded_upper_s; const_var_excluded_upper_s << const_var_excluded_upper;
  std::stringstream const_var_upper_range_s; const_var_upper_range_s << const_var_upper_range;
  std::stringstream const_var_peak_s; const_var_peak_s << const_var_peak;

  // RooFit variables
  TTree& tree = et.tree();
  RooRealVar mass(TString(const_var_name),"mass (MeV)", const_var_lower_range, const_var_upper_range);
  RooRealVar omega(TString(var_name),"obsEtaAll", 0, 0.5);
  RooRealVar catBkg("catBkg", "Bkg", 1., -1, 100);
  RooRealVar catTagCompOSvsSSPion("catTagOSvsSSPion", "catTagOSvsSSPion", -2, 2);
  RooRealVar nSig_sw("nSig_sw", "nSig_sw", -5, 5);
  RooRealVar nBkg_sw("nBkg_sw", "nBkg_sw", -5, 5);
  //RooDataSet data_sig("data_sig", "data_sig", &tree, RooArgSet (mass, omega, catBkg, nSig_sw), "", "nSig_sw");
  //RooDataSet data_bkg("data_bkg", "data_bkg", &tree, RooArgSet (mass, omega, catBkg, nBkg_sw), "", "nBkg_sw");
  RooDataSet data_sig("data_sig", "data_sig", &tree, RooArgSet (mass, omega, catTagCompOSvsSSPion, nSig_sw), "", "nSig_sw");
  RooDataSet data_bkg("data_bkg", "data_bkg", &tree, RooArgSet (mass, omega, catTagCompOSvsSSPion, nBkg_sw), "", "nBkg_sw");

  TCanvas canvas("c", "c", 800, 600);
  TH1D hist("hist", "hist", bins, const_var_lower_range, const_var_upper_range);
  TH1D variable_side (TString(var_name)+"_side", TString(var_name)+"_side", bins, lower_range, upper_range);
  TH1D variable_full (TString(var_name)+"_full", TString(var_name)+"_full", bins, lower_range, upper_range);

  // Cuts
  tree.Draw(TString(const_var_name)+">>hist", 
		"(obsMass>="+TString(const_var_lower_range_s.str())+")"+
		"&&(obsMass<"+TString(const_var_excluded_lower_s.str())+")"+
		"&&(obsMass>="+TString(const_var_excluded_upper_s.str())+")"+
		"&&(obsMass<"+TString(const_var_upper_range_s.str())+")"+
		"&&(catTagCompOSvsSSPion!=0)");


  tree.Draw(TString(var_name)+">>"+TString(var_name)+"_side",
		"(obsMass>="+TString(const_var_lower_range_s.str())+")"+
		"&&(obsMass<="+TString(const_var_excluded_lower_s.str())+")"+
		"&&(obsMass<"+TString(const_var_upper_range_s.str())+")"+
		"&&(catTagCompOSvsSSPion!=0)");

/* this works
  tree.Draw(TString(var_name)+">>"+TString(var_name)+"_side",
		"((obsMass>="+TString(const_var_lower_range_s.str())+")"+
		"&&(obsMass<="+TString(const_var_excluded_lower_s.str())+")"+
		"&&((obsMass<"+TString(const_var_upper_range_s.str())+")"+
		"||((obsMass>="+TString(const_var_upper_range_s.str())+"))"+
		"&&(obsMass<"+TString(const_var_excluded_upper_s.str())+")))"+
		"&&(catTagCompOSvsSSPion!=0)");


  tree.Draw(TString(var_name)+">>"+TString(var_name)+"_side", 
		"(obsMass>="+TString(const_var_lower_range_s.str())+")"+
		"&&(obsMass<"+TString(const_var_excluded_lower_s.str())+")"+
		"&&(obsMass>="+TString(const_var_excluded_upper_s.str())+")"+
		"&&(obsMass<"+TString(const_var_upper_range_s.str())+")"+
		"&&(catTagCompOSvsSSPion!=0)");

*/
  tree.Draw(TString(var_name)+">>"+TString(var_name)+"_full", 
		"(obsMass>="+TString(const_var_lower_range_s.str())+")"+
		"&&(obsMass<="+TString(const_var_upper_range_s.str())+")"+
		"&&(catTagCompOSvsSSPion!=0)");


  // Exponential fit function
  TF1 fit_function("fit_function", "[0]*(exp([1]*(x-"+TString(const_var_peak_s.str())+")))", const_var_lower_range, const_var_upper_range);
  fit_function.SetParameters(100, .005);
  hist.Fit("fit_function");

  // Errorbars

  variable_side.Sumw2();
  variable_side.Draw("E1");
  variable_full.Draw("E1");

  TH1D *variable_sig = (TH1D*) variable_full.Clone(TString(var_name)+"_sig");
  double background = (fit_function.Integral(const_var_lower_range,const_var_upper_range))/((fit_function.Integral(const_var_excluded_upper, const_var_upper_range))+(fit_function.Integral(const_var_lower_range,const_var_excluded_lower)));

  variable_side.Scale(background);
  variable_sig->Add(&variable_side, -1);
  variable_sig->Draw();

  variable_full.SetLineColor(kBlack); // black = full
  variable_full.Draw();
  variable_sig->Draw("same");
  variable_side.Draw("same");
  TLine line(lower_range,0,upper_range,0);
 
  // Plots of tail with bkg(red) and without bkg(blue)
  variable_sig->SetMarkerColor(kBlue);
  variable_sig->SetLineColor(kBlue); // blue = signal
  variable_sig->DrawNormalized();
  variable_side.SetMarkerColor(kRed);
  variable_side.SetLineColor(kRed); // red = side part
  variable_side.DrawNormalized("same");
  line.SetLineColor(kBlue+3.5);
  line.Draw("same");

  // Legend for omega_normalized
  TLegend *leg = new TLegend(0.14,0.80,0.34,0.95);
  leg->SetFillColor(kWhite);
  leg->SetTextSize(0.04);
  leg->SetTextColor(kBlack);
  leg->AddEntry(&variable_side,"tail,bkg!=0","l");
  leg->AddEntry(variable_sig,"tail, bkg==0","l");
  leg->Draw();
  // Plot 
  doocore::lutils::printPlot(&canvas,TString(var_name)+"_normalized","DT2012_Bd2JpsiKS/obsEta/"+TString(var_name)+"/");


  // Comparison Frame[Signal], Rescaled
  RooPlot* omegaFrameSig = omega.frame();
  data_sig.plotOn(omegaFrameSig, RooFit::Name("data_sig_plot"), RooFit::MarkerColor(kRed), RooFit::MarkerStyle(34), RooFit::MarkerSize(2), RooFit::LineColor(kRed), RooFit::Binning(bins));
  RooDataHist BkgSub_Sig("BkgSub_Sig", "BkgSub_Sig", omega, variable_sig);
  double BkgSub_Sig_entries = BkgSub_Sig.sumEntries();
  double data_sig_entries = data_sig.sumEntries();
  BkgSub_Sig.plotOn(omegaFrameSig, RooFit::Name("BkgSub_Sig_plot"),RooFit::MarkerColor(kBlack), RooFit::LineColor(kBlack), RooFit::Rescale(data_sig_entries / BkgSub_Sig_entries), RooFit::Binning(bins));
  omegaFrameSig->Draw();
  line.SetLineColor(kBlack);
  line.Draw("same");
  // Legend for comparison [sig] frame
  TLegend *leg_sig = new TLegend(0.14,0.80,0.36,0.95);
  leg_sig->SetFillColor(kWhite);
  leg_sig->SetTextSize(0.04);
  leg_sig->SetTextColor(kBlack);
  leg_sig->AddEntry("BkgSub_Sig_plot","BkgSub_Sig","lep");
  leg_sig->AddEntry("data_sig_plot", "data_sig", "lep");
  leg_sig->Draw();
  doocore::lutils::printPlot(&canvas,"comparisonSig","DT2012_Bd2JpsiKS/obsEta/"+TString(var_name)+"/");

  // Comparison Frame[Bakcground] Rescaled
  RooPlot* omegaFrameBkg = omega.frame();
  data_bkg.plotOn(omegaFrameBkg, RooFit::Name("data_bkg_plot"), RooFit::MarkerColor(kGreen+3), RooFit::LineColor(kGreen+3), RooFit::Binning(bins));
  RooDataHist BkgSub_Bkg("BkgSub_Bkg", "BkgSub_Bkg", omega, &variable_side);
  double BkgSub_Bkg_entries = BkgSub_Bkg.sumEntries();
  double data_bkg_entries = data_bkg.sumEntries();
  BkgSub_Bkg.plotOn(omegaFrameBkg, RooFit::Name("BkgSub_Bkg_plot"), RooFit::MarkerColor(kOrange), RooFit::LineColor(kOrange), RooFit::Rescale(data_bkg_entries / BkgSub_Bkg_entries), RooFit::Binning(bins));
  omegaFrameBkg->Draw();
  line.SetLineColor(kBlack);
  line.Draw("same");
  // Legend for comparison [bkg] frame
  TLegend *leg_bkg = new TLegend(0.14,0.80,0.36,0.95);
  leg_bkg->SetFillColor(kWhite);
  leg_bkg->SetTextSize(0.04);
  leg_bkg->SetTextColor(kBlack);
  leg_bkg->AddEntry("BkgSub_Bkg_plot","BkgSub_Bkg","lep");
  leg_bkg->AddEntry("data_bkg_plot", "data_bkg", "lep");
  leg_bkg->Draw();
  doocore::lutils::printPlot(&canvas,"comparisonBkg","DT2012_Bd2JpsiKS/obsEta/"+TString(var_name)+"/");

  // Comparison Frame
  RooPlot* omegaFrame = omega.frame();
  data_sig.plotOn(omegaFrame, RooFit::Name("data_sig_plot"),RooFit::MarkerColor(kRed), RooFit::MarkerStyle(34), RooFit::MarkerSize(2), RooFit::LineColor(kRed), RooFit::Binning(bins));
  BkgSub_Sig.plotOn(omegaFrame, RooFit::Name("BkgSub_Sig_plot"), RooFit::MarkerColor(kBlack), RooFit::LineColor(kBlack), RooFit::Binning(bins));
  data_bkg.plotOn(omegaFrame, RooFit::Name("data_bkg_plot"), RooFit::MarkerColor(kGreen+3), RooFit::LineColor(kGreen+3), RooFit::Binning(bins));
  BkgSub_Bkg.plotOn(omegaFrame, RooFit::Name("BkgSub_Bkg_plot"), RooFit::MarkerColor(kOrange), RooFit::LineColor(kOrange), RooFit::Binning(bins));
  omegaFrame->Draw();
  line.SetLineColor(kBlack);
  line.Draw("same");
  // Legend for comparison frame
  TLegend *leg2 = new TLegend(0.14,0.80,0.36,0.95);
  leg2->SetFillColor(kWhite);
  leg2->SetTextSize(0.04);
  leg2->SetTextColor(kBlack);
  leg2->AddEntry("BkgSub_Sig_plot","BkgSub_Sig","lep");
  leg2->AddEntry("BkgSub_Bkg_plot","BkgSub_Bkg","lep");
  leg2->AddEntry("data_sig_plot", "data_sig", "lep");
  leg2->AddEntry("data_bkg_plot", "data_bkg", "lep");
  leg2->Draw();
  doocore::lutils::printPlot(&canvas,"comparisonFrame","DT2012_Bd2JpsiKS/obsEta/"+TString(var_name)+"/");




}

