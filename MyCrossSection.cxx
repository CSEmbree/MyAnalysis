//
//   for sherpa
//
using namespace std;
#include "appl_grid/generic_pdf.h"
#include "MyCrossSection.h"


/******************************************************************
 ** Method Implementations
 ******************************************************************/

//HARDCODED default value for an unused double - how to choose a better number?
const double DEFAULT_DOUBLE = -999;\
const string cn = " MyCrossSection::"; //class name, used for print statemants

MyCrossSection::MyCrossSection(char name[100])
{
  debug=false;
  crosssectionname="";
  ratiotitlelabel="";

  xunits="GeV"; //TODO - default, make generic
  yunits="fb"; //TODO - default, make generic

  frameid.clear();
  divideid.clear();
  framepointer.clear();
  vpdfchi2.clear();

  xlegend=0.90;
  ylegend=0.95;

  //default values for overloading ratio & overlay y-axes 
  ymaxoverlay=DEFAULT_DOUBLE;
  yminoverlay=DEFAULT_DOUBLE;
  ymaxratio=DEFAULT_DOUBLE;
  yminratio=DEFAULT_DOUBLE;

  plotchi2=false;
  plotmarker=false;
  plotband=false;
  ploterrorticks=false;
  staggerpdfpoints=false;
  ratioTheoryOverData=true; //ratio default is (theory/data)

  pdf_function="";
  subprocesssteername="";

  // <<<<<<<<<<<<<<<<< should be rather a vector for general use
  gridnamebasedir=".";
  gridnamesystbasedir=".";
  gridnamedefaultdir=".";
  //gridnamemassdowndir=".";
  //gridnamemassupdir=".";
  //gridnamebeamdowndir=".";
  //gridnamebeamupdir=".";
  // <<<<<<<<<<<<<<<<<

  datanamedir=".";
  ntupname="";
        
  steername=name;
  if (debug) cout<<" MyCrossSection:: steering file= "<<steername<<endl;
  gridname.clear();
  vardesc.clear();
  dataname.clear();
  //corrname.clear();
  leglabel.clear();
  refhistlinestyle.clear();
  refhistlinecolor.clear();
  markerstyle.clear();
  markercolor.clear();
  mydata.clear();
  do_PDFBand = false;
  do_AlphaS = false;
  do_RenormalizationScale = false;
  do_FactorizationScale = false;
  do_TotError = false;
    
  this->ReadSteering(name);

  if (debug) cout<<" MyCrossSection finished reading name= "<<name<<endl;

  this->Initialize();

  if (debug) this->Print();

  return;

}

