
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
./plot atlascms_top_nopdf.txt
./plot atlascms_top_ct10.txt


# compare sherpa and mcfm top
./plot sherpavmcfm_nopdf.txt
./plot sherpavmcfm_pdf.txt


# compare Atlas new data to theory
./plot atlas2012_top.txt


# Atlas top data cross various PDFs - origionally made for claire
./plot atlas_mtt_ptt_top_1.txt
./plot atlas_mtt_ptt_top_2.txt


# same as plots above but for every supported PDF. Just for testing.
./plot atlas_top_allpdf.txt


# compare Atlas new data to theory
#./plot_old atlas2012_alphaS_top.txt