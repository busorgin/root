//////////////////////////////////////////////////////////////////////////
//
// 'MULTIDIMENSIONAL MODELS' RooFit tutorial macro #314
//
// Working with parameterized ranges in a fit. This an example of a
// fit with an acceptance that changes per-event
//
//  pdf = exp(-t/tau) with t[tmin,5]
//
//  where t and tmin are both observables in the dataset
//
// 07/2008 - Wouter Verkerke
//
/////////////////////////////////////////////////////////////////////////

#ifndef __CINT__
#include "RooGlobalFunc.h"
#endif
#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooGaussian.h"
#include "RooExponential.h"
#include "TCanvas.h"
#include "RooPlot.h"
#include "RooFitResult.h"

using namespace RooFit ;


class TestBasic314 : public RooFitTestUnit
{
public:
  TestBasic314(TFile* refFile, bool writeRef, Int_t verbose) : RooFitTestUnit("Fit with non-rectangular observable boundaries",refFile,writeRef,verbose) {} ;
  bool testCode() {

  // D e f i n e   o b s e r v a b l e s   a n d   d e c a y   p d f
  // ---------------------------------------------------------------

  // Declare observables
  RooRealVar t("t","t",0,5) ;
  RooRealVar tmin("tmin","tmin",0,0,5) ;

  // Make parameterized range in t : [tmin,5]
  t.setRange(tmin,RooConst(t.getMax())) ;

  // Make pdf
  RooRealVar tau("tau","tau",-1.54,-10,-0.1) ;
  RooExponential model("model","model",t,tau) ;



  // C r e a t e   i n p u t   d a t a
  // ------------------------------------

  // Generate complete dataset without acceptance cuts (for reference)
  RooDataSet* dall = model.generate(t,10000) ;

  // Generate a (fake) prototype dataset for acceptance limit values
  RooDataSet* tmp = RooGaussian("gmin","gmin",tmin,RooConst(0),RooConst(0.5)).generate(tmin,5000) ;

  // Generate dataset with t values that observe (t>tmin)
  RooDataSet* dacc = model.generate(t,ProtoData(*tmp)) ;



  // F i t   p d f   t o   d a t a   i n   a c c e p t a n c e   r e g i o n
  // -----------------------------------------------------------------------

  RooFitResult* r = model.fitTo(*dacc,Save()) ;



  // P l o t   f i t t e d   p d f   o n   f u l l   a n d   a c c e p t e d   d a t a
  // ---------------------------------------------------------------------------------

  // Make plot frame, add datasets and overlay model
  RooPlot* frame = t.frame(Title("Fit to data with per-event acceptance")) ;
  dall->plotOn(frame,MarkerColor(kRed),LineColor(kRed)) ;
  model.plotOn(frame) ;
  dacc->plotOn(frame,Name("dacc")) ;

  // Print fit results to demonstrate absence of bias
  regResult(r,"rf314_fit") ;
  regPlot(frame,"rf314_plot1") ;

  delete tmp ;
  delete dacc ;
  delete dall ;

  return true;
  }
} ;
