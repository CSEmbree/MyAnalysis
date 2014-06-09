
#include "MyData.h"

/******************************************************************
 ** Method Implementations
 ******************************************************************/

MyData::MyData()
{
  debug=false;

  normtot=false;
  unitsxTeV=false;
  unitsypb=false;

  tableplusminus=false;
  systcorrelationmatrix=false;

  blogy=false; blogx=false; 
  bliny=true;  blinx=true;

  corrfrommatrix=false;
  cov_matrix_ok=false;

  dividebybinwidth=false; 

  sqrts=0.; ymin=0.; ymax=0.;
  datavector=0;
  datavectorstat=0;
  datavectorsyst=0;
  datavectortoterr=0;

  datavectorsystcomp.clear();  

  //xtdatavector=0;
  //xtdatavectorstat=0;
  //xtdatavectorsyst=0;
  scale=1.;
  msize=0.03;
  markerstyle=0;
  markercolor=0;
  markersize=1.0;
  if (debug)  cout<<" MyData msize= "<<msize<<endl;
  //cout<<" datavector= "<<datavector<<endl;
 
  jetalgoR=0;

  observable="";
  //TString reaction;
  jetcut=false;
  lepcut=false;
  deltaRLepJet=false;
  neucut=false;
  cutMtw=false;
  drljcut=false;

  njetcut=false;
  njets=0;
 
  Mtwcut=0.;
  ptjetcut=0.;
  ptlepcut=0.;
  ptneucut=0.;
  yjetmin=-999.; yjetmax=999.;
  ylepmin=-999.; ylepmax=999.;
  deltaRLepJet=0.;
  setframexmin=false;
  framexmin=0.;

  incljets=false;

  corrfilename="";

}

//void MyData::ReadCorrelations(double myscale)
void MyData::ReadCorrelations()
{

  if (debug) cout << " MyData::ReadCorrelations  Read correlations corrfilename= "<<corrfilename<< endl;
  if (debug) cout << " MyData::ReadCorrelations  datavectortoterr: "<< endl; 
  if (debug) datavectortoterr->Print("all");

  cov_matrixsyst = new TMatrixT<double>(datavectortoterr->GetN(), datavectortoterr->GetN());
  cov_matrixtot  = new TMatrixT<double>(datavectortoterr->GetN(), datavectortoterr->GetN());

  corr_matrixsyst= new TMatrixT<double>(datavectortoterr->GetN(), datavectortoterr->GetN());
  corr_matrixtot = new TMatrixT<double>(datavectortoterr->GetN(), datavectortoterr->GetN());

  cov_matrix_ok=true;

  ifstream infile(corrfilename, ios::in);
  if(!infile){ // Check open
   cout << " MyData::ReadCorrelations Can't open " << corrfilename <<endl;
   infile.close();
   exit (1);
  } else {
   if (debug) cout <<" MyData::ReadCorrelations: read data correlations file: " << corrfilename << endl;
  }

 char line[1024];

 int row_num = 0;
 while (1) {
   if (!infile.good()) break;
   infile.getline(line,sizeof(line),'\n');
   if( line[0] == '%' ) continue;
   std::string cpp_line(line);   
   std::vector<std::string> split_line;  split_line.clear();
   split_string(cpp_line, split_line, " ");
   //if (debug) cout << " MyData::ReadCorrelations Read in correlation matrix, line " << line << endl;
   std::vector<double> my_corr_row;   my_corr_row.clear();
   if( cpp_line.size() > 3 ) {
     //if (debug) cout << "Dump numerical contents (should be identical):\n";
     for(int ci = 2; ci < (int) split_line.size(); ci++) {
       int col_num = ci-2;
       double corr_val = atof( split_line.at(ci).c_str() );
       double row_data_err = 0.5*(datavectortoterr->GetErrorYhigh(row_num) + datavectortoterr->GetErrorYlow(row_num));
       double col_data_err = 0.5*(datavectortoterr->GetErrorYhigh(col_num) + datavectortoterr->GetErrorYlow(col_num));
 //      datavectortoterr
       //if (debug) cout << " row_data_err "<<row_data_err<< " col_data_err "<<col_data_err<<endl;
       // correlation matrix divided by total error in row/col
       double cov_val = corr_val*row_data_err*col_data_err;
       //     my_corr_row.push_back(this_val);
       //     TMatrixT<double>
       (*cov_matrixtot )(row_num, col_num) = cov_val;
       (*corr_matrixtot)(row_num, col_num)= corr_val;
     }    /// loop over entries
     if( debug ) cout << "\n";
     row_num++;
   }  /// This is a row of the correlation matrix. Read it in.
 }

 if (debug) {
  cout << " MyData::ReadCorrelation total correlation matrix: " << endl;
  corr_matrixtot->Print();

  cout << " MyData::ReadCorrelation covariance matrix: " << endl;
  cov_matrixtot->Print();

 }
}

