
#include "MyFrame.h"
#include <iostream>
using namespace std;

/******************************************************************
 ** Method Implementations
 ******************************************************************/

MyFrame::MyFrame(Int_t p3, Int_t p4, bool drawpad1, bool drawpad2)
{
  debug=false;

  m_draw1 = drawpad1;
  m_draw2 = drawpad2;
  
  draw_canv = 0;

  m_framename="MyFrame";

  m_p1=0;  m_p2=0;  m_p3=p3;  m_p4=p4;

  m_sxmin=0.0; m_sxmax=1.0;
  m_symin=0.4; m_symax=1.0;
  m_s0=0.0;
  m_lm=0.15; m_rm=0.05; m_bm=0.25; m_small=0.001;
  
  m_logx=false;
  m_logy1=false;
  m_logy2=false;

  m_xmin=0.; m_xmax=1.; m_ymin=0.; m_ymax=1.; m_ymin2=0.; m_ymax2=1.;

  m_SubPad1TitleOffsetX=1.1;
  m_SubPad1TitleOffsetY=0.8;

  m_SubPad1TitleTextX="";
  m_SubPad1TitleTextY="";

  m_SubPad2TitleOffsetX=1.1;
  m_SubPad2TitleOffsetY=0.8;

  m_SubPad2TitleTextX="";
  m_SubPad2TitleTextY="";

  m_nDivX=-1;
  m_nDivY=-1;

}

void MyFrame::DrawFrame(TPad* p){
  //debug = true;
 
  //  char cname[100];
  //sprintf(cname,"frame%s",m_framename.Data());
  m_p1=0, m_p2=0;
  
  TString cname="frame";
  cname+=m_framename;
  if (debug) cout<<" MyFrame cname= "<<cname.Data()<<endl;

 // TCanvas* draw_canv =0;
  if (p==NULL)
   draw_canv = new TCanvas(cname.Data(),cname.Data(), m_p1, m_p2, m_p3, m_p4);
  else
   draw_canv=(TCanvas*)p;   

  draw_canv->SetLeftMargin(0.2);
  draw_canv->SetRightMargin(0.1);
  draw_canv->SetBottomMargin(0.15);
  draw_canv->cd(); 
  //char csubname[100]; sprintf(csubname,"%s_subpad1",cname);
  TString csubname1="subpad1"; 
  csubname1+=cname;
  if (debug) cout<<" MyFrame::DrawFrame:"
                 <<" m_sxmin= "<<m_sxmin
                 <<" m_symin= "<<m_symin
                 <<" m_sxmax= "<<m_sxmax
                 <<" m_symax= "<<m_symax<<endl;


  m_subpad1 = new TPad(csubname1.Data()," ",m_sxmin,m_symin,m_sxmax,m_symax);
  m_subpad1->SetFillColor(0);
  m_subpad1->SetLeftMargin(m_lm);
 //subpad1->SetLeftMargin(0.25);
  m_subpad1->SetRightMargin(m_rm);
  m_subpad1->SetBottomMargin(m_small);

  if (m_logx) m_subpad1->SetLogx();
  if (m_logy1) m_subpad1->SetLogy();
  m_subpad1->Draw(); 

  //sprintf(csubname,"%s_subpad1",cname);



  if (debug) cout<<" MyFrame::DrawFrame:"
                 <<" m_sxmin= "<<m_sxmin
                 <<" m_symin= "<<m_s0
                 <<" m_sxmax= "<<m_sxmax
                 <<" m_symax= "<<m_symin<<endl;


  TString csubname2="subpad1"; 
  csubname2+=cname;
  m_subpad2 = new TPad(csubname2.Data()," Ratio ",m_sxmin,m_s0,m_sxmax,m_symin);
  m_subpad2->SetFillColor(0);
  m_subpad2->SetLeftMargin(m_lm);
  m_subpad2->SetRightMargin(m_rm);
  m_subpad2->SetBottomMargin(m_bm);
  m_subpad2->SetTopMargin(m_small);
  
  if (m_logx) m_subpad2->SetLogx();
  if (m_logy2) m_subpad2->SetLogy();
  m_subpad2->Draw();


  Float_t lscaley=(m_symin-m_s0)/(m_symax-m_symin);
  m_subpad1->cd();


  TH1F *hframe1 = m_subpad1->DrawFrame(m_xmin,m_ymin,m_xmax,m_ymax);

  xaxis1 = hframe1->GetXaxis();
  yaxis1 = hframe1->GetYaxis();
  if (m_nDivY>-1) yaxis1->SetNdivisions(m_nDivY);
  xaxis1->SetTitleOffset(m_SubPad1TitleOffsetX);
  yaxis1->SetTitleOffset(m_SubPad1TitleOffsetY);
  hframe1->SetXTitle(m_SubPad1TitleTextX.Data());
  hframe1->SetYTitle(m_SubPad1TitleTextY.Data());

  m_subpad2->cd();

  if (debug) cout<< " MyFrame::DrawFrame: xmin= "<<m_xmin<<
                                       " xmax= "<<m_xmax<< 
                                       " ymin= "<<m_ymin2<< 
              		               " ymax= "<<m_ymax2<<endl; 

  TH1F *hframe2 = m_subpad2->DrawFrame(m_xmin,m_ymin2,m_xmax,m_ymax2);
  hframe2->SetXTitle(m_SubPad2TitleTextX.Data());
  hframe2->SetYTitle(m_SubPad2TitleTextY.Data());

  //TAxis *xaxisb = hframe1b->GetXaxis();  
  //TAxis *yaxisb = hframe1b->GetYaxis();
  xaxis2 = hframe2->GetXaxis();  
  yaxis2 = hframe2->GetYaxis();
  xaxis2->SetTitleOffset(m_SubPad2TitleOffsetX);
  yaxis2->SetTitleOffset(m_SubPad2TitleOffsetY);
  if (m_nDivX>-1) yaxis2->SetNdivisions(m_nDivX);
  xaxis2->SetLabelSize(xaxis1->GetLabelSize()/lscaley);
  yaxis2->SetLabelSize(yaxis1->GetLabelSize()/lscaley);
  xaxis2->SetTitleSize(xaxis1->GetTitleSize()/lscaley);
  yaxis2->SetTitleSize(yaxis1->GetTitleSize()/lscaley);
  hframe2->Draw();
 
  return;
}



