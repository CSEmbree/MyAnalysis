
#ifndef MyFrame_H
#define MyFrame_H

#include "root.h"

class MyFrame {
 private:

  bool debug;
  TString m_framename;

  Double_t m_sxmin, m_sxmax;
  Double_t m_symin, m_symax;
  Double_t m_s0;
  Double_t m_lm, m_rm, m_bm, m_small;
  
  Int_t m_p1, m_p2, m_p3, m_p4;

  bool m_logx;  
  bool m_logy1;  bool m_logy2;

  //min/max of x-axis

  Double_t m_xmin, m_xmax, m_ymin, m_ymax, m_ymin2, m_ymax2;


  TPad *m_subpad1;
  TPad *m_subpad2;

  Double_t m_SubPad1TitleOffsetX;
  Double_t m_SubPad1TitleOffsetY;

  TString m_SubPad1TitleTextX;
  TString m_SubPad1TitleTextY;

  Double_t m_SubPad2TitleOffsetX;
  Double_t m_SubPad2TitleOffsetY;

  TString m_SubPad2TitleTextX;
  TString m_SubPad2TitleTextY;

  //Division on axis
  Int_t m_nDivX;
  Int_t m_nDivY;

  TAxis *xaxis1;
  TAxis *yaxis1;

  TAxis *xaxis2;
  TAxis *yaxis2;

 public:

  MyFrame(Int_t p3, Int_t p4);
  TCanvas *draw_canv;

  void SetFrameP1(Int_t p1){m_p1=p1;};
  void SetFrameP2(Int_t p2){m_p2=p2;};
  void SetFrameP3(Int_t p3){m_p3=p3;};
  void SetFrameP4(Int_t p4){m_p4=p4;};
  void SaveFile(TString file_name);

  Int_t GetFrameP1() {return m_p1;}; 
  Int_t GetFrameP2() {return m_p2;};
  Int_t GetFrameP3() {return m_p3;};
  Int_t GetFrameP4() {return m_p4;};

  TAxis *GetXAxis1() {return xaxis1;};
  TAxis *GetYAxis1() {return yaxis1;};

  TAxis *GetXAxis2() {return xaxis2;};
  TAxis *GetYAxis2() {return yaxis2;};

  void SetnDivX(Int_t nDivX){m_nDivX=nDivX; return;};
  void SetnDivY(Int_t nDivY){m_nDivY=nDivY; return;};

  void SetSubPad1TitleX(TString SubPad1TitleX){m_SubPad1TitleTextX=SubPad1TitleX; return;};
  void SetSubPad1TitleY(TString SubPad1TitleY){m_SubPad1TitleTextY=SubPad1TitleY; return;};
  void SetSubPad2TitleX(TString SubPad2TitleX){m_SubPad2TitleTextX=SubPad2TitleX; return;};
  void SetSubPad2TitleY(TString SubPad2TitleY) {m_SubPad2TitleTextY=SubPad2TitleY; return;};

  //void SetTitleX(TString TitleX){m_TitleTextX=TitleX; return;};
  //void SetTitleY(TString TitleY){m_TitleTextY=TitleY; return;};

 
  void SetSubPad2TitleOffsetX(Double_t SubPad2TitleOffsetX){m_SubPad2TitleOffsetX=SubPad2TitleOffsetX; return;};
  void SetSubPad2TitleOffsetY(Double_t SubPad2TitleOffsetY){m_SubPad2TitleOffsetY=SubPad2TitleOffsetY; return;};

  void SetXmin(Double_t xmin){m_xmin=xmin; return;};
  void SetXmax(Double_t xmax){m_xmax=xmax; return;};
  void SetYmin1(Double_t ymin){m_ymin=ymin; return;};
  void SetYmax1(Double_t ymax){m_ymax=ymax; return;};
  void SetYmin2(Double_t ymin2){m_ymin2=ymin2; return;};
  void SetYmax2(Double_t ymax2){m_ymax2=ymax2; return;};

  Double_t GetYmax(){return m_ymax;};
  Double_t GetYmin(){return m_ymin;};
  Double_t GetXmax(){return m_xmax;};
  Double_t GetXmin(){return m_xmin;};

  TCanvas *GetCanvas(){ return draw_canv;};

  TPad * GetSubPad1(){return m_subpad1;};
  TPad * GetSubPad2(){return m_subpad2;};


  void SetLogX  (){ m_logx=true;  return;};
  void SetLinX  (){ m_logx=false; return;};

  void SetLogY1  (){ m_logy1=true;  return;};
  void SetLinY1  (){ m_logy1=false; return;};

  void SetLogY2  (){ m_logy2=true;  return;};
  void SetLinY2  (){ m_logy2=false; return;};


  void SetSubPadYmin  (Double_t symin){ m_symin=symin; return;};
  void SetSubPadYmax  (Double_t symax){ m_symax=symax; return;};
  void SetSubPadXmin  (Double_t sxmin){ m_sxmin=sxmin; return;};
  void SetSubPadXmax  (Double_t sxmax){ m_sxmax=sxmax; return;};

  void SetLeftMargin  (Double_t lm){ m_lm=lm;  return;};
  void SetRightMargin (Double_t rm){ m_rm=rm;  return;};
  void SetBottomMargin(Double_t bm){ m_bm=bm;  return;};
  
  void    SetFrameName(TString framename){ m_framename=framename;  return;};
  TString GetFrameName()          {return m_framename;};

  void DrawFrame(TPad* p=NULL);
  void Print();

};


#endif