//void MyData::ReadData(const char fname[200], const char dir[200], double myscale){
void MyData::ReadData(string fname, string dir, double myscale){

 if (debug) cout << " MyData::ReadData:" << endl;
 bool errorinpercent=false;

 filename.Clear();
 //filename=TString(fname);
 filename=fname;
 //filename.ReplaceAll(TString("data/"),TString("")); 
 filename.ReplaceAll(TString(".txt"),TString("")); 
 datavector = new  TGraphAsymmErrors();
 datavector->SetName(filename);
 if (!datavector) cout<<" MyData::ReadData: can not create data vector "<<endl;
 //datavector->Print();

 datavectorsyst = new  TGraphAsymmErrors();
 TString fnamesyst(filename); fnamesyst+="syst";
 datavectorsyst->SetName(fnamesyst);
 if (!datavectorsyst) cout<<" MyData::ReadData: can not create data vector "<<endl;
 //datavectorsyst->Print();

 if (debug) cout << " MyData::ReadData Creating datavectorstat" << endl;
 datavectorstat = new  TGraphAsymmErrors();
 TString fnamestat(filename); fnamestat+="stat";
 datavectorstat->SetName(fnamestat);
 if (!datavectorstat) cout<<" MyData::ReadData: can not create data vector "<<endl;
 if (debug) cout << " MyData::ReadData Created it" << endl;

 if (debug) cout << " MyData::ReadData: Creating datavectortot" << endl;
 datavectortoterr = new  TGraphAsymmErrors();
 TString fnametot(filename); fnamestat+="tot";
 datavectortoterr->SetName(fnametot);
 if (!datavectortoterr) cout<<" MyData::ReadData: can not create data vector "<<endl;
 if (debug) cout << " MyData::ReadData: Created it" << endl;
 //datavectorstat->Print();


 if (debug) cout << " MyData::Read: filename dir= " << TString(dir).Data() <<endl;
 if (debug) cout << " MyData::Read: filename fname= " << fname.c_str() <<endl;

 //TString fnametmp=TString(dir);
 TString fnametmp=dir;
 fnametmp+="/";
 fnametmp+=TString(fname);
 //fnametmp+=fname;
 //if (debug) 
 cout<< " MyData::ReadData read file fname= "<<fnametmp.Data()<<endl;


 ifstream infile(fnametmp.Data(), ios::in);
 if(!infile){ // Check open
  cerr << " MyData::ReadData: Can't open " << fnametmp.Data() <<"\n";
  infile.close();
  exit (1);
 } else {
   if (debug) cout <<" MyData::ReadData: read data file: " << fnametmp.Data() << endl;
 }

 //int iline=0;
 int nsyst=0;
 int isyst=0;
 int nbin=-1;
 int nbinold=-999;
 char line[1024];

 double  xm=0., xl=0.,xh=0., y=0., ey=0.;
 std::vector<double> eyh;
 std::vector<double> eyl;


 //std::vector< std::vector<double>> veceyl;
 //std::vector< std::vector<double>> veceyh;

 eyl.clear(); eyh.clear();
 //veceyl.clear(); veceyh.clear();

 const double BIG=1.e30;
 miny=BIG; maxy=-BIG; minx=BIG; maxx=-BIG;

 //bool nextlinereaddatavalues=false;
 bool nextlinereaddatavalues=true;
 bool newbin=true;

 while (1) {
  //if (debug) cout << " MyData::ReadData: good: " << infile.good() << " eof: " << infile.eof() << endl;
  if (!infile.good()) break;
  infile.getline(line,sizeof(line),'\n');
  //std::string cpp_line(line);   
//
  if (debug) cout<< "line= "<< line << "\n";

  if (line[0] != '%'){ // this is a comment line in the steering
   if (strstr(line,"unitxTeV")!=0) {
    unitsxTeV=true; 
   } else if (strstr(line,"normtot")!=0) {
    normtot=true; 
   } else if (strstr(line,"unitypb")!=0) {
    unitsypb=true; 
   } else if (strstr(line,"framelogy")!=0) {
    blogy=true; bliny=false;
    //cout<<" set logy "<<endl;
   } else if (strstr(line,"frameliny")!=0) {
    blogy=false; bliny=true;
    //cout<<" set linx "<<endl;
   } else if (strstr(line,"framelogx")!=0) {
    blogx=true; blinx=false;
    //cout<<" set logx "<<endl;
   } else if (strstr(line,"dividedbybinwidth")!=0) {
    dividebybinwidth=true;
    if (debug) cout<<" MyData turn on divide by bin width "<<endl; 
   } else if (strstr(line,"framelinx")!=0) {
    blogx=false; blinx=true;
    //cout<<" set linx "<<endl;
   } else if (strstr(line,"framexmin")!=0) {
    char text[100]; float myframexmin;
    sscanf(line," %s %f ",text, &myframexmin);
    setframexmin=true;
    framexmin=myframexmin;
    //if (debug) cout<<" framexmin= "<<framexmin<<endl;
   } else if (strstr(line,"YRAP")!=0) {
    char text[100];
    float myymin, myymax;
    sscanf(line," %s %f %f",text, &myymin,&myymax);
    ymin=myymin; ymax=myymax;
    if (debug) printf(" MyData::ReadData: ymin= %f  ymax = %f  \n",myymin,myymax); 
   } else if (strstr(line,"YJET")!=0) {
    char text[100];
    float myymin, myymax;
    jetcut=true;
    sscanf(line," %s %f %f",text, &myymin,&myymax);
    yjetmin=myymin; yjetmax=myymax;
    if (debug) printf(" MyData::ReadData: ymin= %f  ymax = %f  \n",myymin,myymax); 
   } else if (strstr(line,"YLEP")!=0) {
    lepcut=true;
    char text[100];
    float myymin, myymax;
    sscanf(line," %s %f %f",text, &myymin,&myymax);
    ylepmin=myymin; ylepmax=myymax;
    if (debug) printf(" MyData::ReadData: ymin= %f  ymax = %f  \n",myymin,myymax); 
   } else if (strstr(line,"SQRTS")!=0) {
    char text[100]; float mys;
    sscanf(line," %s %f ",text, &mys);
    sqrts=mys;
    if (debug) printf(" MyData::ReadData: s= %f   \n",mys);
   } else if (strstr(line,"ERRORINPERCENT")!=0) {
    errorinpercent=true;
    if (debug) cout<<" MyData::ReadData: errors given in percent "<<endl;
   } else if (strstr(line,"TABLEPLUSMINUS")!=0) {
    tableplusminus=true;
    if (tableplusminus) cout<<" MyData::ReadData: Syst uncertainties with +/- values "<<endl;
   } else if (strstr(line,"YEAR")!=0) {
    char text[100]; 
    sscanf(line," %s %d ",text, &year);
    //cout<<" year= "<<year<<endl;
   } else if (strstr(line,"JETALGO")!=0) {
    char text[100];     char jetname[100]; 
    sscanf(line," %s %s ",text, jetname);
    jetalgo=jetname;
   } else if (strstr(line,"corrname")!=0) {
    char text[100];     char  name[100]; 
    sscanf(line," %s %[^\n] ",text, name);
    if (debug) cout<<" MyData::ReadData: text= "<<text<<endl;
    corrfrommatrix=true;
    corrfilename=TString(dir);
    corrfilename+="/";
    corrfilename+=TString(name);
    if (debug) cout<<" MyData::ReadData: corrfilename= "<<corrfilename.Data()<<endl;
   } else if (strstr(line,"label")!=0) {
    char text[100];     char labelname[100]; 
    sscanf(line," %s %[^\n] ",text, labelname);
    //if (debug) cout<<" MyData::ReadData text= "<<text<<endl;
    if (debug) cout<<" MyData::ReadData: labelname= "<<labelname<<endl;
    label=TString(labelname);
   } else if (strstr(line,"EXP")!=0) {
    char text[100];     char name[100]; 
    sscanf(line," %s %[^\n] ",text, name);
    experiment=TString(name);
    //cout<<" Reaction= "<<name<<endl;
   } else if (strstr(line,"REACTION")!=0) {
    char text[100];     char name[100]; 
    sscanf(line," %s %[^\n] ",text, name);
    //sscanf(line," %s %s ",text, name);
    reaction=TString(name);
    //cout<<" Reaction= "<<name<<endl;
   } else if (strstr(line,"NAME")!=0) {
    char text[100];     char name[100]; 
    sscanf(line," %s %[^\n] ",text, name);
    //sscanf(line," %s %s ",text, name);
    dataname=TString(name);
   } else if (strstr(line,"COLUMNNAME")!=0) {
    char text[100];     char name[100]; 
    sscanf(line," %s %[^\n] ",text, name);
    //sscanf(line," %s %s ",text, name);
    columnname=TString(name);
    if (debug) cout<<" MyData::ReadData: columnname= "<<columnname.Data()<<endl;

   } else if (strstr(line,"OBS")!=0) {
    char text[100];     char name[100]; 
    sscanf(line," %s %[^\n] ",text, name);
    observable=TString(name);
    if (debug) cout<<" MyData::ReadData: observable= "<<observable.Data()<<endl;
    if (observable.Contains("INCLJETS")) incljets=true;
    if (incljets) cout<<" MyData::ReadData: variable for inclusive jets "<<endl;
    //double *bins = new double[nbins];
   } else if (strstr(line,"titlex")!=0) {
    char text[100];     char name[100]; 
    sscanf(line," %s %[^\n] ",text, name);
    titlex=TString(name);
    //cout<<" TitleX= "<<name<<endl;
   } else if (strstr(line,"titley")!=0) {
    char text[100];     char name[100]; 
    sscanf(line," %s %[^\n] ",text, name);
    titley=TString(name);
    //cout<<" TitleY= "<<name<<endl;
   } else if (strstr(line,"JETALGR")!=0) {
    char text[100]; 
    sscanf(line," %s %d ",text, &jetalgoR);
    //cout<<" jet R= "<<jetalgoR<<endl;
   } else if (strstr(line,"NJETS")!=0) {
    njetcut=true;
    njets=0;
    char text[100]; 
    sscanf(line," %s %d ",text, &njets);
    //cout<<" NJets= "<<njets<<endl;
   } else if (strstr(line,"PTJET")!=0) {
    jetcut=true;
    char text[100]; float mys;
    sscanf(line," %s %f ",text, &mys);
    ptjetcut=mys;
    //cout<<" ptjetcut= "<<ptjetcut<<endl;
   } else if (strstr(line,"PTLEP")!=0) {
    lepcut=true;
    char text[100]; float mys;
    sscanf(line," %s %f ",text, &mys);
    ptlepcut=mys;
    //cout<<" ptlepcut= "<<ptlepcut<<endl;
   } else if (strstr(line,"PTNEU")!=0) {
    neucut=true;
    char text[100]; float mys;
    sscanf(line," %s %f ",text, &mys);
    ptneucut=mys;
    //cout<<" ptneucut= "<<ptneucut<<endl;
   } else if (strstr(line,"MTW")!=0) {
    cutMtw=true;
    char text[100]; float mys;
    sscanf(line," %s %f ",text, &mys);
    Mtwcut=mys;
    //cout<<" Mtwcut= "<<Mtwcut<<endl;
   } else if (strstr(line,"DRLJ")!=0) {
    drljcut=true;
    char text[100]; float mys;
    sscanf(line," %s %f ",text, &mys);
    deltaRLepJet=mys;
   } else if (strstr(line,"SYSTCORRELATIONMATRIX")!=0) { // flag that systematic correlation matrix set
    systcorrelationmatrix=true;
    if (systcorrelationmatrix) cout << " MyData::ReadData systcorrelationmatrix is defined \n";
   } else if (strstr(line,"Syst_")!=0) { // systematics components
    //char text[100];     char  name[100]; 
    nsyst++;
    //sscanf(line," %s %[^\n] ",text, name);
    // read in systematics commponents
    if (debug) cout << " MyData::ReadData: nsyst= "<<nsyst<<endl;

    std::string cpp_line(line);   
    std::vector<std::string> split_line;  split_line.clear();
    split_string(cpp_line, split_line, " ");
    string name=split_line.at(0);
    //for (int ci = 1; ci < (int) split_line.size(); ci++) {
    // double val = atof( split_line.at(ci).c_str() );
    //}

    TGraphAsymmErrors* gtmp= new  TGraphAsymmErrors();
    gtmp->SetName(TString(name));
    if (debug) cout<<" MyData::ReadData: gtmp name= "<<gtmp->GetName()<<endl;
    if (debug) gtmp->Print();
    // read here correlation matrix
    datavectorsystcomp.push_back(gtmp);

   } else {


    //if (newbin) cout<<" start newbin on "<<endl;
    //else        cout<<" start newbin off "<<endl;

    if (nextlinereaddatavalues) {
     if (debug) cout<<" MyData::ReadData: found bin= "<<nbin<<" nbinold= "<<nbinold<<endl;
     if (nbin!=nbinold) nbinold=nbin;

     std::string cpp_line(line);   
     std::vector<std::string> split_line;  split_line.clear();
     split_string(cpp_line, split_line, " ");

     //if (debug) cout << "Dump numerical contents (should be identical):\n";
     //
     // This code assumes that tables are formated
     // with 5 column per line xm xl xh sigma sigma stat
     // and then following lines with 2 colums for each systematic uncertainty
     // or everything in one line with syst everything what follows after statistical uncertainty
     //

     if (split_line.size()>=5) {
      if (split_line.size()>5) newbin=true;
      if (split_line.size()==5) {
       if (newbin)  {newbin=false;
       } else       {newbin=true;}
       if (isyst==0){newbin=true;}
      }
      if (newbin) {
       nbin++;
       eyl.clear(); eyh.clear();  
       isyst=0;
      }

      //if (newbin) cout<<" in splitline newbin on "<<endl;
      //else        cout<<" in splitline newbin off "<<endl;
   
      if (debug) cout << " MyData::ReadData: read line mean xl xh sigma stat"<<endl; 
      for (int ci = 0; ci < (int) split_line.size(); ci++) {
       double val = atof( split_line.at(ci).c_str() );
       if (debug) cout<<" MyData::ReadData: ci= "<<ci<<" val= "<<val<<endl;      
       if (ci==0) xm=val;
       if (ci==1) xl=val;
       if (ci==2) xh=val;
       if (ci==3) y=val;
       if (ci==4) ey=val;
       if (ci>=5) {
	//cout<<" MyData::ReadData: isyst= "<<isyst<<" ci= "<<ci<<" mod= "<<(ci-5)%2<<endl;

        if (tableplusminus) {
	  if ((ci-5)%2==0) {
          isyst++;
          eyh.push_back(atof( split_line.at(ci).c_str() ));
          eyl.push_back(atof( split_line.at(ci+1).c_str() ));
         }
        } else {
         isyst++;
         eyh.push_back(atof( split_line.at(ci).c_str() ));
         eyl.push_back(atof( split_line.at(ci).c_str() ));
        }
       }
      }
     }

     if (tableplusminus&&split_line.size()==2) {
      if (debug) cout << " MyData::ReadData read line syst= "<<isyst<<endl; 
      isyst++;
      eyh.push_back( atof( split_line.at(0).c_str() ));
      eyl.push_back( atof( split_line.at(1).c_str() ));
      newbin=false;
     }
 
     //if (newbin) cout<<" end newbin on "<<endl;
     //else       cout<<" end newbin off "<<endl;

     double myy=y, myey=ey;
     if (errorinpercent) {myey *=y/100.;}
     myy  *=myscale; myey *=myscale;

     if (miny>y) miny=myy;
     if (maxy<y) maxy=myy;
     if (minx>xl) minx=xl;
     if (maxx<xh) maxx=xh;

     float x= (xh+xl)/2.;
     float b= (xh-xl)/2.;

     if (debug) {
      cout<<" MyData::ReadData: nbin= "<<nbin<<" isyst= "<<isyst
          <<" x= "<<x<<" xl= "<<xl<<" xh= "<<xh<<" y= "<<myy<<" ey= " << myey << endl;
      for (int i=0; i<(int)eyl.size(); i++) {
       cout<<" MyData::ReadData: i= "<<i<<" eyh= "<<eyh[i]<<" eyl= "<<eyl[i]<<endl;
      }
     }
     
     datavector->SetPoint(nbin ,xm, myy);
     datavector->SetPointEXlow (nbin ,xm-x+b); 
     datavector->SetPointEXhigh(nbin,x+b-xm); 

     datavectortoterr->SetPoint(nbin ,xm, myy);
     datavectortoterr->SetPointEXlow (nbin ,xm-x+b); 
     datavectortoterr->SetPointEXhigh(nbin,x+b-xm); 

     if (debug) {
      cout<<" MyData::ReadData: datavector: "<<endl;
      datavector->Print();
     }

     datavectorstat->SetPoint(nbin ,xm, myy);
     datavectorstat->SetPointEXlow (nbin ,xm-x+b); 
     datavectorstat->SetPointEXhigh(nbin,x+b-xm); 
     datavectorstat->SetPointEYlow (nbin,myey); 
     datavectorstat->SetPointEYhigh(nbin,myey); 

     if (debug) {
      cout<<" datavectorstat: "<<endl;
      datavectorstat->Print();
     }

     if (debug) cout<<" MyData::ReadData: number of systematics= "<<eyl.size()<<endl;

     if (debug) cout<<" MyData::ReadData: datavectorsystcomp.size() = "<<datavectorsystcomp.size()<<endl;

     if (datavectorsystcomp.size()>0) {
      for (int i=0; i<(int)eyl.size(); i++) {
       if (debug) cout<<" MyData::ReadData: i= "<<i<<endl;
       double myeyh=eyh[i], myeyl=eyl[i];
       if (errorinpercent) {myeyh *=y/100.; myeyl *=y/100.;}
       if (debug) cout<<" MyData::ReadData: huhu i= "<<i<<endl;
       myeyl *=myscale; myeyh *=myscale; 

       if (debug) cout<<" MyData::ReadData: eyl.size()= "<<eyl.size()<<endl;
       if (datavectorsystcomp.size()<i) {
        cout<<" MyData::ReadData datavectorsystcomp["<<i<<"] not found "<<endl;
        continue;
       }
       if (debug) cout<<" MyData::ReadData: nbin= "<<nbin<<" xm= "<<xm<<endl; 
 
       datavectorsystcomp[i]->SetPoint(nbin ,xm, myy);
       datavectorsystcomp[i]->SetPointEXlow (nbin,xm-x+b); 
       datavectorsystcomp[i]->SetPointEXhigh(nbin,x+b-xm); 
       datavectorsystcomp[i]->SetPointEYlow (nbin,myeyl); 
       datavectorsystcomp[i]->SetPointEYhigh(nbin,myeyh); 
      }
     }
    }
   }
  } 
 } 

 if (debug) cout<<" MyData::ReadData: nsyst= "<<nsyst
                <<" datavectorsystcomp.size()= "<<datavectorsystcomp.size()<<endl;  
 if (nsyst!=(int)datavectorsystcomp.size()) 
  cout<<" MyData::ReadData: something is wrong nsyst not equal vector size "<<endl;  

 if (debug) {
  for (int i=0; i<nsyst; i++) {
   cout<<" \n MyData::ReadData: datavectsyst["<<i<<"]: "<<datavectorsystcomp[i]->GetName()<<endl;
   datavectorsystcomp[i]->Print();
  }

  cout<<" \n MyData::ReadData: datavectorstat "<<endl;
  datavectorstat->Print();
 }

 for (int ibin=0; ibin<datavectorstat->GetN(); ibin++) {
  double eytoth=datavectorstat->GetErrorYhigh(ibin);
  double eytotl=datavectorstat->GetErrorYlow (ibin);
  if (eytoth!=eytotl) cout<<" MyData::ReadData: some is wrong eystath= "<<eytoth<<" eystatl= "<<eytotl<<endl;

  for (int i=0; i<nsyst; i++) {
   double meyh=datavectorsystcomp[i]->GetErrorYhigh(ibin);
   double meyl=datavectorsystcomp[i]->GetErrorYlow (ibin);

   eytoth = TMath::Sqrt(pow(eytoth, 2.) + pow(meyh, 2.));
   eytotl = TMath::Sqrt(pow(eytotl, 2.) + pow(meyl, 2.));

   //if (debug) cout<<" MyData::ReadData: ibin= "<<ibin<<" i= "<<i
   //               <<" eyh= "<<meyh<<" eyl= "<<meyl
   //               <<" eytoth= "<<eytoth<<" eytotl= "<<eytotl<<endl;
  }
  datavectortoterr->SetPointEYhigh(ibin,eytoth);
  datavectortoterr->SetPointEYlow (ibin,eytotl);
 }

 if (debug) {
  cout<<" \n MyData::ReadData datavectortoterr "<<endl;
  datavectortoterr->Print();
 }

 //cout<<" fname= "<<fname<<endl;
 if (corrfrommatrix) {
  if (debug) cout<<" MyData::ReadData: read corrfilename= "<<corrfilename<<endl;
  this->ReadCorrelations();
 } else {
  int Nbin=datavectortoterr->GetN();

  corr_matrixtot  = new TMatrixT<double>(Nbin, Nbin);
  corr_matrixsyst = new TMatrixT<double>(Nbin, Nbin);

  //TMatrixT <double >*cov_matrixk  = new TMatrixT<double>(nbin, nbin);
  // need to do this since addition of matrices does not work either += or Plus()
  double cov_matrixk[Nbin][Nbin];
  for (int i=0; i<Nbin; i++) {
   for (int j=0; j<Nbin; j++) {
    cov_matrixk[Nbin][Nbin]=0.;
   }
  }

  cov_matrixtot  = new TMatrixT<double>(Nbin, Nbin);
  cov_matrixsyst = new TMatrixT<double>(Nbin, Nbin);
  if (datavectorsystcomp.size()==1) {
   cout<<" MyData::ReadData: use unit matrix as correlation matrix "<<endl;
   cov_matrixtot->UnitMatrix();
   cov_matrixsyst->UnitMatrix();
  }

  cov_matrix_ok=true;

  //if (debug) {
  // cout<<" MyData::ReadData: print unity matrix "<<nbin<<"x"<<nbin<<endl;
  // cov_matrixtot->Print();
  //}

  if (datavectorsystcomp.size()>1) {
   cout<<" MyData::ReadData: calculate correlation matrix from uncertainty components "
       <<" nsyst= "<<datavectorsystcomp.size()<<endl;

   //
   // put here something for statistical correlation matrix
   // fetch stat uncertainty
   // if (i==j)  (*covMatStat_)[i][j] = pow( gstat->GetErrorYhigh(i), 2 );
   //
   for (int k=0; k<(int)datavectorsystcomp.size(); k++) {
    for ( int ibin=0; ibin<nbin; ibin++ ){
     for ( int jbin=0; jbin<nbin; jbin++ ){
      double cov_val = 0.;

      double iyval=0., jyval=0., xval=0.;
      datavectorsystcomp[0]->GetPoint(ibin,xval,iyval);
      datavectorsystcomp[0]->GetPoint(jbin,xval,jyval);

      // could implement here something for +/- uncertainties
      double ibin_err = 0.5*(datavectorsystcomp[k]->GetErrorYhigh(ibin) + datavectorsystcomp[k]->GetErrorYlow(ibin));
      double jbin_err = 0.5*(datavectorsystcomp[k]->GetErrorYhigh(jbin) + datavectorsystcomp[k]->GetErrorYlow(jbin));
      
      // relative uncertainties
      //ibin_err=1;
      //jbin_err=1;
      //ibin_err/=iyval;
      //jbin_err/=jyval;

      if (debug) cout <<" k= "<<k<<" ibin= "<<ibin<<" ibin_err= "<<ibin_err
          	      <<" jbin= "<<jbin<<" jbin_err= "<<jbin_err<<endl;
     
      cov_val +=ibin_err*jbin_err;
      //(*cov_matrixk)(ibin, jbin) = cov_val;
      cov_matrixk[ibin][jbin]+=cov_val;

     }
    }
   }

   if (debug) cout <<" MyData::ReadData: now fill covarince and correlation matrix "<<endl;

   for (int i=0; i<nbin; i++) {
    for (int j=0; j<nbin; j++) {

     double eystath=0., eystatl=0., eystat=0.;
     if (i==j)  {
      eystath=datavectorstat->GetErrorYhigh(i);
      eystatl=datavectorstat->GetErrorYlow (i);
      if (eystath!=eystatl) cout<<" MyData::ReadData some is wrong eystath= "<<eystath<<" eystatl= "<<eystatl<<endl;
      eystat=0.5*(eystath+eystatl);
     }

     //if (debug) cout << " MyData::ReadData: systematic correlation matrix eystat= "<<eystat << endl;

     (*cov_matrixsyst)(i,j)=cov_matrixk[i][j];
     (*cov_matrixtot )(i,j)=sqrt(pow(eystat,2)+pow(cov_matrixk[i][j],2));

     double sig=cov_matrixk[i][i]*cov_matrixk[j][j];
     if (sig!=0.) sig=sqrt(sig);

     (*corr_matrixsyst)(i,j)=cov_matrixk[i][j]/sig;
     // is the below correct ? changed nothing see below
     (*corr_matrixtot)(i,j) =sqrt(pow(eystat,2)+pow(cov_matrixk[i][j],2))/sqrt(pow(eystat,2)+pow(sig,2));
    }
   }

   //(*cov_matrix)+=(*cov_matrixk);
   //cov_matrix->Plus(*cov_matrix,*cov_matrixk);

   if (debug) {
    cout << " MyData::ReadData: using correlation matrix from uncertainties components " << endl;
    cout << " MyData::ReadData: systematic correlation matrix: " << endl;
    corr_matrixsyst->Print();

    // total correllation matrix is not different, if from stat uncertainty (diagonal elements normalised out)
    //cout << " MyData::ReadData: total correlation matrix: " << endl;
    //corr_matrixtot->Print();

    cout<<" MyData::ReadData: print systematics covariance matrix: "<<endl;
    cov_matrixsyst->Print();
    cout<<" MyData::ReadData: total covariance matrix "<<endl;
    cov_matrixtot->Print();
   }
  }
 }

return;
}



