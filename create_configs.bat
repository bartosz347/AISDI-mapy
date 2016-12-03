mkdir Debug
mkdir Release
( cd Debug && cmake -DCMAKE_BUILD_TYPE=Debug -G"CodeBlocks - MinGW Makefiles" ../ )
cd ..
( cd Release && cmake -DCMAKE_BUILD_TYPE=Release -G"CodeBlocks - MinGW Makefiles" ../ )
cd ..
