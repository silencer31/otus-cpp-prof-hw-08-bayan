#pragma once

#include <string>
#include <vector>

struct Parameters
{
	std::vector<std::string> scan_dirs;		// Директории для сканирования.
	std::vector<std::string> excluded_dirs; // Директории для исключения из сканирования.
	std::vector<std::string> name_masks;	// Маски имен файлов разрешенных для сравнения.
	
	// Глубина сканирования. true - все директории, false - только указанная директория без вложенных.
	bool scan_all_dirs;
	
	unsigned int min_file_size{ 1 };  // Минимальный размер файла.
	unsigned int block_size{ 1 };	  // Размер блока, которым производится чтения файлов.
	
	std::string hash_algorithm;	// Алгоритм хэширования: crc32, md5.

	bool show_help;
	std::string help_text;
};