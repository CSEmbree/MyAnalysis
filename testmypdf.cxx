
#include <iostream>
#include <iomanip>
#include <assert.h>
#include <string>
#include "AtlasStyle.h"

#include "MyPDF.h"
int main(int argc, char** argv) 
//main()
{

//string steeringFileName = "steering/steering_mypdf.txt";
 string steeringDir = "steering/";
 string steeringFileName = "mypdf_CT10.txt";
 string steeringfilename =  steeringDir+steeringFileName;

 int myset=-1;
 if ( argc>1 ) myset = atoi(argv[1]);
 cout<<" testpdf myset= "<<myset<<endl;

 if ( argc>2 ) steeringfilename=string(argv[2]);

 SetAtlasStyle();

 TApplication *theApp = new TApplication("My ROOT Application",0,0);
 theApp->SetReturnFromRun(true);

 TString psfile="pdfratio";
 psfile+=myset;
 psfile+=".eps";
 TString psfile2;

 double debug=true;

 
 TString sname=TString(steeringfilename).ReplaceAll(".txt","");
 sname=TString(sname).ReplaceAll("mypdf_","");
 sname=TString(sname).ReplaceAll("steering/","");

 cout<<" sname= "<<sname<<endl;

 //const int NQ2=4;
 //double Q2value[NQ2]={172.5*172.5,100.,10.,1.};
 const int NQ2=1;
 double Q2value[NQ2]={10000.};

 int nbin=1000; 

 MyPDF *mypdf1 = 0;
 MyPDF *mypdf2 = 0;

 // double xmin=1.0e-3, xmax=0.6;
 double xmin=0.001, xmax=0.3;
 double ymin=1.e-3, ymax=500.;
 ymin=0.1; ymax=50.5;

 double x1=0.2, x2=0.6;
 double y1=0.6, y2=0.95;
 TLegend *leg = new TLegend(x1,y1,x2,y2);
 leg->SetBorderSize(0);
 leg->SetFillColor(0);



  TString cname="c"+sname;
  TCanvas *c2= new TCanvas(cname);

  double Q2=10000.;
  int ifl=0;
  TH1D *h1=new TH1D(sname.Data(),sname.Data(),nbin,xmin,xmax);
  TH1D *h2=new TH1D(sname.Data(),sname.Data(),nbin,xmin,xmax);
  
  mypdf1 = new MyPDF(steeringfilename, ifl, Q2, h1);
  mypdf2 = new MyPDF("MyPDF/NNPDF30nlo.txt", ifl, Q2, h2);
 
  mypdf1->DrawPDFBand();
  mypdf2->DrawPDFBand();

  theApp->Run(kTRUE);
  return 0;  


  /*
 //for (int iq=0; iq<NQ2; iq++) {
 for (int iq=0; iq<1; iq++) {
  double Q2=pow(Q2value[iq],2);
  //double Q2=Q2value[iq];
  //for (int ifl=-5; ifl<6; ifl++) {
  for (int ifl=0; ifl<1; ifl++) {
   TString hname=sname+TString(" Q^{2}= ");
   hname+=int(Q2);
   hname+=TString(" GeV^{2} ifl= ");
   hname+=ifl;
   cout<<" testmypdf hname= "<<hname.Data()<<endl;
   TH1D *h1=new TH1D(hname.Data(),hname.Data(),nbin,xmin,xmax);

   mypdf2 = new MyPDF(steeringfilename, ifl, Q2,h1);

   double w=mypdf2->GetPDFWeight(myset, 0.630916,0.734315);

   TString cname=TString(mypdf2->GetPDFName());
   cname+=TString("ifl=");
   cname+=ifl;
   cname+=TString("Q2=");
   cname+=Q2;

   TCanvas *c2= new TCanvas(cname);
   //ymax=mypdf2->GetMaximum(myset);
   //cout<<" set= "<<myset<<" ymax= "<<ymax<<endl;

   //double ymax2=mypdf2->GetMaximum(0);
   //cout<<" set= 0"<<" ymax= "<<ymax2<<endl;

   TH1F *hframe2 = c2->DrawFrame(xmin,ymin,xmax,ymax);
       hframe2->SetXTitle("x");
       hframe2->SetYTitle("Ratio ");

   TAxis *Xaxis = hframe2->GetXaxis();
   Xaxis->SetMoreLogLabels();

   TAxis *Yaxis = hframe2->GetYaxis();
   //Yaxis->SetMoreLogLabels();
 
 //gPad->SetLogx();
   gPad->SetLogy();

   int n=mypdf2->GetNumPDFMembers();
   int iset2=0;
   //for (int iset=23; iset<25; iset++){
   //for (int iset=myset; iset<myset+2; iset++){
   //for (int iset=myset; iset<myset+1; iset++){
   for (int iset=1; iset<52; iset++){
     //cout<<" iset2= "<<iset2<<endl;
    cout<<" ifl= "<<ifl<<" iset= "<<iset<<" Q2= "<<Q2<<endl;
    TH1D* hpdf=mypdf2->GetPDFRatio(iset,iset2);

    hpdf->SetLineColor(1+iset);
 
 //   double hmax=hpdf->GetMaximum();
 //   double xhmax=hpdf->GetBinCenter(hpdf->FindBin(hmax));
 //   if (ymax<hmax) {
 //    double hmin=Yaxis->GetXmin();
 //    int nbins=Yaxis->GetNbins();
 //    Yaxis->Set(nbins,hmin,hmax*1.1);
 //    ymax=hmax;
 //    cout<<xhmax<<" set new maximum hmax= "<<hmax<<endl;
 //   }
 //
    hpdf->Draw("hist,same");

    //mypdf2->DrawPDFRatio(iset,iset2);

    if (myset!=-1) {
     //if (iset%2==0)
      leg->AddEntry(hpdf,hpdf->GetName(),"l");
    }
   }
   //leg->Draw();
   //gPad->RedrawAxis();
   c2->Print(psfile.Data());
  }
 }
*/


/*
 ymin=1.e-3; ymax=500.;

 TCanvas *c1= new TCanvas();
 TH1F *hframe1 = c1->DrawFrame(xmin,ymin,xmax,ymax);
       hframe1->SetXTitle("x");
       hframe1->SetYTitle("f(x)");
       gPad->SetLogy();
       gPad->SetLogx();

 MyPDF *mypdf1 = 0;
 for (int ifl=0; ifl<3; ifl++) {
   //for (int iq=0; iq<NQ2; iq++) {
  for (int iq=0; iq<1; iq++) {

   double Q2=Q2value[iq];
   TString hname=sname+TString("_Q2_");
   hname+=int(Q2);
   hname+=TString("_ifl_");
   hname+=ifl;
   cout<<" testmypdf hname= "<<hname.Data()<<endl;
   TH1D *h1=new TH1D(hname.Data(),hname.Data(),nbin,xmin,xmax);

   mypdf1 = new MyPDF(steeringfilename, ifl, Q2,h1);
   //hpdf->Clone(hname);
   //cout<<" testmypdf print "<<mypdf1->h_PDFBand_results->GetName()<<endl;
   //mypdf1->h_PDFBand_results->Print("all");

   TH1D* hpdf=mypdf1->GetPdfdefault();
   if (!hpdf) cout<<" testmypdf histo not found "<<endl;
   hpdf->Draw("same");

   //if (debug) {
   // cout<<" testmypdf histo found "<<endl;
   // hpdf->Print("all");
   //}

   mypdf1->h_PDFBand_results->Draw("E2");

  }
 }
 //mypdf1->Print();
*/

 theApp->Run(kTRUE);

 return 0;

/*
    string gridName="grid--TTbar_yttatlas.root";
    string steeringFileName = "steering/steering_mypdf.txt";
  	
    TGraphAsymmErrors *h_PDFBand_results;
    TGraphAsymmErrors *h_AlphaS_results;
*/    
    /////////////////////////////////////////////    
/*
    std::cout<<"****TEST 1 START"<<std::endl;

    MyPDF *mypdf1 = new MyPDF(false);

    mypdf1->Print();
    mypdf1->SetGridName(gridName);
    mypdf1->SetSteeringFilePath(steeringFileName);
    mypdf1->ReadSteering(steeringFileName);
    mypdf1->Initialize();
    
    mypdf1->Print();
    
    mypdf1->InitializeErrorGraphs();
    mypdf1->CalcSystErrors();
        
    h_PDFBand_results=mypdf1->h_PDFBand_results;
        
    h_PDFBand_results->SetTitle("mypdf1_h_pdfband");
    h_PDFBand_results->SetName("mypdf1_h_pdfband");
    h_PDFBand_results->Print("all");
    //h_PDFBand_results->Write();
        

    std::cout<<"****TEST 1 END"<<std::endl;
*/
    /////////////////////////////////////////////    
/*
    std::cout<<"****TEST 2 START"<<std::endl;
    
    //    MyPDF *mypdf2 = new MyPDF(gridName,string("steering_mypdf_pdfband.txt"));
    MyPDF *mypdf2 = new MyPDF(gridName,string("steering/steering_mypdf_CT10.txt"),true,true);
    mypdf2->Print();

    h_PDFBand_results=mypdf2->h_PDFBand_results;
    
    h_PDFBand_results->SetTitle("mypdf2_h_pdfband");
    h_PDFBand_results->SetName("mypdf2_h_pdfband");
    h_PDFBand_results->Print("all");
    //h_PDFBand_results->Write();


    std::cout<<"****TEST 2 END"<<std::endl;
    /////////////////////////////////////////////    
*/
/*
    std::cout<<"****TEST 3 START"<<std::endl;

    MyPDF *mypdf3 = new MyPDF(gridName, 1.0, string("steering/steering_mypdf.txt"), true);
    //mypdf3->setDebug(true);
    
    mypdf3->Print();
    mypdf3->InitializeErrorGraphs();
    mypdf3->CalcSystErrors();
    
    h_AlphaS_results=mypdf3->h_AlphaS_results;
    
    h_AlphaS_results->SetTitle("mypdf3_h_alphaS");
    h_AlphaS_results->SetName("mypdf3_h_alphaS");
    h_AlphaS_results->Print("all");
    //h_AlphaS_results->Write();
    
    delete mypdf3;
    
    
    std::cout<<"****TEST 3 END"<<std::endl;
    /////////////////////////////////////////////    
*/    
    
    //fout->Write();
    //fout->Close();
  	
}
