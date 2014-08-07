# EXAMPLE SETUP SCRIPT FOR LXPLUS THAT Cameron Embree USED.
#
### generic setup to be run at the start of EVERY LXPLUS session
export BASEDIR=/afs/cern.ch/user/c/cembree/public/progs
#
### home directory of installation like APPLGrid
export MYHOME=/afs/cern.ch/user/c/cembree/public
#
#
#
### setup for SLC6
export platform=x86_64-slc6-gcc46-opt
#
export external=/afs/cern.ch/sw/lcg/external
export releases=/afs/cern.ch/sw/lcg/app/releases
export contrib=/afs/cern.ch/sw/lcg/contrib
#
source ${contrib}/gcc/4.6/${platform}/setup.sh
#
#
#
### Add program paths to standard paths $PATH & LDPATH
export ROOTSYS=/afs/cern.ch/sw/lcg/app/releases/ROOT/5.34.05/${platform}/root
export LHAPDFPATH=${BASEDIR}/lhapdf
export FASTJET=/afs/cern.ch/sw/lcg/external/fastjet/3.0.3/${platform}
export HOPPET=${BASEDIR}/hoppet
export APPLGRID=${BASEDIR}/applgrid
export SHERPA=${BASEDIR}/sherpa
#
### explicitly update paths
export PATH=${FASTJET}/bin:${HOPPET}/bin:${APPLGRID}/bin:${SHERPA}/bin:${ROOTSYS}/bin:${LHAPDFPATH}/bin:${PATH}
export LD_LIBRARY_PATH=${FASTJET}/lib:${HOPPET}/lib:${APPLGRID}/lib:${SHERPA}/lib:${ROOTSYS}/lib:${LHAPDFPATH}/lib:${LD_LIBRARY_PATH}
#
#
#
### Link to APPLgrids and PDFSets to local directory
rm PDFSets
#ln -s ${MYHOME}/workspace/MyAnalysis/PDFsets PDFsets
ln -s `lhapdf-config --pdfsets-path` PDFsets
rm MyGrids
ln -s ${MYHOME}/workspace/MyAnalysis/MyGrids MyGrids
#
#
#
export  ARCH="-m64"
export CXXFLAGS=$ARCH
export F90FLAGS=$ARCH
export CFLAGS=$ARCH
export FFLAGS=$ARCH
export LDFLAGS=$ARCH