void MyFrame::DrawFrame2(TPad* p){
  //debug = true;
 
  //hardcoded text size scaling when only a single pad is being displayed
  double scale = 1;
  if( (m_draw1 && ! m_draw2) || (!m_draw1 && m_draw2) ) scale = 1.5;


  // create canvas for pads
  m_p1=0, m_p2=0;
  
  TString cname="frame";
  cname+=m_framename;
  if (debug) cout<<" MyFrame cname= "<<cname.Data()<<endl;
  
  // TCanvas* draw_canv =0;
  if (p==NULL)
    draw_canv = new TCanvas(cname.Data(),cname.Data(), m_p1, m_p2, m_p3, m_p4);
  else
    draw_canv=(TCanvas*)p;   
  
  draw_canv->SetLeftMargin(0.2);
  draw_canv->SetRightMargin(0.1);
  draw_canv->SetBottomMargin(0.15);
  draw_canv->cd(); 
  //char csubname[100]; sprintf(csubname,"%s_subpad1",cname);
  TString csubname1="subpad1"; 
  csubname1+=cname;


  // sub pad 1 size
  if (debug) cout<<" MyFrame::DrawFrame:"
                 <<" m_sxmin= "<<m_sxmin
                 <<" m_symin= "<<m_symin
                 <<" m_sxmax= "<<m_sxmax
                 <<" m_symax= "<<m_symax<<endl;
  
  double xl1 = m_sxmin, yl1 = m_symin, xh1 = m_sxmax, yh1 = m_symax, b1=m_small; //both
  if( m_draw1 && m_draw2 ) { //both
    yl1 = m_symin; // y-low normal
  } else if ( !m_draw1 && m_draw2) { // only pad 2
    yl1 = m_symax; // y-low set to max (small frame will not show)
  } else if ( m_draw1 && !m_draw2 ) { // only pad 1
    yl1 = m_s0; b1 = 0.15; // y-low is now y-low of pad2 (takes whole frame)
  }
  
  m_subpad1 = new TPad(csubname1.Data()," ", xl1, yl1, xh1, yh1);
  m_subpad1->SetFillColor(0);
  m_subpad1->SetLeftMargin(m_lm);
  m_subpad1->SetRightMargin(m_rm);
  m_subpad1->SetBottomMargin(b1);
  
  if (m_logx) m_subpad1->SetLogx();
  if (m_logy1) m_subpad1->SetLogy();
  m_subpad1->Draw(); 

  
  
  // sub pad 2 size
  if (debug) cout<<" MyFrame::DrawFrame:"
                 <<" m_sxmin= "<<m_sxmin
                 <<" m_symin= "<<m_s0
                 <<" m_sxmax= "<<m_sxmax
                 <<" m_symax= "<<m_symin<<endl;
  
  double xl2 = m_sxmin, yl2 = m_s0, xh2 = m_sxmax, yh2 = m_symin;
  if( m_draw1 && m_draw2 ) { // both
    yh2 = m_symin; // y-high normal
  } else if ( !m_draw1 && m_draw2) { // only frame 2
    yh2 = m_symax; // y-high of pad1 (takes whole frame)
  } else if ( m_draw1 && !m_draw2 ) { // only frame 1
    yh2 = m_s0; // y-high set to zero (small frame will not show)
  }
      
  TString csubname2="subpad1"; 
  csubname2+=cname;
  m_subpad2 = new TPad(csubname2.Data()," Ratio ", xl2, yl2, xh2, yh2);
  m_subpad2->SetFillColor(0);
  m_subpad2->SetLeftMargin(m_lm);
  m_subpad2->SetRightMargin(m_rm);
  if( m_draw1 && m_draw2 ) m_subpad2->SetBottomMargin(m_bm);
  if( m_draw1 && m_draw2 ) m_subpad2->SetTopMargin(m_small);
    
  if (m_logx) m_subpad2->SetLogx();
  if (m_logy2) m_subpad2->SetLogy();
  m_subpad2->Draw();
  
  
  //scale second pad to same as first pad if both are being drawn
  Float_t lscaley = 1.;
  if(m_draw1) lscaley = (m_symin-m_s0)/(m_symax-m_symin); 
  

  // apply setting to pad 1
  m_subpad1->cd();
    
  TH1F *hframe1 = m_subpad1->DrawFrame(m_xmin,m_ymin,m_xmax,m_ymax);
  
  xaxis1 = hframe1->GetXaxis();
  yaxis1 = hframe1->GetYaxis();
  if (m_nDivY>-1) yaxis1->SetNdivisions(m_nDivY);
  xaxis1->SetTitleOffset(m_SubPad1TitleOffsetX);
  yaxis1->SetTitleOffset(m_SubPad1TitleOffsetY);
  xaxis1->SetLabelSize(xaxis1->GetLabelSize()/scale);
  yaxis1->SetLabelSize(yaxis1->GetLabelSize()/scale);
  xaxis1->SetTitleSize(xaxis1->GetTitleSize()/scale);
  yaxis1->SetTitleSize(yaxis1->GetTitleSize()/scale);
  if (m_draw1 && !m_draw2) hframe1->SetXTitle(m_SubPad2TitleTextX.Data());
  else hframe1->SetXTitle(m_SubPad1TitleTextX.Data());
  hframe1->SetYTitle(m_SubPad1TitleTextY.Data());
  

  // apply settings to pad 2
  m_subpad2->cd();
  
  if (debug) cout<< " MyFrame::DrawFrame: xmin= "<<m_xmin<<
	       " xmax= "<<m_xmax<< 
	       " ymin= "<<m_ymin2<< 
	       " ymax= "<<m_ymax2<<endl;   
  
  TH1F *hframe2 = m_subpad2->DrawFrame(m_xmin,m_ymin2,m_xmax,m_ymax2);
  hframe2->SetXTitle(m_SubPad2TitleTextX.Data());
  hframe2->SetYTitle(m_SubPad2TitleTextY.Data());
  
  xaxis2 = hframe2->GetXaxis();  
  yaxis2 = hframe2->GetYaxis();
  xaxis2->SetTitleOffset(m_SubPad2TitleOffsetX);
  double distScal = 0;
  if( !m_draw1 && m_draw2 ) distScal = 0.5;
  yaxis2->SetTitleOffset(m_SubPad2TitleOffsetY+distScal);
  if (m_nDivX>-1) yaxis2->SetNdivisions(m_nDivX);
  xaxis2->SetLabelSize(xaxis1->GetLabelSize()/lscaley);
  yaxis2->SetLabelSize(yaxis1->GetLabelSize()/lscaley);
  xaxis2->SetTitleSize(xaxis1->GetTitleSize()/lscaley);
  yaxis2->SetTitleSize(yaxis1->GetTitleSize()/lscaley);
  hframe2->Draw();
  
  return;
}

