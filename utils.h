#ifndef Utils_H 
#define Utils_H 
#include <iostream>
#include "TLatex.h"
#include "TH1.h"
#include "TF1.h"
#include "TPave.h"
#include "TMarker.h"
#include "TGraphErrors.h"
#include "TGraphAsymmErrors.h"
#include "TLine.h"
#include "TArrow.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
using namespace std;


void ATLAS_LABEL(Double_t x,Double_t y,Color_t color=1) {
  // plots ATLAS label
  TLatex l; //l.SetTextAlign(12); l.SetTextSize(tsize); 
  l.SetNDC();
  l.SetTextFont(72);
  l.SetTextColor(color);
  l.DrawLatex(x,y,"ATLAS");
}

TGraphErrors* TH1TOTGraph(TH1 *h1){
  //
  // convert the histogram h1 into a graph
  //
 const char *name="**TH1TOTGraph";  
 if (!h1) cout <<name<< " histogram not found !" << endl;
 //else h1->Print();
 //cout<<name<<" n= "<<h1->GetNbinsX()<<endl;
 //TGraphErrors* g1;
 TGraphErrors* g1= new TGraphErrors();
 //if (!g1) cout<<name<<" graph g1 not created "<<endl;
 //g1->Print();

 Double_t x, y, ex, ey;
 for (Int_t i=0; i<h1->GetNbinsX(); i++) {
   cout<<name<<" i= "<<i<<endl;
   y=h1->GetBinContent(i);
  ey=h1->GetBinError(i);
   x=h1->GetBinCenter(i);
  ex=h1->GetBinWidth(i)/2.;

  cout << i<<" x,y = " << x << " " << y << " ex,ey = " << ex << " " << ey << endl;

  g1->SetPoint(i,x,y);
  g1->SetPointError(i,ex,ey);

 }

 //g1->Print();
 cout<<name<<" return "<<endl;
 return g1;
}

TGraphAsymmErrors* TH1TOTGraphAsymm(TH1 *h1){
  //
  // convert the histogram h1 into a graph
  //
 const char *name="**TH1TOTGraphAsymm";  
 if (!h1) cout <<name<< " histogram not found !" << endl;
 //else h1->Print();
 //cout<<name<<" n= "<<h1->GetNbinsX()<<endl;
 //TGraphErrors* g1;
 TGraphAsymmErrors* g1= new TGraphAsymmErrors();
 //if (!g1) cout<<name<<" graph g1 not created "<<endl;
 //g1->Print();

 Double_t x, y, ex, ey;
 for (Int_t i=0; i<h1->GetNbinsX(); i++) {
  //cout<<name<<" i= "<<i<<endl;
   y=h1->GetBinContent(i+1);
  ey=h1->GetBinError(i+1);
   x=h1->GetBinCenter(i+1);
  ex=h1->GetBinWidth(i+1)/2.;

  //cout << i<<" x,y = " << x << " " << y << " ex,ey = " << ex << " " << ey << endl;

  g1->SetPoint(i,x,y);
  g1->SetPointError(i,ex,ex,ey,ey);

 }

 //g1->Print();
 //cout<<name<<" return "<<endl;
 return g1;
}


//TH1 *TH1Norm(TH1 *h1, bool normtot=true){
//// divide histogram h1 by bin-width and scale by number of entries
// const char *name="**TH1Norm";  
// const bool debug=false; 
// TH1* h1new=(TH1*) h1->Clone(h1->GetName());
//
// Double_t x, y, ey;
// for (Int_t i=0; i<=h1->GetNbinsX(); i++) {
//  y=h1->GetBinContent(i);
//  ey=h1->GetBinError(i);
//  x=h1->GetBinWidth(i);
//
//  if (x!=0) h1new->SetBinContent(i,y/x);
//  else      h1new->SetBinContent(i,0.);
//
//  if (x!=0) h1new->SetBinError(i,ey/x);
//  else      h1new->SetBinError(i,0.);
//
//  if (debug) cout << name<<"bincenter= "<<h1->GetBinCenter(i)<<" Binw = " << x << " y= " << y << endl;  
// } 
//
// //Double_t norm=h1->GetEntries();
// if (normtot) {
//  Double_t norm=h1->Integral();
//  if (norm!=0) {
//   h1new->Scale(1./norm);
//   if (debug) cout << name<<" histo " << h1->GetName() << " divided by " << norm << endl;
//  } else
//   cout << name<<" histo is empty " << h1->GetName() << endl;   
// }
// return h1new;
//}
//
//TH1 *TH1NormToTot(TH1 *h1, Double_t yscale, Double_t xscale=1.){
//// divide histogram h1 scale by total number and scale bin width x
//// assumes that histogram is divided by bin-width
// const char *name="**TH1NormToTot";  
// const bool debug=false;
// if (!h1) cout<<" TH1NormTotot h1 not found "<<endl; 
// TH1D* h1new=(TH1D*) h1->Clone(h1->GetName());
//
// Double_t x, y, ey;
// Double_t sigtot=0.;
// for (Int_t i=0; i<=h1->GetNbinsX(); i++) {
//  y=h1->GetBinContent(i)*yscale;
//  x=h1->GetBinWidth(i);
//  sigtot+=y*x;
// } 
//
// if (debug) cout<<name<<" sigtot= "<<sigtot<<endl;
//
// for (Int_t i=0; i<=h1->GetNbinsX(); i++) {
//  x =h1->GetBinWidth(i);
//  y =h1->GetBinContent(i)*yscale*x;
//  ey=h1->GetBinError(i)  *yscale*x;
//  x =h1->GetBinWidth(i)  *xscale;
//  if (x!=0) h1new->SetBinContent(i,y/x);
//  else      h1new->SetBinContent(i,0.);
//
//  if (x!=0) h1new->SetBinError(i,ey/x);
//  else      h1new->SetBinError(i,0.);
// } 
//
// if (sigtot!=0.)
//  h1new->Scale(1./sigtot);
//
// if (debug) {
//  for (Int_t i=0; i<=h1->GetNbinsX(); i++) {
//   y =h1->GetBinContent(i);
//   x =h1->GetBinWidth(i);
//   cout << name<<" "<<i<<" bincenter= "<<h1->GetBinCenter(i)<<" Binw = " << x << " y= " << y << endl;  
//  }
// }
//
// // h1new->Print("all");
// return h1new;
//}
//
void TH1Print(TH1 *h1){
// 
// prints histogram
//
 const char *name="**TH1Print";  
 Double_t xl,xh,x,dx, y, ey;
 //Double_t xl,xh, y, ey;

  for (Int_t i=0; i<=h1->GetNbinsX(); i++) {
   y =h1->GetBinContent(i);
   ey=h1->GetBinError(i);
   x =h1->GetBinCenter(i);
   xl=h1->GetBinLowEdge(i);
   xh=h1->GetBinLowEdge(i+1);
   dx =h1->GetBinWidth(i);
   cout << name<<" "<<i<<" "<<xl<<" <x< "<<xh<<" y= " << y << " ey= " << ey << endl;  
  }

 return;
}


TH1 *TH1Norm2(TH1 *h1){
// divide histogram h1 by bin-width and scale by number of entries
// and scale by width of non-last entry
 const char *name="**TH1Norm2: ";  
 const bool debug=false; 
 TH1* h1new=(TH1*) h1->Clone(h1->GetName());

 Double_t x, xw, y;
 Int_t ilast=0;
 for (Int_t i=0; i<h1->GetNbinsX(); i++) {
  y=h1->GetBinContent(i);
  xw=h1->GetBinWidth(i);
  if (y!=0) ilast=i;
  if (xw!=0) h1new->SetBinContent(i,y/xw);
  else      h1new->SetBinContent(i,0.);
  if (debug) cout <<name<< i <<" xw= "<<xw<<" x = " << x << " y= " << y << endl;  
 } 

 Double_t xmin=h1->GetBinCenter(0);
 Double_t xmax=h1->GetBinCenter(ilast);
 Double_t len=xmax-xmin;
 
 if (debug) cout<<name<<" ilast= "<<ilast<<" xmin= "<<xmin<<" xmax= "<<xmax<<" len= "<<len<<endl;

 //Double_t norm=h1->GetEntries();
 Double_t norm=h1->Integral();
 if (norm!=0) {
  h1new->Scale(1./norm);
  if (len!=0)  h1new->Scale(1./fabs(len));
  if (debug) cout << name<<" histo " << h1->GetName() 
                  << " divided by " << norm 
                  <<" and "<<fabs(len)
                  << endl;
 } else
  cout << name<<" histo is empty " << h1->GetName() << endl;   

 return h1new;
}


TH1 *TH1DivideByBinWidth(TH1 *h1){
  // divide by bin-width and scale by number of entries
  //  const char *name="**TH1DivideByBinWidth";  
  TH1* h1new=(TH1*) h1->Clone(h1->GetName());


  Double_t x, y;
  for (Int_t i=0; i<h1->GetNbinsX(); i++) {
    y=h1->GetBinContent(i);
    x=h1->GetBinWidth(i);
    if (x!=0) h1new->SetBinContent(i,y/x);
    else      h1new->SetBinContent(i,0.);
    //cout << name<<" x,y = " << x << " " << y << endl;

  }

  return h1new;
}

//



void myText(Double_t x,Double_t y,Color_t color,char *text,Double_t tsize=0.06) {
  // plot a Text
  //
  //Double_t tsize=0.05;
  TLatex l; //l.SetTextAlign(12); 
  l.SetTextSize(tsize); 
  l.SetNDC();
  l.SetTextColor(color);
  l.DrawLatex(x,y,text);
}
 

