FROM gcc:13

WORKDIR /app

COPY . /app/source

RUN apt update \
    && apt install -y cmake nlohmann-json3-dev \
    && apt clean \
    && mkdir -p source/build \
    && cd source/build \
    && cmake .. \
    && cmake --build . \
    && mv EsetKeys /app \
    && rm -r /app/source
