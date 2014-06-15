/*
 * Title:    MyPDFDraw
 * Author:   Cameron S. Embree
 * Contact:  CSEmbree@gmail.com
 * Created:  10-Jun-2013
 * Edited:   10-Jun-2013
 */

#include "MyPDFDraw.h"


/******************************************************************
 ** Method Implementations
 ******************************************************************/

MyPDFDraw::MyPDFDraw(std::string _steeringFile)
{
  SetVariablesDefault();   
  
  steeringFileName = _steeringFile;
  if( FileExists( steeringFileName ) == true ) {
    ReadSteering( steeringFileName );
    
    Print();

  } else {
    std::cout<<" MyPDFDraw::MyPDFDraw: cannot find file: '"<<steeringFileName<<"'"<<std::endl;
  }
  
}


//read the provided steering file and set internal variables depending on what is read
void MyPDFDraw::ReadSteering(const string _fileName)
{
  string fName="";
  if (_fileName.size()>0)
    fName=_fileName;

  if (debug) std::cout<<" MyPDF::ReadSteering: reading steering file named: "<<fName.c_str()<<std::endl;

  //Open the file for reading if it can be read/found
  ifstream infile(fName.c_str(), ios::in);

  if(!infile) {
    std::cerr<<" MyPDF::ReadSteering: WARNING: Can't open "<<fName<<std::endl;
    infile.close();
    exit (1); //TEMP TEST
  } else {
    if (debug) std::cout<<" MyPDF::ReadSteering: Steering file named successfuly opened."<<std::endl;
  }


  pdfSetPath = defaultPDFSetPath;
  string pdfSetDefaultPath = GetEnv("LHAPATH");

  if (pdfSetDefaultPath.size()>0) {
    if (pdfSetDefaultPath.find_last_of("/") == pdfSetDefaultPath.size()-1)
      pdfSetDefaultPath = pdfSetDefaultPath.substr(0,pdfSetDefaultPath.size()-1); //remove trailing slashes if there are any
    pdfSetPath=pdfSetDefaultPath;
    std::cout<<" MyPDF: LHAPATH environment variable found, using path: "<<pdfSetPath<<std::endl;
  } else {
    std::cout<<" MyPDF: LHAPATH environment variable not set, using default: "<<pdfSetPath<<std::endl;
  }

  string line;
  string optionName;
  string text;
  double tmp;

  //load in all valid options

  int w=20; //arbitrary width number for printing nicely formatted debug statements

  //read and set all options and data
  while (infile.good()) {
    getline(infile, line);

    optionName=line.substr(0, line.find(' '));
    text=line.substr(line.find(' ')+1,line.size()); //'text' could be broken up further if needed

    if (debug) {
      std::cout<<"\n MyPDF::ReadSteering: Read in:<<<<<<<<<<<<<<<<<<<"
	  <<"\n"<<setw(w)<<"line:"<<line
	  <<"\n"<<setw(w)<<"optionName:"<<optionName
	  <<"\n"<<setw(w)<<"text:"<<text
	       <<"\n<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<"<<std::endl;
    }

    if (line[0] != '%') { //ignore comments
      if (optionName.compare("debug")==0) {
	debug=true; //debug flag for extra print statements
      } else if (optionName.compare("PDFName")==0) {
	PDFNamesList.push_back( text );
      } else if (optionName.compare("PDFFlavor")==0) {
	PDFFlavorList.push_back( text );
      } else if (optionName.compare("Q2Value")==0) {
	sscanf(text.c_str(), "%d", &tmp);
	Q2List.push_back( tmp );
      } else if (optionName.compare("Nbins")==0) {
	sscanf(text.c_str(), "%d", &tmp);
	nbinsList.push_back( tmp );
      } else if (optionName.compare("Xmin")==0) {
	sscanf(text.c_str(), "%d", &tmp);
	xminList.push_back( tmp );
      } else if (optionName.compare("XMax")==0) {
	sscanf(text.c_str(), "%d", &tmp);
	xmaxList.push_back( tmp );
      }
    }
  }
}


//Print all relevant internal variable values
void MyPDFDraw::Print()
{
  int w=30;               //arbitrary size that makes the formatting look pretty
  string empty="<empty>"; //print this if no input has been provided for that variable
  string ON="ON";         //bool true
  string OFF="OFF";       //bool false
  
  std::cout<<" MyPDF::Print: >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>"
      <<"\n"<<setw(w)<<"debug:"                <<setw(w)<<(debug? "ON":"OFF")
      <<"\n"<<setw(w)<<"steeringFileName:"     <<setw(w)<<(steeringFileName.size()>0? steeringFileName:empty)
    
    
    for(int i = 0; i < pdfNameList.size(); i++) {
      std::cout<<" MyPDF::Print: >>>>>>>>>>>>>>>>>>>>DATA "<<i<<">>>>>>>>>>>>>>>>>>>>>"
	  <<"\n"<<setw(w)<<"PDF Name:"<<setw(w)<< (pdfNameList.size()>0? pdfNameList.at(i):empty)
	  <<"\n"<<setw(w)<<"PDF Flavor:"<<setw(w)<<(pdfFlavorList.size()>0? pdfFlavorList.at(i):empty)
	  <<"\n"<<setw(w)<<"Q2:"<<setw(w)<<(Q2List.size()>0? Q2List.at(i):empty)
	  <<"\n"<<setw(w)<<"num bins:"<<setw(w)<<(nbinsList.size()>0? nbinsList.at(i):empty)
	  <<"\n"<<setw(w)<<"X-min:"<<setw(w)<<(xminList.size()>0? xminList.at(i):empty)
	       <<"\n"<<setw(w)<<"X-max:"<<setw(w)<<(xminList.size()>0? xminList.at(i):empty)<<std::endl;
    }
  
  std::cout<<"\n MyPDF::Print:<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n"<<std::endl;
}


//check for file existence before usage
bool MyPDFDraw::FileExists(const string _fileName)
{
  if (debug) std::cout<<" MyPDF::FileExists: Checking for file "<<_fileName<<" existance..."<<std::endl;

  bool exists = false;

  if ( FILE* file=fopen(_fileName.c_str(),"r") ) {
    fclose(file);
    exists = true; //file found
  } else { 
    exists = false; //file NOT found
  }

  if (debug&&exists) std::cout<<"file: '"<<_fileName<<"' exist "<<exists<<std::endl;
  if (debug&&!exists) std::cout<<"file: '"<<_fileName<<"' not found "<<exists<<std::endl;

  if (debug) std::cout<<" MyPDF::FileExists: file '"<<_fileName<<"' = "<<exists<<std::endl;
  
  return exists;
}

/*
//default values for variables to avoid crashes and check for proper setup before doing anything
void MyPDFDraw::SetVariablesDefault()
{
  if (debug) std::cout<<" MyPDF::setVariablesDefault: Start default values being set."<<std::endl;

  string defStr="";
  double defNum=-1.0;

  debug = false;

  steeringFileName = defaultString;
  pdfNameList.clear();
  pdfFlavorList.clear();
  Q2List.clear();
  nbinsList.clear();
  xminList.clear();
  xmaxList.claer();


  if (debug) std::cout<<" MyPDF::setVariablesDefault: End default values are set."<<std::endl;
}


 //mutator methods
void MyPDFDraw::SetDebug(bool _debug) {
  debug=_debug;
}
void MyPDFDraw::SetSteeringFileName(string _steeringFileName) {
  steeringFileName=_steeringFileName;
}
*/