void MyCrossSection::Initialize() {
  //
  // Perform any initialization needed before plotting
  //

  if (debug) cout<<" MyCrossSection::Initialize()"<<endl;
  cout<<" MyCrossSection::Initialize: Number of grids to produce "<<gridname.size()<<endl;

  //
  // Loop over grid
  //
  for (int  igrid = 0; igrid < gridname.size(); igrid++) {

    MyBand *mybandtmp= new MyBand();
    if (ploterrorticks) mybandtmp->SetErrorTicks(); 
    if (plotband) mybandtmp->SetPlotBand();
    if (plotmarker) {
      mybandtmp->SetPlotMarker();
      if (debug) cout<<" MyCrossSection::Initialize: Plotmarker ON move points "<<endl;
      mybandtmp->MovePDFPoints(); //plot markers are more visible if data points are shifted slightly
    }
    if (staggerpdfpoints) mybandtmp->SetStaggerPDFPoints();
    mybandtmp->SetRatioTheoryOverData(ratioTheoryOverData); //tell the band if this is theory/data for ratio


    myband.push_back(mybandtmp);


    if (debug) cout<<" MyCrossSection::Initialize: Data "<<this->GetDataName(igrid)<<endl;
    if (this->GetDataOk(igrid)) {
      MyData *mydatatmp= new MyData;
      if (debug)
	cout<<" MyCrossSection::Initialize: dataname= "<<TString(this->GetDataName(igrid)).Data()
	    <<" datadir= "<<TString(this->GetDataNameDir(igrid)).Data() <<endl;

      mydatatmp->ReadData(this->GetDataName(igrid),this->GetDataNameDir(igrid));

      mydatatmp->SetMarkerStyle(this->GetMarkerStyle(igrid));
      mydatatmp->SetMarkerColor(this->GetMarkerColor(igrid));
      mydata.push_back(mydatatmp);
    } else {
      if (debug) cout<<" MyCrossSection::Initialize no data found "<<endl;
    }

    //container for mypdf instances per grid type. Each grid can have multiple pdfs
    //std::vector<MyPDF*> pdfStorage; 
    //t_mypdf.push_back(pdfStorage);

    double yfac=1.;
    double xfac=1.;
    bool normtot=false;
    bool divbinwidth=false;
    if (this->GetDataOk(igrid)) {
      //yfac=mydata[igrid]->GetUnitfbFactor(); //old scaling
      //xfac=mydata[igrid]->GetUnitGeVFactor(); //old scaling

      //convert units from data to desired
      yfac = GetUnitScaleY(mydata[igrid]->GetYUnits(), this->GetYUnits()); //new scaling
      xfac = GetUnitScaleX(mydata[igrid]->GetXUnits(), this->GetXUnits()); //new scaling

      
      //mydata[igrid]->Scale(1.,yfac/xfac);
      //mydata[igrid]->Normalise(yfac, xfac); //TODO - scale actual data now?
      cout<<"TEST:: igrid:"<<igrid<<", xfac:"<<xfac<<", yfac:"<<yfac<<endl;


      if (debug) cout<<" MyCrossSection::Initialize: yfac= "<<yfac<<" xfac= "<<xfac<<endl;
      normtot=mydata[igrid]->isNormTot();
      if (debug&&normtot) cout<<" MyCrossSection::Initialize: normalise to total cross section "<<endl;
      divbinwidth=mydata[igrid]->DivideByBinWidth();
      if (debug&&divbinwidth) cout<<" MyCrossSection::Initialize: Data is divided by binwidth "<<endl;
    } else
      cout<<" MyCrossSection::Initialize: data is not ok ! "<<endl;

    if (pdfdata.size()==0) {cout<<" MyCrossSection::Initialize: no PDF found ! "<<endl;}

    if (debug) cout<<" MyCrossSection::Initialize: Number of PDF names pdfdata.size()= "<<  pdfdata.size()<<endl;

    int npdf=0;
    //if (pdfdata.size()>0) npdf=pdfdata.at(igrid).size();
    if (pdfdata.size()>0) npdf=pdfdata.size();
 
    if (debug) cout<<" MyCrossSection::Initialize: npdf= "<<npdf<<endl;

    for (int ipdf=0; ipdf<npdf; ipdf++){
      if (debug) cout<<" MyCrossSection::Initialize: load pdf= "<<pdfdata.at(ipdf).c_str()<<endl;

      MyPDF *newpdf = new MyPDF(GetGridName(igrid), 
				pdfdata.at(ipdf),
                                do_PDFBand,
                                do_AlphaS
				);
                      
      if (debug) {
	cout<<" MyCrossSection::Initialize: print pdf= "<<ipdf
	    <<" name= "<<pdfdata.at(ipdf).c_str()
	    <<" for grid: "<<GetGridName(igrid)<<endl;

	cout<<" MyCrossSection::Initialize: ipdf= "<<ipdf<<" "<<newpdf->GetPDFtype().c_str()<<endl;
	newpdf->h_PDFBand_results->Print("all");

	cout<<" MyCrossSection::Initialize: npoint= "<<newpdf->h_PDFBand_results->GetN()<<endl;
      }

      // get myband information for each pdf
      TGraphAsymmErrors* gtmp= (TGraphAsymmErrors*) newpdf->h_PDFBand_results->Clone((TString) (newpdf->GetPDFtype() + "_pdfband"));

      gtmp->SetName(TString(newpdf->GetPDFtype()));
      if (debug) {
	cout<<" MyCrossSection::Initialize: yfac= "<<yfac<<" xfac= "<<xfac<<endl; 
	if (normtot) cout<<" MyCrossSection::Initialize: normtot on "<<endl; 
	if (divbinwidth) cout<<" MyCrossSection::Initialize: divbinwidth "<<endl; 
      }

      
      this->Normalise(gtmp,yfac,xfac,normtot,divbinwidth);
      if (debug) {cout<<" MyCrossSection::Initialize: fill Band gtmp"<<endl; gtmp->Print();}

      // set my band info for displyaing later
      myband[igrid]->SetPdfBand(gtmp); 
      

      // if data is being forcfully scaled, also scale the theory as well
      if(mydata[igrid]->GetScalex() != 1.0 || mydata[igrid]->GetScaley() != 1.0 ) {
	
	if(debug) cerr<<" MyCrosSection:Initialize: WARN: Forcefully scaling theory to data. "<<endl;
	
	//set theory's scaling to the same as data's
	myband[igrid]->SetScalex( mydata[igrid]->GetScalex() );
	myband[igrid]->SetScaley( mydata[igrid]->GetScaley() );
      }



      TH1D *hdefault=newpdf->GetPdfdefault();
      if (!hdefault) cout<<" MyCrossSection::Initialize: hdefault not found "<<endl;
      if (debug) {
	cout<<" MyCrossSection::Initialize: print default histogram ipdf= "<<ipdf<<endl; 
	hdefault->Print("all");
      }
      TGraphAsymmErrors* gdefaulttmp=TH1TOTGraphAsymm(hdefault); 
      this->Normalise(gdefaulttmp,yfac,xfac,normtot,divbinwidth);
      TString gname=TString(newpdf->GetPDFtype());
      gname+="_default";
      gdefaulttmp->SetName(gname);
      myband[igrid]->SetPdfDefault(gdefaulttmp);   

      if (debug) {
	cout<<" MyCrossSection::Initialize: print normalised default ipdf= "<<ipdf<<endl; 
	gdefaulttmp->Print();
      }

      if (newpdf->GetDoAlphaS() ) {
	if (debug) cout<<" MyCrossSection::Initialize: DoAlphaS "<<endl;

	TGraphAsymmErrors* gtmp= (TGraphAsymmErrors*) newpdf->h_AlphaS_results->Clone((TString) (newpdf->GetPDFtype() + "_alphasband"));

	this->Normalise(gtmp,yfac,xfac,normtot,divbinwidth);
	myband[igrid]->SetAlphasBand(gtmp); 
      } else if (debug) cout<<" MyCrossSection::Initialize: do alphas not set in MyPDF "<<endl;

    }

    cout<<" MyCrossSection::Initialize number of ren scale= "<<renscale.size()<<endl;
    cout<<" MyCrossSection::Initialize number of fac scale= "<<facscale.size()<<endl;

    if (renscale.size()!=facscale.size()) 
      cout<<" MyCrossSection::Initialize Something is wrong #renscale != #facscale "
	  <<" renscale= "<<renscale.size()<<" fac= "<<facscale.size() 
	  <<endl;

    for (int i=0; i<renscale.size(); i++){
      if (debug) cout<<" MyCrossSection::Initialize: scales= "<<renscale[i]<<" facscale= "<<facscale[i]<<endl;

      int pdfset=0;
      //TGraphAsymmErrors *gtmp=newpdfdefault->GetScaleDependence(renscale[i],facscale[i],pdfset);
      //if (!gtmp) cout<<" MyCrossSection::Initialize: gtmp from GetScaleDependence not found ! "<<endl;

      //if (debug) { 
      // gtmp->Print("all"); 
      //}
      cout<<" MyCrossSection::Initialize: nothing implemented "<<endl;
      //myband[igrid]->SetScaleBand(gtmp);   
    }

    std::vector<double> mychi2;

    //TMatrixT  <double> mcorr;
    TMatrixT <double> *mcorr;
    if (mydata[igrid]->CovarianceMatrixOk()){
      if (debug) cout<<" MyCrossSection::Initialize: get correlation matrix igrid= "<<igrid<<endl;
      mcorr= mydata[igrid]->GetCovarianceMatrix();
      if (debug) cout<<" MyCrossSection::Initialize: mcorr done "<<endl;
      if (debug) mcorr->Print();
      if (debug) cout<<" MyCrossSection::Initialize: printed "<<endl;
    } else cout<<" MyCrossSection::Initialize covariance matrix not found igrid= "<<igrid<<endl;

    if (debug) cout<<" MyCrossSection::Initialize: npdf= "<<pdfdata.size()<<endl;
    if (mydata[igrid]->CovarianceMatrixOk()){
      if (debug) {
	cout<<" MyCrossSection::Initialize: before CalcChi2 "<<endl;
	mcorr->Print();
      }

      for (int ipdf=0; ipdf<pdfdata.size(); ipdf++) {
	TGraphAsymmErrors *pdftmp=myband[igrid]->GetPdfBand(ipdf);
	if (!pdftmp) cout<<" MyCrossSection::Initialize: pdftmp not found ! "<<endl;
	double chi2=this->CalcChi2(pdftmp, mydata[igrid]->GetTGraphTotErr(), mcorr);
	if (debug) cout<<" MyCrossSection::Initialize: igrid= "
		       <<igrid<<" ipdf= "<<ipdf<<" chi2= "<<chi2<<endl;
	mychi2.push_back(chi2);
      }
    } else cout<<" MyCrossSection::Initialize:  no correllation matrix found "<<endl;

    vpdfchi2.push_back(mychi2);

    if (debug) cout<<" MyCrossSection::Initialize: finished chi2 calculation "<<endl;

    if (pdfdata.size()<1) {
      TH1D *href=this->GetNormalisedReference(igrid);
      if (!href) cout<<" MyCrossSection::Initialize:  reference histo not found igrid= "<<igrid<<endl;
      
      TGraphAsymmErrors* gref=TH1TOTGraphAsymm(href);
      if (!gref) cout<<" MyCrossSection::Initialize:  reference graph not created igrid= "<<igrid<<endl;
      
      if (debug) {
	cout<<" MyCrossSection::Initialize: ComputePDFBandRatio  dividing gref and "<<mydata[igrid]->GetDataName()<<endl;
      }
 

      /*
      if(mydata[igrid]->GetScalex() != 1.0 || mydata[igrid]->GetScaley() != 1.0 ) {
	cout<<"TEST: scaleit"<<endl;
	ScaleGraph( gref, mydata[igrid]->GetScalex(), mydata[igrid]->GetScaley() ); //TODO - include checking for scalex?
      }
      */
      

      // Flip ratio calculations if steering asks for it
      TGraphAsymmErrors* top;
      TGraphAsymmErrors* bot;
      int errorBarType;
      TString rationame = "";
      if(ratioTheoryOverData) {
      	top = gref; //origonal - reference over the data
      	bot = mydata[igrid]->GetTGraphTotErr();
	bot = mydata[igrid]->GetTGraphTotErr(); //TEST


	rationame += gref->GetName()+igrid;
	rationame +="/";
	rationame +=mydata[igrid]->GetDataName();
	errorBarType = 0;
      } else {
	top = gref; //reference over itself
	bot = gref;
	
	rationame += gref->GetName()+igrid;
	rationame+="/";
	rationame+=gref->GetName()+igrid;
	errorBarType = 2;
      }

      
      //ratiotot.push_back(myTGraphErrorsDivide(gref, mydata[igrid]->GetTGraphTotErr(), 0)); // origional
      ratiotot.push_back(myTGraphErrorsDivide(top, bot, errorBarType)); //TODO - change error bar saving?
      ratiotot[igrid]->SetName(rationame);
      ratiotot[igrid]->SetMarkerStyle(this->GetMarkerStyle(igrid));
      ratiotot[igrid]->SetMarkerColor(this->GetMarkerColor(igrid));
      ratiotot[igrid]->SetLineColor(refhistlinecolor[igrid]);
      ratiotot[igrid]->SetLineStyle(refhistlinestyle[igrid]);
      ratiotot[igrid]->SetFillColor(refhistlinecolor[igrid]);
      ratiotot[igrid]->SetFillStyle(3004+igrid);
      
      if (debug) {
	cout<<" MyCrossSection::Initialize: ratiotot["<<igrid<<"] called '"<<rationame.Data()<<"': "<<endl;
	ratiotot[igrid]->Print();
      }
    } else {
      //TH1D *href=this->GetNormalisedReference(igrid);
      //TGraphAsymmErrors* gref=TH1TOTGraphAsymm(href);
      //myband[igrid]->ComputePDFBandRatio(gref); //should be same as origional

      myband[igrid]->ComputePDFBandRatio(mydata[igrid]->GetTGraphTotErr()); //TEST - origonal
    }
    /*
    // move this to MyBand
    std::vector<TGraphAsymmErrors*> gpdfbandratiotottmp;
    std::vector<TGraphAsymmErrors*> gpdfdefaultratiotottmp;
    //for (int ipdf=0; ipdf<myband[igrid]->GetNPDF(); ipdf++) {
    for (int ipdf=0; ipdf<pdfdata.size(); ipdf++) {
    TGraphAsymmErrors *pdftmp=myband[igrid]->GetPdfBand(ipdf);
    if (debug)   cout<<" MyCrossSection::Initialize: Band for ipdf= "<<ipdf<<" divide "<<endl;
    if (!pdftmp) cout<<" MyCrossSection::Initialize: pdf not found  "<<endl;

    gpdfbandratiotottmp.push_back(myTGraphErrorsDivide(pdftmp,mydata[igrid]->GetTGraphTotErr(),2));
    gpdfbandratiotottmp[ipdf]->SetFillStyle(pdftmp->GetFillStyle());
    gpdfbandratiotottmp[ipdf]->SetFillColor(pdftmp->GetFillColor());

    TGraphAsymmErrors *pdftmpdefault=myband[igrid]->GetPdfDefault(ipdf);
    gpdfdefaultratiotottmp.push_back(myTGraphErrorsDivide(pdftmpdefault,mydata[igrid]->GetTGraphTotErr(),0));
    gpdfdefaultratiotottmp[ipdf]->SetMarkerStyle(0);
    gpdfdefaultratiotottmp[ipdf]->SetMarkerSize(0);
    gpdfdefaultratiotottmp[ipdf]->SetLineColor(pdftmp->GetFillColor());
    }
    gpdfbandratiotot.push_back(gpdfbandratiotottmp);

    gpdfdefaultratiotot.push_back(gpdfdefaultratiotottmp);
    */
  } // igrid loop

  if (debug) cout<<" MyCrossSection::Initialize: end of igrid loop"<<endl;

  // grid in frames
  gridinframe.clear();
  //gridinframedata.clear();

  vframe vigrid; 

  for (int iframe=0; iframe<this->GetFrameNumber(); iframe++) {
    vigrid.clear(); 
    for (int igrid=0; igrid<this->GetNGrid(); igrid++) {
      int jframe=this->GetFrameID(igrid);
      if (debug) cout<<" MyCrossSection::Initialize: igrid= "<<igrid
		     <<" iframe= "<<iframe<<" jframe= "<<jframe<<endl;
      if (jframe==iframe) {
	if (debug) cout << " MyCrossSection::Initialize: igrid= " << igrid 
                        << " push back this grid" << endl;
	vigrid.push_back(igrid);
      }
    }

    if (debug) cout << " MyCrossSection::Initialize: iframe= "<<iframe 
		    << " push back this grid vector size= "<<vigrid.size()
		    << endl;
    if (debug) {
      cout<< " MyCrossSection::Initialize: vigrid vector size= "<<vigrid.size()<<endl;
      for (int i=0; i< vigrid.size(); i++) {
	cout<<" MyCrossSection::Initialize: vigrid["<<i<<"]="<<vigrid[i]<<endl;
      }
    }
    gridinframe.push_back(vigrid);

    if (debug) {
      cout << " MyCrossSection::Initialize: print gridinframe.size()= "
	   <<  gridinframe.size()<<endl;
      for (int i=0; i< gridinframe.size(); i++) {
	for (int j=0; j< gridinframe[i].size(); j++) {
	  cout<<" MyCrossSection::Initialize: gridinframe["<<i<<"]["
	      <<j<<"]= "<<gridinframe[i][j]<<endl;
	}
      }
    }

    const double BIG=1.e30;
    double ymin= BIG;
    double ymax=-BIG;
    double xmin= BIG;
    double xmax=-BIG;
    double datascalex=1.2;
    double scaley=1.2;
    double x,y;

    vframedata vigriddata; 
    vigriddata.clear(); 

    if (debug) cout << " MyCrossSection::Initialize: iframe= "
		    <<iframe<<endl;

    if (debug) cout << " MyCrossSection::Initialize: gridinframe["
		    <<iframe<<"].size()= "<<gridinframe[iframe].size()
		    <<endl;


    for (int i=0; i<gridinframe.size(); i++) {
      //if (debug) cout<<" MyCrossSection::Initialize: i= "<<i<<endl;
      int igrid=gridinframe[iframe][i];
      int jframe=this->GetFrameID(igrid);
      if (iframe!=jframe) continue;
      //if (debug) cout<<" MyCrossSection::Initialize: igrid= "<<igrid<<endl;

      if (debug) cout<<" MyCrossSection::Initialize: gridinframe["<<iframe<<"]["
		     <<i<<"]= "<<gridinframe[iframe][i]<<endl;

      if (this->GetDataOk(igrid)) {
	if (debug) cout<<" MyCrossSection::Initialize: igrid= "<<igrid<<" iframe= "<<iframe
		       <<" push "<<mydata[igrid]->GetDataName()<<endl;

	vigriddata.push_back(mydata[igrid]);

	if (mydata[igrid]->GetLogY1()) {scaley=2.0;}
	if (mydata[igrid]->GetLogX())  {datascalex=1.5;}
	//cout<<" datascalex= "<<datascalex<<" scaley= "<<scaley<<endl;
	y=mydata[igrid]->GetMinY();
	if (ymin>y) ymin=y;

	y=mydata[igrid]->GetMaxY();
	if (ymax<y) ymax=y;

	x=mydata[igrid]->GetMinX();
	if (xmin>x) xmin=x;

	x=mydata[igrid]->GetMaxX();
	if (xmax<x) xmax=x;

      } else cout<<" MyCrossSection::Initialize: data is not ok ?? "<<endl;
    } 

    //gridinframedata.push_back(vigriddata);

    if (debug) cout<<" MyCrossSection::Initialize: datascalex= "<<datascalex<<" scaley= "<<scaley<<endl;
    if   (xmin>0) xmin/=datascalex;
    else  xmin*=datascalex;

    if   (xmax>0) xmax*=datascalex;
    else  xmax/=datascalex;

    if   (ymin>0) ymin/=scaley;
    else  ymin*=scaley;

    if   (ymax>0) ymax*=scaley;
    else  ymax/=scaley;

    if (debug)
      cout<<" MyCrossSection::Initialize: iframe= "<<iframe<<" xmin= "
	  <<xmin<<" xmax= "<<xmax<< " ymin= "<<ymin<<" ymax= "<<ymax<<endl;


    xminframe.push_back(xmin);
    xmaxframe.push_back(xmax);
    yminframe.push_back(ymin);
    ymaxframe.push_back(ymax);

  }

  if (debug) cout<<" MyCrossSection::Initialize: finished  "<<endl;
  return;

}


