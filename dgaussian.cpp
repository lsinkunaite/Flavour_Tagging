#include "RooFit.h"

#include "Riostream.h"
#include <math.h>

#include "dgaussian.h"
#include "RooAbsReal.h"
#include "RooRealVar.h"
#include "RooRandom.h"
#include "RooMath.h"

#include "RooDoubleGaussian.h"
using namespace std;

ClassImp(RooDoubleGaussian)


//_____________________________________________________________________________
  RooDoubleGaussian::RooDoubleGaussian(const char *name, const char *title, 
		RooAbsReal& _x, RooAbsReal& _mean,
		RooAbsReal& _mean2, RooAbsReal& _sigma,
		RooAbsReal& _sigma2) : RooAbsPdf(name,title),
  x("x","Observable",this,_x),
  mean("mean","Mean",this,_mean),
  mean2("mean2", "Mean2", this, _mean2),
  sigma("sigma","Width",this,_sigma),
  sigma2("sigma2", "Width2", this, _sigma2)
{
}



//_____________________________________________________________________________
RooDoubleGaussian::RooDoubleGaussian(const RooDoubleGaussian& other, const char* name) : 
  RooAbsPdf(other,name), x("x",this,other.x), mean("mean",this,other.mean), mean2("mean2", this, other.mean2),
  sigma("sigma",this,other.sigma), sigma2("sigma2", this, other.sigma2)
{
}



//_____________________________________________________________________________
Double_t RooDoubleGaussian::evaluate() const
{
  Double_t arg= x - mean; 
  Double_t arg2= x - mean2; 
  Double_t sig = sigma ;
  Double_t sig2 = sigma2;
  Double_t ret =exp(-0.5*arg*arg/(sig*sig)) ;
  Double_t ret2 = exp(-0.5*(arg2)*(arg2)/((sig2)*(sig2)));
  return ret ; //struct??
  return ret2; //struct??
}



//_____________________________________________________________________________
Double_t RooDoubleGaussian::getLogVal(const RooArgSet* set) const 
{
  // calculate and return the negative log-likelihood of the Poisson                                                                                                                                    
  return RooAbsPdf::getLogVal(set) ;

  Double_t arg= x - mean;  
  Double_t arg2 = x - mean2;
  Double_t sig = sigma ;
  Double_t sig2 = sigma2;
  
  Double_t extra = -0.5*arg*arg/(sig*sig) - log(analyticalIntegral(1,0)) ;
  Double_t extra2 = -0.5*(arg2)*(arg2)/((sig2)*(sig2)) - log(analyticalIntegral(1.0)); //??
  
  return extra ; //struct??
  return extra2; //struct??
  
}


//_____________________________________________________________________________
Int_t RooDoubleGaussian::getAnalyticalIntegral(RooArgSet& allVars, RooArgSet& analVars, const char* /*rangeName*/) const 
{
  if (matchArgs(allVars,analVars,x)) return 1 ;
  if (matchArgs(allVars,analVars,mean)) return 2 ; //??
  if (matchArgs(allVars, analVars, mean2)) return 3; //??
  return 0 ;
}



//_____________________________________________________________________________
Double_t RooDoubleGaussian::analyticalIntegral(Int_t code, const char* rangeName) const 
{
  assert(code==1 || code==2 || code==3) ;

  static const Double_t root2 = sqrt(2.) ;
  static const Double_t rootPiBy2 = sqrt(atan2(0.0,-1.0)/2.0);
  Double_t xscale = root2*sigma;
  Double_t xscale2 = root2*sigma2;
  Double_t ret = 0;
  Double_t ret2 = 0;
  if(code==1){  
    ret = rootPiBy2*sigma*(RooMath::erf((x.max(rangeName)-mean)/xscale)-RooMath::erf((x.min(rangeName)-mean)/xscale));
    ret2 = rootPiBy2*sigma2*(RooMath::erf((x.max(rangeName)-mean2)/xscale2)-RooMath::erf((x.min(rangeName)-mean)/xscale));
 } else if(code==2) {
    ret = rootPiBy2*sigma*(RooMath::erf((mean.max(rangeName)-x)/xscale)-RooMath::erf((mean.min(rangeName)-x)/xscale));
    ret2 = rootPiBy2*sigma2*(RooMath::erf((mean.max(rangeName)-x)/xscale)-RooMath::erf((mean.min(rangeName)-x)/xscale));
  } else if(code==3) {
    ret = rootPiBy2*sigma*(RooMath::erf((mean2.max(rangeName)-x)/xscale2)-RooMath::erf((mean2.min(rangeName)-x)/xscale2));
    ret2 = rootPiBy2*sigma2*(RooMath::erf((mean2.max(rangeName)-x)/xscale2)-RooMath::erf((mean2.min(rangeName)-x)/xscale2));
  } else{
    cout << "Error in RooDoubleGaussian::analyticalIntegral" << endl;
  }
  return ret ;

}




//_____________________________________________________________________________
Int_t RooDoubleGaussian::getGenerator(const RooArgSet& directVars, RooArgSet &generateVars, Bool_t /*staticInitOK*/) const
{
  if (matchArgs(directVars,generateVars,x)) return 1 ;  
  if (matchArgs(directVars,generateVars,mean)) return 2 ;
  if (matchArgs(directVars, generateVars, mean2)) return 3;  
  return 0 ;
}



//_____________________________________________________________________________
void RooDoubleGaussian::generateEvent(Int_t code)
{
  assert(code==1 || code==2 || code==3) ;
  Double_t xgen ;
  Double_t xgen2;
  if(code==1){
    while(1) {    
      xgen = RooRandom::randomGenerator()->Gaus(mean,sigma);
      xgen2 = RooRandom::randomGenerator()->Gaus(mean2,sigma2);
      if (xgen<x.max() && xgen>x.min()) {
	x = xgen ;
	break;
      }
      else if (xgen2<x.max() && xgen2>x.min()) {
	x = xgen2 ;
	break;
      }
    }
  } else if(code==2){
    while(1) {    
      xgen = RooRandom::randomGenerator()->Gaus(x,sigma);
      xgen2 = RooRandom::randomGenerator()->Gaus(x, sigma2);
      if (xgen<mean.max() && xgen>mean.min()) {
	mean = xgen ;
	break;
      }
      else if (xgen2<mean2.max() && xgen2>mean2.min()) {
	mean2 = xgen ;
	break;
      }
    }
  } else {
    cout << "error in RooDoubleGaussian generateEvent"<< endl;
  }

  return;
}


// mean, mean2, sigma, sigma2 = 4x events