void myBoxText(Double_t x, Double_t y,Double_t boxsize,Int_t mcolor,char *text,Int_t lcol=1,Int_t lstyle=1,Double_t tsize=0.06) {
   // plot a Box and a Text
   //
  const char *name="**myBoxText";  
  const bool debug=false;
  //Double_t tsize=0.06;

  TLatex l; l.SetTextAlign(12); //l.SetTextSize(tsize); 
  l.SetNDC(); l.SetTextSize(tsize);
  l.DrawLatex(x,y,text);

  Double_t y1=y-boxsize/2.;
  Double_t y2=y+boxsize/2.;
  Double_t x2=x-0.3*tsize;
  Double_t x1=x2-boxsize;

  if (debug) printf("%s x1= %f x2= %f y1= %f y2= %f \n",name,x1,x2,y1,y2);

  TPave *mbox= new TPave(x1,y1,x2,y2,0,"NDC");

  mbox->SetFillColor(mcolor);
  mbox->SetFillStyle(1001);
  mbox->Draw();

  TLine mline;
  mline.SetLineWidth(2);
  mline.SetLineColor(lcol);
  mline.SetLineStyle(lstyle);
  //Double_t ym=(y1+y2)/2.;
  //mline.DrawLineNDC(x1,y,x2,ym);

}


void myMarkerText(Double_t x,Double_t y,Int_t color,Int_t mstyle,char *text, Double_t tsize=0.05) {
  // plot a Marker and a Text
  //
  //  printf("**myMarker: text= %s\ m ",text);

  TMarker *marker = new TMarker(x-(0.6*tsize),y,8);
  marker->SetMarkerColor(color);  marker->SetNDC();
  marker->SetMarkerStyle(mstyle);
  //marker->SetMarkerSize(2.0);
  marker->Draw();

  TLatex l; l.SetTextAlign(12); //l.SetTextSize(tsize); 
  l.SetNDC(); l.SetTextSize(tsize);
  l.DrawLatex(x,y,text);
  //cout<<" text= "<<text<<endl;
}
void myLineText(Double_t x,Double_t y,Int_t mcolor,Int_t mstyle,char *text, Double_t tsize=0.05) {
  // plot a line and a Text
  //
  TLatex l; l.SetTextAlign(12); l.SetTextSize(tsize); l.SetNDC(); 
  l.DrawLatex(x,y,text); 

  Double_t x1=x-1.3*tsize;
  Double_t x2=x-0.3*tsize;
 
  if (tsize<0.05) {
   x1=x-2.5*tsize;
   x2=x-0.3*tsize;

  }

  //y+=0.5*tsize;

  TLine mline;
  // mline.SetBit(TLine::kLineNDC );  // will set the bit kTRUE
  //mline.SetBit(TLine::kLineNDC,kFALSE);
  mline.SetLineWidth(2);
  mline.SetLineColor(mcolor);
  mline.SetLineStyle(mstyle);
  mline.DrawLineNDC(x1,y,x2,y);
  // mline->Draw();
}

void myLineText(Double_t x,Double_t y,Int_t mcolor,Int_t mstyle,char *text, 
                Double_t tsize, Double_t lsize ) {
  // plot a line and a Text
  TLatex l; l.SetTextAlign(12); l.SetTextSize(tsize); l.SetNDC(); 
  l.DrawLatex(x,y,text); 

  Double_t x1=x-1.5*lsize;
  Double_t x2=x-0.5*lsize;
 
  //  y+=tsize;

  TLine mline;
  // mline.SetBit(TLine::kLineNDC );  // will set the bit kTRUE
  //mline.SetBit(TLine::kLineNDC,kFALSE);
  mline.SetLineWidth(2);
  mline.SetLineColor(mcolor);
  mline.SetLineStyle(mstyle);
  mline.DrawLineNDC(x1,y,x2,y);
  // mline->Draw();
}



void myline(Double_t lmax, Double_t xpercent, Double_t offset=1., Double_t tsize=0.06) {

 const char help[]={"%"};

 TLine *mline = new TLine(0,offset,lmax,offset);
 mline->SetLineColor(1);
 mline->SetLineStyle(2);
 mline->Draw();
 TLine *mlineup = new TLine(0,offset+xpercent,lmax,offset+xpercent);
 mlineup->SetLineColor(1);
 mlineup->SetLineStyle(2);
 mlineup->Draw();
 TLine *mlinedn = new TLine(0,offset-xpercent,lmax,offset-xpercent);
 mlinedn->SetLineColor(1);
 mlinedn->SetLineStyle(2);
 mlinedn->Draw();
 //lmax+=2.;
 lmax*=1.01;
 TArrow *arrow=new TArrow(lmax,offset+xpercent,lmax,offset-xpercent,0.02,"<>");
 arrow->SetLineColor(1);
 arrow->SetLineWidth(1);
 arrow->SetLineStyle(1);
 arrow->Draw();

 TLatex l; l.SetTextAlign(12); l.SetTextSize(1.2*tsize);
 l.SetTextColor(1);
 char arg[100]; 
 //printf("xpercent*1000= %f \n",xpercent*1000);
 // printf("xpercent*1000= %d \n",xpercent*1000);
 if (xpercent*1000.<1){
  char *help=(char*) {(char*) "\275"};
  sprintf(arg,"#pm %3.1f%s",xpercent*1000,help);
} else {
  //printf(" xpercent= %f \n",xpercent);
  sprintf(arg," #pm %3.1f%s ",xpercent*100,help);
 }
 //l.DrawLatex(lmax+0.3,1.,arg);   
 l.DrawLatex(lmax*1.01,offset,arg);   

}

void myline2(Double_t lmin, Double_t lmax, Double_t xpercent, Double_t offset=1., Double_t tsize=0.06) {


 const char help[]={"%"};

 TLine *mline = new TLine(lmin,offset,lmax,offset);
 mline->SetLineColor(1);
 mline->SetLineStyle(2);
 mline->Draw();
 TLine *mlineup = new TLine(lmin,offset+xpercent,lmax,offset+xpercent);
 mlineup->SetLineColor(1);
 mlineup->SetLineStyle(2);
 mlineup->Draw();
 TLine *mlinedn = new TLine(lmin,offset-xpercent,lmax,offset-xpercent);
 mlinedn->SetLineColor(1);
 mlinedn->SetLineStyle(2);
 mlinedn->Draw();
 //lmax+=2.;
 lmax*=1.01;
 TArrow *arrow=new TArrow(lmax,offset+xpercent,lmax,offset-xpercent,0.02,"<>");
 arrow->SetLineColor(1);
 arrow->SetLineWidth(1);
 arrow->SetLineStyle(1);
 arrow->Draw();
TLatex l; l.SetTextAlign(12); l.SetTextSize(1.2*tsize);
 l.SetTextColor(1);
 char arg[100]; 
 //printf("xpercent*1000= %f \n",xpercent*1000);
 // printf("xpercent*1000= %d \n",xpercent*1000);
 if (xpercent*1000.<1){
  char *help=(char*) {(char*) "\275"};
  sprintf(arg,"#pm %3.1f%s",xpercent*1000,help);
} else {
  //printf(" xpercent= %f \n",xpercent);
  //sprintf(arg," #pm %3.1f%s ",xpercent*100,help);
  sprintf(arg," #pm %2.0f%s ",xpercent*100,help);
 }
 //l.DrawLatex(lmax+0.3,1.,arg);   
 l.DrawLatex(lmax*1.01,offset,arg);   

}


void CopyGraphProperties(TGraphErrors* g1, TGraphErrors* g2){
 const char *name="**CopyGraphProperties: ";
 // copy properties of graph g1 to g2

 if (!g1) cout << name<<" g1 does not exist ! " << endl;
 if (!g2) cout << name<<" g2 does not exist ! " << endl;

 g2->SetName       (g1->GetName());
 g2->SetMarkerStyle(g1->GetMarkerStyle());
 g2->SetMarkerColor(g1->GetMarkerColor());
 g2->SetLineColor  (g1->GetLineColor());
 g2->SetLineStyle  (g1->GetLineStyle());
 g2->SetLineWidth  (g1->GetLineWidth());

}





TGraphErrors *TGraphLinNorm(TGraphErrors *g1, Double_t xnorm=180.){
 const char *name="**TGraphLinNorm:";
 Double_t ynorm=1.;
 Double_t x, y;
 const bool debug=false;

 if (!g1) {cout<<name<<" input Graph not found !"<<endl; exit (1);}


 TGraphErrors* g2= new TGraphErrors();
 g2->SetName(g1->GetName());
 g2->SetName(g1->GetName());

 for (Int_t i=0; i<g1->GetN(); i++) {
  g1->GetPoint(i,x,y);
  if (debug) printf("%s %d x= %f y=%f %f \n",name,i,x,y,g1->GetErrorY(i));
  if (fabs(x-xnorm)<1.) ynorm=y;
 }
 
 if (debug) cout<<name<<" ynorm= "<<ynorm<<endl;

 for (Int_t i=0; i<g1->GetN(); i++) {
  g1->GetPoint(i,x,y);
  if (ynorm==0) {cout<<" ynorm= 0 ...exit"<<endl; exit (1);}
  g2->SetPoint(i,x,y/ynorm);
  Double_t ey= g1->GetErrorY(i);
  g2->SetPointError(i,0.,ey/ynorm);
 }

 if (debug) g2->Print();
 return g2;
}

