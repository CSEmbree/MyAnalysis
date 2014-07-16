
/*
 * Title:    MyCrossSection
 * Author:   T. Carli, C. Embree
 * Contact:  tancredi.carli@cern.ch
 * Created:  ##-##-##
 * Edited:   01-Jun-2014
 * Notes:    
 */

#ifndef __MY_CROSS_SECTION_H
#define __MY_CROSS_SECTION_H

using std::string;
#include <sys/time.h> 
#include "root.h"
//
#include "MyData.h"
#include "MyFrame.h"
#include "MyPDF.h"
#include "MyBand.h"
#include "MyGrid.h"
#include "appl_grid/appl_grid.h"


class MyCrossSection {
 private:

  bool debug; 

  string crosssectionname;
  string subprocesssteername;

  string ratiotitlelabel;
  string glabel;

  bool plotchi2;
  bool plotmarker;          // flag for plotting marker points on PDF data
  bool plotband;            // flag for plotting error bars on PDF data
  bool ploterrorticks;      // small perpendicular lines at end of error bars
  bool staggerpdfpoints;    // shift pdf points horizontally for readability
  bool ratioTheoryOverData; // ratio plotting: true-(theory/data), false-(data/theory), (default=true)

  //string ntupdirinput;
  //string ntupdiroutput;

  string ntupname;
  string steername;

  string datanamedir;
  //string gridnamedir;

  string gridnamebasedir;
  string gridnamesystbasedir;
  string gridnamedefaultdir;
  string gridnamemassdowndir;
  string gridnamemassupdir;
  string gridnamebeamdowndir;
  string gridnamebeamupdir;

  double xlegend;
  double ylegend;

  string xunits;
  string yunits;

  double ymaxoverlay;
  double yminoverlay;
  double ymaxratio;
  double yminratio;

   
  
  typedef std::vector<int> vframe; 
  typedef std::vector<MyData*> vframedata; 

  //std::vector<string> framename;
  std::vector<vframe> gridinframe; // igrid per frame
  //std::vector<vframedata> gridinframedata; // data per frame

  // boundery per frame
  std::vector<double> xminframe;
  std::vector<double> xmaxframe;
  std::vector<double> yminframe;
  std::vector<double> ymaxframe;

  // per grid
  //std::vector<string> gridname;
  std::vector<string> gridname;
  std::vector<string> dataname;
  //std::vector<string> corrname;
  std::vector<string> vardesc;
  std::vector<int> markerstyle;
  std::vector<int> markercolor;

  std::vector<string> leglabel; // Label for legend

  std::vector<int> frameid;   // grids/data with same frameid will be overlayed
  std::vector<int> divideid;  // grids/data with same divideid will be divided
  std::vector<int> refhistlinestyle; // line style of reference histogram
  std::vector<int> refhistlinecolor; // line color of reference histogram
  std::vector<MyFrame*> framepointer; // pointer to MyFrame per frameid

  std::vector<string> pdfdata; //names of each pdf to be used
  std::vector<string> overlaynames; //names of each type to overlay (should only be "data","theory","reference")

  //std::vector<std::vector<string> > pdfdata;
  //std::vector<std::vector<MyPDF*> > t_mypdf; //first part of vec is grid id, second is pdf id

  std::vector<MyBand*> myband; 

  std::vector<double> renscale; // values of renormalisation and factorisation scales
  std::vector<double> facscale; //

  //std::vector<TGraphAsymmErrors*> gtotband; //
  //std::vector<TGraphAsymmErrors*> gscaleband; //
  //std::vector<TGraphAsymmErrors*> galphasband; //

  // per grid
  //std::vector<TGraphAsymmErrors*> gpdfband;    // per ipdf
  //std::vector<TGraphAsymmErrors*> gpdfdefault; // per ipdf
  //std::vector<MyBand*>  gpdfband;    // per igrid 
  //std::vector<MyBand*>  gpdfdefault; // per igrid 

  std::vector<std::vector<double>> vpdfchi2;   // vector of chi2 for ipdf and igrid

  // should integrate the below in MyBand
  std::vector<TGraphAsymmErrors*> ratiotot;
  std::vector<TGraphAsymmErrors*> ratiostat;

  std::vector<std::vector<TGraphAsymmErrors*>> gpdfbandratiotot;
  std::vector<std::vector<TGraphAsymmErrors*>> gpdfdefaultratiotot;

