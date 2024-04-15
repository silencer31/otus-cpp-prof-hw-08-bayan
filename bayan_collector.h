#pragma once

#include "traversal.h"

#include <memory>

// Для основной коллекции файлов, которые могут иметь дубликаты.
using fwd_list = std::list<FileWithDuplicates>;

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
	* @brief Выполнить поиск дубликатов файлов.
	*/
	void scan_for_duplicates();

	/**
	* @return пуста ли коллекция c файлами, имеющими дубликаты.
	*/
	bool nothing_found() {
		return main_files_collection.empty();
	}

	/**
	* @return константный итератор на начало коллекции.
	*/
	fwd_list::const_iterator cbegin() {
		return main_files_collection.cbegin();
	}

	/**
	* @return константный итератор на "конец" коллекции.
	*/
	fwd_list::const_iterator cend() {
		return main_files_collection.cend();
	}

private:
	// Для обхода файлов в директориях.
	std::unique_ptr<Traversal> traversal_ptr;

	// Коллекция файлов, каждый из которых имеет свою коллекцию дубликатов.
	fwd_list main_files_collection;
};