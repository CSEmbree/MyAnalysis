
#include "appl_grid/generic_pdf.h"
#include "MyGrid.h"

/******************************************************************
 ** Method Implementations
 ******************************************************************/

const string cn = " MyGrid::";

MyGrid::MyGrid(std::string fname)
{
  debug=false;

  steername = fname;
  gridgeneratorid = "";
  gridntupleid = "";
  gridname = "";
  
  normtot=false;
  dividedbybinwidth=false; 
  
  scaley=1.; //allow artifical constant scale of units if user wants
  scalex=1.;

  xunits=""; //TODO - make default more generic? empty to force user to specify
  yunits=""; //TODO - make default more generic?

  lowestorder=-1; //default

  this->ReadSteering( steername );
  this->ValidateSettings();
}


void MyGrid::ReadSteering(string fname) {
  //
  // read steering from file fname
  //
  string mn = "ReadSteering:"; //Method name, for printing

  steername = fname;
  cout<<cn<<mn<<" steering = \""<<steername<<"\""<<endl;
  

  //ensure steering provided is readable
  ifstream infile(steername.c_str(), ios::in);
  if(!infile){ // Check open
    cerr<<cn<<mn<<" ERROR: Can't open \""<<steername<<"\""<<endl;
    infile.close();
    exit(1);
  } else {
    if (debug) cout<<cn<<mn<<" Reading data file: \""<<steername<<"\""<<endl;
  }

  
  // prepare to read
  string curLine;
  string optionName;
  string optionValue;
  int w = 20; //arbitrary width units that make formatting nice


  // read all options from text file
  while (infile.good()) {
    getline(infile, curLine);

    curLine = trim(curLine); //remove leading and trailing white space
    int optionSep = curLine.find(' '); //optioname ends after first space


    // retrieve the option name and it's value seperatly for further parsing
    optionName  = curLine.substr(0, optionSep);
    optionValue = trim(curLine.substr(optionSep+1, curLine.size())); //'optionValue' could be broken up further if needed


    
    if (debug) {
      cout<<cn<<mn<<" Read in:>>>>>>>>>>>>>>>>>>"
	  <<"\n"<<setw(w)<<"currentLine: \""<<curLine<<"\""
	  <<"\n"<<setw(w)<<" optionName: \""<<optionName<<"\""
	  <<"\n"<<setw(w)<<"optionValue: \""<<optionValue<<"\""
	  <<"\n<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<"<<endl;
    }
    
    
    if(curLine[0] != '%' && !curLine.empty() ) { //ignore lines beginning with comments - could be done better
      
      if ( optionName == "debug" ) {
	debug=true;
	cout<<cn<<mn<<" Debug turned on!"<<endl;
      } else if ( optionName == "dividedbybinwidth" ) {
	dividedbybinwidth=true;
      } else if ( optionName == "gridpath" ) {
	gridpath = optionValue;
      } else if ( optionName == "gridgeneratorid" ) {
	gridgeneratorid = optionValue;
      } else if ( optionName == "gridntupleid" ) {
	gridntupleid = optionValue;
      } else if ( optionName == "NAME" ) {
	gridname = optionValue;
      } else if ( optionName == "normtot" ) {
	normtot=true;
      } else if ( optionName == "xunits" ) {
	xunits = optionValue;
      } else if ( optionName == "yunits" ) {
	yunits = optionValue; 
      } else if ( optionName == "lowestorder" ) {
	sscanf( optionValue.c_str(), "%d ", &lowestorder);
      } else {
	cerr<<cn<<mn<<" WARN: Invalid steering option found '"<<curLine<<"'"<<endl;
	//exit(0); //stop if an invalid option found, it's probably an error!
      }

    } //end if, comment check
  }  
  
  
  if(debug) { 
    cout<<cn<<mn<<" Finished reading steering. REPORT:"<<endl;
    this->Print();
  }
  
  infile.close(); //cleanup

  return;
}