TGraphErrors *myTGraphScaleErrors(TGraphErrors *g1, Double_t escale=1.){
 const char *name="**myTGraphScaleErrors:";
 const bool debug=false;
 if (!g1) {cout<<name<<" input Graph not found !"<<endl; exit (1);}

 TGraphErrors* g2= new TGraphErrors();
 g2->SetName(g1->GetName());
 g2->SetName(g1->GetName());

 Double_t x=0., y=0.;
 for (Int_t i=0; i<g1->GetN(); i++) {
  g1->GetPoint(i,x,y);
  g2->SetPoint(i,x,y);
  Double_t ey= g1->GetErrorY(i);
  Double_t ex= g1->GetErrorX(i);
  g2->SetPointError(i,ex,ey*escale);
 }

 if (debug) g2->Print();
 return g2;
}

TGraphErrors *myTGraphSetXErrorsZero(TGraphErrors *g1){
 const char *name="**myTGraphScaleErrors:";
 const bool debug=false;
 if (!g1) {cout<<name<<" input Graph not found !"<<endl; exit (1);}

 TGraphErrors* g2= new TGraphErrors();
 g2->SetName(g1->GetName());
 g2->SetName(g1->GetName());

 Double_t x=0., y=0.;
 for (Int_t i=0; i<g1->GetN(); i++) {
  g1->GetPoint(i,x,y);
  g2->SetPoint(i,x,y);
  Double_t ey= g1->GetErrorY(i);
  //Double_t ex= g1->GetErrorX(i);
  g2->SetPointError(i,0.,ey);
 }

 if (debug) g2->Print();
 return g2;
}




TGraphErrors* myTGraphErrorsDivide(TGraphErrors* g1,TGraphErrors* g2, Int_t noerr=1, Double_t dx=0.) {
 // Divide two graphs: new=g1/g2 
 //
 // noerr=0: put all errors to zero
 //       1: add errors from two graph quadrically
 //       2: set errors from graph 2 to zero
 
  const char *name="**myTGraphErrorsDivide:";
  const bool debug=false; 
  
  if (debug)cout<<name<<" noerr= "<<noerr<<" dx= "<<dx<<endl;

  if (!g1) printf("%s: g1 does not exist !  \n",name); 
  if (!g2) printf("%s: g2 does not exist !  \n",name); 

  Int_t n1=g1->GetN();
  Int_t n2=g2->GetN();

  if (n1!=n2) {
    printf("%s vector do not have same number of entries !  \n",name); 
   cout << name<<" g1: " << g1->GetName() << " n1= " << n1 << endl;
   cout << name<<" g2: " << g2->GetName() << " n2= " << n2 << endl;
  }

  TGraphErrors* g3= new TGraphErrors();
  g3->SetName(g1->GetName());


  Double_t  x1=0., y1=0., x2=0., y2=0.;
  Double_t dx1=0.,dy1=0.,dx2=0.,dy2=0.;

  Int_t iv=0;
  for (Int_t i1=0; i1<n1; i1++) {
   Int_t matchcount=0;
   for (Int_t i2=0; i2<n2; i2++) {
    g1->GetPoint(i1,x1,y1);
    g2->GetPoint(i2,x2,y2);
    dx1  = g1->GetErrorX(i1);
    dx2  = g2->GetErrorX(i2);
    Double_t emean=(dx1+dx2)/2.;
    //cout<<name<<" fabs= "<<fabs(x1-x2)<<" dx= "<<dx<<endl;

    if (fabs(x1-x2)>=emean && fabs(x1-x2)>dx ) {
      if (debug) printf("%s: %d x1!=x2  %f %f dx1=  %f x1= %f dx= %f  \n",name,iv,x1,x2,dx1,dx2,dx);
    }else{
     matchcount++;
     dx1  = g1->GetErrorX(i1);
     if (y1!=0) dy1  = g1->GetErrorY(i1)/y1;
     if (y2!=0) dy2  = g2->GetErrorY(i2)/y2;
   
     if (debug)
       printf("%s: %d x1=%f x2=%f y1=%f y2=%f  \n",name,iv,x1,x2,y1,y2);

     if (y2!=0.) g3->SetPoint(iv, x1,y1/y2);
     else        g3->SetPoint(iv, x1,y2);

     if (noerr==2) {dy2=0.; dy2=0.;}

     Double_t e=0.;
     if (y1!=0 && y2!=0) e=sqrt(dy1*dy1+dy2*dy2)*(y1/y2); 
     if (noerr==0) {
      g3->SetPointError(iv,dx1,0.);
     } else {
      g3->SetPointError(iv,dx1,e);
     }

     if (debug) {
       //Double_t g3y, g3x,g3e;
       //g3->GetPoint(iv, g3y,g3x);
       //g3e=g3->GetErrorY(iv);
       //printf("%d g3y= %f g3e=%f  \n",iv,g3y,g3e);
     }
     iv++;
    }
    //    printf("**myTGraphErrorsDivide: ...next  \n");
   }
   if (matchcount>1) {cout<<name<<" too many x-points matched ! "<<endl; exit (1);}   
  }  
  if (debug) g3->Print();
  return g3;

}

TGraphErrors* myTGraphErrorsAdd(TGraphErrors* g1,TGraphErrors* g2, Int_t noerr=1) {
 // Add two graphs: new=g1+g2 
 //
 // noerr=0: put all errors to zero
 //       1: add errors from two graph quadrically
 //       2: set errors from graph 2 to zero
 
  const char *name="**myTGraphErrorsAdd:";
  const bool debug=false; 

  if (!g1) printf("%s: g1 does not exist !  \n",name); 
  if (!g2) printf("%s: g2 does not exist !  \n",name); 

  Int_t n1=g1->GetN();
  Int_t n2=g2->GetN();

  if (n1!=n2) {
    printf("%s vector do not have same number of entries !  \n",name); 
   cout << name<<" g1: " << g1->GetName() << " n1= " << n1 << endl;
   cout << name<<" g2: " << g2->GetName() << " n2= " << n2 << endl;
  }

  TGraphErrors* g3= new TGraphErrors();
  g3->SetName(g1->GetName());

  Double_t  x1=0., y1=0., x2=0., y2=0.;
  Double_t dx1=0.,dy1=0.,dx2=0.,dy2=0.;

  Int_t iv=0;
  for (Int_t i1=0; i1<n1; i1++) {
   Int_t matchcount=0;
   for (Int_t i2=0; i2<n2; i2++) {
    g1->GetPoint(i1,x1,y1);
    g2->GetPoint(i2,x2,y2);
    dx1  = g1->GetErrorX(i1);
    dx2  = g2->GetErrorX(i2);
    Double_t emean=(dx1+dx2)/2.;
    //cout<<" fabs= "<<fabs(x1-x2)<<endl;
    if (fabs(x1-x2)>=emean && fabs(x1-x2)!=0) {
      if (debug) printf("%s: %d x1!=x2  %f %f dx=  %f %f !  \n",name,iv,x1,x2,dx1,dx2);
    }else{
     matchcount++;
     dx1  = g1->GetErrorX(i1);
     if (y1!=0) dy1  = g1->GetErrorY(i1)/y1;
     if (y2!=0) dy2  = g2->GetErrorY(i2)/y2;
   
     if (debug)
       printf("%s: %d x1=%f x2=%f y1=%f y2=%f  \n",name,iv,x1,x2,y1,y2);

     g3->SetPoint(iv, x1,y1+y2);

     if (noerr==2) {dy2=0.; dy2=0.;}

     Double_t e=0.;
     e=sqrt(dy1*dy1+dy2*dy2);

     if (noerr==0) {
      g3->SetPointError(iv,dx1,0.);
     } else {
      g3->SetPointError(iv,dx1,e);
     }

     if (debug) {
       //Double_t g3y, g3x,g3e;
       //g3->GetPoint(iv, g3y,g3x);
       //g3e=g3->GetErrorY(iv);
       //printf("%d g3y= %f g3e=%f  \n",iv,g3y,g3e);
     }
     iv++;
    }
    //    printf("**myTGraphErrorsDivide: ...next  \n");
   }
   if (matchcount>1) {cout<<name<<" too many x-points matched ! "<<endl; exit (1);}   
  }  
  if (debug) g3->Print();
  return g3;

}

TGraphErrors* myTGraphRemoveBin(TGraphErrors* g1,Int_t ibin) {
 // Remove bin from Graph
  // const Int_t debug=false; 
 const char *name="**myTGraphRemoveBin: ";
 
 if (!g1) cout << name<<" g1 does not exist ! " << endl;

 Int_t n1=g1->GetN();

 //g3->Clone();
 TGraphErrors* g3= new TGraphErrors();
 if (!g3) cout<<name<<" problem to make new vector ! " << endl;

 CopyGraphProperties(g1,g3); 


 Double_t* X1 = g1->GetX();
 Double_t* Y1 = g1->GetY();
 Double_t* EX = g1->GetEX();
 Double_t* EY = g1->GetEY();

 Int_t iv=0;
 for (Int_t i1=0; i1<n1; i1++) {  //loop over point of graph1
  if (i1!=ibin){
   g3->SetPoint(iv, X1[i1],Y1[i1]);
   g3->SetPointError(iv,EX[i1],EY[i1]);
   iv++;
  }
 }

 return g3;
}


