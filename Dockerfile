FROM gcc:latest

WORKDIR /app

# Wir kopieren alles
COPY . .

RUN ls -R
# Wir nutzen den Wildcard *.cpp Trick
# Das -o my_redis_server benennt das fertige Programm
RUN find . -name "*.cpp" | xargs g++ -o my_redis_server

# Starten
CMD ["./my_redis_server"]