# VeryWeirdPasswordManager
Проект по созданию простого менеджера паролей для предмета "Технологии и методы программирования"

# How to set up: 
(on linux or mac idk)

install docker and docker-compose on your platform:
```
curl -fsSL https://get.docker.com -o get-docker.sh && \
sudo sh ./get-docker.sh --dry-run
```
then install g++ and openssl-dev to run the test client: 
```
apt-get install g++ openssl-dev
```
get repo cloned and cd into it: 
```
git clone https://github.com/nousernameforu/VeryWeirdPasswordManager && \
cd VeryWeirdPasswordManager
```
then run the server with docker-compose: 
```
cd TCP-server && \
docker-compose up  
```

after server started run a client with startup.sh shell script:
```
cd ../TCP-client && \
chmod +x startup.sh && \
./startup.sh
```

Now you can test this crappy code I wrote.
