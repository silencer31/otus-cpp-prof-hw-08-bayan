#pragma once

#include "traversal.h"

#include <memory>

/**
* @brief Класс реализует поиск дубликатов файлов.
*/
class BayanCollector
{
public:
	BayanCollector(std::unique_ptr<Traversal> dt_ptr)
		: traversal_ptr(std::move(dt_ptr))
	{}

	/**
	* Выполнить поиск дубликатов файлов.
	*/
	void scan_for_duplicates();

private:
	// Для обхода файлов в директории.
	std::unique_ptr<Traversal> traversal_ptr;

	// Коллекция уникальных файлов, каждый из которых имеет свою коллекцию дубликатов.
	std::vector<FileWithDuplicates> files_with_duplicates;
};