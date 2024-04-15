#pragma once

#include "traversal.h"

#include <memory>

// ��� �������� ��������� ������, ������� ����� ����� ���������.
using fwd_list = std::list<FileWithDuplicates>;

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
	* @brief ��������� ����� ���������� ������.
	*/
	void scan_for_duplicates();

	/**
	* @return ����� �� ��������� c �������, �������� ���������.
	*/
	bool nothing_found() {
		return main_files_collection.empty();
	}

	/**
	* @return ����������� �������� �� ������ ���������.
	*/
	fwd_list::const_iterator cbegin() {
		return main_files_collection.cbegin();
	}

	/**
	* @return ����������� �������� �� "�����" ���������.
	*/
	fwd_list::const_iterator cend() {
		return main_files_collection.cend();
	}

private:
	// ��� ������ ������ � �����������.
	std::unique_ptr<Traversal> traversal_ptr;

	// ��������� ������, ������ �� ������� ����� ���� ��������� ����������.
	fwd_list main_files_collection;
};