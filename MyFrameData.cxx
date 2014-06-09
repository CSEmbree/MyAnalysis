#include "MyFrameData.h"
#include <iostream>
using namespace std;
MyFrameData::MyFrameData(Int_t p3, Int_t p4, MyData *gdata) : MyFrame(p3,p4) 
{
 
 debug=false;
 //debug = true;
  if (debug) cout<<" MyFrameData starting "<<endl;
  if (!gdata) {
   cout<<" MyFrameData gdata not found ! "<<endl;
   return;
  } else if (debug) cout<<" MyFrameData gdata found ! "<<endl;

  std::cout << " MyFrameData create myframe" << std::endl;
  myframe= new MyFrame(p3,p3);
  if (!myframe) cout<<" MyFrameData myframe not created ! "<<endl;
  else if (debug) cout<<" MyFrameData myframe found ! "<<endl;

  std::cout << " MyFrameData Created" << std::endl;

  Double_t scalex=1.1;
  Double_t scaley=1.1;
  if (gdata->GetLogX ()) scalex=1.5;
  if (gdata->GetLogY1()) scaley=1.5;

  Double_t ymin=gdata->GetMinY()/scaley;
  Double_t ymax=gdata->GetMaxY()*scaley;
  Double_t xmin=gdata->GetMinX();
  if   (xmin>0) xmin/=scalex;
  else xmin*=scalex;
  Double_t xmax=gdata->GetMaxX()*scalex;

  if (debug) {
   cout<<"MyFrameData ymin= "<<ymin<<" ymax= "<<ymax<<endl;
   cout<<"MyFrameData xmin= "<<xmin<<" xmax= "<<xmax<<endl;
  }

  if (gdata->GetLogY1()) myframe->SetLogY1();
  if (gdata->GetLogX ()) myframe->SetLogX();

  myframe->SetFrameName(gdata->GetFileName());
  myframe->SetSubPad2TitleX(gdata->GetTitleX());
  myframe->SetSubPad1TitleY(gdata->GetTitleY());

  //cout<<" xmin from gdata= "<<gdata->GetFrameXmin()<<endl;
  if (gdata->GetSetFrameXmin()) xmin=gdata->GetFrameXmin();

  //cout<<"MyFrameData xmin= "<<xmin<<" xmax= "<<xmax<<endl;
  myframe->SetXmin(xmin);
  myframe->SetXmax(xmax);
  myframe->SetYmin1(ymin);
  myframe->SetYmax1(ymax);

  if (debug) myframe->Print();


  //myframe->SetLogY1();
  //myframe->SetLogX();
  if (debug) cout<<" MyFrameData Draw now the frame "<<endl;
  myframe->DrawFrame();
  myframe->GetSubPad1()->cd();
  if (debug) cout<<" MyFrameData MyFrameData finished ! "<<endl;
  return;
}

void MyFrameData::SaveFile(TString file_name)
{
    //int status = mkdir(file_name);//, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

  myframe->SaveFile(file_name);
}