TGraphErrors* myTGraphDividebyBinWidth(TGraphErrors* g1){
 // Remove bin from Graph
 const Int_t debug=false; 
 const char *name="**myTGraphDividebyBinWidth: ";
 
 if (!g1) cout << name<<" g1 does not exist ! " << endl;

 Int_t n1=g1->GetN();

 //g3->Clone();
 TGraphErrors* g3= new TGraphErrors();
 if (!g3) cout<<name<<" problem to make new vector ! " << endl;

 CopyGraphProperties(g1,g3); 

 Double_t* X1 = g1->GetX();
 Double_t* Y1 = g1->GetY();
 Double_t* EX = g1->GetEX();
 Double_t* EY = g1->GetEY();

 Int_t iv=0;
 for (Int_t i1=0; i1<n1; i1++) {  //loop over point of graph1
  Double_t ex= 2*EX[i1]; // stored errors is only half of binwidth
  Double_t  y= Y1[i1]/ex;
  Double_t ey= EY[i1]/ex;

  if (debug) cout<<name<<i1<<" ex= "<<ex<<" y= "<<Y1[i1]<<" y/ex= "<<y<<endl;

  g3->SetPoint     (iv, X1[i1],y);
  g3->SetPointError(iv, EX[i1],ey);
  iv++;
 }

 return g3;
}

TGraphErrors* myTGraphDividebyNumber(TGraphErrors* g1, Double_t xdiv=1.){
 // Remove bin from Graph
 const Int_t debug=false; 
 const char *name="**myTGraphDividebyNumber: ";
 
 if (!g1) cout << name<<" g1 does not exist ! " << endl;

 Int_t n1=g1->GetN();

 //g3->Clone();
 TGraphErrors* g3= new TGraphErrors();
 if (!g3) cout<<name<<" problem to make new vector ! " << endl;

 CopyGraphProperties(g1,g3); 

 Double_t* X1 = g1->GetX();
 Double_t* Y1 = g1->GetY();
 Double_t* EX = g1->GetEX();
 Double_t* EY = g1->GetEY();

 Int_t iv=0;
 for (Int_t i1=0; i1<n1; i1++) {  //loop over point of graph1
  Double_t  y= Y1[i1]/xdiv;
  Double_t ey= EY[i1]/xdiv;

  if (debug) cout<<name<<i1<<" xdiv= "<<xdiv<<" y= "<<Y1[i1]<<" y/xdiv= "<<y<<endl;

  g3->SetPoint     (iv, X1[i1],y);
  g3->SetPointError(iv, EX[i1],ey);
  iv++;
 }

 return g3;
}

TGraphErrors* myGetTGraphErrors(TGraphAsymmErrors* g1) {
 // Make from a TGraphAsymmError a TGraph with errors=0
  // const Int_t debug=false; 
 const char *name="**myGetTGraphErrors: ";
 
 if (!g1) cout << name<<" g1 does not exist ! " << endl;

 Int_t n1=g1->GetN();

 TGraphErrors* g2= new TGraphErrors();
 if (!g2) cout<<name<<" problem to make new vector ! " << endl;
 g2->SetName(g1->GetName());

 Double_t* X1 = g1->GetX();
 Double_t* Y1 = g1->GetY();
 // Double_t* EXhigh1 = g1->GetEXhigh();
 Double_t* EXlow1 =  g1->GetEXlow();
 //Double_t* EYhigh1 = g1->GetEYhigh();
 //Double_t* EYlow1 =  g1->GetEYlow();

 for (Int_t i1=0; i1<n1; i1++) {  //loop over point of graph1
   g2->SetPoint(i1, X1[i1],Y1[i1]);
   g2->SetPointError(i1,EXlow1[i1],0.);
 }

 return g2;
}

TGraphAsymmErrors* myTGraphRemoveBin(TGraphAsymmErrors* g1,Int_t ibin) {
 // Remove bin from Graph
  // const Int_t debug=false; 
 const char *name="**myTGraphRemoveBin:";
 
 if (!g1) cout << name<<" g1 does not exist ! " << endl;

 Int_t n1=g1->GetN();

 TGraphAsymmErrors* g3= new TGraphAsymmErrors();
 if (!g3) cout<<name<<" problem to make new vector ! " << endl;
 g3->SetName(g1->GetName());
 g3->SetName       (g1->GetName());
 g3->SetMarkerStyle(g1->GetMarkerStyle());
 g3->SetMarkerColor(g1->GetMarkerColor());
 g3->SetLineColor  (g1->GetLineColor());

 // Double_t   x1=0.,   y1=0., x2=0., y2=0.;
 //Double_t dx1h=0., dx1l=0.;
 //Double_t dy1h=0., dy1l=0.;
 //Double_t dy2h=0., dy2l=0.;

 Double_t* X1 = g1->GetX();
 Double_t* Y1 = g1->GetY();
 Double_t* EXhigh1 = g1->GetEXhigh();
 Double_t* EXlow1 =  g1->GetEXlow();
 Double_t* EYhigh1 = g1->GetEYhigh();
 Double_t* EYlow1 =  g1->GetEYlow();

 Int_t iv=0;
 for (Int_t i1=0; i1<n1; i1++) {  //loop over point of graph1
  if (i1!=ibin){
   g3->SetPoint(iv, X1[i1],Y1[i1]);
   g3->SetPointError(iv,EXlow1[i1],EXhigh1[i1],EYlow1[i1],EYhigh1[i1]);
   iv++;
  }
 }

 return g3;
}


TGraphAsymmErrors* myTGraphErrorsDivide(TGraphAsymmErrors* g1,TGraphAsymmErrors* g2, Int_t noerr=1) {
 // Divide two TGraphAsymmErrors: new=g1/g2 
 //
 // noerr=0: put all errors to zero
 //       1: add errors from two graph quadrically
 //       2: set errors from graph 2 to zero

 const bool debug=false; 
 const char *name="**myTGraphErrorsDivide:";
 
 if (!g1) cout << name<<" g1 does not exist ! " << endl;
 if (!g2) cout << name<<" g2 does not exist ! " << endl;

 Int_t n1=g1->GetN();
 Int_t n2=g2->GetN();

 if (n1!=n2) {
   printf("%s: vector do not have same number of entries !  \n",name); 
  cout <<name<< " g1: " << g1->GetName() << " n1= " << n1 << endl;
  cout <<name<< " g2: " << g2->GetName() << " n2= " << n2 << endl;
 }

 TGraphAsymmErrors* g3= new TGraphAsymmErrors();
 if (!g3) cout<<name<<" problem to make new vector ! " << endl;
 g3->SetName       (g1->GetName());
 g3->SetMarkerStyle(g1->GetMarkerStyle());
 g3->SetMarkerColor(g1->GetMarkerColor());
 g3->SetLineColor  (g1->GetLineColor());

 Double_t   x1=0.,   y1=0., x2=0., y2=0.;
 Double_t dx1h=0., dx1l=0.;
 Double_t dy1h=0., dy1l=0.;
 Double_t dy2h=0., dy2l=0.;

 // Double_t* X1 = g1->GetX();
 //Double_t* Y1 = g1->GetY();
 Double_t* EXhigh1 = g1->GetEXhigh();
 Double_t* EXlow1 =  g1->GetEXlow();
 Double_t* EYhigh1 = g1->GetEYhigh();
 Double_t* EYlow1 =  g1->GetEYlow();

 //Double_t* X2 = g2->GetX();
 //Double_t* Y2 = g2->GetY();
 Double_t* EXhigh2 = g2->GetEXhigh();
 Double_t* EXlow2 =  g2->GetEXlow();
 Double_t* EYhigh2 = g2->GetEYhigh();
 Double_t* EYlow2 =  g2->GetEYlow();

 Int_t iv=0;
 for (Int_t i1=0; i1<n1; i1++) {  //loop over point of graph1
  Int_t matchcount=0;
  for (Int_t i2=0; i2<n2; i2++) {//loop over point of graph2
   g1->GetPoint(i1,x1,y1);
   g2->GetPoint(i2,x2,y2);
   Double_t emean=(EXhigh1[i1]+EXhigh2[i2]+EXlow1[i1]+EXlow2[i2])/4.;
   if (fabs(x1-x2)>emean) {
     //cout <<name<<" x1 and x2 not the same x1= "<<x1<<" x2= "<<x2<<endl; 
   } else { // do something only if x1=x2
     matchcount++;
     //cout <<name<<" x1 and x2 match x1= "<<x1<<" x2= "<<x2<<endl; 
    dx1h  = EXhigh1[i1];
    dx1l  = EXlow1[i1];
    if (y1!=0.) dy1h  = EYhigh1[i1]/y1;
    else        dy1h  = 0.;
    if (y2!=0.) dy2h  = EYhigh2[i2]/y2;
    else        dy2h  = 0.;
    if (y1!=0.) dy1l  = EYlow1 [i1]/y1;
    else        dy1l  = 0.;
    if (y2!=0.) dy2l  = EYlow2 [i2]/y2;
    else        dy2l  = 0.;
   
    if (debug)
     printf("%s: %d %d dy1=%f %f dy2=%f %f sqrt= %f %f \n",name,i1,i2,dy1l,dy1h,dy2l,dy2h,
             sqrt(dy1l*dy1l+dy2l*dy2l),sqrt(dy1h*dy1h+dy2h*dy2h));

    if (y2!=0.) g3->SetPoint(iv, x1,y1/y2);
    else        g3->SetPoint(iv, x1,y2);
    Double_t el=0.; Double_t eh=0.;

    if (noerr==2) {dy2l=0.; dy2h=0.;}
    if (y1!=0. && y2!=0.) el=sqrt(dy1l*dy1l+dy2l*dy2l)*(y1/y2);
    if (y1!=0. && y2!=0.) eh=sqrt(dy1h*dy1h+dy2h*dy2h)*(y1/y2);
   

    if (debug) printf("dx1h=%f  dx1l=%f  el=%f  eh=%f \n",dx1h,dx1l,el,eh);
    if (noerr==0) {
     g3->SetPointError(iv,dx1l,dx1h,0,0);
    } else {
     g3->SetPointError(iv,dx1l,dx1h,el,eh);
    }

    iv++;
   }
  }
  if (matchcount>1) {cout<<name<<" too many x-points matched ! "<<endl; exit (1);}
 }  
 return g3;
}