TH1D *MyGrid::GetReference() {
  //
  // get reference histogram from original grid
  // 
  string mn = "GetReference:"; //method name, for debugging

  appl::grid *tmpgrid = 0;
  if (this->file_exists( gridpath )) {

    if (debug) cout<<cn<<mn<<" Opening "<<gridpath<<endl;
    tmpgrid = new appl::grid( gridpath );
  } else {
    cout<<cn<<mn<<" ERROR: File not found "<<gridpath<<endl;
  }

  if (!tmpgrid) cout<<cn<<mn<<" ERROR: Grid not found ! "<<endl;

  TH1D *href=(TH1D*)tmpgrid->getReference();
  if (!href) {
    cout<<cn<<mn<<" ERROR: Reference histo not found for \""<<gridpath<<"\""<<endl;
    exit(0);
  }

  int ntot=tmpgrid->run();
  href->Scale(1./ntot);

  return href;
}


bool MyGrid::ValidateSettings() {
  //
  // Check that the minimum number of settings have been set from steering
  //
  string mn = "ValidateSettings:";
  bool valid = true;
  string DEFAULT_STRING = "";

  if(debug) cout<<cn<<mn<<" Validating steering options set..."<<endl;


  ifstream infiletmp(steername.c_str(), ios::in);
  if(!infiletmp){ 
    cerr<<cn<<mn<<" ERROR: Can't open steering file \""<<steername<<"\""<<endl;
    infiletmp.close();
    valid = false;
  }

  if( gridname == DEFAULT_STRING ) {
    //no names may cause problems with multiple plots with the same data - avoid it entirely
    cerr<<cn<<mn<<" ERROR: 'NAME' must be set in steering, found \""<<gridname<<"\""<<endl;
    valid = false;
  }

  if( xunits == DEFAULT_STRING ) {
    //TODO - no xunits read in, check Grid for internal xunits (not implimented on applgrid end yet)
    cerr<<cn<<mn<<" ERROR: 'xunits' must be set in steering, found \""<<xunits<<"\""<<endl;
    valid = false;
  }

  if( yunits == DEFAULT_STRING ) {
    //TODO - no xunits read in, check Grid for internal yunits (not implimented on applgrid end yet)
    cerr<<cn<<mn<<" ERROR: 'yunits' must be set in steering, found \""<<yunits<<"\""<<endl;
    valid == false;
  }


  if(debug) cout<<cn<<mn<<" Settings are: "<<(valid? "VALID":"NOT VALID")<<endl;

  return valid;
}


void MyGrid::Print() {
  //
  // Display internal settings set from reading steering
  //
  string mn = "Print:";

  int w1=25; //arbitrary spacing size that makes formatting look pretty for data names
  int w2=50; //arbitrary spacing size that makes formatting look pretty for data options

  cout<<cn<<mn<<" >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>"
      <<"\n"<<setw(w1)<<"DEBUG: "            <<setw(w2)<<(debug? "ON":"OFF")
      <<"\n"<<setw(w1)<<"Steering File: "    <<setw(w2)<<steername
      <<"\n"<<setw(w1)<<"Grid PATH: "        <<setw(w2)<<gridpath
      <<"\n"<<setw(w1)<<"Grid NTuple ID: "   <<setw(w2)<<gridntupleid
      <<"\n"<<setw(w1)<<"Grid Generator ID: "<<setw(w2)<<gridgeneratorid
      <<"\n"<<setw(w1)<<"xunits: "           <<setw(w2)<<xunits
      <<"\n"<<setw(w1)<<"yunits: "           <<setw(w2)<<yunits
      <<"\n"<<setw(w1)<<"normtot: "          <<setw(w2)<<(normtot? "YES":"NO")
      <<"\n"<<setw(w1)<<"dividedbybinwidth: "<<setw(w2)<<(dividedbybinwidth? "YES":"NO")
      <<"\n"
      <<"\n"<<setw(w1)<<"NAME: "             <<setw(w2)<<gridname
      <<"\n"<<setw(w1)<<"Lowest Order: "     <<setw(w2)<<lowestorder<<endl;
    cout<<cn<<mn<<" <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<"<<endl;            
  
    return;
}


