#!/bin/bash
# ================================
# Script de compilación para Linux
# ================================

set -e

echo "======================================"
echo "Compilación para Linux"
echo "======================================"

# Colores para output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Verificar dependencias
echo -e "${YELLOW}Verificando dependencias...${NC}"

# Lista de paquetes necesarios según la distribución
if command -v apt-get &>/dev/null; then
	echo "Distribución basada en Debian/Ubuntu detectada"
	PACKAGES="build-essential cmake libcurl4-openssl-dev libssl-dev nlohmann-json3-dev"
	if [ "$1" != "--no-qt" ]; then
		PACKAGES="$PACKAGES qt6-base-dev qt6-tools-dev"
	fi
	echo "Instalar con: sudo apt-get install $PACKAGES"
elif command -v dnf &>/dev/null; then
	echo "Distribución basada en Fedora/RHEL detectada"
	PACKAGES="gcc gcc-c++ cmake libcurl-devel openssl-devel nlohmann-json-devel"
	if [ "$1" != "--no-qt" ]; then
		PACKAGES="$PACKAGES qt6-qtbase-devel qt6-qttools-devel"
	fi
	echo "Instalar con: sudo dnf install $PACKAGES"
elif command -v pacman &>/dev/null; then
	echo "Distribución basada en Arch detectada"
	PACKAGES="base-devel cmake curl openssl nlohmann-json"
	if [ "$1" != "--no-qt" ]; then
		PACKAGES="$PACKAGES qt6-base qt6-tools"
	fi
	echo "Instalar con: sudo pacman -S $PACKAGES"
fi

# Crear directorio de build
echo -e "${YELLOW}Creando directorio de build...${NC}"
mkdir -p build
cd build

# Configurar CMake
echo -e "${YELLOW}Configurando proyecto con CMake...${NC}"
if [ "$1" == "--no-qt" ]; then
	echo "Compilando solo versión Console (sin Qt)"
	cmake -DCMAKE_BUILD_TYPE=Release -DBUILD_QT_VERSION=OFF ..
else
	echo "Compilando versiones Console + Qt GUI"
	cmake -DCMAKE_BUILD_TYPE=Release -DBUILD_QT_VERSION=ON ..
fi

# Compilar
echo -e "${YELLOW}Compilando...${NC}"
cmake --build . -j$(nproc)

# Verificar resultados
echo ""
echo "======================================"
if [ -f "bin/EsetKeysConsole" ]; then
	echo -e "${GREEN}✓ EsetKeysConsole compilado exitosamente${NC}"
	ls -lh bin/EsetKeysConsole
else
	echo -e "${RED}✗ Error: No se pudo compilar EsetKeysConsole${NC}"
fi

if [ "$1" != "--no-qt" ] && [ -f "bin/EsetKeysQt" ]; then
	echo -e "${GREEN}✓ EsetKeysQt compilado exitosamente${NC}"
	ls -lh bin/EsetKeysQt
elif [ "$1" != "--no-qt" ]; then
	echo -e "${RED}✗ Error: No se pudo compilar EsetKeysQt${NC}"
fi
echo "======================================"

echo ""
echo "Binarios disponibles en: $(pwd)/bin/"
