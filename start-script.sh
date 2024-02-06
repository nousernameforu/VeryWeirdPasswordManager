#/bin/bash

container_name="cpp-tcp-server"
echo "Container name is: "
echo $container_name


#build docker image 
docker build -t cpp-tcp-server .

#check if docker with same name is running or exists
if docker ps -a --format '{{.Names}}' | grep $container_name; then
	echo "Will delete existing container now"
	docker stop $container_name && docker rm $container_name
fi

#run docker image 
docker run --name cpp-tcp-server -p 8080:8080 cpp-tcp-server