void MyCrossSection::ReadSteering(char fname[100]) {
  //
  // read steering from file fname
  //
  string mn = "ReadSteering: "; //Method name, for printing

  steername=fname; 
  if (debug) cout<<" MyCrossSection::ReadSteering: steering "<<steername<<endl;

  ifstream infiletmp(steername.c_str(), ios::in);
  //if( debug ) std::cout << " MyCrossSection::ReadSteering: Why am I not here? " << std::endl;
  if(!infiletmp){ // Check open
    cerr << " MyCrossSection::ReadSteering Can't open " << steername <<"\n";
    infiletmp.close();
    exit (1);
  } else {
    if (debug) cout <<" MyCrossSection::ReadSteering: read data file: " << steername << endl;
  }

  int iline=0;
  int nsyst=1;

  char line[1024]; char text[256]; char name[256];  int intVal; double doubleVal;

  while (1) {
    //if (debug) cout << " good: " << infile.good() << " eof: " << infile.eof() << endl;
    if (!infiletmp.good()) break;
    infiletmp.getline(line,sizeof(line),'\n');
    if(line[0] != 'g') continue;
    if (debug) cout << "line " << line << " compare with gridname\n";

    if (strstr(line,"gridname")!=0) {
      if( strstr(line,"gridnamedefaultdir")!=0 ) continue;
      if( strstr(line,"gridnamebasedir")!=0 ) continue;
      if( strstr(line,"gridnamesystbasedir")!=0 ) continue;
      sscanf(line," %s %[^\n] ",text, name);
      if (debug) cout<<" MyCrossSection::ReadSteering: "<<text<<" "<<name<<endl;
      string myname=name;
      if (debug) cout << "Push back " << myname << endl;
      gridname.push_back(myname);
    }
  }

  
  if (debug) cout<<" MyCrossSection::ReadSteering: number of grids "<<gridname.size()<<endl;

  // set up options vector, one option for each grid
  for (int igrid=0; igrid<gridname.size(); igrid++){
    if (debug) cout<<" MyCrossSection::ReadSteering: gridname["<<igrid<<"]= "<<gridname[igrid]<<endl;
    markerstyle.push_back(20);
    markercolor.push_back(1);
    //mcscalex.push_back(1.);
    //datascalex.push_back(1.);
    //scaley.push_back(1.);
    frameid.push_back(-1);
    divideid.push_back(-1);
    refhistlinestyle.push_back(1.);
    refhistlinecolor.push_back(1.);
  }

  ifstream infile(steername.c_str(), ios::in);

  int igrid=-1;

  // read all options from text file
  while (infile.good()) {
    infile.getline(line,sizeof(line),'\n');
    std::string cpp_line(line);
    std::vector<std::string> colon_split_cpp_line;  colon_split_cpp_line.clear();
    split_string(cpp_line, colon_split_cpp_line, ":");
    
    if (debug) cout<< " MyCrossSection::ReadSteering: line= "<< line << "\n";
    if(line[0] != '%' ) { //ignore lines beginning with comments - could be done better

      switch( toupper(line[0]) ) { //use first letter of option to find where to start
      case 'A':	break;
      case 'B': break; 
      case 'C': break;
      case 'D':
	if (strstr(line,"debug")!=0) {
	  debug=true;
	  if (debug) cout<<cn<<mn<<" Debug turned on"<<endl;
	} else if (strstr(line,"datanamedir")!=0) {
	  sscanf(line," %s %[^\n] ",text, name);
	  datanamedir = string(name);
	} else if (strstr(line,"divideid")!=0) {
	  sscanf(line," %s %d ",text, &intVal);
	  divideid[igrid]=intVal;
	} else if (strstr(line,"dataname")!=0) {
	  sscanf(line," %s %[^\n] ",text, name);
	  dataname.push_back(string(name));  
	}
	break;
      case 'E': break;
      case 'F': 
	if (strstr(line,"frameid")!=0) {
	  sscanf(line," %s %d ",text, &intVal);
	  frameid[igrid]=intVal;
	} else if (strstr(line,"facscale")!=0) {
	  sscanf(line," %s %[^\n] ",text, name);
	  if (debug) cout<<"text: "<<text<<"name: "<<name<<endl;
	  std::vector<string> *parsedNames;
	  std::string facnames = name;
	  char delimeter = ' ';
	  parsedNames = ParseString(facnames, delimeter);

	  if (debug) cout<<cn<<mn<<" found '"<<parsedNames->size()<<"' fac scales "<<endl;
	  if (debug) cout<<cn<<mn<<" fac for grid: '"<<facnames<<"'"<<endl;
	  
	  for (int i=0; i<parsedNames->size(); i++){
	    facscale.push_back(atof( (parsedNames->at(i)).c_str()) );
	  }
	  
	  do_FactorizationScale=true;
	}
	break;
      case 'G': 
	if (strstr(line,"Gridnamedir")!=0) {
	  sscanf(line," %s %[^\n] ",text, name);
	  gridnamebasedir = string(name);
	} else if (strstr(line,"gridnamebasedir")!=0) {
	  sscanf(line," %s %[^\n] ",text, name);
	  gridnamebasedir = string(name);
	} else if (strstr(line,"gridnamesystbasedir")!=0) {
	  sscanf(line," %s %[^\n] ",text, name);
	  gridnamesystbasedir = string(name);
	} else if (strstr(line,"gridnamedefaultdir")!=0) {
	  sscanf(line," %s %[^\n] ",text, name);
	  gridnamedefaultdir = string(name);
	} else if (strstr(line,"gridname")!=0) {
	  sscanf(line," %s %[^\n] ",text, name);
	  igrid++;
	}
	break;
      case 'H': break;
      case 'I': break;
      case 'J': break;
      case 'K': break;
      case 'L':
	if (strstr(line,"leglabel")!=0) {
	  sscanf(line," %s %[^\n] ",text, name);
	  leglabel.push_back(string(name));
	} 
	break;
      case 'M':
	if (strstr(line,"markerstyledata")!=0) {
	  sscanf(line," %s %d ",text, &intVal);
	  if (igrid<0) cerr<<cn<<mn<<" Something wrong ! '"<<intVal<<"'"<<endl;
	  markerstyle[igrid]=intVal;
	} else if (strstr(line,"markercolordata")!=0) {
	  sscanf(line," %s %d ",text, &intVal);
	  markercolor[igrid]=intVal; 
	}
	break;
      case 'N':
	if (strstr(line,"ntupname")!=0) {
	  sscanf(line," %s %[^\n] ",text, name);
	  ntupname=string(name); 
	}
	break;
      case 'O': break;
      case 'P': 
	if (strstr(line,"pdffunction")!=0) {
	  sscanf(line," %s %[^\n] ",text, name);
	  pdf_function=string(name);
	} else if (strstr(line,"plotmarker")!=0) {
	  plotmarker=true;
	} else if (strstr(line,"plotband")!=0) {
	  plotband=true;
	} else if (strstr(line,"ploterrorticks")!=0) {
	  ploterrorticks=true;
	} else if (strstr(line,"plotchi2")!=0) {
	  plotchi2=true;
	} else if (strstr(line,"pdfdata")!=0) {
	  sscanf(line," %s %[^\n] ",text, name);
	  if (debug) cout<<"text: "<<text<<"name: "<<name<<endl;
	  std::vector<string> *parsedNames;
	  std::string pdfSteeringFileNames = name;
	  char delimeter = ',';
	  parsedNames = ParseString(pdfSteeringFileNames, delimeter);
	  	 	  
	  for (int iname=0; iname<parsedNames->size(); iname++)
	    pdfdata.push_back(parsedNames->at(iname));
	}
	break;
      case 'Q': break;
      case 'R':
	if (strstr(line,"ratiotitlelabel")!=0) {
	  sscanf(line," %s %[^\n] ",text, name);
	  ratiotitlelabel=name;
	} else if (strstr(line,"reflinestyle")!=0) {
	  sscanf(line," %s %d ",text, &intVal);
	  refhistlinestyle[igrid]=intVal;
	} else if (strstr(line,"reflinecolor")!=0) {
	  sscanf(line," %s %d ",text, &intVal);
	  if (debug) cout<<cn<<mn<<" reflinecolor:  "<<intVal<<endl;
	  refhistlinecolor[igrid]=intVal;
	} else if (strstr(line,"ratio")!=0) {
	  sscanf(line, "%s %s",text,name);
	  
	  //determine how ratio will be computed
	  if (strstr(name,"theory/data")!=0)      ratioTheoryOverData = true; // ratio will be (theory/data)
	  else if (strstr(name,"data/theory")!=0) ratioTheoryOverData = false; // ratio will be (data/theory)
	  else                                    ratioTheoryOverData = true; //default - (theory/data) for legacy steering files
	} else if (strstr(line,"renscale")!=0) {
	  sscanf(line," %s %[^\n] ",text, name);
	  if (debug) cout<<"text: "<<text<<"name: "<<name<<endl;
	  std::vector<string> *parsedNames;
	  std::string rennames = name;
	  char delimeter = ' ';
	  parsedNames = ParseString(rennames, delimeter);
	  if (debug) cout<<cn<<mn<<" found '"<<parsedNames->size()<<"' ren scales"<<endl;
	  if (debug) cout<<cn<<mn<<" pdfsteering for grid: "<<rennames<<"'"<<endl;
	  for (int i=0; i<parsedNames->size(); i++){
	    renscale.push_back(atof( (parsedNames->at(i)).c_str()) );
	  }
	  do_RenormalizationScale=true;
	}
	break;
      case 'S': 
	if (strstr(line,"subprocesssteername")!=0) {
	  sscanf(line," %s %[^\n] ",text, name);
	  subprocesssteername=string(name);
	} else if (strstr(line,"staggerpdfpoints")!=0) {
	  staggerpdfpoints=true;
	}
	break;
      case 'T': break;
      case 'U': break;
      case 'V':
	if (strstr(line,"vardesc")!=0) {
	  sscanf(line," %s %[^\n] ",text, name);
	  vardesc.push_back(string(name));
	} 
	break;
      case 'W': break;
      case 'X': 
	if (strstr(line,"xlegend")!=0) {
	  sscanf(line," %s %f ",text, &xlegend);
	} else if (strstr(line,"xunits")!=0) {
	  sscanf(line," %s %s ",text, name);
	  xunits = string(name);
	}
	break;
      case 'Y': 
	if (strstr(line,"ylegend")!=0) {
	  sscanf(line," %s %f ",text, &ylegend);
	} else if (strstr(line,"ymaxoverlay")!=0) {
	  sscanf(line," %s %lf ",text, &ymaxoverlay);
	} else if (strstr(line,"yminoverlay")!=0) {
	  sscanf(line," %s %lf ",text, &yminoverlay);
	} else if (strstr(line,"ymaxratio")!=0) {
	  sscanf(line," %s %lf ",text, &ymaxratio);
	} else if (strstr(line,"yminratio")!=0) {
	  sscanf(line," %s %lf ",text, &yminratio);
	} else if (strstr(line,"yunits")!=0) {
	  sscanf(line," %s %s ",text, name);
	  yunits = string(name);
	}
	break;
      case 'Z': break;
      default: 
	cerr<<cn<<mn<<" WARN: Invalid steering option found '"<<line<<"'"<<endl;
	//exit(0); //stop if an invalid option found, it's probably an error!
      } //end switch
    }; //end comment check
  }; //end file reading

  if(debug) { 
    cout<<cn<<mn<<" Finished reading steering. REPORT:"<<endl;
    Print();
  }

  return;
};


