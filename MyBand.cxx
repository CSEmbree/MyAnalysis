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
 plotband=false;
 plotmarker=false;
 ploterrorticks=false;
 plotstaggered=false;

 gpdfbandratio.clear();
 gpdfband.clear(); 
 gpdfdefault.clear();;
 gscaleband.clear();
 ratioitself = false;

 yminratio = 0.0;
 ymaxratio = 0.0;
 
 scalex = 1.0;
 scaley = 1.0;
}


void MyBand::DrawPDFBandRatio() 
{
 if (debug) cout <<" MyBand::DrawPDFBandRatio: gpdfbandratio.size()= " << gpdfbandratio.size()
		 <<", plotband="<<plotband
		 <<", plotmarker="<<plotmarker<<endl;


 // Visually prepare plots
 // stagger horizontal position of pdf points for readability
 if (plotstaggered)  {
   if (debug) cout<<" MyBand::DrawPDFBandRatio: Staggering points."<<endl;
   MovePDFPoints();
 }


 //if data was artificially scaled, then convolute  must be equally scaled
 //if(scalex != 1.0 || scaley != 1.0 ) {
 //  this->ScaleOverlayGraphs(scalex, scaley);
 //}
 


 // plot each pdf info onto ratio
 for (int ipdf = 0; ipdf <  gpdfbandratio.size(); ipdf++) {
   if (debug) cout<<" MyBand::DrawPDFBandRatio: ipdf= "<<ipdf<<endl;

   
   //Part 1 of 2) draw default lines in case no draw parameters like 'plotmarker' are set 
   // ensure default ratio data is available
   if (!gpdfdefaultratio.at(ipdf)) {
     cout<<" MyBand::DrawPDFBandandRatio: WARNING: gpdfdefaultratio not found ! "<<endl;
     return;
   }
   
   if (debug) { 
     cout<<" MyBand::DrawPDFBandandRatio: gpdfdefaultratio["<<ipdf<<"] Data: "<<endl;
     gpdfdefaultratio.at(ipdf)->Print("");
   }
   
   // always print a default error for each PDF
   // colors and settings set in 'ComputePDFBandRatio(TGraphAsymmErrors *gref) 
   gpdfdefaultratio.at(ipdf)->SetMarkerSize(0);
   // allow small perpednicular lines at end of error bar to be on or off on default
   if (ploterrorticks) gpdfdefaultratio.at(ipdf)->Draw("P,same");
   else                gpdfdefaultratio.at(ipdf)->Draw("PZ,same");   
   
   
   if(ratioitself == false) { //(CONVOLUTE/DATA)  

     //Part 2 of 2) draw additional data lines based on steering parameters
     // ensure sure band ratio data was provided
     if (!gpdfbandratio.at(ipdf)) {
       cout<<" MyBand::DrawPDFBandandRatio: WARNING: gpdfbandratio not found ! "<<endl;
       return;
     }
     
     if (debug) { 
       cout<<" MyBand::DrawPDFBandandRatio: gpdfbandratio["<<ipdf<<"] Data: "<<endl;
       gpdfbandratio.at(ipdf)->Print("");
     }
    

     //for(int i=0; i<gpdfbandratio.at(ipdf)->GetN(); i++) {
     //  gpdfbandratio.at(ipdf)->SetPointEXlow (i, 0.0);//TEST
     //  gpdfbandratio.at(ipdf)->SetPointEXhigh(i, 0.0);//TEST
     //}

     // plot data based on style's requested in steering
     TString bandratiostyle = GetBandRatioStyle(); // determine style
     gpdfbandratio.at(ipdf)->Draw(bandratiostyle); // plot based on style
   } else { //(DATA/CONVOLUTE)
     
     gpdfbandratio.at(ipdf)->Draw("E2"); //P2
   }
 }

 return;
}


