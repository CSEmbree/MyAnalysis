/*
 * Title:    MyPDF
 * Author:   T. Carli, C. Embree
 * Contact:  tancredi.carli@cern.ch
 * Created:  01-Jun-2013
 * Edited:   01-Jun-2014
 * Notes:    
 */

#ifndef __MY_BAND_H
#define __MY_BAND_H


#include <stdlib.h> // exit()
#include <sstream>  // needed for internal io
#include <iomanip>
//#include <fstream> 
//#include <vector>
//#include <string>
//root
#include "root.h"
using namespace std;

class MyBand {

 public:

  MyBand();

  void SetPlotBand()         {plotband=true;};
  void SetPlotMarker()       {plotmarker=true;};
  void SetErrorTicks()       {ploterrorticks=true;};
  void SetStaggerPDFPoints() {staggerpdfpoints=true;};

  void SetPdfBand       (TGraphAsymmErrors *mypdfband){
   TGraphAsymmErrors* gtmp=
     (TGraphAsymmErrors*) mypdfband->Clone((TString) (mypdfband->GetName()+TString("_pdfband")));
   gpdfband.push_back(gtmp);

   if (debug) cout <<" MyBand::SetPdfBand: gpdfband.size()= " << gpdfband.size()<<endl;
   return;
  };  

  void SetPdfDefault    (TGraphAsymmErrors *mypdfband){

   if (!mypdfband) {
    cout <<" MyBand::SetPdfDefault: graph not found ! " <<endl;
    return;
   }

   if (debug) cout <<" MyBand::SetPdfDefault:  name= " << mypdfband->GetName()<<endl;
   if (debug) mypdfband->Print();

   gpdfdefault.push_back(mypdfband);

   return;
  };  
  //void SetRenScaleBand  (TGraphAsymmErrors *myrenscaleband) {grenscaleband=myrenscaleband;};  
  //void SetFacScaleBand  (TGraphAsymmErrors *myfacscaleband) {gfactscaleband=myfacscaleband;};  
  void SetScaleBand       (TGraphAsymmErrors *myscaleband){
   gscaleband.push_back(myscaleband);
   if (debug) cout <<" MyBand::SetPdfBand: gscaleband.size()= " << gscaleband.size()<<endl;
   return;
  };  

  //void SetScaleBand   (TGraphAsymmErrors *myscaleband)    {gscaleband=myscaleband;};  
  void SetAlphasBand    (TGraphAsymmErrors *myalphasband)   {galphasband=myalphasband;};  

  int GetNPDF()  {return gpdfband.size();};
  int GetNScale(){return gscaleband.size();};
 
  TGraphAsymmErrors *GetPdfBand(int ipdf)    {
   if (ipdf>gpdfband.size()) {
    cout<<" MyBand::GetPdfBand: something is wrong "<<endl;
    return 0;
   }
   return gpdfband[ipdf];
  };
  /*
  TGraphAsymmErrors *GetPdfBandRatio(int ipdf, MyData *mydata)    {
   if (ipdf>gpdfbandratio.size()) {
    cout<<" MyBand::GetPdfBand: something is wrong "<<endl;
    return 0;
   }

   return gpdfbandratio[ipdf];
  };
  */
  TGraphAsymmErrors *GetPdfDefault(int ipdf) {
   if (ipdf>gpdfband.size()) {
    cout<<" MyBand::GetPdfBand: something is wrong "<<endl;
    return 0;
   }
   return gpdfdefault[ipdf];
  };

  TGraphAsymmErrors *GetScaleBand(int iscale)    {
   if (iscale>gscaleband.size()) {
    cout<<" MyBand::GetScaleBand: something is wrong "<<endl;
    return 0;
   }
   return gscaleband[iscale];
  };  

  //TGraphAsymmErrors *GetScaleBand()    {return gscaleband;};  
  //TGraphAsymmErrors *GetRenScaleBand() {return grenscaleband;};  
  //TGraphAsymmErrors *GetFacScaleBand() {return gfactscaleband;};  
  TGraphAsymmErrors *GetAlphasBand()     {return galphasband;};  
  TGraphAsymmErrors *GetTotBand()     {return gtotband;};  
 

  void DrawPDFBand();  
  void ComputePDFBandRatio(TGraphAsymmErrors* gref);  
  void DrawPDFBandRatio();  
  void ComputePDFBandRatioRange();  
  void MovePDFPoints();
  TString GetBandStyle();
  TString GetBandRatioStyle();

  TGraphAsymmErrors* myTGraphErrorsDivide(TGraphAsymmErrors* g1,TGraphAsymmErrors* g2, Int_t noerr);

  double GetYmin() { return yminratio;};
  double GetYmax() { return ymaxratio;};

  void SetScalex( double scale ) { 
    cerr<<" MyBand::SetScalex: WARN: Depricated scaling 'scalex="<<scale<<"'"<<endl;
    scalex = scale;
  };
  
  void SetScaley( double scale ) { 
    cerr<<" MyBand::SetScalex: WARN: Depricated scaling 'scaley="<<scale<<"'"<<endl;
    scaley = scale;
  };

  

  private:
  //VARIABLES

  bool debug;
  bool plotband;
  bool plotmarker;
  bool ploterrorticks;
  bool staggerpdfpoints;

  std::vector<TGraphAsymmErrors *> gpdfband ; // per pdftype
  std::vector<TGraphAsymmErrors *> gpdfbandratio; // per pdftype
  std::vector<TGraphAsymmErrors *> gpdfdefault; // per pdftype
  std::vector<TGraphAsymmErrors *> gpdfdefaultratio; // per pdftype
  //TGraphAsymmErrors *grenscaleband;
  //TGraphAsymmErrors *gfactscaleband;
  std::vector<TGraphAsymmErrors *> gscaleband; // per scale
  TGraphAsymmErrors *galphasband;
  TGraphAsymmErrors *gtotband;

  double yminratio;
  double ymaxratio;

  double scalex; //for forcefully scaling theory to data, if data is being scaled
  double scaley;

  void Scale(TGraphAsymmErrors *g1, double scalex, double scaley);
  void ScaleRatioGraphs(double scalex, double scaley);
  void ScaleOverlayGraphs(double scalex, double scaley);
};

#endif
