/********************************
*                               *
* Adds s-weights to data sample *
*                               *
********************************/

// ROOT
#include <TCanvas.h> 
#include <TFile.h>
#include <TTree.h>
#include <TMath.h>
#include <TLine.h>
#include <TLegend.h>
#include <TString.h>

// RooFit
#include <RooRealVar.h> 
#include <RooDataSet.h> 
#include <RooStats/SPlot.h>
#include <RooAbsPdf.h> 
#include <RooAddPdf.h> 
#include <RooGaussian.h>
#include <RooExponential.h> 
#include <RooProdPdf.h>
#include <RooConstVar.h> 
#include <RooPlot.h> 
#include <RooAbsDataStore.h>
#include <RooTreeDataStore.h>

// doocore
#include "doocore/io/EasyTuple.h"
#include "doocore/lutils/lutils.h"
#include <doocore/config/EasyConfig.h>

#include "etaroof.h"
 
using namespace RooFit; 
using namespace std;


int add_sw (std::string variable, std::string configfilename)

{ 
    doocore::lutils::setStyle();
    doocore::config::EasyConfig config(configfilename);  

    // Total yield
    int n_events = config.getDouble("sweights.pdf.background.yield") + config.getDouble("sweights.pdf.signal.yield");

    // Extracts mass bounds
    doocore::io::EasyTuple et(config.getString("general.name.file"), config.getString("general.name.tree"));
    std::pair< double, double > mass_bounds = doocore::lutils::MedianLimitsForTuple(et.tree(), config.getString("sweights.observables.mass")); 	

    // Observables
    // If mass bounds are defined, they are used in further calculations, if not, they are extracted out of the data set
    RooRealVar mass(TString(config.getString("sweights.observables.mass")), "mass (MeV)", ((config.getDouble("sweights.observables.mass.lower_bound") != 0.) ? (config.getDouble("sweights.observables.mass.lower_bound")) : (mass_bounds.first)), ((config.getDouble("sweights.observables.mass.upper_bound") != 0.) ? (config.getDouble("sweights.observables.mass.upper_bound")) : (mass_bounds.second)));
    RooRealVar time("obsTime", "obsTime", 0, 30);
    RooRealVar catVar_obsTime("catVar_obsTime", "catVar",1);
    RooRealVar var(variable.c_str(),variable.c_str(), 0); 

    // Builds Gaussian signal PDFs 
    RooRealVar sigMean("sigMean", "sigMean", ((config.getDouble("sweights.observables.mass.lower_bound") != 0.) ? (config.getDouble("sweights.observables.mass.lower_bound")) : (mass_bounds.first)), ((config.getDouble("sweights.observables.mass.upper_bound") != 0.) ? (config.getDouble("sweights.observables.mass.upper_bound")) : (mass_bounds.second)));
    RooRealVar sigWidth("sigWidth", "sigWidth", config.getDouble("sweights.pdf.signal.gaussians.sigma1"), 2, 20); 
    RooGaussian PDF_Mass_Sig("PDF_Mass_Sig", "gaussian PDF", mass, sigMean, sigWidth);
    RooRealVar sigWidth2("sigWidth2", "sigWidth2", config.getDouble("sweights.pdf.signal.gaussians.sigma2"), 2, 20);
    RooGaussian PDF_Mass_Sig2("PDF_Mass_Sig2", "gaussian 2nd PDF", mass, sigMean, sigWidth2);

    // Builds Expo background PDF 
    RooRealVar expo("expo", "expo shape parameter", config.getDouble("sweights.pdf.background.exponential.slope"), -100., 10); 
    RooExponential PDF_Mass_Bkg("PDF_Mass_Bkg", "Exponential PDF", mass, expo); 

    // Builds double Gaussian mass signal PDF
    RooRealVar nFrac("nFrac", "Fraction of 2 Gaussians", config.getDouble("sweights.pdf.signal.gaussians.fraction"), 0,1);
    RooAddPdf PDF_Mass_Sig_Comp("PDF_Mass_Sig", "PDF_Mass_Sig", RooArgList(PDF_Mass_Sig, PDF_Mass_Sig2), RooArgList(nFrac));

    // Constructs signal+background PDF 
    RooRealVar nSig("nSig","#signal events", config.getDouble("sweights.pdf.signal.yield"),0., n_events); 
    RooRealVar nBkg("nBkg","#background events", config.getDouble("sweights.pdf.background.yield"), 0., n_events); 
    RooAddPdf model_mass("model_mass","model_mass",RooArgList(PDF_Mass_Sig_Comp, PDF_Mass_Bkg), RooArgList(nSig, nBkg));

    doocore::io::EasyTuple *data_mc;
    RooDataSet *data;
    data_mc = new doocore::io::EasyTuple(config.getString("general.name.file"), config.getString("general.name.tree"), RooArgSet(mass, time, catVar_obsTime, var));
    data = &data_mc->ConvertToDataSet();
    // Performs extended Maximum Likelihood (ML) fit of composite PDF
    model_mass.fitTo(*data, Extended(),NumCPU(6)); 

    // SPlot
    RooStats::SPlot* sData = new RooStats::SPlot("sData","SPlot",*data, &model_mass, RooArgList(nSig,nBkg));
    RooDataSet dataSig(data->GetName(),data->GetTitle(),data,*data->get(),0,"nSig_sw");
    
    const TTree* Tree_Sig = dataSig.tree();
    Tree_Sig->SaveAs(TString(variable)+"_signal_sw.root");

    delete data, data_mc;
} 