void MyFrame::Print() {

 cout<<" >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> "<<endl;
 cout<<m_framename<<": "
     <<" m_sxmin= "<<m_sxmin
     <<" m_symin= "<<m_symin
     <<" m_sxmax= "<<m_sxmax
     <<" m_symax= "<<m_symax<<endl;

 cout<<m_framename<<": "
     <<" m_xmin= "<<m_xmin
     <<" m_ymin= "<<m_ymin
     <<" m_xmax= "<<m_xmax
     <<" m_ymax= "<<m_ymax<<endl;

 cout<<m_framename<<": "
     <<" m_lm= "<<m_lm
     <<" m_rm= "<<m_rm
     <<" m_bm= "<<m_bm
     <<" m_small= "<<m_small
     <<endl;

 if (m_logx)  cout<<" Print in logx "<<endl;
 if (m_logy1) cout<<" Print in logy upper frame"<<endl;
 if (m_logy2) cout<<" Print in logy lower frame"<<endl;

 cout<<m_framename<<": "
     <<" m_SubPad1TitleOffsetX= "<<m_SubPad1TitleOffsetX
     <<" m_SubPad1TitleOffsetY= "<<m_SubPad1TitleOffsetY
     <<" m_SubPad2TitleOffsetY= "<<m_SubPad2TitleOffsetX
     <<" m_SubPad2TitleOffsetY= "<<m_SubPad2TitleOffsetY
     <<endl;

 //cout<<" m_nDivX= "<<m_nDivX<<" m_nDivY= "<<<m_nDivY<<endl;
 cout<<" <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< "<<endl;

}

void MyFrame::SaveFile(TString file_name)
{
 draw_canv->Print(file_name);
}
