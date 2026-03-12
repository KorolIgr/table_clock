#!/usr/bin/env python3
"""
Скрипт для преобразования HTML-файлов в строковые константы C++ для встраивания в прошивку ESP8266
"""

import os
import re
from pathlib import Path

def html_to_cpp_constant(html_content, constant_name):
    """Преобразует HTML-контент в строковую константу C++ с использованием сырых строк (raw strings)"""
    # Экранируем двойные кавычки и обратные слэши
    escaped_content = html_content.replace('\\', '\\\\').replace('"', '\\"')
    
    # Создаем сырую строку C++
    cpp_code = f'static const char {constant_name}[] PROGMEM = R"rawhtml(\n{escaped_content})rawhtml";\n'
    
    return cpp_code

def process_html_files(input_dir, output_file):
    """Обрабатывает все HTML-файлы в директории и создает один выходной файл C++"""
    input_path = Path(input_dir)
    output_path = Path(output_file)
    
    if not input_path.exists():
        print(f"Директория {input_path} не найдена")
        return
    
    html_files = list(input_path.glob("*.html"))
    
    if not html_files:
        print(f"HTML-файлы не найдены в {input_path}")
        return
    
    with open(output_path, 'w', encoding='utf-8') as output:
        output.write("// Автоматически сгенерированный файл с HTML-шаблонами\n")
        output.write("#pragma once\n")
        output.write('#include <Arduino.h>\n\n')
        
        for html_file in html_files:
            print(f"Обработка {html_file.name}...")
            
            with open(html_file, 'r', encoding='utf-8') as f:
                content = f.read()
            
            # Генерируем имя константы из имени файла
            # Удаляем расширение и недопустимые символы
            constant_name = html_file.stem.upper()
            if not re.match(r'^[A-Z][A-Z0-9_]*$', constant_name):
                # Если имя не соответствует правилам для C++, делаем его допустимым
                constant_name = re.sub(r'[^A-Z0-9_]', '_', constant_name)
                if not re.match(r'^[A-Z]', constant_name):
                    constant_name = 'PAGE_' + constant_name
            
            constant_name = f"{constant_name}_HTML"
            
            cpp_constant = html_to_cpp_constant(content, constant_name)
            output.write(cpp_constant)
            output.write('\n')

if __name__ == "__main__":
    import sys
    
    input_directory = "data/html"
    output_file = "include/web_pages.h"
    
    if len(sys.argv) > 1:
        input_directory = sys.argv[1]
    
    if len(sys.argv) > 2:
        output_file = sys.argv[2]
    
    process_html_files(input_directory, output_file)
    print(f"Готово! HTML-файлы из {input_directory} преобразованы в {output_file}")