void MyCrossSection::Print() {
  //
  // Display internal setting states read from steering
  //

  int w=30; //arbitrary spacing size that makes the formatting look pretty

  cout<<" MyCrossSection::Print: >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>"
      <<"\n"<<setw(w)<<"steering: "            <<setw(w)<<steername
      <<"\n"<<setw(w)<<"subprocesssteername: " <<setw(w)<<subprocesssteername
      <<"\n"<<setw(w)<<"pdf_function: "        <<setw(w)<<pdf_function
    //<<"\n"<<setw(w)<<"ntupdir= "<<ntupdirinput<<" ntupoutput= "<<ntupdiroutput
      <<"\n"<<setw(w)<<"Dir of grids: "        <<setw(w)<<gridnamebasedir
      <<"\n"<<setw(w)<<"Dir of data: "         <<setw(w)<<datanamedir
      <<"\n"<<setw(w)<<"Num of grids: "        <<setw(w)<<gridname.size()
      <<"\n"<<setw(w)<<"Ratio: "               <<(ratioTheoryOverData? "(theory/data)":"(data/theory)")<<endl;

  for (int  i = 0; i <   gridname.size(); i++) {
    if (this->GetDataOk(i)) {
      cout<<"\n"<<setw(w)<<"INFO FOR GRID NUM:"<<setw(w)<<i
	  <<"\n"<<setw(w)<<"dataname.size(): " <<setw(w)<<dataname.size()
	//<<"\n"<<setw(w)<<"events.size(): "   <<setw(w)<<events.size()
	  <<"\n"<<setw(w)<<"gridname.size(): " <<setw(w)<<gridname.size()
                
	  <<"\n"<<setw(w)<<"grid: "         <<setw(w)<<gridname[i]
	  <<"\n"<<setw(w)<<"data: "         <<setw(w)<<dataname[i]
	//<<"\n"<<setw(w)<<"events: "       <<setw(w)<<events[i]
	  <<"\n"<<setw(w)<<"style: "        <<setw(w)<<this->GetMarkerStyle(i)
	  <<"\n"<<setw(w)<<"color:"         <<setw(w)<<this->GetMarkerColor(i)
	  <<"\n"<<setw(w)<<"frameid:"       <<setw(w)<<this->GetFrameID(i)
	  <<"\n"<<setw(w)<<"divideid:"      <<setw(w)<<this->GetDivideID(i)<<endl;
    } else {
      cout<<"\n"<<setw(w)<<"INFO FOR GRID NUM:"<<setw(w)<<i
	  <<"\n"<<setw(w)<<"grid:"             <<setw(w)<<gridname[i]
	  <<"\n"<<setw(w)<<"frameid: "         <<setw(w)<<this->GetFrameID(i)
	  <<"\n"<<setw(w)<<"divideid: "        <<setw(w)<<this->GetDivideID(i)<<endl;
    }
  }
    
  cout<<" MyCrossSection::Print: <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<"<<endl;
}


bool MyCrossSection::file_exists(const string& s) {
  //
  // Helper to make sure a file exists
  //

  if ( FILE* testfile=fopen(s.c_str(),"r") ) {
    fclose(testfile);
    return true;
  }
  else return false;
}


void MyCrossSection::Normalise(TH1D* h1, double yscale, double xscale=1., bool normtot=false)
{
  //
  // reference is divided by binwidth
  //

  double x, y, ey;
  double sigtot=0.;

  for (int i=0; i<=h1->GetNbinsX(); i++) {
    y=h1->GetBinContent(i)*yscale;
    x=h1->GetBinWidth(i);
    sigtot+=y*x;
  }

  //
  //if (debug) cout<<" MyCrossSection::Normalize: sigtot= "<<sigtot<<endl;

  for (int i=0; i<=h1->GetNbinsX(); i++) {
    x =h1->GetBinWidth(i);
    y =h1->GetBinContent(i)*yscale*x;
    ey=h1->GetBinError(i)  *yscale*x;
    x =h1->GetBinWidth(i)  *xscale;
    if (x!=0) h1->SetBinContent(i,y/x);
    else      h1->SetBinContent(i,0.);
    //cout << "BinWidth: " << h1->GetBinWidth(i)  << ", xscale: " << xscale 
    //     << ", x: " << x << ", bin content: " << h1->GetBinContent(i) << "\n";

    if (x!=0) h1->SetBinError(i,ey/x);
    else      h1->SetBinError(i,0.);
  }

  if (normtot) {
    //if (debug) cout<<" MyCrossSection::Normalise: sigtot= "<<sigtot<<endl;
    if (sigtot!=0.) {
      //if (debug) std::cout << " MyCrossSection::Normalise: What is the name? " << h1->GetName() << std::endl;
      //if (debug) std::cout << " MyCrossSection::Normalise: New norm = " << h1->Integral() << std::endl;
    }
    h1->Scale(1. / sigtot);
  }

  /*
    for ( int ibin=1 ; ibin<=h->GetNbinsX() ; ibin++ )
    {
    double width = h->GetBinLowEdge(ibin+1) - h->GetBinLowEdge(ibin);
    h->SetBinContent( ibin, h->GetBinContent(ibin)/width );
    }
  */
  //if (debug) std::cout << " MyCrossSection::Normalise: return" << std::endl;
  
  return;
}


void MyCrossSection::Normalise(TGraphAsymmErrors* g1, double yscale, double xscale=1., bool normtot=false, bool divbinwidth=true) {
  //
  // Normalise a graph
  //

  // convolution is divided by binwidth
  Double_t x, y, ey;
  Double_t sigtot=0.;

  Double_t* X1 = g1->GetX();
  Double_t* Y1 = g1->GetY();
  Double_t* EXhigh1 = g1->GetEXhigh();
  Double_t* EXlow1  = g1->GetEXlow();
  Double_t* EYhigh1 = g1->GetEYhigh();
  Double_t* EYlow1  = g1->GetEYlow();

  if (!g1) cout<<" MyCrossSection::Normalise graph not found ! "<<endl;
  int Nbin=g1->GetN();
  //cout<<" MyCrossSection::Normalize: n= "<<Nbin<<endl;

  for (Int_t i=0; i<Nbin; i++) {
    y=Y1[i]*yscale;
    double binw=EXhigh1[i]+EXlow1[i];
    //cout<<" binw= "<<binw<<endl;
    if (divbinwidth) sigtot+=y*binw;
    else             sigtot+=y;
    sigtot+=y;
  }

  //if (debug) cout<<"MyCrossSection::Normalise: sigtot= "<<sigtot<<endl;

  double scal=yscale;
  for (Int_t i=0; i<Nbin; i++) {
    if (normtot) scal=yscale/sigtot;
    double binw=1.;
    if (divbinwidth) binw=EXhigh1[i]+EXlow1[i];
    //if (debug) cout<<" MyCrossSection::Normalise: i= "<<i<<" scal= "<<scal<<endl;

    double y =Y1[i]*scal*binw;
    double yl=EYlow1[i] *scal*binw;
    double yh=EYhigh1[i]*scal*binw;
    if (divbinwidth) {
      binw*=xscale;
      yl/=binw;  
      yh/=binw;
      y /=binw;
 
    }
    g1->SetPoint(i, X1[i],y);
    g1->SetPointError(i,EXlow1[i],EXhigh1[i],yl,yh);
  }

  //if (debug) std::cout << " MyCrossSection::Normalise: return" << std::endl;
  return;
}


TH1D *MyCrossSection::GetReference(int igrid) {
  //
  // get reference histogram from original grid
  // 

  string fname=this->GetGridName(igrid);
  if (debug) cout<<" MyCrossSection::GetReference: Grid Name "<<fname.c_str()<<endl;
  appl::grid *tmpgrid = 0;
  if (this->file_exists(fname)) {
    //if (debug) cout<<" MyCrossSection::GetReference: grid file "<<fname<<" exist ! "<<endl;
    //isBooked.push_back(false);
    if (debug) cout<<" MyCrossSection::GetReference: open "<<fname<<endl;
    tmpgrid = new appl::grid(fname);
    //if (debug) cout << " MyCrossSection::GetReference: grid CMS energy " 
    //                << tmpgrid->getCMSScale() << endl;
    //mygrid.push_back(tmpgrid);
    //if (debug) cout << " MyCrossSection::GetReference: tmpgrid pushed" << endl;
    //events.push_back(int(mygrid[igrid]->run()));
    //if (debug) cout << " MyCrossSection::GetReference: events pushed" << endl;
  } else {
    cout<<" MyCrossSection::GetReference:  file not found "<<fname.c_str()<<endl;
  }

  if (!tmpgrid) 
    cout<<" MyCrossSection::GetReference: tmpgrid not found ! "<<endl;

  TH1D *href=(TH1D*)tmpgrid->getReference();
  if (!href) {
    cout<<" MyCrossSection::GetReference: reference histo not found igrid= "<<igrid<<endl;
    cout<<" MyCrossSection::GetReference: grid name=" << this->GetGridName(igrid) << endl;
    exit(0);
  }
  //if (debug) cout <<" MyCrossSection::GetReference Got reference" <<endl;
  int ntot=tmpgrid->run();
  //if (debug) cout<<" MyCrossSection::GetReference ntot= "<<ntot << endl;
  href->Scale(1./ntot);

  return href;
}


void MyCrossSection::Draw(int igrid) {
  //
  // Draw data for a grid by index
  //

  if (debug) cout << " MyCrossSection::Draw igrid " << igrid << endl;
  mydata[igrid]->DrawData();
  if (debug) mydata[igrid]->Print();
  return;
}


