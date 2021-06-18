import numpy as np
import csv
import os
import sys
import ROOT

xsec = 0
n = 0

for subdir, dirs, files in os.walk('/scratch/physdoe_project_root/physdoe_project/nastein/' + sys.argv[1]):
  for file in files:
    filepath = subdir + os.sep + file
    if filepath.endswith("results.html"):
      with open(filepath) as inp:
        for line in inp:
          if line.startswith("&nbsp"):
            parts = line.split(' ')

            #print(parts[1])
            xsec += float(parts[1])
            n += 1


xsec = xsec/n
print('Average xsec for ' +  sys.argv[1] + ' = ' + str(xsec) )
print('Writing to file ' + sys.argv[1] + '.root now' )
f = ROOT.TFile('/scratch/physdoe_project_root/physdoe_project/nastein/' + sys.argv[1] + '/' + sys.argv[1] + '.root', "UPDATE")
x = ROOT.TVectorD(2)
x[0] = xsec
x[1] = int(f.Delphes.GetEntries())
print('Total number of events for ' +  sys.argv[1] + ' = ' + str(x[1]) )
x.Write("File_details")
f.Close()
