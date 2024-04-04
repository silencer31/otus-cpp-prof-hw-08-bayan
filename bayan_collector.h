#pragma once

#include "traversal.h"

#include <memory>

/**
* @brief ����� ��������� ����� ���������� ������.
*/
class BayanCollector
{
public:
	BayanCollector(std::unique_ptr<Traversal> dt_ptr)
		: traversal_ptr(std::move(dt_ptr))
	{}

	/**
	* ��������� ����� ���������� ������.
	*/
	void scan_for_duplicates();

private:
	// ��� ������ ������ � ����������.
	std::unique_ptr<Traversal> traversal_ptr;

	// ��������� ���������� ������, ������ �� ������� ����� ���� ��������� ����������.
	std::vector<FileWithDuplicates> files_with_duplicates;
};