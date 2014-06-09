#include <iostream>
#include <string>
#include <vector>

#include "root.h"
#include "utils.h"

#include "LHAPDF/LHAPDF.h"
#include "hoppet_v1.h"
#include "appl_grid/appl_grid.h"


#include "appl_grid/generic_pdf.h"
#include "MyEvent.h"
#include "MyData.h"
#include "MyFrameData.h"
#include "MyGrid.h"
#include "MyCrossSection.h"

// lhapdf routines



//extern "C" double alphasPDF_(const double *);
extern "C" void evolvepdf_(const double& , const double& , double* ); 
extern "C" double alphaspdf_(const double& Q);


double alphasPDF(const double& Q) { // is in LHAPDF.h
  //cout<<" alphas("<<Q<<")= "<<LHAPDF::alphasPDF(Q)<<endl;
  //return LHAPDF::alphasPDF(Q);
  //return alphaspdf_(Q);
}

void GetPdf(const double& x, const double& Q, double* xf) { 
  //cout<<"GetPDF x="<<x<<" Q="<<Q<<endl;
  for (int i=0; i<12; i++) xf[i]=0.;
 
  evolvepdf_( x, Q, xf); // is in LHAPDF.h
  //cout<<"testgridfromsherpa:GetPDf after evolvePDF x= "<<x<<" Q= "<<Q<<endl;
  //for (int i=0; i<=12; i++) cout<<" f["<<i<<"]="<<xf[i]<<endl;
  
/*  
  for (int i=-6; i<6; i++) xf[i]=LHAPDF::xfx(x, Q,i);
  cout<<"LHAPDF xfx()= "<<endl;
  for (int i=-6; i<6; i++)   
    cout<<" f["<<i<<"]="<<xf[i];
   cout<<endl;


   //hoppeteval_( x, Q, xf);    
 */
   return; 
 }

TH1D* divide( const TH1D* h1, const TH1D* h2 ) {

  bool debug=false;

  if ( h1==NULL || h2==NULL ) return NULL;
 
  TH1D* h = (TH1D*)h1->Clone();
  TString hname=TString("ratio")+h1->GetName()+TString("_")+h2->GetName();
  h->SetName(hname);

  if (debug) std::cout << "histograms " << h1->GetTitle() <<TString("_")<< h2->GetTitle() << std::endl;
  TString htit=TString("ratio")+h1->GetTitle()+"_"+h2->GetTitle();
  h->SetTitle(htit);

  for ( int i=1 ; i<=h1->GetNbinsX() ; i++ ) { 
    double b  = h2->GetBinContent(i);
    double be = h2->GetBinError(i);
    double t  = h1->GetBinContent(i);
    double te = h1->GetBinError(i);

    double r  = ( b!=0 ? t/b : 0 );
    //    double re = ( b!=0 ? sqrt((r+1)*r/b) : 0 );
    double re = 0;
    
    h->SetBinContent( i, r );
    h->SetBinError( i, re ) ;

    if (debug) std::cout << "\tx=" << h->GetBinCenter(i) << "\tratio=" << r << std::endl;
  } 
 double hmin = h->GetBinContent(1);
  double hmax = h->GetBinContent(1);
  
  for ( int i=2 ; i<=h->GetNbinsX() ; i++ ) { 
    double d = h->GetBinContent(i);
    if ( hmin>d ) hmin=d;
    if ( hmax<d ) hmax=d; 
  }

  if (debug) std::cout << "\tmin ratio = " << hmin << "\tmax ratio = " << hmax << std::endl;
  
  if ( h->GetMaximum()<1.01 ) h->SetMaximum(1.01);
  if ( h->GetMinimum()>0.99 ) h->SetMinimum(0.99);

  return h;
}



