FROM gcc:13

WORKDIR /app

COPY . /app/source

RUN apt update \
    && apt install -y cmake nlohmann-json3-dev libssl-dev libcurl4-openssl-dev \
       qt6-base-dev qt6-tools-dev-tools \
    && apt clean \
    && mkdir -p source/build \
    && cd source/build \
    && cmake .. \
    && cmake --build . \
    && mv EsetKeysQt /app \
    && rm -r /app/source
    