void MyData::Print() {

 cout<<"\n MyData::Print>>>>>>>>>>>>>>>>>>>>>>>>>>>> \n "<<endl;
 if (debug) cout<<" debug flag on "<<endl;
 cout<<" reaction= "<<reaction.Data()<<endl;
 cout<<" observable= "<<observable.Data()<<endl;
 cout<<" filename= "<<filename.Data()<<endl;
 cout<<" name= "<<dataname.Data()<<endl;
 //cout<<" titlex= "<<titlex.Data()<<endl;
 //cout<<" titley= "<<titley.Data()<<endl;
 cout<<" label= "<<label.Data()<<endl;
 cout<<" s= "<<sqrts<<" GeV "<<endl;
 cout<<" year= "<<year<<endl;

 //cout<<" msize= "<<msize<<" markerstyle= "<<markerstyle<<" markercolor= "<<markercolor<<endl;

 if (blogy) cout<<" axis y log-scale"<<endl;
 if (bliny) cout<<" axis y lin-scale"<<endl;
 if (blogx) cout<<" axis x log-scale"<<endl;
 if (blinx) cout<<" axis x lin-scale"<<endl;
 if (ymin!=0 || ymax!=0)
  cout<<" ymin= "<<ymin<<" ymax= "<<ymax<<endl;

 cout<<"\n Cuts: "<<endl;
 cout<<" Jet "<<jetalgo.Data()<<" R = "<< float(jetalgoR)/10.<<endl;
 if (njetcut) cout<<"\t Njets=> "<<njets<<endl;
 if (jetcut) cout<<"\t Ptjet> "<<ptjetcut<<" GeV "<<yjetmin<<" <yjet< "<<yjetmax<<endl;
 if (lepcut) cout<<"\t Ptlep> "<<ptlepcut<<" GeV "<<ylepmin<<" <ylep< "<<ylepmax<<endl;
 if (neucut) cout<<"\t Ptneu> "<<ptneucut<<" GeV "<<" MTw> " <<Mtwcut<<" GeV"<<endl;
 if (cutMtw) cout<<"\t Mtw> "<<Mtwcut<<" GeV "<<endl;
 if (drljcut)cout<<"\t dRlepJet> "<<deltaRLepJet<<" GeV "<<endl;

 if (systcorrelationmatrix) cout << " systcorrelationmatrix on \n";
 if (tableplusminus) cout << " Table has +/- values on \n";
 cout<<"  columnname= "<<columnname.Data()<<endl;

 // 
 //cout<<" datavector= "<<datavector<<endl;
 if (!datavector){
  cout<<"\n datavector not found "<<endl;
 } else {
  cout<<" datavector= "<<datavector->GetName()<<endl;
  datavector->Print();
 }

 /*
 if (!datavectorsyst){
  cout<<" datavectorsyst not found "<<endl;
 } else {
  cout<<" datavectorsyst= "<<datavectorsyst->GetName()<<endl;
  datavectorsyst->Print();
 }
 if (!datavectorstat){
  cout<<" datavectorstat not found "<<endl;
 } else {
  cout<<" datavectorstat= "<<datavectorstat->GetName()<<endl;
  datavectorstat->Print();
 }
 */

 cout<<" <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< \n "<<endl;

}