  string pdf_function;
 
  std::vector<MyData*> mydata;       // information about data from steering file
  std::vector<MyGrid*> mygrid;       // information about data from steering file
  generic_pdf *mypdf;
  
  std::vector<string>* ParseString(std::string rawData, char delimeter); //For parsing steering input
  bool validateOverlayStyle(std::vector<std::string > names); //For validating overlay names


 public:

  bool do_PDFBand;
  bool do_AlphaS;
  bool do_RenormalizationScale;
  bool do_FactorizationScale;
  bool do_TotError;

  bool overlayData;
  bool overlayTheory;
  bool overlayReference;

  MyCrossSection(char name[100]);

  generic_pdf * GetSubProcess() { return mypdf;};

  double GetUnitScaleX(std::string fromUnits, std::string toUnits);
  double GetUnitScaleY(std::string fromUnits, std::string toUnits);

  void SetSubProcess(generic_pdf *subpro) { mypdf=subpro; return;};
  void split_string(std::string str, std::vector<std::string>& split_results, std::string delimiters);

  void Initialize();
  void ReadSteering(char fname[100]);

  bool file_exists(const string& s);
  
  //return number pdf pdfs for a particular grid
  int GetNPDF(int igrid) {
   //return t_mypdf.at(igrid).size();
   return myband.at(igrid)->GetNPDF();
  };
  
/*
  //return the total number of pdfs (can have multiple per grid name)
  int GetNPDF() {
    int pdfCount=0;
    for(int x=0; x<t_mypdf.size(); x++) {
        for(int y=0; y<t_mypdf.at(x).size(); y++) {
            pdfCount++;
        }
    }
    
    return pdfCount;
  };
*/  
  //int reflinestyle;
  void SetLineColor(int igrid, int rl) {refhistlinecolor[igrid]=rl; return;};
  void SetLineStyle(int igrid, int rl) {refhistlinestyle[igrid]=rl; return;};

  void Normalise(TH1D* h, double yscale, double xscale, bool normtot, bool divwbinwidt);
  void Normalise(TGraphAsymmErrors* g1, double yscale, double xscale, bool normtot, bool divwbinwidt);

  int GetNGrid(){return gridname.size();};

  string GetXUnits(){ return xunits ;};
  string GetYUnits(){ return yunits ;};
  
  //double CalcChi2(TGraphAsymmErrors *g_theory, TGraphAsymmErrors *g_data, TMatrixT<double> data_cov_matrix);
  double CalcChi2(TGraphAsymmErrors *g_theory, TGraphAsymmErrors *g_data, TMatrixT<double> *data_cov_matrix);
        
  //string GetNtupDirInput(){ return ntupdirinput;};
  //string GetNtupDirOutput(){ return ntupdiroutput;};
  string GetNtupName(){ return ntupname;};

  //TODO - Is this function needed if the path is now gotten with "GetGridSteeringPath"
  // Left for legacy code for now
  string GetGridName(int igrid){ 
    
    if(gridnamebasedir.compare("")==0)
        return gridname[igrid];
    else
        return gridnamebasedir+"/"+gridname[igrid];    
  };

  string GetGridSteeringPath(int igrid) {
    if(gridnamebasedir.compare("")==0) return gridname[igrid];
    else                               return gridnamebasedir+"/"+gridname[igrid];
  }
  
//  std::vector<string> *GetPDFData(int igrid) {
//    if(igrid>pdfdata.size()) {
//        std::cout<<" MyCrossSection:: GetPDFData: ERROR: pdfdata not found for igrid: "<<igrid<<std::endl;
//        return NULL;
//    }
//    else
//        return &(pdfdata.at(igrid));
//  };

  //std::vector<string> *GetPDFData(int ipdf) {
   string *GetPDFData(int ipdf) {
   if(ipdf>pdfdata.size()) {
    cout<<" MyCrossSection:: GetPDFData: ERROR: pdfdata not found for ipdf: "<<ipdf<<endl;
    return NULL;
   } else
     return &(pdfdata.at(ipdf));
  };

