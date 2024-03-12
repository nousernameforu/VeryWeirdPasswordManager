#/bin/sh

#cp ../TCP-server/FileHandeling.h FileHandeling.h && \
#cp ../TCP-server/FileHandeling.cpp FileHandeling.cpp && \

# Compile the FileHandeling lib
g++ -c FileHandeling.cpp -o FileHandeling.o && ar rcs libFileHandeling.a FileHandeling.o && \
# Compile the FileEncryption lib
g++ -c FileEncryption.cpp -o FileEncryption.o && ar rcs libFileEncryption.a FileEncryption.o && \
# Compile the main app (Used just for running the server now)
g++ test-client.cpp -o test-client -L. -lFileHandeling  -lFileEncryption -lssl -lcrypto && \
./test-client
