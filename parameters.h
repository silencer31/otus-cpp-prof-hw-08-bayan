#pragma once

#include <string>
#include <vector>

struct Parameters
{
	std::vector<std::string> scan_dirs;		// Директории для сканирования.
	std::vector<std::string> excluded_dirs; // Директории для исключения из сканирования.
	std::vector<std::string> name_masks;	// Маски имен файлов разрешенных для сравнения.
	int scan_depth{ 0 };		// Глубина сканирования.
	int min_file_size{ 1 };	// Минимальный размер файла.

};