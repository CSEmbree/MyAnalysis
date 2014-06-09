/*
 */

#include "MyBand.h"


/******************************************************************
 ** Method Implementations
 ******************************************************************/
MyBand::MyBand()
{
 //
 // constructor to make Band
 //
 debug=true;
 plotband=true;
 plotmarker=false;

 gpdfbandratio.clear();
 gpdfband.clear(); 
 gpdfdefault.clear();;
 gscaleband.clear();

  yminratio=0.;
  ymaxratio=0.;

}

void MyBand::DrawPDFBandRatio() 
{

 if (debug) cout<<" MyBand::DrawPDFBandRatio: npdf= "<<gpdfbandratio.size()<<endl;

 for (int ipdf = 0; ipdf <  gpdfbandratio.size(); ipdf++) {
  if (debug) cout<<" MyBand::DrawPDFBandRatio: ipdf= "<<ipdf<<endl;
  if (!gpdfbandratio.at(ipdf)) {
   cout<<" MyBand::ComputePDFBandRange: gpdfbandratio not found ! "<<endl;
   return;
  }

  if (debug) { 
   cout<<" MyBand::ComputePDFBandRange: gpdfbandratio["<<ipdf<<"]: "<<endl;
   gpdfbandratio.at(ipdf)->Print("");
  }

  if (plotmarker) gpdfbandratio.at(ipdf)->Draw("P1 same");
  else            gpdfbandratio.at(ipdf)->Draw("E2 same");

  if (!gpdfdefaultratio.at(ipdf)) {
   cout<<" MyBand::ComputePDFBandRange: gpdfdefaultratio not found ! "<<endl;
   return;
  }

  if (debug) { 
   cout<<" MyBand::ComputePDFBandRange: gpdfdefaultratio["<<ipdf<<"]: "<<endl;
   gpdfdefaultratio.at(ipdf)->Print("");
  }

  gpdfdefaultratio.at(ipdf)->SetMarkerSize(0);
  gpdfdefaultratio.at(ipdf)->Draw("P1 same");
 }

}

void MyBand::DrawPDFBand() 
{
 if (debug) cout <<" MyBand::DrawPDFBand: gpdfband.size()= " << gpdfband.size()<<endl;

 for (int ipdf = 0; ipdf <  gpdfband.size(); ipdf++) {

  if (debug) {
   cout<<" MyBand::DrawPDFBand: print gpdfband: ipdf= "<<ipdf<<endl;
   gpdfband.at(ipdf)->Print();
  }

  if (plotband) gpdfband.at(ipdf)->Draw("E2 same");   
  if (plotmarker) {
   gpdfband.at(ipdf)->Draw("P0 same");   
  }
  if (debug) cout<<" MyBand::DrawPDFBand: gpdfdefault ipdf= "<<ipdf<<endl;
  gpdfdefault.at(ipdf)->SetMarkerStyle(0);
  gpdfdefault.at(ipdf)->SetMarkerSize(0.);

  if (debug) {
   cout<<" MyBand::DrawPDFBand: print gpdfdefault: ipdf= "<<ipdf<<endl;
   gpdfdefault.at(ipdf)->Print();
  }

  gpdfdefault.at(ipdf)->Draw("P1,same");

 }

 if (debug) cout << " MyPDFBand::DrawPDFBand finished " <<endl;
}

void MyBand::MovePDFPoints(){

 if (debug) cout<<" MyBand::MovePDFPoints: npdf= "<<gpdfband.size()<<endl;

 for (int ipdf = 0; ipdf <gpdfband.size(); ipdf++) {
   if (debug) cout<<" MyBand::MovePDFPoints: ipdf= "<<ipdf<<endl;
   if (!gpdfband.at(ipdf)) {
    cout<<" MyBand::MovePDFPoints: gpdfband not found ! "<<endl;
   }

   for (int i=0; i<gpdfband.at(ipdf)->GetN(); i++){
    double xval, xval1, xval2, yval, yval2;
    gpdfband.at(ipdf)->GetPoint(i  , xval , yval);
    gpdfband.at(ipdf)->GetPoint(i+1, xval1, yval2);

    double binw=15.;
    xval2=xval+pow(-1,ipdf)*binw/gpdfband.size();
    gpdfband.at(ipdf)->SetPoint(i, xval2, yval);
   }
   //}
 }
} 