TGraphAsymmErrors* myTGraphNormToSum(TGraphAsymmErrors* g1) {
 // normalize Graph by Sum over all points 
 const bool debug=false; 
 const char *name="**myTGraphNormToSum:";

 if (!g1) cout << name<<" g1 does not exist ! " << endl;

 Int_t n1=g1->GetN();
 Double_t sum=0.;
 Double_t* X1 = g1->GetX();
 Double_t* Y1 = g1->GetY();
 Double_t* EXhigh1 = g1->GetEXhigh();
 Double_t* EXlow1 =  g1->GetEXlow();
 Double_t* EYhigh1 = g1->GetEYhigh();
 Double_t* EYlow1 =  g1->GetEYlow();

 // calculate sum
 for (Int_t i=0; i<n1; i++) {  //loop over point of graph1
  sum+=Y1[i];
 }  

 if (debug) cout<<name<<" sum= "<<sum<<endl;

 TGraphAsymmErrors* g3= new TGraphAsymmErrors();
 if (!g3) cout<<name<<" problem to make new vector ! " << endl;
 g3->SetName(g1->GetName());
 g3->SetName       (g1->GetName());
 g3->SetMarkerStyle(g1->GetMarkerStyle());
 g3->SetMarkerColor(g1->GetMarkerColor());
 g3->SetLineColor  (g1->GetLineColor());

 // divide points and errors sum
 for (Int_t i=0; i<n1; i++) {  //loop over point of graph1
  g3->SetPoint(i,X1[i],Y1[i]/sum);
  g3->SetPointError(i,EXlow1[i],EXhigh1[i],EYlow1[i]/sum,EYhigh1[i]/sum);
 }  

 // if (debug) g3->Print();
 return g3;
}


TGraphErrors * FuncToGraph(TF1 *f1, const Int_t n, const Double_t xMin=0, const Double_t xMax=1)  { 
//
// turn a function into a Graph
// n: number of point in Graph
// Xmin/xMax: minimum and maximum
//
  const bool debug=false;
  const char *name="**FuncToGraph: ";

  //cout << " f1= " << f1 << endl;
  TGraphErrors * g1 = new TGraphErrors();
  if (!f1) { cout << name<<"  f1 not found " << endl; return g1;  }
  else if (debug) f1->Print();
  
  if (debug) cout<<name<<"n="<<n<<" xmin= "<<xMin<<" xMax= "<<xMax<<endl;

  char gname[500]; sprintf(gname,"g%s",f1->GetName()); g1->SetName(gname);
  Double_t binw=(xMax-xMin)/n;
  for(Int_t i = 0; i < n; i++) { 
    //for(Int_t i = 0; i < xMax; i++) { 
    Double_t x = xMin + i*binw;
    Double_t y = f1->Eval(x);

    if (debug) cout <<name<< i << " x= " << x << " y= " << y << endl;

    g1->SetPoint(i, x, y);
    g1->SetPointError(i, 0., 0.);
    //}    
  }
  return g1;
 }
//

//

TGraphAsymmErrors* myMakeBand(TF1* f0, TF1* f1,TF1* f2) {
  // make a band out of functions
  const char *name="**myMakeBand";  
  //const Int_t debug=0; 

  TGraphAsymmErrors* g3= new TGraphAsymmErrors();
  //  TString gname=(TString)f1->GetName()+(TString)f1->GetName()+(TString)f2->GetName();
  //TString gname=f1->GetName()+f1->GetName()+f2->GetName();
  //g3->SetName(gname);

  if (!f0) {cout << name<<"  f0 not found ! " << endl; return g3;}
  if (!f1) {cout << name<<"  f1 not found ! " << endl; return g3;}
  if (!f2) {cout << name<<"  f2 not found ! " << endl; return g3;}

  const Double_t NBIN=1000.;
  Double_t xmin, xmax;
  xmax=f0-> GetMinimumX();
  xmin=f0-> GetMaximumX();


  cout << name<< " xmin= " << xmin << " xmax= " << xmax << endl;



  Double_t  x1=0., y1=0., x2=0., y2=0., y0=0, x3=0.;

  Double_t dx=(xmax-xmin)/NBIN;
  for (Int_t i=0; i<NBIN; i++) {

    x1=xmin+i*dx;
    y0=f0->Eval(x1);
    y1=f1->Eval(x1);
    y2=f2->Eval(x1);

    if (i==NBIN-1) x2=x1; 
    else           x2=xmin+(i+1)*dx; 

    if (i==0)      x3=x1; 
    else           x3=xmin+(i-1)*dx;    

    Double_t tmp=y2;
    if (y1<y2) {y2=y1; y1=tmp;}

    Double_t y3=y0;
    g3->SetPoint(i,x1,y3);

    Double_t binwl=(x1-x3)/2.;
    Double_t binwh=(x2-x1)/2.;
    if (binwl==0.)  binwl= binwh;
    if (binwh==0.)  binwh= binwl;
    g3->SetPointError(i,binwl,binwh,(y3-y2),(y1-y3));

  }  
  return g3;

}

 

TGraphAsymmErrors* myMakeBand(TGraphErrors* g0, TGraphErrors* g1,TGraphErrors* g2,Int_t noorder=1) {
  // make a band out of 3 graphs
  // default is g0: middle 
  //         g1/g2: contain lower and upper values
  //
  const bool debug=false;
  const char *name="**myMakeBand: ";  

  if (!g0) {cout<<name<<" g0 not found ! "<<endl; exit (1);}
  if (!g1) {cout<<name<<" g1 not found ! "<<endl; exit (1);}
  if (!g2) {cout<<name<<" g2 not found ! "<<endl; exit (1);}

  TGraphAsymmErrors* g3= new TGraphAsymmErrors();
  g3->SetName       (g1->GetName());
  g3->SetMarkerStyle(g1->GetMarkerStyle());
  g3->SetMarkerColor(g1->GetMarkerColor());
  g3->SetLineColor  (g1->GetLineColor());  


  Double_t  x1=0., x2=0., x3=0., y[3];
  //Double_t dx1=0.;
  Double_t dum;
  for (Int_t i=0; i<g1->GetN(); i++) {
    g0->GetPoint(i, x1,y[0]);
    g1->GetPoint(i, x1,y[1]);
    g2->GetPoint(i, x1,y[2]);

    if (i==g1->GetN()-1) x2=x1;
    else                 g2->GetPoint(i+1,x2,dum);

    if (i==0)            x3=x1;
    else                 g2->GetPoint(i-1,x3,dum);

    Double_t tmp, yup, yce, ydn;
    if (noorder==0) {
     yce=y[0]; ydn=y[1]; yup=y[2];
     if (debug) cout<<" yce= "<<yce<<" ydn= "<<ydn<<" yup= "<<yup<<endl;
    } else if (noorder==1) {
     tmp=y[2];
     if (debug) cout<<" y[0]= "<<y[0]<<" y[1]= "<<y[1]<<" y[2]= "<<y[2]<<endl;
     if (y[1]<y[2]) {y[2]=y[1]; y[1]=tmp;}

     yce=y[0]; ydn=y[1]; yup=y[2];
     if (debug) cout<<" yce= "<<yce<<" ydn= "<<ydn<<" yup= "<<yup<<endl;
    } else {
      //cout<<i<<" before: "<<endl;
      //for (Int_t j=0; j<3; j++) cout<<" y["<<j<<"]= "<<y[j];
      //cout<<endl;

     for(Int_t p=0;p<2;p++) {
      for(Int_t j=0;j<2;j++) {
       if(y[j]>y[j+1]) {
        tmp=y[j+1]; y[j+1]=y[j]; y[j]=tmp;
       }
      }
     }

     //cout<<" after: "<<endl;
     //for (Int_t j=0; j<3; j++) cout<<" y["<<j<<"]= "<<y[j];
     //cout<<endl; 
     ydn=y[0]; yce=y[1]; yup=y[2];
    }
    //cout<<" ydn= "<<ydn<<" yce= "<<yce<<" yup= "<<yup<<endl;

    g3->SetPoint(i,x1,yce);

//   Double_t binwl=(x1-x3)/2.;
//   Double_t binwh=(x2-x1)/2.;
//   if (binwl==0.)  binwl= binwh;
//   if (binwh==0.)  binwh= binwl;

//    Double_t ex1=g1->GetErrorX(i+1);
//    Double_t ex2=g1->GetErrorX(i-1);
    Double_t ex0=g1->GetErrorX(i);
    Double_t binwl=0., binwh=0.;
//if (ex1==0&&ex2==0) {

    if (ex0==0) {
     binwl=(x1-x3)/2.;
     binwh=(x2-x1)/2.;
     if (binwl==0.)  binwl= binwh;
     if (binwh==0.)  binwh= binwl;
    } else {
     binwl=ex0;
     binwh=ex0;
    }

    Double_t dxl=yce-ydn;
    Double_t dxh=yup-yce;

    if (noorder==0) {
     cout<<" dxl= "<<dxl<<" dxh= "<<dxh<<endl; 
     if (dxl<0) {tmp=-dxl; dxl=dxh; dxh=tmp;} 
     if (dxh<0) {tmp=-dxh; dxh=dxl; dxh=tmp;} 
    }    
    g3->SetPointError(i,binwl,binwh,dxl,dxh);

  }
  return g3;

}

