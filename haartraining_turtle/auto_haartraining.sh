#!/bin/sh
# Automate Haar Training

rm -r negatives.dat positives.dat samples.dat samples.vec Samples/*  haarcascade
echo -e "\n \t### Cleaned Previous Output files ###"

find ./Negative_Images -name '*.png' >negatives.dat

if [ $? -eq 0 ]; then
	echo -e "\n\t ### Created Negatives dat file ###"
	find ./Positive_Images -name '*.png' > positives.dat
else
	echo -e "\n\t\tError Creating Negatives dat file!!"
fi

if [ $? -eq 0 ]; then
	echo -e "\n\t ### Created Positives dat file ###"
	perl createtestsamples.pl positives.dat negatives.dat Samples 100 "./createsamples -bgcolor 0 -bgthresh 0 -maxxangle 1.1 -maxyangle 1.1 maxzangle 0.5 -maxidev 40 -w 20 -h 20"
else
	echo -e "\n\t\tError Creating Positives dat file!!!"
fi

if [ $? -eq 0 ]; then
	echo -e "\n\t ### Successfully Created Samples for Haartraining ###"
	find Samples/ -name '*.vec' > samples.dat
else
	echo -e "\n\t\tError Creating Haartraining Samples!!!"
fi

if [ $? -eq 0 ]; then
	echo -e "\n\t ### Successfully Accumulated Different Vector files into 'samples.dat' ###"
	./mergevec samples.dat samples.vec
else
	echo -e "\n\t\tError Creating 'samples.dat'!!!"
fi

if [ $? -eq 0 ]; then
	echo -e "\n\t ### Successfully Created Unified Sample Data for haar training i.e 'samples.vec' ### "
	opencv_haartraining -data haarcascade -vec samples.vec -bg negatives.dat -nstages 20 -nsplits 2 -minhitrate 0.999 -maxfalsealarm 0.5 -npos 19  -nneg 15 -w 20 -h 20 -nonsym -mem 2048 -mode ALL
else
	echo -e "\n\t\tError Creating Unified Sample Data i.e. samples.vec !!!"
fi

if [ $? -eq 0 ]; then
	echo -e "\n\t ### Successfully Executed 'opencv_haartraining' and produced XML file ###"
else
	echo -e "\n\t\tError!!! Executing 'opencv_haartraining'  !!!"
fi