   TString GetPdfName(int ipdf) {
   if(ipdf>pdfdata.size()) {
    cout<<" MyCrossSection::GetPDFData: ERROR: pdfdata not found for ipdf: "<<ipdf<<endl;
    return TString("");
   } else {
     TString pdfname=TString(pdfdata.at(ipdf));
     pdfname.ReplaceAll(TString(".txt"),TString("")); 
     pdfname.ReplaceAll(TString("MyPDF/"),TString("")); 
     //pdfname.ReplaceAll(gridnamedir,TString(""));  
     return pdfname;
   }
  };

/*
 std::vector<MyPDF*> *GetMyPDF(int igrid) {
  if (igrid>t_mypdf.size()) {
   cout<<" MyCrossSection:: GetMyPDF(int): ERROR: mypdf not found for igrid: "<<igrid<<endl;
   return NULL;
  } else
   return &(t_mypdf.at(igrid));
  };
  
  MyPDF *GetMyPDF(int igrid, int ipdf) {
    if(igrid>t_mypdf.size() || ipdf>t_mypdf.at(igrid).size()) {
        std::cout<<" MyCrossSection:: GetMyPDF(int, int): ERROR: mypdf not found for igrid: "<<igrid<<" and ipdf: "<<ipdf<<std::endl;
        return NULL;
    }
    else
        return t_mypdf.at(igrid).at(ipdf);
  };
  
  std::vector< std::vector<MyPDF*> > *GetMyPDF() {
    if(t_mypdf.size()==0) {
        std::cout<<" MyCrossSection:: GetMyPDF: ERROR: mypdf not found for igrid: "<<std::endl;
        return NULL;
    }
    else
        return &t_mypdf;
  };
*/  
  
  //TString GetTStringGridName(int igrid){ return ((TString) (gridnamebasedir+"/"+gridname[igrid]));  };
  TString GetVarDesc(int igrid){ return ((TString) (vardesc.at(igrid))); };
 /*
  string GetGridName(int i){
   TString name=TString(gridname[i]); 
   name.ReplaceAll(".txt","");
   return string(name.Data());
  };

  string GetGridFileName(int i){
    //return string(this->GetGridName(i).Data())+".root";
    return this->GetGridName(i)+".root";
  }

  string GetGridFullFileName(int i){
    //return ntupdirinput+"/"+this->GetGridFileName(i);
    return this->GetGridDir()+"/"+this->GetGridFileName(i);
  }
 */

  MyData *GetMyData(int igrid){ 
   if (igrid>mydata.size()){
    cout<<" MyCrossSection::GetMyData something is wrong mydata too short igrid= "<<igrid<<endl;
    return 0;
   }
   return mydata[igrid];
  };

  string GetGridDir(){ return gridnamebasedir;};

  bool GetDataOk(int igrid){
   bool flag=true;
   //if (debug) cout<<" MyCrossSection::GetDataOk igrid= "<<igrid
   //               <<" dataname.size()= "<<dataname.size()<<endl;
   if (dataname.size()<=igrid) flag=false;
   //if (debug) cout<<" MyCrossSection::GetDataOk flag= "<<flag<<endl;
   return flag;
  };

  bool GetGridOk(int igrid){
   bool flag=true;
   //if (debug) cout<<" MyCrossSection::GetDataOk igrid= "<<igrid
   //               <<" dataname.size()= "<<dataname.size()<<endl;
   if (gridname.size()<=igrid) flag=false;
   //if (debug) cout<<" MyCrossSection::GetDataOk flag= "<<flag<<endl;
   return flag;
  };
  
  string GetDataName(int igrid){
   string bad="";
   if (!this->GetDataOk(igrid)) return bad;
   else return dataname[igrid]; //datanamedir+"/"+dataname[igrid];
  };

  string GetDataNameDir(int igrid){
   string bad="";
   if (!this->GetDataOk(igrid)) return bad;
   else return datanamedir; //datanamedir+"/"+dataname[igrid];
  };
  
  int GetMarkerStyle(int igrid) {
   if (markerstyle.size()<=igrid) {
    cout<<" MyCrossSection::GetMarkerStyle something is wrong markserstyle too short igrid= "<<igrid<<endl;
    return 0;
   }
   return markerstyle[igrid];
  };
  
  int GetMarkerColor(int igrid) {
   if (markercolor.size()<=igrid) {
    cout<<" MyCrossSection::GetMarkerColor something is wrong markercolor too short igrid= "<<igrid<<endl;
    return 0;
   }
   return markercolor[igrid];
  };
  
  void SetStyleMarker (int igrid, int ms) {
   if (igrid>markerstyle.size()){
     cout<<" MyCrossSection::SetStyleMarker something is wrong markerstyle too short igrid= "<<igrid<<endl;
   } else markerstyle[igrid]=ms; 
   return;
  };

