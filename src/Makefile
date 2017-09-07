CXX = g++
CFLAGS = -g -Wall 
all: aos
OBJS = Start.o Data.o Experiment.o Parameter.o Result.o Vector.o\
	Model.o ModelAROMA.o ModelAROMAD.o ModelCWS.o ModelCWSD.o ModelOASIS.o 

aos: $(OBJS)	
	$(CXX) $(CFLAGS) -o aos $(OBJS) 
	# -O1 -larmadillo

AOS_main.o: Start.cpp
	$(CXX) $(CFLAGS) -c Start.cpp

Data.o: Data.h
	$(CXX) $(CFLAGS) -c Data.h Data.cpp

Experiment.o: Experiment.h Experiment.cpp
	$(CXX) $(CFLAGS) -c Experiment.h Experiment.cpp

Parameter.o: Parameter.h Parameter.cpp
	$(CXX) $(CFLAGS) -c Parameter.h Parameter.cpp

Result.o: Result.h Result.cpp
	$(CXX) $(CFLAGS) -c Result.h Result.cpp	

Model.o: Model.h Model.cpp
	$(CXX) $(CFLAGS) -c Model.h Model.cpp
	
Vector.o: Vector.h Vector.cpp
	$(CXX) $(CFLAGS) -c Vector.h Vector.cpp

modelAROMA.o: modelAROMA.h modelAROMA.cpp
	$(CXX) $(CFLAGS) -c modelAROMA.h modelAROMA.cpp

modelAROMAD.o: modelAROMAD.h modelAROMAD.cpp
	$(CXX) $(CFLAGS) -c modelAROMAD.h modelAROMAD.cpp

modelCWS.o: modelCWS.h modelCWS.cpp
	$(CXX) $(CFLAGS) -c modelCWS.h modelCWS.cpp

modelCWSD.o: modelCWSD.h modelCWSD.cpp
	$(CXX) $(CFLAGS) -c modelCWSD.h modelCWSD.cpp

modelOASIS.o: modelOASIS.h modelOASIS.cpp
	$(CXX) $(CFLAGS) -c modelOASIS.h modelOASIS.cpp

clean:
	rm -rf *o *gch
