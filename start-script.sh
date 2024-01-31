#/bin/bash

#build docker image 
docker build -t cpp-tcp-server .

#run docker image 
docker run -p 8080:8080 cpp-tcp-server