TGraphAsymmErrors *MyCrossSection::GetPDFRatio(int iset1, int iset2, int itype, int igrid) {
  //
  // Compute and return the ratio of two graphs 
  //
 
  //MyPDF * mypdf=t_mypdf[igrid][itype];
  TGraphAsymmErrors * mypdf=myband.at(igrid)->GetPdfBand(itype);
 
  //TString ratio_to_ref_name = (TString) gpdfband[igrid][itype]->GetName() + "_set";
  TString ratio_to_ref_name = (TString) mypdf->GetName() + "_set";
  ratio_to_ref_name +=iset1;

  if (igrid>=this->GetNGrid()){
    cout<<" MyCrossSection::GetPDFRatio igrid== "<< igrid<<endl;
    exit (0);
  }

  // if (itype>=this->GetNPDF()){ // not correct for two dim vector
  if (itype>=this->GetNPDF(igrid)){ // not correct for two dim vector
    cout<<" MyCrossSection::GetPDFRatio  "<<" itype= "<<itype<<endl;
    exit (0);
  } 


  int numPDF= this->GetNPDF(igrid);
  if (iset1>=numPDF||iset2>=numPDF||iset1<0||iset2<0){
    cout<<" MyCrossSection::GetPDFRatio iset too large iset2= "<< iset2<<" iset1= "<<iset1<<endl;
    exit (0);
  }

  if (iset2!=0) {
    ratio_to_ref_name +=TString("/set");
    ratio_to_ref_name +=iset2;
  }
  if (debug) cout<<" MyCrossSection::GetPDFRatio: "<< ratio_to_ref_name.Data()<<endl;


  // Flip ratio calculations if steering asks for it
  //int top = -1, bot = -1;
  //if(ratioTheoryOverData) {
  // top = iset1; //origional
  // bot = iset2; //origional
  //} else {
  // top = iset2;
  // bot = iset1;
  //}
  // TGraphAsymmErrors* gpdfratio=myTGraphErrorsDivide(myband.at(igrid)->GetPdfBand(top),
  //						    myband.at(igrid)->GetPdfBand(bot),2);

  TGraphAsymmErrors* gpdfratio=myTGraphErrorsDivide(myband.at(igrid)->GetPdfBand(iset1),
						    myband.at(igrid)->GetPdfBand(iset2),2);
  gpdfratio->SetLineColor(markercolor[igrid]);
  gpdfratio->SetMarkerColor(markercolor[igrid]);
  gpdfratio->SetName(ratio_to_ref_name);
  
  return gpdfratio;
}


TGraphAsymmErrors *MyCrossSection::GetPDFTypeRatio(int iset1, int iset2, int igrid) {
  //
  //
  //

  TGraphAsymmErrors * mypdf=myband.at(igrid)->GetPdfBand(iset2);
  //TString ratio_to_ref_name = (TString) gpdfband[igrid][iset2]->GetName() + "_set";
  TString ratio_to_ref_name = (TString) mypdf->GetName() + "_set";
  ratio_to_ref_name +=iset1;

  int numPDF= this->GetNPDF(igrid);
  if (iset1>=numPDF||iset2>=numPDF||iset1<0||iset2<0){
    cout<<" MyCrossSection::GetPDFTypeRatio iset too large iset2= "<< iset2<<" iset1= "<<iset1<<endl;
    exit (0);
  }

  if (iset2!=0) {
    ratio_to_ref_name +=TString("/set");
    ratio_to_ref_name +=iset2;
  }
  if (debug) cout<<" MyCrossSection::GetPDFTypeRatio: "<< ratio_to_ref_name.Data()<<endl;


  // Flip ratio calculations if steering asks for it
  //int top = -1, bot = -1;
  //if(ratioTheoryOverData) {
  //  top = iset1; bot = iset2; //origional
  //} else {
  //  top = iset2; bot = iset1;
  //}

  TGraphAsymmErrors* gpdfratio=myTGraphErrorsDivide(myband.at(igrid)->GetPdfBand(iset1),
						    myband.at(igrid)->GetPdfBand(iset2),2);
  gpdfratio->SetLineColor(markercolor[igrid]);
  gpdfratio->SetMarkerColor(markercolor[igrid]);
  gpdfratio->SetName(ratio_to_ref_name);
 
  return gpdfratio;
}