  void SetColorMarker (int igrid, int mc) {
   if (igrid>markerstyle.size()){
     cout<<" MyCrossSection::SetColorMarker something is wrong markercolor too short igrid= "<<igrid<<endl;
   } else markercolor[igrid]=mc; 
   return;
  };

  void SetLegendLabel (int igrid, string mylabel) {
   if (igrid>leglabel.size()){
     cout<<" MyCrossSection::SetLegendLabel something is wrong leglabel too short igrid= "<<igrid<<endl;
   } else leglabel[igrid]=mylabel; 
   return;
  };

  string GetLegendLabel(int igrid) {
   if (leglabel.size()<=igrid) {
    cout<<" MyCrossSection::GetLegendLabel something is wrong leglabel too short igrid= "<<igrid<<endl;
    return 0;
   }
   return leglabel[igrid];
  };

  int GetFrameID(int igrid){ 
   if (frameid.size()<=igrid) {
     cout<<" MyCrossSection::GetFrameID something is wrong frameid too short frameid.size()= "
         << frameid.size()<<" igrid= "<<igrid<<endl;
     return -1;
   }
   return frameid[igrid];
  };
  
  MyFrame *GetMyFrame(int igrid ) {
   if (framepointer.size()<=igrid) {
     cout<<" MyCrossSection::GetMyFrame something is wrong divideid too short framepointer= "
         <<framepointer.size()<<" igrid= "<<igrid<<endl;
     return 0;
   }
   return framepointer[igrid];
  };
  
  int GetDivideID(int igrid){ 
   if (divideid.size()<=igrid) {
     cout<<" MyCrossSection::GetDivideID something is wrong divideid too short igrid= "<<igrid<<endl;
     return 1.;
   }
   return divideid[igrid];
  };

  int GetFrameNumber();

  TH1D *GetReference(int igrid);
  void Draw(int igrid);
  TGraphAsymmErrors * GetPDFTypeRatio(int iset1, int iset2=0, int igrid=0);
  TGraphAsymmErrors * GetPDFRatio(int iset1, int iset2=0, int itype=0, int igrid=0);

  
  void DrawData(int igrid){
   mydata[igrid]->DrawData();
   return;
  };

  bool DoubleDataSetName(int idata);

  void SetRatioTitle(string mytitle) {ratiotitlelabel=mytitle; return;};
  string GetRatioTitle() { return ratiotitlelabel;};

  void SetLegendX(double x) {xlegend=x; return;};
  void SetLegendY(double y) {ylegend=y; return;};


  string stringToUpper(std::string s);
  
  void DrawReference(int igrid);
  TGraphAsymmErrors* GetReferenceRatio(int igrid);
  TH1D* GetNormalisedReference(int igrid);

  void DrawinFrame(int iframe);

  double GetObsMax(int igrid){
   if (!mydata[igrid])  cout<<" GetObsMax grid not found= mydata["<<igrid<<"]"<<endl;
   double obsmax=mydata[igrid]->GetMaxX(); 
   //if (debug) cout<<" obsmax= "<<obsmax<<endl;
   return obsmax;
  }

  TH1D *GetHisto(int igrid,string name="htest"){
   if (!mydata[igrid]) cout<<" MyCrossSection::GetHisto mydata not filled ! "<<endl;
   int nobs=mydata[igrid]->GetNBins(); 
   if (debug) 
    cout<<" MyCrossSection::GetHisto nobs= "<<nobs<<endl;
   double *xbins=mydata[igrid]->GetBins();

   string hname=name+this->GetGridName(igrid);
   //cout<<" book histogram hname= "<<hname<<endl;
   TH1D *htest = new TH1D(hname.c_str(),hname.c_str(),nobs,xbins);
   if (!htest) cout<<" htest["<<igrid<<"] not found "<<endl;
   //htest->Print("all");
   return htest;
  }; 

  TGraphAsymmErrors* myTGraphErrorsDivide(TGraphAsymmErrors* g1,TGraphAsymmErrors* g2, Int_t noerr=1);
  TGraphAsymmErrors* TH1TOTGraphAsymm(TH1 *h1);

  void Print();

  void ScaleGraph(TGraphAsymmErrors *g, double scalex, double scaley); //TODO - remove this?
};


#endif
