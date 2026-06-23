# Usamos una versión específica de GCC
FROM gcc:12.2.0

# Actualizamos repositorios e instalamos valgrind y make
RUN apt-get update && apt-get install -y \
    valgrind \
    make \
    gdb \
    && rm -rf /var/lib/apt/lists/*

# Carpeta de trabajo dentro del contenedor
WORKDIR /usr/src/Fundamentostps

# Comando por defecto: abrir bash
CMD ["bash"]
