Scripts written to analyse the data taken at the A2 hall tagger in Mainz in July and August 2025.

<sixteenSiPMCoincidence.C> is the most important script, and most of the other scripts use the outputs it produces.
It looks for coincidence events, then produces histograms providing more information about the coincidences.
The important one is the totalEventEnergy histogram, which is a sum of the energies of all the channels that were trigged in a coincidence.

N.B. A coincidence is defined by the following occuring within 25 ns:
    1. A large hit in channel 0 (> 1000 ADC),
    2. A hit in channel 2 (directly behind channel 0),
    3. Another hit in a later layer.


<alternateLayout16SiPMsCoincidences.C> does the same but for the 1555 layout of readout.

<taggerPositionEnergy.C> compares the totalEventEnergy histograms for multiple runs of choice.

<totalEnergyMultipleRuns.C> combines the totalEventEnergy histogram runs at the same tagger position.

<multiRunTaggerPositionEnergy.C> does as above but for the combined histograms

<blockArrangements.C> compares the runs at Tagger 18 with blocks moved around.

<renameTaggers.sh> is to deal with the runs with incorrect naming schemes.
A few runs were labelled with the wrong tagger in the root file name, and some have "-" where there should be "_"


The root file from each subrun is stored in runData/ and the output histograms and files are saved in outputHistos/
