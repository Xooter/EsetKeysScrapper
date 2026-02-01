.PHONY: all linux windows docker clean help

# Variables
BUILD_DIR = build
BUILD_DIR_WIN = build-windows
DOCKER_IMAGE = esetkeys
NPROC = $(shell nproc 2>/dev/null || echo 4)

help:
	@echo "======================================"
	@echo "EsetKeys - Sistema de Compilación"
	@echo "======================================"
	@echo ""
	@echo "Objetivos disponibles:"
	@echo "  make linux          - Compilar para Linux (Consola + Qt)"
	@echo "  make linux-console  - Compilar para Linux (solo Consola)"
	@echo "  make windows        - Cross-compilar para Windows (Consola + Qt)"
	@echo "  make windows-console- Cross-compilar para Windows (solo Consola)"
	@echo "  make docker         - Compilar imagen Docker"
	@echo "  make docker-extract - Compilar y extraer binario de Docker"
	@echo "  make all            - Compilar todo (Linux + Windows)"
	@echo "  make clean          - Limpiar archivos de compilación"
	@echo "  make clean-all      - Limpiar todo incluyendo binarios"
	@echo ""

# Compilar todo
all: linux windows

# Linux con Qt
linux:
	@echo "Compilando para Linux (Consola + Qt)..."
	@mkdir -p $(BUILD_DIR)
	@cd $(BUILD_DIR) && \
		cmake -DCMAKE_BUILD_TYPE=Release -DBUILD_QT_VERSION=ON .. && \
		cmake --build . -j$(NPROC)
	@echo "✓ Compilación Linux completada: $(BUILD_DIR)/bin/"

# Linux solo consola
linux-console:
	@echo "Compilando para Linux (solo Consola)..."
	@mkdir -p $(BUILD_DIR)
	@cd $(BUILD_DIR) && \
		cmake -DCMAKE_BUILD_TYPE=Release -DBUILD_QT_VERSION=OFF .. && \
		cmake --build . -j$(NPROC)
	@echo "✓ Compilación Linux completada: $(BUILD_DIR)/bin/"

# Windows con Qt (requiere vcpkg o MinGW configurado)
windows:
	@echo "Cross-compilando para Windows (Consola + Qt)..."
	@mkdir -p $(BUILD_DIR_WIN)
	@cd $(BUILD_DIR_WIN) && \
		if [ -n "$$VCPKG_ROOT" ]; then \
			cmake -DCMAKE_TOOLCHAIN_FILE=$$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake \
			      -DVCPKG_TARGET_TRIPLET=x64-mingw-static \
			      -DCMAKE_BUILD_TYPE=Release \
			      -DBUILD_QT_VERSION=ON \
			      -DCMAKE_SYSTEM_NAME=Windows \
			      -DCMAKE_C_COMPILER=x86_64-w64-mingw32-gcc \
			      -DCMAKE_CXX_COMPILER=x86_64-w64-mingw32-g++ \
			      ..; \
		else \
			cmake -DCMAKE_TOOLCHAIN_FILE=../windows-toolchain.cmake \
			      -DCMAKE_BUILD_TYPE=Release \
			      -DBUILD_QT_VERSION=ON \
			      ..; \
		fi && \
		cmake --build . -j$(NPROC)
	@echo "✓ Compilación Windows completada: $(BUILD_DIR_WIN)/bin/"

# Windows solo consola
windows-console:
	@echo "Cross-compilando para Windows (solo Consola)..."
	@mkdir -p $(BUILD_DIR_WIN)
	@cd $(BUILD_DIR_WIN) && \
		if [ -n "$$VCPKG_ROOT" ]; then \
			cmake -DCMAKE_TOOLCHAIN_FILE=$$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake \
			      -DVCPKG_TARGET_TRIPLET=x64-mingw-static \
			      -DCMAKE_BUILD_TYPE=Release \
			      -DBUILD_QT_VERSION=OFF \
			      -DCMAKE_SYSTEM_NAME=Windows \
			      -DCMAKE_C_COMPILER=x86_64-w64-mingw32-gcc \
			      -DCMAKE_CXX_COMPILER=x86_64-w64-mingw32-g++ \
			      ..; \
		else \
			cmake -DCMAKE_TOOLCHAIN_FILE=../windows-toolchain.cmake \
			      -DCMAKE_BUILD_TYPE=Release \
			      -DBUILD_QT_VERSION=OFF \
			      ..; \
		fi && \
		cmake --build . -j$(NPROC)
	@echo "✓ Compilación Windows completada: $(BUILD_DIR_WIN)/bin/"

# Docker
docker:
	@echo "Compilando imagen Docker..."
	@docker build -t $(DOCKER_IMAGE):latest .
	@echo "✓ Imagen Docker creada: $(DOCKER_IMAGE):latest"

# Docker y extraer binario
docker-extract: docker
	@echo "Extrayendo binario de Docker..."
	@CONTAINER_ID=$$(docker create $(DOCKER_IMAGE):latest) && \
		docker cp $$CONTAINER_ID:/app/EsetKeysConsole ./EsetKeysConsole && \
		docker rm $$CONTAINER_ID && \
		chmod +x ./EsetKeysConsole
	@echo "✓ Binario extraído: ./EsetKeysConsole"
	@ls -lh ./EsetKeysConsole

# Limpiar archivos de compilación
clean:
	@echo "Limpiando archivos de compilación..."
	@rm -rf $(BUILD_DIR) $(BUILD_DIR_WIN)
	@echo "✓ Limpieza completada"

# Limpiar todo
clean-all: clean
	@echo "Limpiando binarios extraídos..."
	@rm -f EsetKeysConsole EsetKeysConsole.exe
	@docker rmi -f $(DOCKER_IMAGE):latest 2>/dev/null || true
	@echo "✓ Limpieza completa"

# Test rápido (compilar y ejecutar versión consola Linux)
test-console: linux-console
	@echo "Ejecutando test..."
	@$(BUILD_DIR)/bin/EsetKeysConsole --help || echo "Binario necesita argumentos"