/*
void MyData::PrintXt() {

 cout<<" MyData::PrintXt>>>>>>>>>>>>>>>>>>>>>>>>>>>> "<<endl;
 if (debug) cout<<" debug flag on "<<endl;
 cout<<" s= "<<sqrts<<" ymin= "<<ymin<<" ymax= "<<ymax<<endl;
 cout<<jetalgo.Data()<<" R = "<< float(jetalgoR)/10.<<" year= "<<year<<endl;
 cout<<" msize= "<<msize<<" markerstyle= "<<markerstyle<<" markercolor= "<<markercolor<<endl;
 //cout<<" xtdatavector= "<<xtdatavector<<endl;
 if (!xtdatavector){
  cout<<" xtdatavector not found "<<endl;
 } else {
  cout<<" xtdatavector= "<<datavector->GetName()<<endl;
  xtdatavector->Print();
 }
 cout<<" <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< "<<endl;

}

void MyData::DrawXt(char name[100], float x, float y){

  if (debug) cout<<" DrawXt: name= "<<name<<" x= "<<x<<" y= "<<y<<endl;

  this->TransformPtToXt();

  this->DrawLegend(name,x,y);

  if (debug) 
   this->PrintXt();

   this->DrawDataXt();

  return;

}

void MyData::TransformPtToXt(){
//
// transform dsigma/dpt to dsigma/dxt
//
//

 xtdatavector    = new TGraphAsymmErrors();
 xtdatavectorstat= new TGraphAsymmErrors();
 xtdatavectorsyst= new TGraphAsymmErrors();

 TString fname("xt"); fname+=datavector->GetName();
 xtdatavector->SetName(fname);
 TString fnamestat= datavector->GetName(); fnamestat+="stat";
 xtdatavectorstat->SetName(fnamestat);
 TString fnamesyst= datavector->GetName(); fnamesyst+="stat";
 xtdatavectorsyst->SetName(fnamesyst);

 if (!datavector) {cout<<" Transform: datavector not found !"<<endl;}

 double * X1 = datavector->GetX();
 double * Y1 = datavector->GetY();
 double * EXhigh = datavector->GetEXhigh();
 double * EXlow =  datavector->GetEXlow();

 double * EYhigh = datavector->GetEYhigh();
 double * EYlow =  datavector->GetEYlow();

 if (!datavectorsyst) {cout<<" Transform: datavectorsyst not found !"<<endl;}
 double * EYhighsyst = datavectorsyst->GetEYhigh();
 double * EYlowsyst  = datavectorsyst->GetEYlow();

 if (!datavectorstat) {cout<<" Transform: datavectorstat not found !"<<endl;}
 double * EYhighstat = datavectorstat->GetEYhigh();
 double * EYlowstat  = datavectorstat->GetEYlow();


 double ymean=(ymax-ymin)/2.;
 //double fac=2.*exp(+ymean)/sqrts;
 //double fac=2.*exp(-ymin)/sqrts;
 double  fac=2./sqrts;

 if (debug)
  cout<<" Transform: ymean= "<<ymean<<" sqrts= "<<sqrts<<" fac= "<<fac
      <<" number of points= "<<datavector->GetN()<<endl;

 for (int i=0; i<datavector->GetN(); i++) {
 
  double x=X1[i]*fac;
  double exl=EXlow[i]*fac;
  double exh=EXhigh[i]*fac;

  double facy=X1[i]*X1[i]*X1[i]/(2*3.141592654);
  double y= Y1[i]*facy;
  double eyl= EYlow[i]*facy;
  double eyh= EYhigh[i]*facy;

  double eylsyst= EYlowsyst[i]*facy;
  double eyhsyst= EYhighsyst[i]*facy;

  double eylstat= EYlowstat[i]*facy;
  double eyhstat= EYhighstat[i]*facy;

  //cout<<" i= "<<i<<" pt= "<<X1[i]<<" sigma= "<<Y1[i]<<" fac= "<<fac<<endl;
  //cout<<" i= "<<i<<" x= "<<x<<" y= "<<y<<" facy="<<facy<<endl;

  xtdatavector->SetPoint(i, x, y);
  xtdatavector->SetPointError(i,exl,exh,eyl,eyh);

  xtdatavectorstat->SetPoint(i, x, y);
  xtdatavectorstat->SetPointError(i,exl,exh,eylstat,eyhstat);

  xtdatavectorsyst->SetPoint(i, x, y);
  xtdatavectorsyst->SetPointError(i,exl,exh,eylsyst,eyhsyst);
 }

 if (debug) cout<<" Transform finished ! "<<endl;
 //xtdatavectorsyst->Print();

 return;

}

*/
void MyData::PutData(TH1* hdata, double mys, double myymin, double myymax){
  //
  // input data in histogram
  //
 if (!hdata) cout<<" MyData:: PutData: hdata not found "<<endl;
 ymin=myymin;
 ymax=myymax;
 sqrts=mys;

 if (debug) cout<<" MyData::PutData: input data s= "<<sqrts
                <<" ymin= "<<ymin<<" ymax= "<<ymax<<endl;

 if (debug) 
  hdata->Print();
 

 TGraphAsymmErrors* g1= new TGraphAsymmErrors();
 if (!g1) cout<<" MyData:: PutData: graph g1 not created "<<endl;
 // else g1->Print();

 double x, y, ex, ey;
 int iv=0;
 for (int i=0; i<hdata->GetNbinsX(); i++) {
   y=hdata->GetBinContent(i+1);
  ey=hdata->GetBinError(i+1);
   x=hdata->GetBinCenter(i+1);
   ex=hdata->GetBinWidth(i+1)/2.; 
  if (y!=0 && ey!=0){
    // cout << i<<" x,y = " << x << " " << y << " ex,ey = " << ex << " " << ey << endl;
   g1->SetPoint(iv,x,y);
   g1->SetPointError(iv,ex,ex,ey,ey);
   iv++;
  }
 }
 
 //g1->Print();
 datavector=g1;

 // if (debug){
 //cout<<"MyData::PutData: after conversion to TGraphAsymmError:"<<endl;
 //if (datavector) datavector->Print();
  //}

  //cout<<"MyData::PutData return :"<<endl;

 return;
}

