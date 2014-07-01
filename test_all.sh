
#!/bin/zsh
#
# just plot theory calculation with fine binning
#./plot topfinept.txt

# compare Atlas EPJ data to CMS
./plot atlasepjcms_top.txt


# compare Atlas EPJ data to ATLAS new data
./plot atlasepjatlas.txt


# compare Atlas new data to CMS data (PDFs: CT10 and HERA)
#  NOTE: articially scaled CMS data in CMS data steering file
./plot atlascms_top.txt


# compare Atlas new data to CMS data (NO PDFs: Grid references)
#  NOTE: articially scaled CMS data in CMS data steering file
./plot atlascms_top_nopdf.txt


# compare Atlas new data to theory
./plot atlas2012_top.txt


# compare Atlas new data to theory
#./plot_old atlas2012_alphaS_top.txt