void MyBand::ComputePDFBandRatio(TGraphAsymmErrors *gref) 
{

 if (debug) {
  cout <<" MyBand::ComputePDFBandRatio: gpdfband.size()= " << gpdfband.size()<<endl;
  cout <<" MyBand::ComputePDFBandRatio: gref name: " << gref->GetName()<<endl;
 }

 for (int ipdf = 0; ipdf <  gpdfband.size(); ipdf++) {
   if (!gpdfband.at(ipdf)) cout<<" MyBand::ComputePDFBandRatio: gpdfband not found ipdf= "<<ipdf<<endl;

   TGraphAsymmErrors *gratio=myTGraphErrorsDivide(gpdfband.at(ipdf),gref,2);
   TString rationame=gpdfband.at(ipdf)->GetName();
   rationame+="/";
   rationame+=gref->GetName();
   if (debug) cout<<" MyBand::ComputePDFBandRatio: rationame= "<<rationame.Data()<<endl;
   gratio->SetName(rationame);
   gratio->SetMarkerStyle(gpdfband.at(ipdf)->GetMarkerStyle());
   gratio->SetMarkerColor(gpdfband.at(ipdf)->GetMarkerColor());
   gratio->SetLineColor(gpdfband.at(ipdf)->GetLineColor());
   gratio->SetLineStyle(gpdfband.at(ipdf)->GetLineStyle());
   gratio->SetFillStyle(gpdfband.at(ipdf)->GetFillStyle());
   gratio->SetFillColor(gpdfband.at(ipdf)->GetFillColor());

   gpdfbandratio.push_back(gratio);

   if (debug) {
    cout<<" MyBand::ComputePDFBandRatio: gratio: "<<endl;
    gratio->Print();
   }

   TGraphAsymmErrors *gratiodefault=myTGraphErrorsDivide(gpdfdefault.at(ipdf),gref,0);
   TString rationamedefault=gpdfdefault.at(ipdf)->GetName();
   rationame+="/";
   rationame+=gref->GetName();
   if (debug) cout<<" MyBand::ComputePDFBandRatio: rationamedefault= "<<rationamedefault.Data()<<endl;
   gratiodefault->SetName(rationamedefault);
   gratiodefault->SetMarkerStyle(gpdfband.at(ipdf)->GetMarkerStyle());
   gratiodefault->SetMarkerColor(gpdfband.at(ipdf)->GetMarkerColor());
   gratiodefault->SetLineColor(gpdfband.at(ipdf)->GetLineColor());
   gratiodefault->SetLineStyle(gpdfband.at(ipdf)->GetLineStyle());
   gratiodefault->SetFillColor(gpdfband.at(ipdf)->GetFillColor());

   if (debug) {
    cout<<" MyBand::ComputePDFBandRatio: gratiodefault: "<<endl;
    gratiodefault->Print();
   }

   gpdfdefaultratio.push_back(gratiodefault);

 }

 if (debug) cout << " MyBand::ComputePDFBandRatio finished " <<endl;
}


void MyBand::ComputePDFBandRatioRange(){

 double xmin=0.; double xmax=0.; 
 double ymin=0.; double ymax=0.;

 const double BIG=1.e36;
 double Ymin=BIG, Ymax=-BIG, Xmin=BIG, Xmax=-BIG;

 if (debug) cout<<" MyBand::ComputePDFBandRatioRange:number of pdfs= "<<gpdfbandratio.size()<<endl;

 for (int ipdf = 0; ipdf <  gpdfbandratio.size(); ipdf++) {
  if (!gpdfbandratio.at(ipdf)) {
   cout<<" MyBand::ComputePDFBandRatioRange: gpdfbandratio not found ! "<<endl;
   return;
  }
  gpdfbandratio.at(ipdf)->ComputeRange(xmin,ymin,xmax,ymax);
  if (ymin<Ymin) Ymin=ymin;
  if (ymax>Ymax) Ymax=ymax;
 }

 yminratio=Ymin;
 ymaxratio=Ymax;

 if (debug) {
  cout<<" MyBand::ComputePDFBandRatioRange: ymin= "<<yminratio<<" ymax= "<<ymaxratio<<endl;
 }
 return;
};  

TGraphAsymmErrors* MyBand::myTGraphErrorsDivide(TGraphAsymmErrors* g1,TGraphAsymmErrors* g2, Int_t noerr) {
// Divide two TGraphAsymmErrors: new=g1/g2
//
// noerr=0: put all errors to zero
//       1: add errors from two graph quadrically
//       2: set errors from graph 2 to zero
//       3: set errors from graph 1 to zero

  //const bool debug=false;
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

    Double_t* EXhigh1 = g1->GetEXhigh();
    Double_t* EXlow1 =  g1->GetEXlow();
    Double_t* EYhigh1 = g1->GetEYhigh();
    Double_t* EYlow1 =  g1->GetEYlow();

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
                Double_t el=0.;
                Double_t eh=0.;

                if (noerr==2) {
                    dy2l=0.;
                    dy2h=0.;
                }

                if (noerr==3) {
                    dy1l=0.;
                    dy1h=0.;
                }

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
        //if (matchcount>1) {cout<<name<<" too many x-points matched ! "<<endl; exit (1);}
        if (matchcount>1) {
            cout<<name<<" too many x-points matched ! "<<endl;
        }
    }
    return g3;
}
