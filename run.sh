cmake --no-warn-unused-cli -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE -DCMAKE_BUILD_TYPE:STRING=Debug -DCMAKE_C_COMPILER:FILEPATH=gcc -DCMAKE_CXX_COMPILER:FILEPATH=g++ -S. -B./bin -G "Unix Makefiles" ;
cmake --build ./bin --config Debug --target clean -j 18 -- ;
cmake --build ./bin --config Debug --target all -j 18 -- ;
bin/src/mirror_image.exe -I data -O data/mirrored_images;