void MyBand::DrawPDFBand() 
{
  if (debug) cout <<" MyBand::DrawPDFBand: gpdfband.size()= " << gpdfband.size()
		  <<", plotband="<<plotband
		  <<", plotmarker="<<plotmarker<<endl;
  

  // Visually prepare plots
  // optionally stagger horizontal position of pdf points for readability
  if (plotstaggered) {
    if (debug) cout<<" MyBand::DrawPDFBand: Staggering points."<<endl;
    MovePDFPoints();
    //SetXErrorsZero();
  }

  //if data was artificially scaled, then convolute must be equally scaled
  //if(scalex != 1.0 || scaley != 1.0 ) {
  //  this->ScaleRatioGraphs(scalex, scaley);
  //}
  //  if(scalex != 1.0 || scaley != 1.0 ) {
  //  this->ScaleOverlayGraphs(scalex, scaley);
  //}

  
  // plot PDF info for overlay
  for (int ipdf = 0; ipdf <  gpdfband.size(); ipdf++) {
    if (debug) cout<<" MyBand::DrawPDFBand: print gpdfband: ipdf= "<<ipdf<<endl;
    
    //Part 1 of 2) draw default lines in case no draw parameters like 'plotmarker' are set 
    // ensure default ratio data is available
    if (!gpdfdefault.at(ipdf)) {
      cout<<" MyBand::DrawPDFBand: WARNING: gpdfdefault not found ! "<<endl;
      return;
    }
    
    if (debug) { 
      cout<<" MyBand::DrawPDFBand: gpdfdefault["<<ipdf<<"] Data: "<<endl;
      gpdfdefault.at(ipdf)->Print("");
    }
    
    // always draw default plot info in case optional steering settings are not set
    // set default color to be displayed
    gpdfdefault.at(ipdf)->SetMarkerSize(0);
    gpdfdefault.at(ipdf)->SetMarkerStyle(gpdfband.at(ipdf)->GetMarkerStyle());
    gpdfdefault.at(ipdf)->SetMarkerColor(gpdfband.at(ipdf)->GetMarkerColor());
    gpdfdefault.at(ipdf)->SetLineColor(gpdfband.at(ipdf)->GetLineColor());
    gpdfdefault.at(ipdf)->SetLineStyle(gpdfband.at(ipdf)->GetLineStyle());
    gpdfdefault.at(ipdf)->SetFillColor(gpdfband.at(ipdf)->GetFillColor());
    

 
    // allow small perpendicular lines at end of error bar to be on or off on default
    //    if (!plotmarker && !plotband) {
    //	if(ploterrorticks) gpdfdefault.at(ipdf)->Draw("P,same");  //no band or marker, draw line for data
    //	else               gpdfdefault.at(ipdf)->Draw("PZ,same"); //same but with NO error ricks      
    //}

    if(!plotmarker) {
      if(!plotband) {
	if(ploterrorticks) gpdfdefault.at(ipdf)->Draw("P,same");  //no band or marker, draw line for data
	else               gpdfdefault.at(ipdf)->Draw("PZ,same"); //same but with NO error ricks
      }
      //there is a band, do not display any marker or line for data
    } 

       

    // Part 2 of 2) draw additional data lines based on extra parameters from steering
    // ensure sure band ratio data was provided
    if (!gpdfband.at(ipdf)) {
      cout<<" MyBand::DrawPDFBand: WARNING: gpdfband not found ! "<<endl;
      return;
    }
    
    if (debug) { 
      cout<<" MyBand::DrawPDFBand: gpdfband["<<ipdf<<"] Data: "<<endl;
      gpdfband.at(ipdf)->Print("");
    }

    
    // plot data based on style's requested in steering
    TString bandstyle = GetBandStyle(); // determine style
    gpdfband.at(ipdf)->Draw(bandstyle); // plot based on style
  }
  
  if (debug) cout << " MyPDFBand::DrawPDFBand finished " <<endl;
  return;
}