void MyData::DrawData(){

 if (debug) cout << " MyData::DrawData " << endl;
 for(int pi = 0; pi < datavector->GetN(); pi++) {
 /*
  if (debug) {
   Double_t x_val_dv;    Double_t  y_val_dv;
   datavector->GetPoint(pi, x_val_dv, y_val_dv);
   Double_t x_val_dvs;    Double_t y_val_dvs;
   datavectorsyst->GetPoint(pi, x_val_dvs, y_val_dvs);
   Double_t ylowerr = datavectorsyst->GetErrorYlow(pi);
   Double_t yhigherr = datavectorsyst->GetErrorYhigh(pi);

   std::cout << "pi: " << pi << ", x_val_dv: " << x_val_dv << ", x_val_vs: " << x_val_dvs << ", y_val_dv: " << y_val_dv << ", dvs: " << y_val_dvs << " -" << ylowerr << " +" << yhigherr << "\n";
  };
 */
 };

 if (!datavector) cout<<" DrawData: datavector not found "<<endl;
 datavector->SetMarkerStyle(markerstyle);
 datavector->SetMarkerColor(markercolor);
 datavector->SetMarkerSize(markersize);
 //datavector->Draw("P same");

 if (!datavectorstat) cout<<" DrawData: datavectorstat not found "<<endl;
 datavectorstat->SetMarkerStyle(markerstyle);
 datavectorstat->SetMarkerColor(markercolor);
 datavectorstat->SetMarkerSize(markersize);
 datavectorstat->Draw("e1");
 //datavectorstat->Draw("P same");

 if (!datavectortoterr) cout<<" DrawData: datavectortoterr not found "<<endl;
 datavectortoterr->SetMarkerStyle(markerstyle);
 datavectortoterr->SetMarkerColor(markercolor);
 datavectortoterr->SetMarkerSize(markersize);
 datavectortoterr->Draw("PX same");

 if (debug) {
  cout<<" MyData::DrawData   datavectorstat "<<endl;
  datavectorstat->Print("all");
  cout<<" MyData::DrawData   datavectortoterr "<<endl;
  datavectortoterr->Print("all");
 }

  return;
};

