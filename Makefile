CC = g++
I4COPTER_BASE=../../I4Copter/trunk/
I4COPTER_FLIGHTCONTROL=$(I4COPTER_BASE)FlightControl/
I4COPTER_COPTERHARDWARE=$(I4COPTER_BASE)System/CopterHardware/
I4COPTER_DRIVE=$(I4COPTER_COPTERHARDWARE)Drive/

all:
	$(CC) -Ivisualization_library -D SIMULATOR -I /usr/include/freetype2/ -I hardware -I $(I4COPTER_FLIGHTCONTROL) -I $(I4COPTER_COPTERHARDWARE) -I $(I4COPTER_DRIVE) -I $(I4COPTER_BASE) -lGL -lGLEW -lglut -lfreetype -lode -lSDL_net -o simquadcopter-vls visualization_library/vl/*.cpp visualization_library/vlut/*.cpp visualization_library/vlGLUT/*.cpp visualization.cpp quadcopter.cpp balance.cpp udpremote.cpp flightcontrol.cpp LoadPLY2.cpp hardware/*.cpp $(I4COPTER_FLIGHTCONTROL)FlightControl.cpp $(I4COPTER_FLIGHTCONTROL)Axis.cpp $(I4COPTER_FLIGHTCONTROL)Controller.cpp $(I4COPTER_COPTERHARDWARE)PhysicalConfig.cpp

old:
	$(CC) balance.cpp main.cpp quadcopter.cpp udpremote.cpp -o simquadcopter -lGL -lode -lGLU -lSDL_net -g `sdl-config --cflags --libs`

vl:
	$(CC) -Ivisualization_library -Lvisualization_library -lvl -lvlut -lvlGLUT -lode -lSDL_net -o simquadcopter-vl visualization.cpp quadcopter.cpp balance.cpp udpremote.cpp

vl-static:
	$(CC) -Ivisualization_library -I /usr/include/freetype2/ -lGL -lGLEW -lglut -lfreetype -lode -lSDL_net -o simquadcopter-vls visualization_library/vl/*.cpp visualization_library/vlut/*.cpp visualization_library/vlGLUT/*.cpp visualization.cpp quadcopter.cpp balance.cpp udpremote.cpp




clean:
	@echo Cleaning up...
	@rm simquadcopter
	@rm simquadcopter-vl
	@rm simquadcopter-vls
	@echo Done.
