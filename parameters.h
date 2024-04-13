#pragma once

#include "type_aliases.h"

enum class HashAlgoritm {
	CRC32 = 0,
	MD5 = 1
};

struct Parameters
{
	str_vector scan_dirs;	 // Директории для сканирования.
	str_vector exclude_dirs; // Директории для исключения из сканирования.
	str_vector file_masks;	 // Маски имен файлов разрешенных для сравнения.
	
	// Глубина сканирования. true - все директории, false - только указанная директория без вложенных.
	bool scan_all_dirs{ false };
	
	unsigned int min_file_size{ 1 };  // Минимальный размер файла.
	unsigned int block_size{ 1 };	  // Размер блока, которым производится чтения файлов.
	
	HashAlgoritm hash_algorithm{HashAlgoritm::CRC32};	// Алгоритм хэширования: crc32, md5.

	bool show_help{ false };
	std::string help_text;
};