void MyData::DrawDataStatOnly(){
   if (!datavectorstat) cout<<" DrawDataStatOnly: datavectorstat not found "<<endl;
   datavectorstat->SetMarkerStyle(markerstyle);
   datavectorstat->SetMarkerColor(markercolor);
   datavectorstat->SetMarkerSize(1.2);
   datavectorstat->Draw("e1");
   return;
};
/*
void MyData::DrawDataXtStatOnly(){
   if (!xtdatavectorstat) cout<<" DrawDataStatOnly: datavectorstat not found "<<endl;
   xtdatavectorstat->SetMarkerStyle(markerstyle);
   xtdatavectorstat->SetMarkerColor(markercolor);
   xtdatavectorstat->SetMarkerSize(1.2);
   return;
};

void MyData::DrawDataXt(){

   if (!xtdatavector) cout<<" DrawDataXt: xtdatavector not found "<<endl;
   else if (debug) xtdatavector->Print();

   xtdatavector->SetMarkerStyle(markerstyle);
   xtdatavector->SetMarkerColor(markercolor);
   xtdatavector->SetMarkerSize(1.2);
   xtdatavector->Draw("P");

   if (!xtdatavectorsyst) cout<<" DrawDataXt: xtdatavectorsyst not found "<<endl;
   xtdatavectorsyst->SetMarkerStyle(markerstyle);
   xtdatavectorsyst->SetMarkerColor(markercolor);
   xtdatavectorsyst->SetMarkerSize(1.2);
   xtdatavectorsyst->Draw("||");

   if (!xtdatavectorstat) cout<<" DrawDataXt: xtdatavectorstat not found "<<endl;
   xtdatavectorstat->SetMarkerStyle(markerstyle);
   xtdatavectorstat->SetMarkerColor(markercolor);
   xtdatavectorstat->SetMarkerSize(1.2);

   return;

};
*/

