# etaroof.cpp
-Adds s-weights.
	-If (sig) != (bkg), adds s-weights to data file; then if (sig) selected, only sw-signal is used, if (bkg), only sw-background.
	-If n_bins is not specifically identified in the variable's description, n_bins general is used.
	-If mass bounds are defined, they are used in further calculations, if not, they are extracted from the data set.

# etareftimef.cpp
- Creates 20 plots of full histogram and histogram per particular time bin only for every evenly filled time bin.
- Plots Chi2/NDF results of variable for 20 evenly filled time bins.

# Sample configuration file:
general 
 {
    name
     {    
       file "/localdata/NTuples/Bd2JpsiKS/Data/2012/DT2012_Bd2JpsiKS_Detached_Stripping20r0_DVv33r5_FTfromDV_v1_20130701_rniet_combined_bestPV_preselected_20130715_ccauet_FitTuple.root"
       tree "Bd2JpsiKS"
     }

    sig "true"
    bkg "false"

    n_bins "45"

    variables
     {
       obsEtaAll
	{
	  min "0.1"
	  max "0.5"
	  n_bins "40"
	}
       obsEtaOS
	{
	  min "0.1"
	  max "0.5"
	}
       obsEtaSSPion
	{
	  min "0.2"
	  max "0.42"
	}
       varKS0TauSignificance
	{
	  min "0"
	  max "200"
	}
    }

  }

sweights
  { 
    observables 
	{
	mass "obsMass" 
	 {
	   lower_bound "5230"
  	   upper_bound "5330"
 	   peak_position "5280"
	 }
	
	eta "obsEtaAll"
 	 {
   	   lower_bound "0"
   	   upper_bound "0.5"
	 }

	time "obsTime"
 	 {
   	   lower_bound "0"
   	   upper_bound "30"
 	 }
      }

    pdf 
     {
       signal 
	{
	  gaussians
	   {
	     mean "0.35"
	     sigma1 "7"
	     sigma2 "15"
	     fraction "0.5"
	   }	
	  yield "20000"	
	 }

 	background
	 {
	   exponential
	    {
	      slope "-0.005"
	    }
	   yield "80000"
	 }
      }
}