TGraphAsymmErrors* myMakeBand(TH1* h0, TH1* h1,TH1* h2,Int_t noorder=1) {
  // make a band out of 3 histogram
  // default is g0: middle g1/g2: contain lower and upper values
  //
  //const Int_t debug=0;
  const char *name="**myMakeBand: ";  

  if (!h0) {cout<<name<<" h0 not found ! "<<endl; exit (1);}
  if (!h1) {cout<<name<<" h1 not found ! "<<endl; exit (1);}
  if (!h2) {cout<<name<<" h2 not found ! "<<endl; exit (1);}

  TGraphErrors* g0=TH1TOTGraph(h0);
  if (!g0) {cout<<name<<" problem to make g0  ! "<<endl; exit (1);}

  TGraphErrors* g1=TH1TOTGraph(h1);
  if (!g1) {cout<<name<<" problem to make g1  ! "<<endl; exit (1);}

  TGraphErrors* g2=TH1TOTGraph(h2);
  if (!g2) {cout<<name<<" problem to make g2  ! "<<endl; exit (1);}

  TGraphAsymmErrors* band=myMakeBand(g0,g1,g2,noorder);
  if (!band) {cout<<name<<" problem to make bank  ! "<<endl; exit (1);}

  return band;
}

void myAddtoBand(TGraphErrors* g1, TGraphAsymmErrors* g2, bool addinquadrature=false,bool setzeroone=false) {
  // add the graph g1 to the band g2
  //
  // errors are taken as max
  // or add in quadrature
  //
  Double_t  x1=0., y1=0.,  y2=0., y0=0;
  const char *name="**myAddtoBand";
  const bool debug=false;  

  if (g1->GetN()!=g2->GetN())
   cout << name<< " graphs have not the same # of elements g1= " 
        << g1->GetN() <<" g2 = "<<g2->GetN()
        << endl;

  Double_t* EYhigh = g2-> GetEYhigh();
  Double_t* EYlow  = g2-> GetEYlow();

  for (Int_t i=0; i<g1->GetN(); i++) {
    g1->GetPoint(i, x1,y1);
    g2->GetPoint(i, x1,y2);

    if (setzeroone) {
     if (y1==0) y1=1;
     if (y2==0) y2=1;
    }
    //    if (i==g1->GetN()-1) x2=x1;
    //    else                 g2->GetPoint(i+1,x2,dum);
    //    if (i==0)            x3=x1;
    //    else                 g2->GetPoint(i-1,x3,dum);


    Double_t eyh=0., eyl=0.;
    //if (y1<y2) {y2=y1; y1=tmp;}
    //Double_t y3=1.;

    if (debug) printf("%d: y1=%f y2=%f Eyhigh= %f Eylow= %f \n",i,y1,y2,EYhigh[i],EYlow[i]);


    //Double_t ymean=(y1+y2)/2.;
    //g2->SetPoint(i,x1,ymean);
    //y0=ymean-y1;
    y0=y1-y2;
    //if (y0!=0) {
     if (y0>=0){
      eyh=EYhigh[i];
      //eyh-=ymean-y2;
      if (addinquadrature) 
       eyh=sqrt(eyh*eyh+y0*y0);
      else
       if(fabs(y0)>eyh) eyh=fabs(y0);

      if (debug) printf("high: %d: y0=%f eyh=%f  \n",i,y0,eyh);
      g2->SetPointEYhigh(i,eyh);
     } else {
      eyl=EYlow[i];
      //eyl-=y2-ymean;
      if (addinquadrature) 
       eyl=sqrt(eyl*eyl+y0*y0);
      else
	if(fabs(y0)>eyl) eyl=fabs(y0);

     if (debug) printf("low: %d: y0=%f eyl=%f  \n",i,y0,eyl);
      g2->SetPointEYlow (i,eyl);
     }
     //}
  }
  return;

}

void myAddtoBand(TH1* h1, TGraphAsymmErrors* g2) {
  // add the histo h1 to the band g2
  const char *name="**myAddtoBand";
 
  TGraphErrors* g0=TH1TOTGraph(h1);
  if (!g0) {cout<<name<<" problem to make g0  ! "<<endl; exit (1);}
 
  myAddtoBand(g0,g2);

  return;
 }

//TGraphErrors* myAverage(TGraphErrors* g1, TGraphErrors* g2) {
void  myAverage(TGraphErrors* g1, TGraphErrors* g2) {
  // average g1 and g2 
  // average stored in g1
  //
  const char *name="**myAverage: ";
  const bool debug=false;

  Double_t   x1=0.,  x2=0.,  y1=0.,   y2=0.;  
  Double_t  ex1=0., ex2=0., ey1=0.,  ey2=0.;  
  if (g1->GetN()!=g2->GetN())
   cout << name<< " graphs have not the same # of elements " 
        << " g1: "<<g1->GetN()<<" g2= "<<g2->GetN()<<endl;
 
 
  //TGraphErrors* g3= new  TGraphErrors(g1->GetName());
  //if (!g3) cout<<" problem to make vector g3 " <<endl;

 if (debug)  {
   cout<<name<<" g2: "<<endl;
   g2->Print();
  }
  
  for (Int_t i=0; i<g1->GetN(); i++) {
    g1->GetPoint(i, x1,y1);
    g2->GetPoint(i, x2,y2);
    ey1  = g1->GetErrorY(i);
    ey2  = g2->GetErrorY(i);
    ex1  = g1->GetErrorX(i);
    ex2  = g2->GetErrorX(i);
    if (fabs(x1-x2)<=(ex1+ex2)/2.) {
      Double_t y3=(y1+y2)/2.;
      //Double_t y3=(y1/ey1+y2/ey1)/(1./ey1+1./ey2);
     g1->SetPoint(i,x1,y3);

     Double_t ey3=ex1*ex1+ex2*ex2;
     if (ey3!=0.) ey3=sqrt(ey3);
     if (debug) cout<<name<<x1<<" y1= "<<y1<<" y2= "<<y2<<" y3= "<<y3<<endl;
     g1->SetPointError (i,ex1,ey3);
   } else
      if (debug) cout<<" x values not the same: x1= "<<x1<<" x2= "<<x2<<endl;
  }

  if (debug)  {
   cout<<name<<" g1: "<<endl;
   g1->Print();
  }
  return;
}



void DrawGraphWithband(TGraphAsymmErrors *g1) {
  char *name=(char*) "**DrawGraph: ";

 if (!g1) cout <<name<<" g1 does not exist ! " << endl;
 
 g1->Draw("e2z");
 // g1->SetLineWidth(3);
 //g1->Draw("z");

 return;
}


TGraphErrors* myTGraphErrorsOrder(TGraphErrors* g1) {
 // Order graph g1 in increasing order in x
 // 
 const bool debug=false; 
 const char *name="**myTGraphErrorsOrder: ";
 
 if (!g1) {cout << name<<" g1 does not exist ! " << endl; return 0; }

 Int_t n1=g1->GetN();
 
 TGraphErrors* g2= new TGraphErrors();
 if (!g2) cout<<name<<" problem to make new vector g2 ! " << endl;

 TGraphErrors* g3= new TGraphErrors();
 if (!g3) cout<<name<<" problem to make new vector g3 ! " << endl;
 g3=g1;
 
 if (debug) {
  cout<<name<<" g3: "<<endl;
  g3->Print();
 }
 Double_t xmin; Int_t imin=0;
 for (Int_t i1=0; i1<n1; i1++) {  //loop over points of graph1
  Int_t n3=g3->GetN();
  if (debug) cout<<name<<i1<<" n3= "<<n3<<endl;
  if (n3==0) break;

  Double_t*  X3 = g3->GetX(); 
  Double_t*  Y3 = g3->GetY(); 
  Double_t* EX3 = g3->GetEX(); 
  Double_t* EY3 = g3->GetEY(); 
  xmin=1.e99;
  for (Int_t i3=0; i3<n3; i3++) {  //loop over points of graph1
   if (debug) cout<<name<<i3<<" X3= "<<X3[i3]<<" xmin= "<<xmin<<" imin= "<<imin<<endl;
   if (X3[i3]<xmin) { imin=i3; xmin=X3[i3];}
  }
  g2->SetPoint     (i1, X3[imin], Y3[imin]);
  g2->SetPointError(i1,EX3[imin],EY3[imin]);
  g3->RemovePoint(imin);
 }

 delete g3;
 if (debug) {
  cout<<name<<" ordered vector : "<<endl;
  g2->Print();
 }
 return g2;
}