void MyData::DrawData(char name[100], float x, float y){
 
 this->DrawLegend(name,x,y);
 //if (debug) 
 // this->PrintXt();

 this->DrawData();

 return;
} 

void MyData::DrawExperimentandYear(double x, double y) {
  
 Double_t tsize=0.05;
 this->DrawExperiment(x,y);
 x+=0.088;
 //y-=0.05;
 TLatex l; l.SetTextAlign(12); //l.SetTextSize(tsize); 
 l.SetNDC(); l.SetTextSize(tsize);

 char mydate[100];
 sprintf(mydate,"%d",year);
 //cout<<" year= "<<year<<" mydate= "<<mydate<<endl;
 l.DrawLatex(x,y,mydate);

 return;
};



void MyData::DrawLegend(char name[100], float x, float y){
 
   char text1[100]; char text2[100]; char text3[100];char text4[100];

   TMarker *marker = new TMarker(x-(0.6*msize),y,8);
   marker->SetMarkerColor(markercolor);  marker->SetNDC();
   marker->SetMarkerStyle(markerstyle);
   marker->Draw();
   TLatex l1; l1.SetTextAlign(12); l1.SetNDC(); l1.SetTextSize(msize);
   TLatex l2; l2.SetTextAlign(12); l2.SetNDC(); l2.SetTextSize(msize);
   TLatex l3; l3.SetTextAlign(12); l3.SetNDC(); l3.SetTextSize(msize);
   TLatex l4; l4.SetTextAlign(12); l4.SetNDC(); l4.SetTextSize(msize);
   sprintf(text1,"%s",name);
   sprintf(text2,"%d",year);
   sprintf(text3,"#sqrt{s} = %4.0f GeV",sqrts);
   double fractpart, intpart;
   fractpart=modf (ymax*10., &intpart);

   //cout<<" ymax= "<<ymax<<" fractpart= "<<fractpart<<" intpart= "<<intpart<<endl;

   if (fractpart<0.01) sprintf(text4,"%3.1f< |y| < %3.1f",ymin,ymax);
   else                sprintf(text4,"%3.2f< |y| < %3.2f",ymin,ymax);

   if (fabs(ymin)<0.01) {
    if (fractpart<0.01)  sprintf(text4," |y| < %3.1f",ymax);
    else                 sprintf(text4," |y| < %3.2f",ymax);

   }
   //cout<<" test4= "<<text4<<endl;
   //sprintf(text,"%s  %s %3.1f< |y| < %3.1f",name,text1,ymin,ymax);
   l1.DrawLatex(x,y,text1);
   l2.DrawLatex(x+0.09,y,text2);
   l3.DrawLatex(x+0.16,y,text3);
   l4.DrawLatex(x+0.34,y,text4);

   return;
}

//
void MyData::ScaleGraph(TGraphAsymmErrors *g1, double scalex, double scaley){

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
};

void MyData::Scale(double scalex, double scaley){
  this->ScaleGraph(datavector      ,scalex,scaley);
  this->ScaleGraph(datavectorstat  ,scalex,scaley);
  this->ScaleGraph(datavectorsyst  ,scalex,scaley);
  this->ScaleGraph(datavectortoterr,scalex,scaley);
  miny*=scaley; maxy*=scaley; minx*=scalex; maxx*=scalex;  
  return;
};

void MyData::split_string(std::string str, std::vector<std::string>& split_results, std::string delimiters)
{
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
}

