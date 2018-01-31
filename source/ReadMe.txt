#win32 vs2010 compile
1. compile boost
         
   decompess boost_1_58_0.tar.gz
   
   bootstrap.bat
   
   b2.exe

2. cmake project
   
   decompress Kunhou_Arm.zip
   
   edit file "Kunhou_Arm/source/CMakeX64_Boost.txt" 
   replace the path>>link_directories("e:/Kunhou_Arm/lib")
   replace the path>>set(BOOST_ROOT "e:/boost_1_58_0")
   replace the path>>set(BOOST_LIBRARYDIR "e:/boost_1_58_0/stage")
   
   cd Kunhou_Arm
   
   mkdir build-d
   
   cd build-d
   
   cmake ../source