/*
// Shift horizontal position of PDF data points for readability
void MyBand::MovePDFPoints(){

 if (debug) cout<<" MyBand::MovePDFPoints: npdf= "<<gpdfband.size()<<endl;

 // stagger PDF points, making them more visible
 for (int ipdf = 0; ipdf <gpdfband.size(); ipdf++) {
   if (debug) cout<<" MyBand::MovePDFPoints: ipdf= "<<ipdf<<endl;
   if (!gpdfband.at(ipdf)) {
    cout<<" MyBand::MovePDFPoints: gpdfband not found ! "<<endl;
   }

   // stagger each individual point for each pdf on overlay
   for (int i=0; i<gpdfband.at(ipdf)->GetN(); i++){
    double xval, xval1, xval2, yval, yval2;
    gpdfband.at(ipdf)->GetPoint(i  , xval , yval);
    gpdfband.at(ipdf)->GetPoint(i+1, xval1, yval2);

    double binw=15.;
    xval2=xval+pow(-1,ipdf)*binw/gpdfband.size();
    gpdfband.at(ipdf)->SetPoint(i, xval2, yval);
   }

   // stagger each individual point for each pdf on ratio
   for (int i=0; i<gpdfbandratio.at(ipdf)->GetN(); i++){
    double xval, xval1, xval2, yval, yval2;
    gpdfbandratio.at(ipdf)->GetPoint(i  , xval , yval);
    gpdfbandratio.at(ipdf)->GetPoint(i+1, xval1, yval2);

    double binw=15.;
    xval2=xval+pow(-1,ipdf)*binw/gpdfband.size();
    gpdfbandratio.at(ipdf)->SetPoint(i, xval2, yval);
   }
 }

 if (debug) cout<<" MyBand::MovePDFPoints: Finished moving points."<<endl;
 return;
} 
*/



// Shift horizontal position of PDF data points for readability
void MyBand::MovePDFPoints(){

 if (debug) cout<<" MyBand::MovePDFPoints: npdf= "<<gpdfband.size()<<endl;
 int fracRange = 4; //change here to alter the fraction of the error range in which stagger a point

 // stagger PDF points, making them more visible
 for (int ipdf = 0; ipdf <gpdfband.size(); ipdf++) {
   cout<<" MyBand::MovePDFPoints: ipdf= "<<ipdf<<endl;
   if (!gpdfband.at(ipdf)) {
    cout<<" MyBand::MovePDFPoints: gpdfband not found ! "<<endl;
   }

   // stagger each individual point for each pdf on overlay
   for (int i=0; i<gpdfband.at(ipdf)->GetN(); i++){
     double x=0, y=0, range=0, error=0, xh=0, xl=0, dr=0, newx=0;
     gpdfband.at(ipdf)->GetPoint(i, x, y);
          
     error = gpdfband.at(ipdf)->GetErrorX( i ); //total error for this point
     range = error / fracRange;                 //the distance points will be staggered across
          
     dr = range / gpdfband.at(ipdf)->GetN();    //DeltaR is dist to stagger based on num of pdfs
     newx = x + pow(-1,(ipdf+1))*(dr*(ipdf+1));

     gpdfband.at(ipdf)->SetPoint(i, newx, y);

     cout<<"TEST-overl: i="<<i<<", error="<<error<<", range="<<range<<", dr="<<dr<<", newx="<<newx<<endl;
   }


   for (int i=0; i<gpdfbandratio.at(ipdf)->GetN(); i++){
     double x=0, y=0, range=0, error=0, xh=0, xl=0, dr=0, newx=0;
     gpdfbandratio.at(ipdf)->GetPoint(i, x, y);
          
     error = gpdfbandratio.at(ipdf)->GetErrorX( i ); //total error for this point
     range = error / fracRange;                      //the distance points will be staggered across
          
     dr = range / gpdfbandratio.at(ipdf)->GetN();    //DeltaR is dist to stagger based on num of pdfs
     newx = x + pow(-1,ipdf)*(dr*(ipdf+1));

     gpdfbandratio.at(ipdf)->SetPoint(i, newx, y);
     cout<<"TEST-ratio: i="<<i<<", error="<<error<<", range="<<range<<", dr="<<dr<<", newx="<<newx<<endl;
   }  
 }

 //this could be done better
 SetXErrorsZero(); // setting X-errors to zero, not as meaningful when staggered?
 
 if (debug) cout<<" MyBand::MovePDFPoints: Finished moving points."<<endl;
 return;
} 