void MyCrossSection::DrawinFrame(int iframe) {
  //
  // Draw data overlay and ratio for either data and grid reference or PDFs
  //

  MyFrame *myframe= new MyFrame(600,600);
  framepointer.push_back(myframe);

  double x,y;
  TString name="";

  int iplotdata=0;

  //int jframe=this->GetFrameID(iframe);
  if (debug) cout<<" MyCrossSection::DrawInFrame: gridinframe["<<iframe<<"].size()= "
		 <<gridinframe[iframe].size()<<endl;

  for (int i=0; i<gridinframe[iframe].size(); i++) {
    int igrid=gridinframe[iframe][i];
    int jframe=this->GetFrameID(igrid);
    if (iframe!=jframe) continue;
    if (debug) cout<<" MyCrossSection::DrawInFrame: i= "<<i
		   <<" igrid= "<<igrid<<" iframe= "<<iframe<<endl;

    //if (gridinframedata[iframe][igrid]>0){
    if (mydata[igrid]>0){

      //name=gridinframedata[iframe][igrid]->GetDataName();
      //if (debug) cout<<" MyCrossSection::DrawInFrame: gridinframedata["<<iframe<<"]["<<igrid<<"]="
      //          <<name.Data()<<endl;

      //if (debug) cout<<" MyCrossSection::DrawInFrame: before igrid iframe "<<endl;
      //cout<<" name= "<<gridinframedata[igrid][iframe]->GetDataName()<<endl;

      //if (debug) cout<<" MyCrossSection::DrawInFrame: before iframe igrid "<<endl;
      //cout<<" MyCrossSection::DrawInFrame:  name= "<<gridinframedata[iframe][igrid]->GetDataName()<<endl;

      //if (debug) cout<<" MyCrossSection::DrawInFrame: before print "<<endl;
      //gridinframedata[iframe][igrid]->Print();
      //if (debug) cout<<" MyCrossSection::DrawInFrame: after print "<<endl;
      //if (gridinframedata[iframe][igrid]->GetLogY1()) {
      if (mydata[igrid]->GetLogY1()) {
	//if (debug) cout<<" MyCrossSection::DrawInFrame: before setting myframe LogY myframe="<<myframe<<endl;
	myframe->SetLogY1();
      }
      //if (gridinframedata[iframe][igrid]->GetLogX ()) {
      if (mydata[igrid]->GetLogX ()) {
	//if (debug) cout<<" MyCrossSection::DrawInFrame: before setting myframe log X myframe= "<<myframe<<endl;
	myframe->SetLogX();
      }
    }
  }    /// loop over grid frames

  if (!myframe) cout<<" MyCrossSection::DrawinFrame: myframe not found ! "<<endl;
  if (gridinframe[iframe].size()<1) 
    cout<<" MyCrossSection::DrawInFrame: gridinframe[iframe].size()  too small ! "<<endl;

  //if (debug) cout<<" MyCrossSection::DrawInFrame: before setting igrid= "<<igrid<<" iframe= "<<iframe<<endl;
  int igrid=gridinframe[iframe][0];
  if (debug) cout<<" MyCrossSection::DrawInFrame: plotting igrid= "<<igrid<<" in iframe= "<<iframe<<endl;

  TString titx="", tity="";
  if (this->GetDataOk(igrid)) {
    //titx=gridinframedata[iframe][igrid]->GetTitleX();
    //tity=gridinframedata[iframe][igrid]->GetTitleY();
    //name=gridinframedata[iframe][igrid]->GetDataName();
    titx=mydata[igrid]->GetTitleX();
    tity=mydata[igrid]->GetTitleY();
    name=mydata[igrid]->GetDataName();
    name+=iframe;
  } else {
    titx=this->GetGridName(igrid);
    //tity=this->GetGridName(igrid);
    name+=this->GetGridName(igrid);
    name+=iframe;
  }

  myframe->SetFrameName(name); //TEST
  myframe->SetSubPad2TitleX(titx);
  myframe->SetSubPad1TitleY(tity);

  double xmin=  xminframe[iframe];
  double xmax=  xmaxframe[iframe];
  double ymin=  yminframe[iframe];
  double ymax=  ymaxframe[iframe];

  if (debug)
    cout<<" MyCrossSection::DrawInFrame: frame= "<<iframe<<" xmin= "
	<<xmin<<" xmax= "<<xmax<< " ymin= "<<ymin<<" ymax= "<<ymax<<endl;


    
  //allow for fixed Y-axis min/max for overlay pad if steering file provided
  if( yminoverlay != DEFAULT_DOUBLE ) ymin = yminoverlay;
  if( ymaxoverlay != DEFAULT_DOUBLE ) ymax = ymaxoverlay;
  
  //ensure user provided appropreate bounds (ymax must be > ymin)
  if( ymin > ymax) {
    cout<<" MyCrossSection::DrawinFrame: ERROR: Invalid overlay max and minimums set!"
	<<" ymin = '"<<ymin<<"'"
	<<" ymax = '"<<ymax<<"'"<<endl;
    exit(0); //Do no further drawing if the steering file was set incorrectly
  }



  //set appropreate xmin/max and ymin/max based on data
  myframe->SetXmin( xmin);
  myframe->SetXmax( xmax);
  myframe->SetYmin1(ymin);
  myframe->SetYmax1(ymax);
  myframe->SetYmin2(1.1);
  myframe->SetYmax2(0.9);
  myframe->DrawFrame();



  //if (debug) cout<<" SubPad = "<< myframe->GetSubPad1()<<endl;
  myframe->GetSubPad1()->cd();

  if (debug)
    cout<<" MyCrossSection::DrawinFrame: number of grids= "<<gridinframe[iframe].size()
	<<" for frame= "<<iframe<<endl;
 
  // const int npdf=gpdfband[igrid].size();
  //const int npdf=myband[igrid]->GetNPDF();
  const int npdf=pdfdata.size();
  if (npdf<1) {
    cout<<" MyCrossSection::DrawinFrame: No PDF information found ...plotting reference"<<endl;
  }


  //compute an optimal position for the legend for this plot
  y=ylegend;
  int namesize=0;
  if (npdf<1) {
    namesize=5; //why choose 5 as default?
    
    TString mylabel=mydata[igrid]->GetLabel();
    int mysize=TString(mylabel).Sizeof(); 
    if (mysize>namesize) namesize=mysize;
    //y-=0.1;
  } else {
    //y-=0.07*npdf; //origional
    for (int ipdf=0; ipdf<npdf; ipdf++) {
      if (debug) cout<<" MyCrossSection::DrawinFrame: label= "
		     <<this->GetPdfName(ipdf).Data()<<endl;
     
      int mysize=this->GetPdfName(ipdf).Sizeof(); 
      if (mysize>namesize) namesize=mysize;
    }
    if (plotchi2) namesize+=10;
  }

  if (debug) cout<<" MyCrossSection::DrawinFrame: xlegend= "<<xlegend
		 <<" ylegend= "<<ylegend<<endl;

  //origional
  //  double x1=xlegend, x2=0.95;
  //  double y1=y, y2=ylegend;
  //  x2+=namesize*0.02;
 

  // place legend based on the YTop and XRight position and set xLeft and YBottom by:
  //  XLeft = the length of the largest name in the legend 
  //  YBottom = the number of entries in the legend
  double x1 = xlegend-(namesize*0.02), x2=xlegend;
  double y1 = 0, y2=ylegend;
  
  
  // ydimention is based of what elements are being put in legend.
  if(npdf<1) {
    // size based on no PDFs meaning legend is full of just grid data
    y1 = ylegend-0.2;
  } else {
    // size based on number of pdfs
    y1 = ylegend-(npdf*0.11);
  }
  
  
  if (debug) cout<<" MyCrossSection::DrawinFrame: namesize="<<namesize<<", npdf="<<npdf
		 <<", x1="<<x1<<", x2="<<x2<<", y1="<<y1<<", y2="<<x2<<std::endl;


  // prepare legend for this Plot
  TLegend *leg = new TLegend(x1,y1,x2,y2);
  leg->SetBorderSize(0);
  leg->SetFillColor(0);
  leg->SetMargin(0.1);
  

  const double BIG=1.36;
  double Ymin=BIG, Ymax=-BIG, Xmin=BIG, Xmax=-BIG;
  int iframeRepeatCheck = -1; // turn on or off certain features if they are being done in the same frame twice

  for (int i=0; i<(int)gridinframe[iframe].size(); i++) {
    int igrid=gridinframe[iframe][i];
    int jframe=this->GetFrameID(igrid);
    if (iframe!=jframe) {
      //cout<<"TEST: NOT EQUAL: iframe:"<<iframe<<", jframe:"<<jframe<<endl; //TODO - check does this do anything?
      continue;
    } else {
      //cout<<"TEST: EQUAL, SKIPPING: iframe:"<<iframe<<", jframe:"<<jframe<<endl;
    }

    if (debug) cout<<" MyCrossSection::DrawinFrame: for i= " << i << " igrid= "<<igrid<<endl;
    TH1D* href=0;
    //cout << "Is data ok? " << this->GetDataOk(igrid) << endl;



    //plot the overlay portion of graph
    if (npdf<1) { // NO PDFs
      href=this->GetNormalisedReference(igrid);
      if (!href) cout<<" MyCrossSection::DrawinFrame: reference not found ! "<<endl;
      
      /*
      if(mydata[igrid]->GetScalex() != 1.0 || mydata[igrid]->GetScaley() != 1.0 ) {
      	if(debug) cerr<<" MyCrosSection::DrawinFrame: WARN: Forcefully scaling reference histo. "
		      <<"\n\txscale: "<<mydata[igrid]->GetScalex()
		      <<"\n\tyscale: "<<mydata[igrid]->GetScaley()<<endl;
      	
      	//set theory's scaling to the same as data's
      	href->Scale( mydata[igrid]->GetScaley() ); //TODO - include checking for scalex?
      }
      */
      
      //no PDF, plot reference histogram for overlay
      href->Draw("same,hist"); //no PDF overlay draw
      if (debug) {
	cout<<" MyCrossSection::DrawinFrame: print reference histogram"<<endl;
	href->Print("all");
      }
    } else { //PDFs provided
      if (debug) cout<<" MyCrossSection::DrawinFrame: draw band igrid= "<<igrid<<endl;
      
      //draw each PDF band on top pad of plot for overlay
      myband.at(igrid)->DrawPDFBand();
    }


    // Draw data to the frame and add data name to the legend
    if (this->GetDataOk(igrid)) {
      if (!mydata[igrid]) cout<<" MyCrossSection::DrawinFrame: mydata["<<igrid<<"] not found "<<endl;
      else if (debug)     cout<<" MyCrossSection::DrawinFrame: mydata["<<igrid<<"]  found "<<endl;

   
      // Only plot data once (ignore repeats) unless repeated data is on a new frame
      if ( DoubleDataSetName(igrid) == false || iframeRepeatCheck != iframe ) { 
	//repeated data was not found on that frame
	iframeRepeatCheck = iframe;
	
	if (debug) cout<<" MyCrossSection::DrawinFrame: plot data "<<dataname[igrid]<<endl;
       	// any scaling of data is already done in the MyData class from it's own steering
	mydata[igrid]->DrawData(); // plot actual data

	
	TString mylabel=mydata[igrid]->GetLabel();	
	if (debug) cout<<" MyCrossSection::DrawinFrame: mylabel= "<<mylabel.Data()<<endl;
	
	// label sqrts before data on legend if MyData Steering requested it
	if(mydata[igrid]->GetPlotSqrts())  {
	  TString sqrtslabel = "";
	  double sqrtsval = mydata[igrid]->GetSQRTS() * GetUnitScaleX(mydata[igrid]->GetXUnits(), this->GetXUnits()); //new
	  TString sqrtsunits = mydata[igrid]->GetXUnits();

	  sqrtslabel.Form("#sqrt{s}= %0.0f %s", 
			  sqrtsval, 
			  sqrtsunits.Data() ); //TODO - too hardcoded? make generic
	  leg->AddEntry((TObject*)0, sqrtslabel, "");
	}
	
	//add the data label to the legend
	leg->AddEntry(mydata[igrid]->GetTGraphTotErr(),mylabel,"ep");
	
      }
    } else { //Data not OK
      href=this->GetReference(igrid);
      if (debug) cout<<" MyCrossSection::DrawinFrame: Data not ok igrid= "<<igrid<<endl;
      if (debug) cout<<" MyCrossSection::DrawinFrame: now draw reference igrid= "<<igrid<<endl;
      if (!href){cout<<" MyCrossSection::DrawinFrame: hreference not found "<<endl; exit;}
      href->Draw("hist,same");
    }
    

    if (debug) cout<<" MyCrossSection::DrawinFrame: npdf= "<<npdf<<endl;    
    if (debug) cout<<" MyCrossSection::DrawinFrame: vpdfchi2.size()= "<< vpdfchi2.size()<<endl;
    if (debug) cout<<" MyCrossSection::DrawinFrame: vpdfchi2.at("<<igrid<<").size()= "<< vpdfchi2.at(igrid).size()<<endl;


    //All data names should be added to the legend before PDF names
    if(this->GetFrameID(igrid) != this->GetFrameID(igrid+1) ) {
      for (int ipdf=0; ipdf<npdf; ipdf++) {
	
	//retrieve a PDF name
	TString pdfname="";
	pdfname+= this->GetPdfName(ipdf);
	
	
	if (plotchi2) {
	  //pdfname+=" #C^2 ";
	  pdfname+=" Chi2= ";
	  char tmp[30];
	  sprintf(tmp,"%5.2f",vpdfchi2.at(igrid).at(ipdf));
	  pdfname+=TString(tmp);
	}
	if (debug) cout<<" MyCrossSection::DrawinFrame: pdfname= "<<pdfname.Data()<<endl;
	
	
	//Before adding PDF titles to legend, create a title for the PDFs within the legend
	if (ipdf==0 ) { 
	  TString pdfDataTitle="NLO QCD with:"; //"subtitle" for the following PDFs on the legend
	  leg->AddEntry((TObject*)0, pdfDataTitle.Data(), "");
	}
	
	//Add PDF titles to legend
	if (plotmarker) leg->AddEntry(myband.at(igrid)->GetPdfBand(ipdf),pdfname,"p");
	else            leg->AddEntry(myband.at(igrid)->GetPdfBand(ipdf),pdfname,"f");	
      }      
    }
    

    // when no pdfs, provide reference histogram info after each data name in legend
    if (npdf<1) { 
      cout<<" MyCrossSection::DrawinFrame: No PDF's requested: Adding references to legend."<<endl;
      
      // create a nice legend name for reference histos
      TString curLegLable = "";
      if (leglabel.size()>0) curLegLable = leglabel[igrid].c_str();
      else                   curLegLable = "reference";
      
      if (debug) cout<<" MyCrossSection::DrawinFrame: Added '"<<curLegLable<<"' to legend."<<endl;	
      leg->AddEntry(href, curLegLable, "l");
    }	    
    

    // Draw the correctly filled legend
    leg->Draw();
    
    if (debug) cout<<" MyCrossSection::DrawinFrame: legend prepared "<<endl;
  }



  // RATIO DRAWING
  myframe->GetSubPad2()->cd();
  myframe->SetSubPad2TitleOffsetX(0.8);

  // setup ratio labels
  TString titlename="";
  if(ratioTheoryOverData) titlename="NLO QCD/Data";
  else                    titlename="Data/NLO QCD";

  if (ratiotitlelabel.size()>0) titlename=ratiotitlelabel;

  if (!this->GetDataOk(igrid)) titlename="";
  myframe->GetYAxis2()->SetTitle(titlename.Data());

  if (debug) cout<<" MyCrossSection::DrawinFrame: npdf= "<<npdf<<endl;


  // determine ratio view size
  for (int i=0; i<(int)gridinframe[iframe].size(); i++) {
    int igrid=gridinframe[iframe][i];
    int jframe=this->GetFrameID(igrid);
    if (iframe!=jframe) continue;

    if (debug) cout<<" MyCrossSection::DrawinFrame: gridinframe["<<iframe<<"]["<<i<<"] = " 
		   << gridinframe[iframe][i] 
		   << " Draw reference for igrid= "<<igrid<<endl;
    

    /*
    //plot just the data if no pdfs are to be overlayed
    if (npdf<1) {
      ratiotot[igrid]->ComputeRange(xmin,ymin,xmax,ymax);
      if (ymin<Ymin) Ymin=ymin;
      if (ymax>Ymax) Ymax=ymax;	
    } else {
      myband[igrid]->ComputePDFBandRatioRange();
      Ymin=myband[igrid]->GetYmin();
      Ymax=myband[igrid]->GetYmax();
    }
    */

    
    if(ratioTheoryOverData) {
      if (npdf<1) {
	ratiotot[igrid]->ComputeRange(xmin,ymin,xmax,ymax);
	if (ymin<Ymin) Ymin=ymin;
	if (ymax>Ymax) Ymax=ymax;	
      } else {
	myband[igrid]->ComputePDFBandRatioRange();
	Ymin=myband[igrid]->GetYmin();
	Ymax=myband[igrid]->GetYmax();
      }
    } else {
	TH1D *href=this->GetNormalisedReference(igrid);
	TGraphAsymmErrors* gref=TH1TOTGraphAsymm(href);
	TGraphAsymmErrors *tmp = myTGraphErrorsDivide(mydata[igrid]->GetTGraphTotErr()
						     ,gref,2);
	tmp->ComputeRange(xmin,ymin,xmax,ymax);
	if (ymin<Ymin) Ymin=ymin;
	if (ymax>Ymax) Ymax=ymax;
    }
    



    if (debug) cout<<" MyCrossSection::DrawinFrame: Ymin= "<<Ymin<<" Ymax= "<<Ymax<<endl;
    /*
      for (int ipdf=0; ipdf<npdf; ipdf++) {
      gpdfbandratiotot[igrid][ipdf]->ComputeRange(xmin,Ymin,xmax,Ymax);
      if (ymin<Ymin) Ymin=ymin;
      if (ymax>Ymax) Ymax=ymax;
      }
    */
  }

    


  //allow for explicit fixed Y-axis min/max for ratio pad if steering file provided them
  if( yminratio != DEFAULT_DOUBLE ) Ymin = yminratio;
  if( ymaxratio != DEFAULT_DOUBLE ) Ymax = ymaxratio;
  
  //ensure user provided appropreate bounds (ymax must be > ymin)
  if( Ymin > Ymax ) { 
    cerr<<" MyCrossSection::DrawinFrame: ERROR: Invalid ratio max and minimums set!"
	<<" Ymin = '"<<Ymin<<"'"
	<<" YmX = '"<<Ymax<<"'"<<endl;
    exit(0); //Do no further drawing if the steering file was set incorrectly
  }
 

  //Set computed min and max on Yaxis with small buffer on top and bottom for visibility
  if (debug) cout<<" MyCrossSection::DrawinFrame: Ymin= "<<Ymin<<" Ymax= "<<Ymax<<endl;
  myframe->GetYAxis2()->SetRangeUser(Ymin*0.9,Ymax*1.1); //<--Do this per each because each run is chaning the min and max??


  // Draw ratio onto plots
  TGraphAsymmErrors* ratiodata;  
  if(ratioTheoryOverData) { // (THEORY / DATA)
    // draw data ratio (background bands) before pdf ratio (relative to bands)


    //FIRST - draw data ratio to itself in background    
    for (int i=0; i<(int)gridinframe[iframe].size(); i++) {
      int igrid=gridinframe[iframe][i];
      if(debug) cout<<" MyCrossSection::DrawinFrame: computing (theory/data) ratio."<<endl;
      
      ratiodata=myTGraphErrorsDivide(mydata[igrid]->GetTGraphTotErr()
				     ,mydata[igrid]->GetTGraphTotErr(),2);
      if (!ratiodata) cout<<" MyCrossSection::DrawinFrame: ratiodata not found ! "<<endl;
      ratiodata->SetMarkerStyle(mydata[igrid]->GetTGraphTotErr()->GetMarkerStyle());
      ratiodata->SetMarkerColor(mydata[igrid]->GetTGraphTotErr()->GetMarkerColor());
      ratiodata->SetMarkerSize(0);
            
      // provide different data in ratio section should have different color
      ratiodata->SetFillColor(kGray); //TODO - Choose a more predictable color based on data color?
      
      //draw the ratio of (theory/data)
      if (debug) std::cout<<" MyCrossSection::DrawinFrame: Drawing ratiodata for '"<<igrid<<"'"<<std::endl;      
      ratiodata->Draw("E2,same");
    } 


    //SECOND - Draw the reference or pdf ratio over the data ratio in forground
    for (int i=0; i<(int)gridinframe[iframe].size(); i++) {
      int igrid=gridinframe[iframe][i];
      if (debug) cout<<" MyCrossSection::DrawinFrame: DrawPDFBandRatio igrid= "<<igrid<<endl;
      
      // no pdf means only need to display each set of ratios overlayed to the data
      if (npdf<1) {
	ratiotot[igrid]->SetMarkerStyle(0); //circle
	ratiotot[igrid]->SetMarkerSize(0); //don't show the plotted point
	
	//PDF ratio drawing needs no scaling because it was generated from scaled data
	ratiotot[igrid]->Draw("p,same"); 
      } 
            
      //draw the PDF ratio data
      myband[igrid]->DrawPDFBandRatio();
    }
    // finished drawing ratio (theory/data)

  } else { // (DATA / THEORY)
    // draw pdf ratio (background bands) before data ratio (relative to bands)

    //FIRST - Draw the reference or pdf ratio to itself in the background 
    for (int i=0; i<(int)gridinframe[iframe].size(); i++) {
      int igrid=gridinframe[iframe][i];
      
      // no pdf means only need to display each set of ratios overlayed to the data
      if (npdf<1) {
		
	//if(mydata[igrid]->GetScalex() != 1.0 || mydata[igrid]->GetScaley() != 1.0 ) {
	//  if(debug) cerr<<" MyCrosSection::DrawinFrame: WARN: Forcefully scaling ratiotot. "<<endl;	
	//  //set reference ratio scaling to the same as data's
	//  ScaleGraph(ratiotot[igrid], mydata[igrid]->GetScalex(), mydata[igrid]->GetScaley() ); //TODO
	//}
	
	ratiotot[igrid]->SetMarkerStyle(0); //circle
	ratiotot[igrid]->SetMarkerSize(0); //don't show the plotted point
	//ratiotot[igrid]->SetFillColor(refhistlinecolor[igrid]);
	//ratiotot[igrid]->SetFillStyle(3004+igrid);

	// TODO - check problem with reference uncertainy seeming large?
	ratiotot[igrid]->Draw("E2,same"); //no PDF ratio draw
      } 
    }
    

    //TODO - Move up into FIRST section for loop for each igrid?
    if (debug) cout<<" MyCrossSection::DrawinFrame: DrawPDFBandRatio igrid= "<<igrid<<endl;
    //draw the PDF ratio data
    myband[igrid]->DrawPDFBandRatio();
    
    
    //SECOND - Draw the data ratio over the pdf or reference histo ratio in the forground
    if(debug) cout<<" MyCrossSection::DrawinFrame: computing (data/theory) ratio(s) for each pdf."<<endl;
    for (int i=0; i<(int)gridinframe[iframe].size(); i++) {
      int igrid=gridinframe[iframe][i];

      if(npdf<1) {
	// when no pdfs, plot the data over reference histogram
	TH1D *href=this->GetNormalisedReference(igrid);
	TGraphAsymmErrors* gref=TH1TOTGraphAsymm(href);
	
	ratiodata=myTGraphErrorsDivide(mydata[igrid]->GetTGraphTotErr(),gref,0);
	if (!ratiodata) cout<<" MyCrossSection::DrawinFrame: ratiodata not found ! "<<endl;
	ratiodata->SetMarkerStyle(mydata[igrid]->GetTGraphTotErr()->GetMarkerStyle());
	ratiodata->SetMarkerColor(mydata[igrid]->GetTGraphTotErr()->GetMarkerColor());
	ratiodata->SetMarkerSize(mydata[igrid]->GetTGraphTotErr()->GetMarkerColor());

	//ratiodata->SetLineColor(gref->GetMarkerColor()); 
	ratiodata->SetLineColor(kBlack);  //errorbar is black for a single pdf
	
	//draw the ratio of (data/theory) for each theory
	if (debug) std::cout<<" MyCrossSection::DrawinFrame: Drawing ratiodata for '"<<igrid<<"'"<<std::endl;      
	ratiodata->Draw("P,same");      
      }
      
      for(int ipdf = 0; ipdf < npdf; ipdf++) {
	// when there are pdfs, plot data over pdf band data
	ratiodata=myTGraphErrorsDivide(mydata[igrid]->GetTGraphTotErr()
				       ,myband[igrid]->GetPdfBand(ipdf),2);
		
	if (!ratiodata) cout<<" MyCrossSection::DrawinFrame: ratiodata not found ! "<<endl;
	ratiodata->SetMarkerStyle(mydata[igrid]->GetTGraphTotErr()->GetMarkerStyle());
	ratiodata->SetMarkerColor(mydata[igrid]->GetTGraphTotErr()->GetMarkerColor());
	ratiodata->SetMarkerSize(mydata[igrid]->GetTGraphTotErr()->GetMarkerColor());

	// color of ratio is chosen for most readability
	if(npdf==1) ratiodata->SetLineColor(kBlack);  //errorbar is black for a single pdf
	else        ratiodata->SetLineColor(myband[igrid]->GetPdfBand(ipdf)->GetMarkerColor());  //errorbar color depends on pdf band color	

	//draw the ratio of (data/theory) for each theory
	if (debug) std::cout<<" MyCrossSection::DrawinFrame: Drawing ratiodata for '"<<igrid<<"'"<<std::endl;      
	ratiodata->Draw("P,same");
      }
    }
    // finished drawing ratio (data/theory)
  }
  
  

  //Always plot a line at 1, where the ratios are relative to
  TLine *myline= new TLine(myframe->GetXmin(),1.,myframe->GetXmax(),1.);
  myline->Draw(); 

  myframe->GetSubPad1()->RedrawAxis();
  myframe->GetSubPad2()->RedrawAxis();

  if (debug) cout << " MyCrossSection::DrawInFrame: End of DrawInFrame" << endl;
  return;
}