std::vector<std::vector<double> > MakeMyCkm() { 
  //
  // make by hand CKM matrix
  //
  cout<<" MakeMyCkm"<<endl;
  /* snippet from sherpa code
     MODEL/Main/Standard_Model.C
     default is  m_ckmorder=0
The order of expansion in the lambda parameter is defined through CKMORDER, with default ‘0’ corresponding to a unit matrix. The parameter convention for higher expansion terms reads:

    CKMORDER = 1, the CABIBBO parameter has to be set, it parametrizes lambda and has the default value ‘0.2272’.
    CKMORDER = 2, in addition the value of A has to be set, its default is ‘0.818’.
    CKMORDER = 3, the order lambda^3 expansion, ETA and RHO have to be specified. Their default values are ‘0.349’ and ‘0.227’, respectively. 

  CMatrix CKM(3);

  for (int i=0;i<3;i++) {
    for (int j=i;j<3;j++) CKM[i][j] = CKM[j][i] = Complex(0.,0.);
    CKM[i][i] = Complex(1.,0.);
  }

 double Cabibbo=0.0,A=.8,rho,eta;

  m_ckmorder     = p_dataread->GetValue<int>("CKMORDER",0);  
  if (m_ckmorder>0) {
    Cabibbo    = p_dataread->GetValue<double>("CABIBBO",0.2272);
    CKM[0][0] += sqr(Cabibbo)/2. * Complex(-1.,0.);
    CKM[1][1] += sqr(Cabibbo)/2. * Complex(-1.,0.);
    CKM[0][1] += Cabibbo * Complex( 1.,0.);
    CKM[1][0] += Cabibbo * Complex(-1.,0.);
  }
  if (m_ckmorder>1) {
    A          = p_dataread->GetValue<double>("A",0.818);
    CKM[1][2] += A*sqr(Cabibbo)  * Complex( 1.,0.);
    CKM[2][1] += A*sqr(Cabibbo)  * Complex(-1.,0.);
  }
  if (m_ckmorder>2) {
    eta        = p_dataread->GetValue<double>("ETA",0.349);
    rho        = p_dataread->GetValue<double>("RHO",0.227);
    CKM[0][2] += A*pow(Cabibbo,3) * Complex(rho,-eta);
    CKM[2][0] += A*pow(Cabibbo,3) * Complex(1.-rho,-eta);
  }
*/

  
   std::vector<std::vector<double> > m_ckm2;
   m_ckm2 = std::vector<std::vector<double> >(13, std::vector<double>(13,0));
   for (int i=0; i<13; i++){
    for (int j=0; j<13; j++){
      //cout<<" i= "<<i<<" j= "<<j<<endl;
     if (i==j) m_ckm2[i][j]=1;
     else      m_ckm2[i][j]=0;
    }
   }
  
   /* 
    std::vector<std::vector<double> > m_ckm2;
    m_ckm2 = std::vector<std::vector<double> >(13, std::vector<double>(13,0));

    m_ckm2[3][8]  =   0.049284000000000001417976847051249933 ;
    m_ckm2[8][3]  =   0.049284000000000001417976847051249933 ;
    
    m_ckm2[5][8]  =   0.950624999999999942268402719491859898 ;
    m_ckm2[8][5]  =   0.950624999999999942268402719491859898 ;
    
    m_ckm2[5][10] =   0.049284000000000001417976847051249933 ;
    m_ckm2[10][5] =   0.049284000000000001417976847051249933 ;
    
    m_ckm2[3][10] =   0.950624999999999942268402719491859898 ;
    m_ckm2[10][3] =   0.950624999999999942268402719491859898 ;

    m_ckm2[4][9] =   0.049284000000000001417976847051249933 ;
    m_ckm2[9][4] =   0.049284000000000001417976847051249933 ;
    
    m_ckm2[7][4] =   0.950624999999999942268402719491859898 ;
    m_ckm2[4][7] =   0.950624999999999942268402719491859898 ;
    
    m_ckm2[7][2] =   0.049284000000000001417976847051249933 ;
    m_ckm2[2][7] =   0.049284000000000001417976847051249933 ;
    
    m_ckm2[9][2] =   0.950624999999999942268402719491859898 ;
    m_ckm2[2][9] =   0.950624999999999942268402719491859898 ;
   */
  return m_ckm2;
}