// Shift horizontal position of PDF data points for readability
void MyBand::SetXErrorsZero() {

 if (debug) cout<<" MyBand::SetXErrorsZero: Zeroing X errors..."<<endl;

 // stagger PDF points, making them more visible
 for (int ipdf = 0; ipdf <gpdfband.size(); ipdf++) {
   cout<<" MyBand::SetXErrorsZero: ipdf= "<<ipdf<<endl;
   if (!gpdfband.at(ipdf)) {
    cout<<" MyBand::SetXErrorsZero: gpdfband not found ! "<<endl;
   }

   for (int i=0; i<gpdfband.at(ipdf)->GetN(); i++){
     gpdfband.at(ipdf)->SetPointEXlow (i, 0.0);
     gpdfband.at(ipdf)->SetPointEXhigh(i, 0.0);
   }
   
   for (int i=0; i<gpdfbandratio.at(ipdf)->GetN(); i++){
     gpdfbandratio.at(ipdf)->SetPointEXlow (i, 0.0);
     gpdfbandratio.at(ipdf)->SetPointEXhigh(i, 0.0);
   }
 }

 if (debug) cout<<" MyBand::MovePDFPoints: Finished zero-ing X-errors."<<endl;
 return;
} 


TString MyBand::GetBandStyle() {
    
   if (debug) cout<<" MyBand::GetBandStyle: Creating band style." <<endl;
   
   TString bandstyle = "";
    if ( plotmarker == true )      bandstyle+="P";
    if ( ploterrorticks == false ) bandstyle+="Z";
    if ( plotband == true )        bandstyle+="E2";    
    bandstyle+=",same";
    

    if (debug) cout<<" MyBand::GetBandStyle: Band style is '"<<bandstyle<<"'." <<endl;

    return bandstyle;
}


TString MyBand::GetBandRatioStyle() {
    
   if (debug) cout<<" MyBand::GetBandRatioStyle: Creating band style." <<endl;
   
   TString bandstyle = "";
    if ( plotmarker == true )      bandstyle+="P";
    if ( ploterrorticks == false ) bandstyle+="Z";
    if ( plotband == true )        bandstyle+="E2"; //TEST - E2 for band, E1 for points     
    bandstyle+=",same";
    

    if (debug) cout<<" MyBand::GetBandRatioStyle: Band style is '"<<bandstyle<<"'." <<endl;

    return bandstyle;
}