void MyCrossSection::DrawReference(int igrid) {
  //
  // Draw reference histograms for grid with index igrid
  //
  TH1D *href=this->GetReference(igrid);
  if (!href) cout<<" MyCrossSection::DrawReference: reference histo not found igrid="<<igrid<<endl;
  //href->Print("all");
  href->SetLineStyle(refhistlinestyle[igrid]);
  href->SetLineColor(refhistlinecolor[igrid]);
  href->Draw("same");
  return;
}

TH1D* MyCrossSection::GetNormalisedReference(int igrid) {
  //
  // Get reference histograms for grid with index igrid
  // and normalise according to datascalex and scaley
  //
  TH1D *href=this->GetReference(igrid);
  if (!href) {
    cout<<" MyCrossSection::GetNormalisedReference: reference histo not found igrid="<<igrid<<endl;
    return 0;
  }

  href->SetLineStyle(refhistlinestyle[igrid]);
  href->SetLineColor(refhistlinecolor[igrid]);

  if (this->GetDataOk(igrid)) {
    //double xscale=mydata[igrid]->GetUnitGeVFactor(); //old scaling
    //double yscale=mydata[igrid]->GetUnitfbFactor(); //old scaling
   
    //convert units from data to desired
    double xscale = GetUnitScaleX(mydata[igrid]->GetXUnits(), this->GetXUnits()); //new scaling
    double yscale = GetUnitScaleY(mydata[igrid]->GetYUnits(), this->GetYUnits()); //new scaling
    
    bool  normtot=mydata[igrid]->isNormTot();


    if(mydata[igrid]->GetScalex() != 1.0) xscale += mydata[igrid]->GetScalex();
    if(mydata[igrid]->GetScaley() != 1.0) yscale += mydata[igrid]->GetScaley();


    this->Normalise(href,yscale,xscale,normtot);


    if(mydata[igrid]->GetScalex() != 1.0 || mydata[igrid]->GetScaley() != 1.0 ) {
      if(debug) cerr<<" MyCrosSection::DrawinFrame: WARN: Forcefully scaling reference histo. "
		    <<"\n\txscale: "<<mydata[igrid]->GetScalex()
		    <<"\n\tyscale: "<<mydata[igrid]->GetScaley()<<endl;
      
      //set theory's scaling to the same as data's
      href->Scale( mydata[igrid]->GetScaley() ); //TODO - include checking for scalex?
    }
  }

  

  

  
  return href;
}

/*
  TGraphAsymmErrors* MyCrossSection::GetReferenceRatio(int igrid) {
  //
  // Get reference histograms for grid with index igrid
  // and divide data associated to grid
  //
  TH1D *href=this->GetReference(igrid);
  TGraphAsymmErrors* gref=TH1TOTGraphAsymm(href);
  //gref->Print();
  TGraphAsymmErrors* ratio=myTGraphErrorsDivide(mydata[igrid]->GetTGraphTotErr(),gref);
  return ratio;
  };
*/