void MyGrid::ScaleGraph(TGraphAsymmErrors *g1, double scalex, double scaley) {

 Double_t* X1 = g1->GetX();
 Double_t* Y1 = g1->GetY();
 Double_t* EXhigh1 = g1->GetEXhigh();
 Double_t* EXlow1 =  g1->GetEXlow();
 Double_t* EYhigh1 = g1->GetEYhigh();
 Double_t* EYlow1 =  g1->GetEYlow();

 for (Int_t i=0; i<g1->GetN(); i++) {
  g1->SetPoint(i,X1[i]*scalex,Y1[i]*scaley);
  g1->SetPointError(i,EXlow1[i]*scalex,EXhigh1[i]*scalex,
                      EYlow1[i]*scaley,EYhigh1[i]*scaley);
 }
 
 return;
}

/*
void MyGrid::Scale(double scalex, double scaley) {
  
  return;
};

void MyGrid::Normalise(double yscale, double xscale=1.) {

  return;
}
*/

void MyGrid::NormaliseGraph(TGraphAsymmErrors* g1, double yscale, double xscale=1., bool normtot=false, bool divbinwidth=true) {
  //
  // Normalise a graph  
  //
  string mn = "NormaliseGraph:";

  // convolution is divided by binwidth
  Double_t x, y, ey;
  Double_t sigtot=0.;

  Double_t* X1 = g1->GetX();
  Double_t* Y1 = g1->GetY();
  Double_t* EXhigh1 = g1->GetEXhigh();
  Double_t* EXlow1  = g1->GetEXlow();
  Double_t* EYhigh1 = g1->GetEYhigh();
  Double_t* EYlow1  = g1->GetEYlow();

  if (!g1) cout<<cn<<mn<<" ERROR: Graph not found ! "<<endl;
  int Nbin=g1->GetN();

  for (Int_t i=0; i<Nbin; i++) {
    y=Y1[i]*yscale;
    double binw=EXhigh1[i]+EXlow1[i];
    //cout<<" binw= "<<binw<<endl;
    if (divbinwidth) sigtot+=y*binw;
    else             sigtot+=y;
    sigtot+=y;
  }

  double scal=yscale;
  for (Int_t i=0; i<Nbin; i++) {
    if (normtot) scal=yscale/sigtot;
    double binw=1.;
    if (divbinwidth) binw=EXhigh1[i]+EXlow1[i];

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

  return;
}


void MyGrid::split_string(std::string str, std::vector<std::string>& split_results, std::string delimiters) {
  //
  // Helper to split strings by a string delimter 
  //

  // Skip delimiters at beginning.
  string::size_type lastPos = str.find_first_not_of(delimiters, 0);
  // Find first "non-delimiter".
  string::size_type pos     = str.find_first_of(delimiters, lastPos);

  while (string::npos != pos || string::npos != lastPos)
  {
    // Found a token, add it to the vector.
    split_results.push_back(str.substr(lastPos, pos - lastPos));
    // Skip delimiters.  Note the "not_of"
    lastPos = str.find_first_not_of(delimiters, pos);
    // Find next "non-delimiter"
    pos = str.find_first_of(delimiters, lastPos);
  }

  return;
}


bool MyGrid::file_exists(const string& s) {
  //
  // Helper to make sure a file exists
  //

  if ( FILE* testfile=fopen(s.c_str(),"r") ) {
    fclose(testfile);
    return true;
  }
  else return false;
}


std::string MyGrid::trim(std::string s) {
  // 
  // Helper to remove leading and trailing white space from steering file reading
  //  credit to: http://www.toptip.ca/2010/03/trim-leading-or-trailing-white-spaces.html
  //
  std::string reducedS = s;

  size_t p = reducedS.find_first_not_of(" \t");
  reducedS.erase(0, p);
  
  p = reducedS.find_last_not_of(" \t");
  if (string::npos != p) reducedS.erase(p+1);

  return reducedS;
}
