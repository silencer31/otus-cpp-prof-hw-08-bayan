#pragma once

#include <string>
#include <vector>

struct Parameters
{
	std::vector<std::string> scan_dirs;		// ���������� ��� ������������.
	std::vector<std::string> excluded_dirs; // ���������� ��� ���������� �� ������������.
	std::vector<std::string> name_masks;	// ����� ���� ������ ����������� ��� ���������.
	int scan_depth{ 0 };		// ������� ������������.
	int min_file_size{ 1 };	// ����������� ������ �����.

};