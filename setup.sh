echo "-----> LOCAL SETUP STARTED!"

###########################################
#               basic setup               #
###########################################

echo "-----> Basic setup performed during /.bashrc from script $BASIC_SETUP"


###########################################
# link source code from other directories #
###########################################

#lhapdf sets
echo "-----> Linking LHAPDF sets..."
#ln -s `lhapdf-config --pdfsets-path`  PDFsets
ln -s /afs/cern.ch/work/c/cembree/public/PDFsets  PDFsets

#R and B root files
echo "-----> Linking sherpaNTuples..."
ln -s /afs/cern.ch/work/c/cembree/public/sherpatop21inputs .

#other MyAnalysis files
echo "-----> Linking various MyAnalysis files..."
export SRC=/afs/cern.ch/user/c/cembree/public/workspace/MyAnalysis

if (test -f MyCrossSection.cxx) then
 echo MyCrossSection file found
else
 ln -s  $SRC/MyCrossSection.cxx .
 ln -s  $SRC/MyCrossSection.h .
fi

if (test -f MyData.cxx) then
 echo MyData file found
else
 ln -s  $SRC/MyData.cxx .
 ln -s  $SRC/MyData.h .
fi

if (test -f MyFrame.cxx) then
 echo MyFrame file found
else
 ln -s  $SRC/MyFrame.cxx .
 ln -s  $SRC/MyFrame.h .
 ln -s  $SRC/MyFrameData.cxx .
 ln -s  $SRC/MyFrameData.h .
fi

if (test -f MyPDF.cxx) then
 echo MyPDF found
else
 ln -s  $SRC/MyPDF.cxx .
 ln -s  $SRC/MyPDF.h . 
fi

if (test -f MyBand.cxx) then
 echo MyPDF found
else
 ln -s  $SRC/MyBand.cxx .
 ln -s  $SRC/MyBand.h .
fi

echo "-----> LOCAL SETUP COMPLETE!"
echo ""