TGraphErrors* myTGraphErrorsMerge(TGraphErrors* g1, TGraphErrors* g2) {
 // Merge bins in g1 to bins given in g2
 // g1 must have finer binning than g2

 const bool debug=false; 
 const char *name="**myTGraphErrorsMerge: ";
 
 if (!g1) {cout << name<<" g1 does not exist ! " << endl; return 0; }
 if (!g2) {cout << name<<" g2 does not exist ! " << endl; return 0; }

 Int_t n1=g1->GetN();
 Int_t n2=g2->GetN();
 
 if (n2>n1) {cout<<name<<" can not merge n1= "<<n1<<" n2= "<<n2<<endl; return 0;}

 TGraphErrors* g3= new TGraphErrors();
 if (!g3) cout<<name<<" problem to make new vector g3 ! " << endl;
 TGraphErrors* g4= new TGraphErrors();
 if (!g4) cout<<name<<" problem to make new vector g4 ! " << endl;
 //char newname[500]; 
 //sprintf(newname,"%sMergedBinsFrom%s",g1->GetName(),g2->GetName());
 //g3->SetName(newname);

 Double_t x=0., dx=0.;
 // Double_t gysum, gy, dgy, gx, dgx;
 Double_t gysum, gy, gx, dgx;
 Double_t xmin=0, xmax=0;
 const Int_t Nslice=10;

 Double_t*  X1 = g1->GetX();
 Double_t*  Y1 = g1->GetY();
 Double_t* EX1 = g1->GetEX();
 Double_t* EY1 = g1->GetEY();

 Double_t BIG=1.e20;
 Double_t x1minp=BIG, x1maxp=-BIG, x1minm=BIG, x1maxm=-BIG;
 
 Double_t*  X2 = g2->GetX();
 Double_t*  Y2 = g2->GetY();
 Double_t* EX2 = g2->GetEX();
 // Double_t* EY2 = g2->GetEY();

 // graph g1 had been divided by bin-width
 for (Int_t i1=0; i1<n1; i1++) {  //loop over points of graph1
  if (debug) cout<<name<<i1<<" X1= "<<X1[i1]<<" Y1= "<<Y1[i1]<<endl;
   //g4->SetPoint     (i1, X1[i1], Y1[i1]*2*EX1[i1]);
   //g4->SetPointError(i1,EX1[i1],EY1[i1]*2*EX1[i1]);
   g4->SetPoint     (i1, X1[i1], Y1[i1]);
   g4->SetPointError(i1,EX1[i1],EY1[i1]);
 
   if (X1[i1]>0.) {
    if   (x1maxp<=X1[i1]) x1maxp=X1[i1]; 
    if   (x1minp> X1[i1]) x1minp=X1[i1]; 
   } else {
    if   (x1maxm<=X1[i1]) x1maxm=X1[i1]; 
    if   (x1minm> X1[i1]) x1minm=X1[i1]; 
   }
 }

 if (debug) cout<<" x1minp= "<<x1minp<<" x1maxp= "<<x1maxp<<endl;
 if (debug) cout<<" x1minm= "<<x1minm<<" x1maxm= "<<x1maxm<<endl;
 if (debug) cout<<" n2= "<<n2<<endl; 

 for (Int_t i2=0; i2<n2; i2++) {  //loop over points of graph2
  Double_t xi=X2[i2]-EX2[i2], xj=X2[i2]+EX2[i2]; 
  if (debug) cout<<name<<i2<<" Xi[i]= "<<xi<<" X2[i+1]= "<< xj <<" Y2= "<<Y2[i2]<<endl;

  if   (xi>xj){xmin=xj; xmax=xi; }
  else        {xmin=xi; xmax=xj; }
  dgx=(xmax-xmin)/Nslice;
  gysum=0.;
  //if (debug) cout<<name<<i<<" dgx= "<<dgx<<endl;
  Int_t iflag=0;
  for (Int_t i=0; i<Nslice; i++) {  //loop over points of graph2
   gx=xmin+dgx/2.+i*dgx;
   //gy=g4->Eval(gx);
   TSpline *spline=0;
   gy=g4->Eval(gx,spline,"S");
   // if vector does not cover full range of bin, set to zero
   // no extrapolation
   if (gx>0) {if (gx<=x1minp||gx>x1maxp) iflag=1;}
   else      {if (gx<=x1minm||gx>x1maxm) iflag=1;}
   if (iflag==1) {gy=0.; gysum=0.;}

   gysum+=gy; 
   if (debug) cout<<name<<i<<" iflag= "<<iflag<<" gx= "<<gx<<" gysum= "<<gysum<<" gy= "<<gy<<endl;
  }
  gysum/=Nslice;
  x= X2[i2]; dx=(xmax-xmin);

  if (debug) cout<<name<<i2<<" fill: x= "<<x<<" dx= "<<dx<<" gy="<<gysum<<" gy/dx= "<<gysum/dx<<endl;

  g3->SetPoint     (i2, x  , gysum);
  g3->SetPointError(i2,dx/2, 0.);
 }


 delete g4;
 return g3;
}

TGraphErrors *myTGraphNormToSum(TGraphErrors *g1){
 const char *name="**myTGraphNormToSum:";
 Double_t ynorm=0.;
 Double_t x, y, ex;
 const bool debug=false;

 if (!g1) {cout<<name<<" input Graph not found !"<<endl; exit (1);}

 TGraphErrors* g2= new TGraphErrors();
 g2->SetName(g1->GetName());

 for (Int_t i=0; i<g1->GetN(); i++) {
  g1->GetPoint(i,x,y);
  if (debug) printf("%s %d x= %f y=%f %f \n",name,i,x,y,g1->GetErrorY(i));
  ynorm+=y;
 }
 
 if (debug) cout<<name<<" ynorm= "<<ynorm<<endl;

 for (Int_t i=0; i<g1->GetN(); i++) {
  g1->GetPoint(i,x,y);
  ex  = g1->GetErrorX(i);
  //  if (ynorm==0) {cout<<" ynorm= 0 ...exit"<<endl; exit (1);}
  if (ynorm==0) {cout<<name<<" ynorm= 0 ..."<<endl; return 0;}
  g2->SetPoint(i,x,y/ynorm);
  Double_t ey= g1->GetErrorY(i);
  g2->SetPointError(i,ex,ey/ynorm);
 }

 if (debug) g2->Print();
 return g2;
}


TGraphErrors* myTGraphMinusOffset(TGraphErrors* g1, Double_t offset) {
 // substract offset from each x-value
 // 
 const bool debug=false; 
 const char *name="**myTGraphMinusOffset: ";
 
 if (!g1) {cout << name<<" g1 does not exist ! " << endl; return 0; }

 Int_t n1=g1->GetN();
 
 TGraphErrors* g2= new TGraphErrors();

 if (!g2) cout<<name<<" problem to make new vector g2 ! " << endl;

 Double_t*  X1 = g1->GetX(); 
 Double_t*  Y1 = g1->GetY(); 
 Double_t* EX1 = g1->GetEX(); 
 Double_t* EY1 = g1->GetEY(); 
 for (Int_t i1=0; i1<n1; i1++) {  //loop over points of graph1
  g2->SetPoint     (i1, X1[i1]-offset, Y1[i1]);
  g2->SetPointError(i1,EX1[i1],EY1[i1]);
 }

 if (debug) {
  cout<<name<<" vector minus offset : "<<endl;
  g2->Print();
 }
 return g2;
}

TGraphErrors* myTGraphMinusYOffset(TGraphErrors* g1, Double_t offset) {
 // substract offset from each x-value
 // 
 const bool debug=false; 
 const char *name="**myTGraphMinusOffset: ";
 
 if (!g1) {cout << name<<" g1 does not exist ! " << endl; return 0; }

 Int_t n1=g1->GetN();
 
 TGraphErrors* g2= new TGraphErrors();

 if (!g2) cout<<name<<" problem to make new vector g2 ! " << endl;

 Double_t*  X1 = g1->GetX(); 
 Double_t*  Y1 = g1->GetY(); 
 Double_t* EX1 = g1->GetEX(); 
 Double_t* EY1 = g1->GetEY(); 
 for (Int_t i1=0; i1<n1; i1++) {  //loop over points of graph1
  g2->SetPoint     (i1, X1[i1], Y1[i1]-offset);
  g2->SetPointError(i1,EX1[i1],EY1[i1]);
 }

 if (debug) {
  cout<<name<<" vector minus offset : "<<endl;
  g2->Print();
 }
 return g2;
}


TGraphErrors* myTGraphScaleX(TGraphErrors* g1, Double_t scale) {
 // substract offset from each x-value
 // 
 const bool debug=false; 
 const char *name="**myTGraphScaleX: ";
 
 if (!g1) {cout << name<<" g1 does not exist ! " << endl; return 0; }

 Int_t n1=g1->GetN();
 
 TGraphErrors* g2= new TGraphErrors();

 if (!g2) cout<<name<<" problem to make new vector g2 ! " << endl;

 Double_t*  X1 = g1->GetX(); 
 Double_t*  Y1 = g1->GetY(); 
 Double_t* EX1 = g1->GetEX(); 
 Double_t* EY1 = g1->GetEY(); 
 for (Int_t i1=0; i1<n1; i1++) {  //loop over points of graph1
  g2->SetPoint     (i1, X1[i1]*scale, Y1[i1]);
  g2->SetPointError(i1,EX1[i1]*scale,EY1[i1]);
 }

 if (debug) {
  cout<<name<<" vector minus offset : "<<endl;
  g2->Print();
 }
 return g2;
}

