# Makefile для сборки веб-интерфейса Table Clock

.PHONY: build-web clean

# Целевой файл для сборки веб-ресурсов
build-web:
	python build_html.py

# Очистка сгенерированных файлов
clean:
	del include\web_pages.h

# Сборка веб-интерфейса как часть общей сборки
all: build-web

# Запуск сборки по умолчанию
.DEFAULT_GOAL := all