void MyBand::ComputePDFBandRatio(TGraphAsymmErrors *gref) 
{
 
  if(!gref) {
    cout<<" MyBand::ComputePDFBandRatio: WARN: No ratio graph provided!"
	<< "\n\t Will perform (pdf/pdf) for ratio.";
    ratioitself = true;
  } else {
    if (debug) {
      cout <<" MyBand::ComputePDFBandRatio: gpdfband.size()= " << gpdfband.size()<<endl;
      cout <<" MyBand::ComputePDFBandRatio: gref name: " << gref->GetName()<<endl;
    }
  }
  
  //if data was artificially scaled, then scale convolute equally before visual ranges can be determined
  if(scalex != 1.0 || scaley != 1.0 ) {
    this->ScaleOverlayGraphs(scalex, scaley);
  }
 

 // compute PDF band ratio for each PDF
 for (int ipdf = 0; ipdf <  gpdfband.size(); ipdf++) {
   if (!gpdfband.at(ipdf)) cout<<" MyBand::ComputePDFBandRatio: gpdfband not found ipdf= "<<ipdf<<endl;

   // set ratio data visual settings for printing later
   //TGraphAsymmErrors *gratio=myTGraphErrorsDivide(gpdfband.at(ipdf),gref,2); //origonal
   TGraphAsymmErrors *g1, *g2;

   //set visuals for the ratio
   g1 = gpdfband.at(ipdf);
   if(ratioitself) { 
     g2 = gpdfband.at(ipdf);
     ratioitself = true;
   } else {
     g2 = gref;
   }

   TGraphAsymmErrors *gratio=myTGraphErrorsDivide(g1,g2,2);
   TString rationame=g1->GetName();
   rationame+="/";
   rationame+=g2->GetName();
   if (debug) cout<<" MyBand::ComputePDFBandRatio: rationame= "<<rationame.Data()<<endl;
   gratio->SetName(rationame);
   gratio->SetMarkerStyle(g1->GetMarkerStyle());
   gratio->SetMarkerColor(g1->GetMarkerColor());
   gratio->SetLineColor(g1->GetLineColor());
   gratio->SetLineStyle(g1->GetLineStyle());
   gratio->SetFillStyle(g1->GetFillStyle());
   gratio->SetFillColor(g1->GetFillColor());

   gpdfbandratio.push_back(gratio);



   if (debug) {
    cout<<" MyBand::ComputePDFBandRatio: gratio: "<<endl;
    gratio->Print();
   }



   // set defaul tratio visual settings for printing later
   //TGraphAsymmErrors *gratiodefault=myTGraphErrorsDivide(gpdfdefault.at(ipdf),gref,0); //origonal
   TGraphAsymmErrors *gratiodefault=myTGraphErrorsDivide(gpdfdefault.at(ipdf),gpdfdefault.at(ipdf),0);
   TString rationamedefault=gpdfdefault.at(ipdf)->GetName();
   rationame+="/";
   rationame+=g2->GetName();
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
 return;
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
 
  cout<<"TEST: Ratio after pdf("<<ipdf<<"): Ymax="<<Ymax<<", Ymin="<<Ymin<<endl;
 }

 yminratio=Ymin;
 ymaxratio=Ymax;

 if (debug) cout<<" MyBand::ComputePDFBandRatioRange: ymin= "<<yminratio<<" ymax= "<<ymaxratio<<endl;

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

		// 2: set errors from graph 2 to zero
                if (noerr==2) {
                    dy2l=0.;
                    dy2h=0.;
                }

		// 3: set errors from graph 1 to zero
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



void MyBand::Scale(TGraphAsymmErrors *g1, double scalex, double scaley) {
  
  Double_t* X1 = g1->GetX();
  Double_t* Y1 = g1->GetY();
  Double_t* EXhigh1 = g1->GetEXhigh();
  Double_t* EXlow1  = g1->GetEXlow();
  Double_t* EYhigh1 = g1->GetEYhigh();
  Double_t* EYlow1  = g1->GetEYlow();
  
  for (Int_t i=0; i<g1->GetN(); i++) {
    g1->SetPoint(i, X1[i]*scalex, Y1[i]*scaley);
    g1->SetPointError(i, EXlow1[i]*scalex, EXhigh1[i]*scalex,
                      EYlow1[i]*scaley, EYhigh1[i]*scaley);
  }

  return;
};


void MyBand::ScaleOverlayGraphs(double scalex, double scaley) {
  
  for (int ipdf = 0; ipdf <  gpdfband.size(); ipdf++) {
    this->Scale(gpdfband.at(ipdf),    scalex, scaley);
    this->Scale(gpdfdefault.at(ipdf), scalex, scaley);
  }

  return;
}

void MyBand::ScaleRatioGraphs(double scalex, double scaley) {

  for (int ipdf = 0; ipdf <  gpdfband.size(); ipdf++) {
    this->Scale(gpdfdefaultratio.at(ipdf), scalex, scaley);
    this->Scale(gpdfbandratio.at(ipdf), scalex, scaley);
  }  

  return;
}
