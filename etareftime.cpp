#include "doocore/io/EasyTuple.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TH1D.h"
#include <string>
#include <vector>
#include <TString.h>
#include <TAxis.h>
#include <fstream>
#include <iostream>
#include <TF1.h>
#include <TLine.h>
#include <sstream>
#include "doocore/lutils/lutils.h"
#include <TGraph.h>
#include <TGraphErrors.h>
#include <RooCategory.h>
#include <TLine.h>
#include <TStyle.h>
#include <TLegend.h>
#include <TFitResultPtr.h>
#include <TFitResult.h>
#include <TPaveStats.h>
#include <TBox.h>
#include <TList.h>
#include <time.h>
#include <TRandom3.h>
#include <math.h>
using namespace std;

main(int argc, char *argv[])
{

  doocore::lutils::setStyle();
  string var_name = "obsEtaSSPion";
  doocore::io::EasyTuple et((var_name)+"_signal_sw.root", "dataset");
  //doocore::io::EasyTuple et("/localdata/NTuples/Bd2JpsiKS/Data/2012/DT2012_Bd2JpsiKS_Detached_Stripping20r0_DVv33r5_FTfromDV_v1_20130701_rniet_combined_bestPV_preselected_20130715_ccauet_FitTuple.root", "Bd2JpsiKS");

  TTree& tree = et.tree();
  TCanvas canvas("c", "c", 800, 600);
  double lower_bound = 0.2; double upper_bound = 0.42;
  int n_bins = 45;
  bool sw = true;

  std::vector<TH1D*>histograms;


  TH1D *ref_time = new TH1D ("ref_time", "ref_time", n_bins, lower_bound, upper_bound);
  tree.Draw(TString(var_name)+">>ref_time");

  // # of evenly filled bins
  int n = 20;

  // test uniform
  int n_rand = 2000000;
  TH1D *h1 = new TH1D ("h1", "h1", n_bins, lower_bound, upper_bound);
  TH1D *h2 = new TH1D ("h2", "h2", n_bins, lower_bound, upper_bound);

  TCanvas arual("arual", "arual", 800, 600);
  TRandom3 irand;
  for (int i=1;i<=n_rand;i++){
	if (i%2 == 1) {
	h1->AddBinContent((int)(irand.Uniform(1,n_bins+1)), (irand.Uniform(lower_bound,upper_bound)));}
	else { h2->AddBinContent((int)(irand.Uniform(1,(n_bins+1))), (irand.Uniform(lower_bound,upper_bound)));}
  }
  cout<<"CHI_TEST = "<<h1->Chi2Test(h2, "P CHI/NDF")<<endl;
  h2->Draw();
  h1->SetLineColor(kRed);
  h1->Draw("same");
  arual.SaveAs("chi2u.pdf");
  	
/*
TCanvas nautea("nautea", "nautea", 800, 600);
TH1D *h5 = new TH1D ("h5", "h5", (2000), lower_bound, upper_bound);
TH1D *h6 = new TH1D ("h6", "h6", (2000), lower_bound, upper_bound);
int m = 1;
int k =1;
for (int i=1;m<=2000;i++){
	if (i%2 == 1) {
	h5->AddBinContent(m, (irand2.Gaus(0.5,.1)));
	m++;}
	else {
h6->AddBinContent(k,(irand2.Gaus(0.5,.1)));
k++;
}
}
cout<<"CHI_TEST = "<<h5->Chi2Test(h6, "P CHI/NDF")<<endl;
h5->Draw();
h6->SetLineColor(kRed);
h6->Draw("same");
nautea.SaveAs("nautea.pdf");
*/


//gaussian distribution
/*
double prob(double sigma, double mu, double x) {
  prob = (1/((sigma)*(std::pow((2*(atan(1)*4)),0.5))))*(exp(-((std::pow((x-mu),2))/(2*(std::pow((sigma),2))))));
  return prob;}
double sigma = .1;
double mu =  .5;

int m = 1;
int k =1;
for (int i=1;m<=2000;i++){
	if (i%2 == 1) {
	h5->AddBinContent(m, (irand2.Gaus(0.5,.1)));
	m++;}
	else {
h6->AddBinContent(k,(irand2.Gaus(0.5,.1)));
k++;
}
}
cout<<"CHI_TEST = "<<h5->Chi2Test(h6, "P CHI/NDF")<<endl;
h5->Draw();
h6->SetLineColor(kRed);
h6->Draw("same");
nautea.SaveAs("nautea2.pdf");
*/

TCanvas einstein("einstein", "einstein", 800, 600);
TH1D *h7 =new TH1D ("h7", "h7", n_bins, lower_bound, upper_bound);
TH1D *h8 = new TH1D("h8", "h8", n_bins, lower_bound, upper_bound);
TF1 *f2 = new TF1("f2", "gaus", lower_bound, upper_bound);
f2->SetParameters(1, 0.5, 0.07);
h7->FillRandom("f2", n_rand);
h8->FillRandom("f2", n_rand);
cout <<"CHI_TEST = "<<h7->Chi2Test(h8, "P CHI/NDF")<<endl;
h7->Draw();
h8->SetLineColor(kRed);
h8->Draw("same");
einstein.SaveAs("einstein.pdf");


TCanvas celsius("celsius", "celsius", 800, 600);
TH1D *h9 =new TH1D ("h9", "h9", n_bins, lower_bound, upper_bound);
TH1D *h10 = new TH1D("h10", "h10", n_bins, lower_bound, upper_bound);
 for (int i=1;i<=n_rand;i++){
	if (i%2 == 1) {
	h9->AddBinContent((int)(irand.Uniform(1,n_bins+1)), ref_time->GetRandom());}
	else { h10->AddBinContent((int)(irand.Uniform(1,(n_bins+1))), ref_time->GetRandom());}
  }
   //cout<< "random = "<< hist_normalised->GetRandom()<<endl;
   //cout<< "random = "<< hist_normalised->GetRandom()<<endl;
 cout<<"CHI_TEST = "<< h9->Chi2Test(h10, "P CHI/NDF")<<endl;
 h9->Draw();
 h10->SetLineColor(kRed+1);
 h10->Draw("same");
 celsius.SaveAs("celsius.pdf");



  // creates n obsEta histograms for each time bin
  for (int i=1;i<=n;i++) {
    std::stringstream i_s;i_s<<i;
    TH1D * eta_time = new TH1D ("eta_time_"+TString(i_s.str()), "eta_time", n_bins, lower_bound, upper_bound);
    histograms.push_back(eta_time);
  }

  // fills histograms from a tree
  int j = 1;
  std::vector<TH1D*>::iterator ii;
  for(ii=histograms.begin(); ii !=histograms.end(); ii++) {
    std::stringstream j_s;j_s<<j;
    tree.Draw(TString(var_name)+">>eta_time_"+TString(j_s.str()), "(nSig_sw)*(catVar_obsTime=="+TString(j_s.str())+")");
    //tree.Draw(TString(var_name)+">>eta_time_"+TString(j_s.str()), "(catVar_obsTime=="+TString(j_s.str())+")");
    j++;}

  // x[n] = order of time bin; ex[n] = 0 (error on x-value); y1[n] = p0 from fit function; ey1[n] = error on y1[n]; 
  // y2[n] = p1 from fit function; ey2[n] = error on y2[n]; chi2[n] = Chi2 value of original hist and hist per time bin
  double x[n]; double ex[n];  double y1[n]; double ey1[n];  double y2[n]; double ey2[n]; double chi2[n]; double ref_mean;
  const TH1D *chi_hist = new TH1D("chi_hist","chi_hist",n_bins,1,n_bins);
  double test[n]; double res[n]; double dchi2[n]; double test_chi[n]; double ref_sum; double hist_sum; double test_for_chi; double dtest_chi[n]; double ndf[n]; double test_test_chi2[n]; double chi22[n];

  for (int i=1;i<=n;i++) {

if (i>2) { cout<<i <<" chi2[0] before before initial = " <<chi2[0]<<endl; 
		cout<< "test_test_chi2[0] before initial = "<<test_test_chi2[0]<<endl;
		cout<<"chi22[0] bf initial = "<< chi22[0]<<endl;}//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    std::stringstream i_s;i_s<<i;
    // fills x[n] and ex[n] arrays
    x[i-1] = i;ex[i-1]=0;
    histograms[i-1]->Sumw2();
    //TH1D *hist_normalised = dynamic_cast<TH1D*>(histograms[i-1]->DrawNormalized());
    TH1D *hist_normalised=dynamic_cast<TH1D*>(histograms[i-1]);
    hist_normalised->SetDirectory(0);

    // div_time = ratio histogram
    TH1D *div_time = new TH1D ("div_time", "div_time", n_bins, lower_bound, upper_bound);
    div_time->SetDirectory(0);
    TCanvas b("b", "b", 800, 600);
    ref_time->Sumw2();
    //TH1D * ref_time_normalised = dynamic_cast<TH1D*>(ref_time->DrawNormalized());
    TH1D * ref_time_normalised = dynamic_cast<TH1D*>(ref_time);
    ref_time_normalised->SetDirectory(0);
    //hist_normalised->Draw();
    ref_time_normalised->SetLineColor(kRed);
    ref_time_normalised->SetMarkerColor(kRed);
    ref_time_normalised->Draw();
    hist_normalised->Draw("same");

    const TH1D * ref_time_norm_const = dynamic_cast<TH1D*>(ref_time_normalised);
    //chi2[i-1] = hist_normalised->Chi2Test(ref_time_norm_const, "NORM P CHI2/NDF", res);
    //test_test_chi2[i-1] = chi2[i-1];
    test_test_chi2[i-1] = hist_normalised->Chi2Test(ref_time_norm_const, "NORM P CHI2/NDF", res);
    chi2[i-1] = test_test_chi2[i-1];
    chi22[i-1] = test_test_chi2[i-1];
    dchi2[i-1] = std::pow(chi22[i-1], 0.5);

    cout <<"chi2[0] initial = " << chi2[0]<<endl;
    cout<<"test_test_chi2[0] = " <<test_test_chi2[0]<<endl;

    hist_sum = 0;
    ref_sum = 0;
    test_for_chi =0;
    for (int j=hist_normalised->GetXaxis()->GetFirst(); j<=hist_normalised->GetXaxis()->GetLast(); j++) {
	hist_sum += hist_normalised->GetBinContent(j);
        ref_sum += ref_time_norm_const->GetBinContent(j);
	}
    ndf[i-1] = ((hist_normalised->GetXaxis()->GetLast())-(hist_normalised->GetXaxis()->GetFirst())+1)-1;

    for (int j=hist_normalised->GetXaxis()->GetFirst(); j<=hist_normalised->GetXaxis()->GetLast(); j++) {
	if (sw) {
	test_for_chi += (std::pow((((hist_sum)*(ref_time_norm_const->GetBinContent(j)))-((ref_sum)*(hist_normalised->GetBinContent(j)))),2))/((std::pow((hist_sum),2))*(std::pow((ref_time_norm_const->GetBinError(j)),2))+   (std::pow((ref_sum),2))*(std::pow((hist_normalised->GetBinError(j)),2)));}
	else {
        test_for_chi += ( std::pow(( ( (hist_normalised->GetBinContent(j)) * (ref_sum) ) - ( (ref_time_norm_const->GetBinContent(j)) * (hist_sum) )),2) ) / ( (hist_normalised->GetBinContent(j))+(ref_time_norm_const->GetBinContent(j)) );}	
    }
    
    if (sw) {test_chi[i-1] = (test_for_chi)/(ndf[i-1]);}
    else {test_chi[i-1] = (test_for_chi) / ( (hist_sum) * (ref_sum)*(ndf[i-1]) );}
    dtest_chi[i-1] = std::pow ( (test_chi[i-1]), 0.5);
    cout << "test for chi 2: chi2/ndf = " <<test_chi[i-1] << " chi2 = " << test_chi[i-1] * ndf [i-1] << endl;

    // 20 plots of normalized ref_time (red) and histograms[n] (black)
    doocore::lutils::printPlot(&b, TString(var_name)+"_comp_"+TString(i_s.str()), "DT2012_Bd2JpsiKS/obsEta/"+TString(var_name)+"/bins/");
    div_time->Sumw2();
    div_time->Divide(hist_normalised, ref_time_normalised/*, 1./(double)hist_normalised->GetEntries(), 1./(double)ref_time_normalised->GetEntries()*/);

    // Counts weighted average (1/(error))
    // Calculates arithmetic mean of y
    double numerator = 0;
    double denominator = 0; 
    double mean = 0;
    double y_sum = 0;
    for (int j=div_time->GetXaxis()->GetFirst(); j<=div_time->GetXaxis()->GetLast(); j++) {
    	if ((div_time->GetBinError(j)) != 0) {
    	numerator += (1/(div_time->GetBinError(j)) * (j));
    	denominator += (1/(div_time->GetBinError(j)));}
	y_sum += div_time->GetBinContent(j);
    }

    mean = numerator/denominator;
    double x_mean = (lower_bound) + (((upper_bound) - (lower_bound))*(mean/n_bins));

    // linear fit function
    TF1 fit_function("fit_function", "[0]+[1]*(x-[2])", lower_bound, upper_bound);
    fit_function.SetParameters(1, 0);
    fit_function.FixParameter(2, x_mean);

    div_time->Draw(); 
    TFitResultPtr r = div_time->Fit("fit_function", "S");
    r->Print("V"); 

    y1[i-1] = fit_function.GetParameter(0);
    y2[i-1] = fit_function.GetParameter(1);
    ey1[i-1] = fit_function.GetParError(0);
    ey2[i-1] = fit_function.GetParError(1);
    div_time->Draw();

    // 20 plots of div_time after a linear fit
    doocore::lutils::printPlot(&b, TString(var_name)+"_ref_"+TString(i_s.str()), "DT2012_Bd2JpsiKS/obsEta/"+TString(var_name)+"/bins/");
    }

    // p0 plotted as a function of catVar_obsTime
    TGraphErrors gr(n, x, y1, ex, ey1);
    // fit function for p0 variable
    double numerator_p0 = 0;
    double denominator_p0 = 0;
    double mean_p0 = 0;
    for (int m=1;m<=20;m++) {
  	numerator_p0 += (1/(gr.GetErrorY(m)) * (m));
  	denominator_p0 += (1/(gr.GetErrorY(m)));
    }
    mean_p0 = numerator_p0/denominator_p0;

    // fit function for p0 variable
    TF1 fit_function_p0("fit_function_p0", "[0]+[1]*(x-[2])", 0, n);
    fit_function_p0.SetParameters(1, 0);
    fit_function_p0.FixParameter(2, mean_p0);

    gr.Fit(&fit_function_p0);
    gr.PaintStats(&fit_function_p0);
    //gr.SetObjectStat(1);
    TFitResultPtr r2 = gr.Fit("fit_function_p0", "S");
    r2->Print("V");

    gr.SetTitle("Title");
    gr.GetXaxis()->SetTitle("catVar_obsTime");gr.GetXaxis()->CenterTitle();
    gr.GetYaxis()->SetTitle("p0-0.2_0.42-45");gr.GetYaxis()->CenterTitle();
    gr.SetMarkerColor(8); gr.SetMarkerStyle(21);
    gStyle->SetOptFit(0111);
    gr.Draw("AP");
    gPad->Update();
    TPaveStats *st = (TPaveStats*)gr.GetListOfFunctions()->FindObject("stats");
    st->SetX1NDC(0.72);
    st->SetX2NDC(0.92);
    st->SetY1NDC(0.21);
    st->SetY2NDC(0.41);

    gPad->Modified(); gPad->Update();
    doocore::lutils::printPlot(&canvas, TString(var_name)+"_p0", "DT2012_Bd2JpsiKS/obsEta/"+TString(var_name)+"/bins/");

    // p1 plotted as a function of catVar_obsTime
    TGraphErrors gr2(n, x, y2, ex, ey2);
    double numerator_p1 = 0;
    double denominator_p1 = 0;
    double mean_p1 = 0;
    for (int j=1;j<=20;j++) {
  	numerator_p1 += (1/(gr2.GetErrorY(j)) * (j));
  	denominator_p1 += (1/(gr2.GetErrorY(j)));
    }
    mean_p1 = numerator_p1/denominator_p1;

    // fit function for p1 variable
    TF1 fit_function_p1("fit_function_p1", "[0]+[1]*(x-[2])", 0, n);
    fit_function_p1.SetParameters(1, 0);
    fit_function_p1.FixParameter(2, mean_p1);

    gr2.Fit(&fit_function_p1);
    TFitResultPtr r3 = gr2.Fit("fit_function_p1", "S");
    r3->Print("V");
    gr2.PaintStats(&fit_function_p1);
    gr2.SetTitle("Title");
    gr2.GetXaxis()->SetTitle("catVar_obsTime"); gr2.GetXaxis()->CenterTitle();
    gr2.GetYaxis()->SetTitle("p1-0.2_0.42-45"); gr2.GetYaxis()->CenterTitle();
    gr2.SetMarkerColor(4); gr2.SetMarkerStyle(21);
    gStyle->SetOptFit(0111);
    gr2.Draw("AP");
    gPad->Update(); 
    TPaveStats *st2 = (TPaveStats*)gr2.GetListOfFunctions()->FindObject("stats");
    st2->SetX1NDC(0.72);
    st2->SetX2NDC(0.92);
    st2->SetY1NDC(0.72);
    st2->SetY2NDC(0.92);

    doocore::lutils::printPlot(&canvas, TString(var_name)+"_p1", "DT2012_Bd2JpsiKS/obsEta/"+TString(var_name)+"/bins/");
 

    // Draws Chi2 distribution as a function of time bins
    TGraphErrors chi_graph(n, x, chi22, 0, dchi2);
    chi_graph.GetXaxis()->SetTitle(TString(var_name)); chi_graph.GetXaxis()->CenterTitle();
    chi_graph.GetYaxis()->SetTitle("Chi2/NDF"); chi_graph.GetYaxis()->CenterTitle();
    chi_graph.SetMarkerColor(2); chi_graph.SetMarkerStyle(21);
    chi_graph.Draw("AP");
    doocore::lutils::printPlot(&canvas, TString(var_name)+"_chi2", "DT2012_Bd2JpsiKS/obsEta/"+TString(var_name)+"/bins/");

    TGraphErrors test_graph(n, x, test_chi, 0, dtest_chi);
    test_graph.GetXaxis()->SetTitle(TString(var_name)); test_graph.GetXaxis()->CenterTitle();
    test_graph.GetYaxis()->SetTitle("Chi2"); test_graph.GetYaxis()->CenterTitle();
    test_graph.SetMarkerColor(1); test_graph.SetMarkerStyle(21);
    test_graph.Draw("AP");
    doocore::lutils::printPlot(&canvas, TString(var_name)+"_Test", "DT2012_Bd2JpsiKS/obsEta/"+TString(var_name)+"/bins/");

  return 0;
}