TGraphErrors* myTGraphzero(TGraphErrors* g1, char actionname[]="x") {
  // name: x  zero x-component
  // name: y  zero y-component
  // name: ex zero x-errors
  // name: ey zero y-errors
 // 
 const bool debug=false; 
 const char *name="**myTGraphzero: ";
 
 if (!g1) {cout << name<<" g1 does not exist ! " << endl; return 0; }

 Int_t n1=g1->GetN();
 
 TGraphErrors* g2= new TGraphErrors();
 TString g2name=g1->GetName();
 g2name+="zero";
 g2->SetName(g2name);
 if (!g2) cout<<name<<" problem to make new vector g2 ! " << endl;

 Double_t*  X1 = g1->GetX(); 
 Double_t*  Y1 = g1->GetY(); 
 Double_t* EX1 = g1->GetEX(); 
 Double_t* EY1 = g1->GetEY(); 
 for (Int_t i1=0; i1<n1; i1++) {  //loop over points of graph1
  if (strcmp(actionname,"x") ==0)  X1[i1]=0.;
  if (strcmp(actionname,"ex")==0) EX1[i1]=0.;
  if (strcmp(actionname,"y") ==0)  Y1[i1]=0.;
  if (strcmp(actionname,"ey")==0) EY1[i1]=0.;
  g2->SetPoint     (i1, X1[i1], Y1[i1]);
  g2->SetPointError(i1,EX1[i1],EY1[i1] );
 }

 if (debug) {
  cout<<name<<" vector zero : "<<actionname<<endl;
  g2->Print();
 }
 return g2;
}
TGraphAsymmErrors* myTGraphCutY(TGraphAsymmErrors* g1,Double_t ymax) {
 // Remove bin from Graph
 const Int_t debug=false; 
 const char *name="**myTGraphCutY:";
 
 if (!g1) cout << name<<" g1 does not exist ! " << endl;

 Int_t n1=g1->GetN();

 TGraphAsymmErrors* g3= new TGraphAsymmErrors();
 if (!g3) cout<<name<<" problem to make new  TGraphAsymmErrors ! " << endl;


 TString newname=g1->GetName();
 newname="cut_"+newname;
 //newname+=ymax;
 g3->SetName(newname);
 g3->SetMarkerStyle(g1->GetMarkerStyle());
 g3->SetMarkerColor(g1->GetMarkerColor());
 g3->SetLineColor  (g1->GetLineColor());


 Double_t* X1 = g1->GetX();
 Double_t* Y1 = g1->GetY();
 Double_t* EXhigh1 = g1->GetEXhigh();
 Double_t* EXlow1 =  g1->GetEXlow();
 Double_t* EYhigh1 = g1->GetEYhigh();
 Double_t* EYlow1 =  g1->GetEYlow();



 for (Int_t i1=0; i1<n1; i1++) {  //loop over point of graph1

  Double_t g1y1hig=EYhigh1[i1];
  Double_t g1y1low=EYlow1[i1];
  Double_t g1y1   =Y1[i1];
  if (Y1[i1]+EYhigh1[i1]>ymax) g1y1hig=(ymax-Y1[i1]);
  if (Y1[i1]+EYlow1[i1] >ymax) g1y1low=0.;
  if (Y1[i1]>ymax) g1y1=ymax; 

  g3->SetPoint(i1, X1[i1],g1y1);
  g3->SetPointError(i1,EXlow1[i1],EXhigh1[i1],g1y1low,g1y1hig);
 }

 if (debug){
  cout<<name<<" g1 = "<<g1->GetName()<<endl;
  g1->Print();

  cout<<name<<" g3 = "<<g3->GetName()<<endl;
  g3->Print();
 }

 return g3;
}



TGraphAsymmErrors *myTGraphScaleY(TGraphAsymmErrors *g1, Double_t escale=1.){
 const char *name="**myTGraphScale:";
 const bool debug=false;
 if (!g1) {cout<<name<<" input Graph not found !"<<endl; exit (1);}

 TGraphAsymmErrors* g2= new TGraphAsymmErrors();
 g2->SetName(g1->GetName());
 g2->SetName(g1->GetName());

 Double_t* X1 = g1->GetX();
 Double_t* Y1 = g1->GetY();
 Double_t* EXhigh1 = g1->GetEXhigh();
 Double_t* EXlow1 =  g1->GetEXlow();
 Double_t* EYhigh1 = g1->GetEYhigh();
 Double_t* EYlow1 =  g1->GetEYlow();

 for (Int_t i=0; i<g1->GetN(); i++) {
 
  double x=X1[i], y=Y1[i]*escale;
  double exl=EXlow1[i];
  double exh=EXhigh1[i];
  double eyl=EYlow1[i]*escale;
  double eyh=EYhigh1[i]*escale;
  g2->SetPoint(i, x, y);
  g2->SetPointError(i,exl,exh,eyl,eyh);
 }

 if (debug) g2->Print();
 return g2;
}

TGraphErrors* myTGraphEmptyRemoveBins(TGraphErrors* g1) {
 // Remove empty bins from Graph
  // const Int_t debug=false; 
 const char *name="**myTGraphEmptyRemoveBins: ";
 
 if (!g1) cout << name<<" g1 does not exist ! " << endl;

 Int_t n1=g1->GetN();

 TGraphErrors* g3= new TGraphErrors();
 if (!g3) cout<<name<<" problem to make new vector ! " << endl;

 CopyGraphProperties(g1,g3); 

 Double_t* X1 = g1->GetX();
 Double_t* Y1 = g1->GetY();
 Double_t* EX = g1->GetEX();
 Double_t* EY = g1->GetEY();

 Int_t iv=0;
 for (Int_t i1=0; i1<n1; i1++) {  //loop over point of graph1
   // cout<<" y= "<<Y1[i1]<<" ey= "<<EY[i1]<<endl;
  if (Y1[i1]!=0 && EY[i1]!=0){
    //cout<<iv<<" include "<<endl;
   g3->SetPoint(iv, X1[i1],Y1[i1]);
   g3->SetPointError(iv,EX[i1],EY[i1]);
   iv++;
  }
 }

 //g3->Print();

 return g3;
}


TGraphAsymmErrors* myTGraphAsymmErrorsMerge(TGraphAsymmErrors* g1, TGraphAsymmErrors* g2) {
 // Merge bins in g1 to bins given in g2
 // g1 must have finer binning than g2

 const bool debug=false; 
 const char *name="**myTGraphAsymmErrorsMerge: ";
 
 if (!g1) {cout << name<<" g1 does not exist ! " << endl; return 0; }
 if (!g2) {cout << name<<" g2 does not exist ! " << endl; return 0; }

 Int_t n1=g1->GetN();
 Int_t n2=g2->GetN();
 
 if (n2>n1) {cout<<name<<" can not merge n1= "<<n1<<" n2= "<<n2<<endl; return 0;}


 TGraphAsymmErrors* g3= new TGraphAsymmErrors();
 if (!g3) cout<<name<<" problem to make new vector g3 ! " << endl;
 TGraphAsymmErrors* g4= new TGraphAsymmErrors();
 if (!g4) cout<<name<<" problem to make new vector g4 ! " << endl;
 //char newname[500]; 
 //sprintf(newname,"%sMergedBinsFrom%s",g1->GetName(),g2->GetName());
 //g3->SetName(newname);
//added first two line (YD)
 //Double_t* X1 = g1->GetX();
 //Double_t* Y1 = g1->GetY();
 //Double_t* EXhigh1 = g1->GetEXhigh();
 //Double_t* EXlow1 =  g1->GetEXlow();
 //Double_t* EYhigh1 = g1->GetEYhigh();
 //Double_t* EYlow1 =  g1->GetEYlow();

 Double_t* X2 = g2->GetX();
 Double_t* Y2 = g2->GetY();
 Double_t* EXhigh2 = g2->GetEXhigh();
 Double_t* EXlow2 =  g2->GetEXlow();
 //Double_t* EYhigh2 = g2->GetEYhigh();
// Double_t* EYlow2 =  g2->GetEYlow();
 //change end

 //original
 Double_t x=0., dx=0.;
 // Double_t gysum, gy, dgy, gx, dgx;
 Double_t gysum, gy, gx, dgx;
 Double_t xmin=0, xmax=0;
 const Int_t Nslice=10;

 //Double_t*  X1 = g1->GetX();
 //Double_t*  Y1 = g1->GetY(); YD

 Double_t BIG=1.e20;
 Double_t x1minp=BIG, x1maxp=-BIG, x1minm=BIG, x1maxm=-BIG;
 
 for (Int_t i2=0; i2<n2; i2++) {  //loop over points of graph2
  //change EX to low or high
  Double_t xi=X2[i2]-EXlow2[i2], xj=X2[i2]+EXhigh2[i2]; 
  if (debug) cout<<name<<i2<<" Xi[i]= "<<xi<<" X2[i+1]= "<< xj <<" Y2= "<<Y2[i2]<<endl;

  if   (xi>xj){xmin=xj; xmax=xi; }
  else        {xmin=xi; xmax=xj; }
  dgx=(xmax-xmin)/Nslice;
  gysum=0.;
  
  //if (debug) cout<<name<<i<<" dgx= "<<dgx<<endl;
  Int_t iflag=0;
  for (Int_t i=0; i<Nslice; i++) {  //loop over points of graph2
   gx=xmin+dgx/2.+i*dgx;
   //gy=g4->Eval(gx);
   TSpline *spline=0;
   gy=g4->Eval(gx,spline,"S");
   // if vector does not cover full range of bin, set to zero
   // no extrapolation
   if (gx>0) {if (gx<=x1minp||gx>x1maxp) iflag=1;}
   else      {if (gx<=x1minm||gx>x1maxm) iflag=1;}
   if (iflag==1) {gy=0.; gysum=0.;}

   gysum+=gy; 
   if (debug) cout<<name<<i<<" iflag= "<<iflag<<" gx= "<<gx<<" gysum= "<<gysum<<" gy= "<<gy<<endl;
  }

  gysum/=Nslice;
  x= X2[i2]; double dxlow=(x-xmin); double dxhigh=(xmax-x);

  if (debug) cout<<name<<i2<<" fill: x= "<<x<<" dx= "<<dx<<" gy="<<gysum<<" gy/dx= "<<gysum/dx<<endl;
  g3->SetPoint     (i2, x  , gysum);
  g3->SetPointError(i2,dxlow,dxhigh,0.,0.);
 }
 delete g4;
 return g3;
}

#endif
