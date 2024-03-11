#pragma once

#include <string>
#include <vector>


struct Parameters
{
	std::vector<std::string> scan_dirs;		// Директории для сканирования.
	std::vector<std::string> exclude_dirs;	// Директории для исключения из сканирования.
	std::vector<std::string> file_masks;	// Маски имен файлов разрешенных для сравнения.
	
	// Глубина сканирования. true - все директории, false - только указанная директория без вложенных.
	bool scan_all_dirs{ false };
	
	unsigned int min_file_size{ 1 };  // Минимальный размер файла.
	unsigned int block_size{ 1 };	  // Размер блока, которым производится чтения файлов.
	
	std::string hash_algorithm;	// Алгоритм хэширования: crc32, md5.

	bool show_help{ false };
	std::string help_text;
};