int main(int argc, char** argv) { 

  bool debug=true;
  // use a default atlas inclusive grid
  //std::string inputname = "atlas-incljets04-eta1.root";
  //if ( argc>1 ) inputname = argv[1];
  //std::cout << "reading grid " << inputname << std::endl; 


 // now calculate all the cross sections

  //  const string _pdfname = "cteq66.LHgrid";  
  const string _pdfname = "CT10nlo_as_0112.LHgrid";  
  int Npdf = 0;
  // setup gavins code
  //LHAPDF::initPDFSet(_pdfname.c_str());
  //LHAPDF::initPDF(Npdf);
  LHAPDF::initPDFByName(_pdfname.c_str(), 0);
  LHAPDF::getDescription();

  //std::string prefixPath();
  //std::string pdfsetsPath();
  //std::string pdfsetsIndexPath();
  //
  // set-up test histogram
  //
  char name[]="atlas2012_Wjets.txt";
  MyCrossSection *mycross= new MyCrossSection(name);
 
  const int NGrids=mycross->GetNGrid();
  //if (debug) cout<<" Number of grids= "<<mycross->GetNGrid()<<endl;
  TH1D *href  [NGrids];
  TH1D *hgrid [NGrids];
  MyFrameData *myframe[NGrids];

  const int NSUB=25;
  TH1D *hallsublo;   TH1D *hallsubnlo;
  TH1D *hnlo[NSUB];  TH1D *hlo[NSUB];
// normalised test histogram
  for (int igrid=0; igrid<NGrids;igrid++){

   //string outFileName="output_"+mycross->GetGridFileName(igrid);
   //TFile* fout = new TFile(outFileName.c_str(), "recreate");
  
   double evtot=mycross->GetTotalEventNumber(igrid);
   cout<<"testgridfromsherpa evtot= "<<evtot<<endl;
   href[igrid]=mycross->GetReference(igrid);
   if (!href[igrid]) cout<<" href not found "<<endl;
   //href[igrid]->Print("all");
   else {
    href[igrid]=(TH1D*)TH1Norm(href[igrid],evtot);
    cout<<"testgridfromsherpa href after normalisation evtot= "<<evtot<<endl;
    href[igrid]->Print("all");
   }
   // put this inside MyCross
   //double xsec = test->convolute(GetPdf, alphasPDF , nLoops, mur[i], muf[i]);  
   appl::grid *mygrid=mycross->GetGrid(igrid);
   
   std::vector<std::vector<double> > ckm2=MakeMyCkm();
   mygrid->setckm(ckm2);
   cout<<" set ckm matrix done " <<endl;

   int nloops=1;
   hgrid[igrid] = mygrid->convolute(GetPdf, alphasPDF,nloops);  
   //cout<<" hgrid before normalisation "<<endl;
   //hgrid[igrid]->Print("all");

   hgrid[igrid]=(TH1D*)TH1Norm(hgrid[igrid],1.);
   cout<<" hgrid after normalisation evtot= "<<evtot<<endl;
   hgrid[igrid]->Print("all");

   TH1D* hratio=divide(hgrid[igrid],href[igrid]);
   hratio->Print("all");

   // load results per subprocess
   TFile *f1= new TFile(TString(mycross->GetGridFullFileName(igrid)));
   if (!f1) cout<<" file not found ! "<<endl;

   for ( int isub=0; isub<NSUB; isub++) {
    TString hname="htestLOsub"; hname+=isub; hname+="atlas2012_Wjets_pt1";
    hlo[isub]=(TH1D*)f1->Get(hname);
    if (!hlo[isub]) cout<<" Histogame not found: "<<hname<<endl;
    else { 
     hlo[isub]->SetLineColor(1+isub);
     hlo[isub]->SetLineStyle(1);
     if (isub==0) hallsublo=(TH1D*)hlo[isub]->Clone("hallsublo");
     else         hallsublo->Add(hlo[isub]);
     //hlo=(TH1D*)TH1Norm(hlo,evtot);
     cout<<" Print Histo: "<<hlo[isub]->GetTitle()<<endl;
     hlo[isub]->Print("all");

     TH1D * hreffromgridlo = mygrid->convolute_subproc(isub,GetPdf,alphasPDF,0);  
     hreffromgridlo=(TH1D*)TH1Norm(hreffromgridlo,1.);
     hreffromgridlo->Print("all");
     TH1D* hratio=divide(hlo[isub],hreffromgridlo);
     cout<<" Print ratio Histo: "<<hlo[isub]->GetTitle()<<" / "<<hreffromgridlo->GetTitle()<<endl;
     hratio->Print("all");
    }
   
    TString hname2="htestNLOsub"; hname2+=isub;hname2+="atlas2012_Wjets_pt1";
    hnlo[isub]=(TH1D*)f1->Get(hname2);
    if (!hnlo[isub]) cout<<" Histogam not found: "<<hname2<<endl;
    else {
     hnlo[isub]->SetLineColor(kMagenta+4-isub);
     hnlo[isub]->SetLineStyle(1);
     if (isub==0) hallsubnlo=(TH1D*)hnlo[isub]->Clone("hallsubnlo");
     else         hallsubnlo->Add(hnlo[isub]);


     cout<<" Print Histo: "<<hnlo[isub]->GetTitle()<<endl;
     //hnlo[isub]=(TH1D*)TH1Norm(hnlo[isub],evtot);
     hnlo[isub]->Print("all");

     TH1D * hreffromgridnlo = mygrid->convolute_subproc(isub,GetPdf,alphasPDF);  
     hreffromgridnlo=(TH1D*)TH1Norm(hreffromgridnlo,1.);
     hreffromgridnlo->Print("all");
     TH1D* hratio=divide(hnlo[isub],hreffromgridnlo);
     hratio->Print("all");
    }
    
   }

   //mygrid->vconvolute(GetPdf, alphasPDF, int     nloops, 
   //                                    double  rscale_factor=1,
   //                                    double  fscale_factor=1,
   //                                    double Escale=1 );

   // perform the convolution to the max number of loops in grid
   //std::vector<double> vconvolute_subproc(int subproc, 
   //     void   (*pdf)(const double& , const double&, double* ), 
   //     double (*alphas)(const double& ), 
   //     int     nloops, 
   //     double  rscale_factor=1, double Escale=1 ); 

/*
   cout<<" cross section is= "<<endl;
   std::vector<double>  vxsec   = mygrid->vconvolute(GetPdf, alphasPDF); 
   for (int i=0; i<vxsec.size(); i++) cout<<i<<" xsec= "<<vxsec[i]<<endl;

   cout<<"testgridfromsherpa LO only; "<<endl;
   std::vector<double>  vxseclo = mygrid->vconvolute(GetPdf, alphasPDF,0); 
   for (int i=0; i<vxseclo.size(); i++) cout<<i<<" xseclo= "<<vxseclo[i]<<endl;
*/
  }
 
  TApplication *theApp = new TApplication("My ROOT Application",0,0);
  theApp->SetReturnFromRun(true);



  for (int igrid=0; igrid<mycross->GetNGrid();igrid++){

    MyData *mydata=mycross->GetMyData(igrid);
    if (!mydata) cout<<" makegridfromsherpa mydata["<<igrid<<"] not found "<<endl;  
    //else cout<<" makegridfromsherpa mydata["<<igrid<<"] read "<<endl;

    MyFrameData *myframe= new MyFrameData(600,600,mydata);
    if (!myframe) cout<<" makegridfromsherpa mframe not found "<<endl;
    else cout<<" frame created "<<endl;
  //
    mydata->DrawData();
    if (!href[igrid])  cout<<" reference not found ! "<<endl;
    else href[igrid]->Draw("same");
    href  [igrid]->Print("all");

    for ( int isub=0; isub<NSUB; isub++) {
     hnlo[isub]->Draw("same");
     //hlo ->Draw("same");
    }

    //hallsublo ->SetLineStyle(3); 
    //hallsublo ->SetLineColor(3); 
    //hallsubnlo->SetLineStyle(1); 
    //hallsubnlo->SetLineColor(4); 
    //hallsublo ->Draw("same"); 
    //hallsubnlo->Draw("same"); 
    //hallsubnlo->Print("all"); 

    if (!hgrid[igrid])  cout<<" histo from grid not found ! "<<endl;
    hgrid[igrid]->SetLineColor(1);
    hgrid[igrid]->SetLineStyle(3);
    //hgrid[igrid]->Draw("same");
    //hgrid  [igrid]->Print("all");

    //mycross->DrawReference(igrid);

    TGraphAsymmErrors *gdata=mydata->GetTGraph();
    TGraphAsymmErrors *gref=TH1TOTGraphAsymmErrors(href[igrid]);

    TGraphAsymmErrors *gratio=myTGraphErrorsDivide(gref,gdata);

    //myframe->GetMyFrame()->SetYmin2();
    myframe->GetMyFrame()->GetSubPad2()->cd();
    gratio->Draw("e");
  }

  gPad->Update();
  theApp->Run(kTRUE);

  //get name of grid from user and create from grid file
  //appl::grid g(gridname);
  //g.trim(); // trim away uneeded memory
  //std::cout << "setting up lhapdf" << std::endl; 

  // initialise lhapdf

  //const string _pdfname = "cteq6mE.LHgrid";  
  //const string _pdfname = "CT10.LHgrid";  
  //int iset = 0;

  //LHAPDF::initPDFSet( _pdfname, iset );
  // initpdfset_(_pdfname.c_str());
  // initpdf_( iset );

  //TH1D *href= g.getReference();
  //double ynorm= g.run();
  //href->Scale(1./ynorm);


  //href->Print("all");

  //TString fname=inputname+"-stand";
  //TFile f(fname, "recreate");

  //  or get into a histogram
  //TH1D* hxsec = g.convolute( evolvepdf_, alphaspdf_ ); 
  //hxsec->SetName("xsec");
  //hxsec->Print("all");

  //f.Write();
  //f.Close();

  return 0;

}

