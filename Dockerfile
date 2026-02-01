# ================================
# Build Stage
# ================================
FROM alpine:3.19 AS build

WORKDIR /build

# Instalar dependencias de compilación
RUN apk add --no-cache \
	build-base \
	cmake \
	curl-dev \
	openssl-dev \
	nlohmann-json \
	git

# Copiar archivos del proyecto
COPY . .

# Configurar y compilar (solo versión consola)
RUN cmake -S . -B build \
	-DCMAKE_BUILD_TYPE=Release \
	-DBUILD_QT_VERSION=OFF \
	&& cmake --build build --target EsetKeysConsole -j$(nproc)

# Verificar que el binario se creó correctamente
RUN ls -lh /build/build/bin/EsetKeysConsole || ls -lh /build/build/EsetKeysConsole

# ================================
# Runtime Stage
# ================================
FROM alpine:3.19

WORKDIR /app

# Instalar solo dependencias de runtime necesarias
RUN apk add --no-cache \
	libcurl \
	libssl3 \
	libcrypto3 \
	ca-certificates \
	libstdc++

# Copiar el binario compilado
COPY --from=build /build/build/bin/EsetKeysConsole /app/EsetKeysConsole 

# Dar permisos de ejecución
RUN chmod +x /app/EsetKeysConsole

# Usuario no privilegiado para seguridad
RUN adduser -D -u 1000 appuser && \
	chown -R appuser:appuser /app

USER appuser

ENTRYPOINT ["/app/EsetKeysConsole"]
