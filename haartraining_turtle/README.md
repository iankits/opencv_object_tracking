

* What I did:

1. Compiled mergevec manually for MacOS
2. Made changes in perl script "createtestsamples.pl" to add vector output. 
3. So that the different vectors of the samples can be later on merged using ./mergevec And
4. Run "opencv_haartraining" on data set to generate haarcascade.xml dataset.
5. create xml file from the dataset created by opencv_haartraining using command

$ ./convert_cascade --size="25x25" haarcascade haarcascade-inter.xml

* I automated the above steps 1-4:

$ ./auto_haartraining.sh

* Written a c file using haartraining lib to run sample video to detect turtle.

To compile (Run Script):

$ . build.detect
$ ./turtledetect ../samples/sample_95.mp4

 * To compile mergevec.cpp in MacOS

1. download haartraing folder which contains all files
2. Follow this incase not able to compilehttp://stackoverflow.com/questions/6079504/cannot-compile-mergevec-cpp-from-haartraining-tutorial

and use following command for compiling: 

g++ `pkg-config --cflags opencv` `pkg-config --libs opencv` -o mergevec mergevec.cpp cvboost.cpp cvcommon.cpp cvsamples.cpp cvhaarclassifier.cpp cvhaartraining.cpp