TGraphAsymmErrors* MyCrossSection::TH1TOTGraphAsymm(TH1 *h1) {
  //
  // convert the histogram h1 into a graph
  //
  const char *name="MyCrossSection::TH1TOTGraphAsymm";
  if (!h1) cout <<name<< " histogram not found !" << endl;
  //else h1->Print();
  //cout<<name<<" n= "<<h1->GetNbinsX()<<endl;
  TGraphAsymmErrors* g1= new TGraphAsymmErrors();
  if (!g1) cout<<name<<" graph g1 not created "<<endl;
  //g1->Print();
  g1->SetName(h1->GetName());

  Double_t x, y, ex, ey;
  for (Int_t i=0; i<h1->GetNbinsX(); i++) {
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

TGraphAsymmErrors* MyCrossSection::myTGraphErrorsDivide(TGraphAsymmErrors* g1,TGraphAsymmErrors* g2, Int_t noerr) {
  // Divide two TGraphAsymmErrors: new=g1/g2
  //
  // noerr=0: put all errors to zero
  //       1: add errors from two graph quadrically
  //       2: set errors from graph 2 to zero
  //       3: set errors from graph 1 to zero

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

int MyCrossSection::GetFrameNumber() {
  //
  // count frames/canvases that are given in the steering files
  //
  int nframe=0;
  vector<int> frameid2;
  frameid2.clear();
  if (debug) cout<<" MyCrossSection::GetFrameNumber: frameid.size()= "
		 <<frameid.size()<<endl;
  for (int i=0; i<frameid.size(); i++) {
    int iframe=frameid[i];
    bool found=false;
    for (int i2=0; i2<frameid2.size(); i2++) {
      int iframe2=frameid2[i2];
      if (iframe==iframe2) {
	found=true;
      };
    };
    if (!found) frameid2.push_back(iframe);
    if (debug) cout<<" MyCrossSection::GetFrameNumber: iframe= "
		   <<iframe<<" found= "<<found<<endl;
  };
  if (debug) cout<<" MyCrossSection::GetFrameNumber: Number of frames= "<<frameid2.size()<<endl;
  return frameid2.size();
};



void MyCrossSection::split_string(string str, vector<string>& split_results, string delimiters) {
  //
  // Split string by a delimeter
  //

  // Skip delimiters at beginning.
  string::size_type lastPos = str.find_first_not_of(delimiters, 0);
  // Find first "non-delimiter".
  string::size_type pos     = str.find_first_of(delimiters, lastPos);

  while (string::npos != pos || string::npos != lastPos) {
      // Found a token, add it to the vector.
      split_results.push_back(str.substr(lastPos, pos - lastPos));
 
      // Skip delimiters.  Note the "not_of"
      lastPos = str.find_first_not_of(delimiters, pos);

      // Find next "non-delimiter"
      pos = str.find_first_of(delimiters, lastPos);
    }
}

vector<string>* MyCrossSection::ParseString(string rawData, char delimeter) {
  //
  // retrieve each element of a string by a delimeter
  //

  stringstream lineStream(rawData);
  string cell;
  vector<string> *parsedDataVec;
  parsedDataVec = new vector<string>();
  parsedDataVec->clear();

  if (debug) cout<<" MyCrossSection::ParseString: Start parsing data: '"<<rawData<<"'; Delimeterized by: "<<delimeter<<endl;

  while(getline(lineStream,cell,delimeter)) {
    if (debug) cout<<" MyCrossSection::ParseString: found: "<<cell<<endl;
    cell.erase( remove(cell.begin(), cell.end(), ' '), cell.end() ); //remove any whitespace
    parsedDataVec->push_back(cell);
  }
    
  if (debug) cout<<" MyCrossSection::ParseString: End found "<<parsedDataVec->size()<<" parts."<<endl;
    
  if(parsedDataVec->size()==0) parsedDataVec->push_back(rawData);    
    
  return parsedDataVec;
}


double MyCrossSection::CalcChi2(TGraphAsymmErrors *g_theory, TGraphAsymmErrors *g_data, TMatrixT<double> *data_cov_matrix) {
  // 
  // Calculate the Chi2 
  //

  if (debug) cout<<" MyCrossSection::CalcChi2:         "<<endl;
  //
  /// Fill in the theory covariance matrix
  TMatrixT<double> theory_cov_matrix(g_theory->GetN(), g_theory->GetN());
  for (int pi = 0; pi < g_theory->GetN(); pi++) {
    for (int pi2 = 0; pi2 < g_theory->GetN(); pi2++) {
      if ( pi != pi2 ) theory_cov_matrix(pi, pi2) = 0;
      if ( pi == pi2 ) {
	// is this reasonable ?
	double theory_uncertainty = 0.5*(g_theory->GetErrorYhigh(pi) + g_theory->GetErrorYlow(pi));
	theory_cov_matrix(pi, pi2) = theory_uncertainty*theory_uncertainty;
      }
    }
  }

  if (debug) {
    cout<<" MyCrossSection::CalcChi2: theory uncertainty matrix done "<<endl;
    theory_cov_matrix.Print();
  }

  if (debug) {
    cout<<" MyCrossSection::CalcChi2: At Start, dump contents of data cov matrix: \n "<<endl;
    data_cov_matrix->Print();
    cout<<endl;
  }

  TMatrixT<double> tot_cov_matrix = theory_cov_matrix + *data_cov_matrix;
  TMatrixT<double> invertex_cov_matrix = tot_cov_matrix.Invert(); //includes theory errors in diagonal elements

  if (debug) {

    cout<<"\n MyCrossSection::CalcChi2: After adding theory, dump contents of cov matrix: \n "<<endl;
    tot_cov_matrix.Print();

    cout<<endl;
    cout<<"\n MyCrossSection::CalcChi2: After inversion, dump contents of cov matrix: \n"<<endl;
    invertex_cov_matrix.Print();

  }

  // Loop over bins and determine data-theory matrices
  TMatrixT<double> row_data_minus_theory(1, g_theory->GetN());
  TMatrixT<double> col_data_minus_theory(g_theory->GetN(), 1);

  for (int pi = 0; pi < g_theory->GetN(); pi++) {
    Double_t data_val;
    Double_t theory_val;
    Double_t x_val;

    g_theory->GetPoint(pi, x_val, theory_val);
    g_data->GetPoint(pi, x_val, data_val);
    row_data_minus_theory(0,pi) = data_val - theory_val;
    col_data_minus_theory(pi,0) = data_val - theory_val;

    //if (debug) cout<<" MyCrossSection::CalcChi2: At "<<x_val<<" data = "<<data_val
    //              <<" theory = "<<theory_val<<", content = "<<row_data_minus_theory(0,pi)<<endl;
  }  // pi

  TMatrixT<double> cov_times_col = invertex_cov_matrix*col_data_minus_theory;
  //assert( cov_times_col.GetNrows() == g_theory->GetN());
  //assert( cov_times_col.GetNcols() == 1);
  if (debug) {
    cout<<"\n MyCrossSection::CalcChi2: After first multiplication matrix is: \n"<<endl;
    cov_times_col.Print();
    /*
      for (int pi = 0; pi < g_theory->GetN(); pi++) {
      cout<<cov_times_col(pi, 0)<<"\t";
      }
      cout<<"\n";
    */
  }
 
  TMatrixT<double> result = row_data_minus_theory*cov_times_col;
  //assert( result.GetNrows() == 1);
  //assert( result.GetNcols() == 1);

  double chi2 = result(0,0);
  if (debug) cout<<" MyCrossSection::CalcChi2: End chi2 = "<<chi2<<endl;

  return chi2;
}


bool MyCrossSection::DoubleDataSetName(int idata) {
  //
  // check id data-set name appears twice in dataname vector
  //

  bool doubledataset=false;
  if (idata>dataname.size())
    cout<<" MyCrossSection::DoubleDataSetName: number of data sets too small "
	<<dataname.size()<<" idata= "<<idata<<endl;
  
  if (debug) cout<<" MyCrossSection::DoubleDataSetName: "
		 <<" dataname["<<idata<<"]= "<<dataname[idata]
		 <<" compared to... "<<endl;
  
  //check each data name for duplicates
  for (int jdata=0; jdata<dataname.size(); jdata++) {
    if (jdata<idata) { //only check future data to be displayed
      if(jdata!=idata) { //don't check youself
	if (debug) cout<<" MyCrossSection::DoubleDataSetName: "
		       <<" dataname["<<jdata<<"]= "<<dataname[jdata]<<endl;
	
	if (dataname[idata].compare(dataname[jdata])== 0){
	  if (debug) cout<<" MyCrossSection::DoubleDataSetName: Duplicate data found."<<endl;
	  
	  doubledataset=true;
	}
      }
    }
  }
  
  if (debug) cout<<" MyCrossSection::DoubleDataSetname: found duplicate="
		 <<(doubledataset? "yes":"no")<<endl;

  return doubledataset;
}


void MyCrossSection::ScaleGraph(TGraphAsymmErrors *g1, double scalex, double scaley) {
  //
  // Scale a a graph 'g1' by some x and y scalers
  //

  cout<<" MyCrossSection::ScaleGraph: WARN: Artifical scaling of graph data by: "
      <<"\n\tscalex: "<<scalex
      <<"\n\tscaley: "<<scaley<<endl;

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

double MyCrossSection::GetUnitScaleX(std::string fromUnits, std::string toUnits) {
  std:: string mn = "GetUnitScaleX"; //Name of method, for printing
  int n = 1; //scale value to return based on fromUnits and toUnits

  cout<<cn<<mn<<" Converting '"<<fromUnits<<"' to '"<<toUnits<<"'"<<endl;
  
  // Possible units to convert from and to
  std::string MEV = "MEV", GEV = "GEV", TEV = "TEV";
  
  // Shift fromUnits and toUnits to be in same case as above
  std::string from = stringToUpper(fromUnits), to = stringToUpper(toUnits);
  

  // Compute scaling
  // scale down to electron volts
  if      (from == MEV) n = -6; // n = -6; 
  else if (from == GEV) n = -9;
  else if (from == TEV) n = -12;
  else    cerr<<cn<<mn<<" ERROR: Unexpected units from '"<<from<<"'"<<endl;

  // scale up from electron volts to desired units
  if      (to == MEV) n += 6; //n += 6;
  else if (to == GEV) n += 9;
  else if (to == TEV) n += 12;
  else    cerr<<cn<<mn<<" ERROR: Unexpected units to '"<<to<<"'"<<endl;


  if(debug) cout<<cn<<mn<<" Will scale by 1e"<<n<<endl;
  
  return pow(10, n);
}


double MyCrossSection::GetUnitScaleY(std::string fromUnits, std::string toUnits) {
  std:: string mn = "GetUnitScaleY"; //Name of method, for printing
  double n = 1.0; //scale value to return based on fromUnits and toUnits

  cout<<cn<<mn<<" Converting '"<<fromUnits<<"' to '"<<toUnits<<"'"<<endl;

  // Possible units to convert from and to
  std::string FB = "FB", PB = "PB";
    
  // Shift fromUnits and toUnits to be in same case as above
  std::string from = stringToUpper(fromUnits), to = stringToUpper(toUnits);
  

  // Compute scaling
  // scale down to barns
  if      (from == FB) n = -12;
  else if (from == PB) n = -15;
  else    cerr<<cn<<mn<<" ERROR: Unexpected units from '"<<from<<"'"<<endl;

  // scale up from barns to desired units
  if      (to == FB) n += 12;
  else if (to == PB) n += 15;
  else    cerr<<cn<<mn<<" ERROR: Unexpected units to '"<<to<<"'"<<endl;
  

  if(debug) cout<<cn<<mn<<" Will scale by 1e"<<n<<endl;

  return pow(10, n);
}

string MyCrossSection::stringToUpper(std::string s) {
  string upperString = s;
  for (int i = 0; i < s.length(); i++) upperString[i]=toupper(s[i]);

  return upperString;
}
