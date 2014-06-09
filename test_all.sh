
#!/bin/zsh
#
# just plot theory calculation with fine binning
./plot topfinept.txt

# compare Atlas EPJ data to CMS
./plot atlasepjcms_top.txt

# compare Atlas EPJ data to ATLAS new data
./plot atlasepjatlas.txt

# compare Atlas new data to CMS data
./plot atlascms_top.txt

# compare Atlas new data to theory
./plot atlas2012_top.txt

# compare Atlas new data to theory
#./plot_old atlas2012_alphaS_top.txt
