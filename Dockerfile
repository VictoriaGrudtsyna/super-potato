# Базовый образ с поддержкой C++ и Boost
FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

# Устанавливаем основные инструменты для сборки
RUN apt-get update && apt-get install -y --fix-missing\
    cmake \
    wget \
    git \
    libboost-all-dev \
    libpq-dev \         
    libssl-dev \         
    postgresql \         
    postgresql-contrib \ 
    build-essential \
    && rm -rf /var/lib/apt/lists/*

# Устанавливаем библиотеку Boost.Beast
# Boost.Beast входит в состав Boost, поэтому устанавливать его отдельно не нужно

# Скачиваем json.hpp и создаем правильную структуру папок
RUN mkdir -p /usr/local/include/nlohmann && \
    wget -q https://github.com/nlohmann/json/releases/download/v3.11.2/json.hpp -O /usr/local/include/nlohmann/json.hpp

# Создаем рабочую директорию
WORKDIR /app

# Копируем файлы проекта в контейнер
COPY . .

# Собираем проект
RUN mkdir build && cd build && cmake .. && make

# Указываем порт, на котором будет работать сервер
EXPOSE 8080

# Команда для запуска приложения
CMD ["./build/boost_api_server"]