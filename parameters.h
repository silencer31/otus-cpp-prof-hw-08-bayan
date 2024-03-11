#pragma once

#include <string>
#include <vector>


struct Parameters
{
	std::vector<std::string> scan_dirs;		// ���������� ��� ������������.
	std::vector<std::string> exclude_dirs;	// ���������� ��� ���������� �� ������������.
	std::vector<std::string> file_masks;	// ����� ���� ������ ����������� ��� ���������.
	
	// ������� ������������. true - ��� ����������, false - ������ ��������� ���������� ��� ���������.
	bool scan_all_dirs{ false };
	
	unsigned int min_file_size{ 1 };  // ����������� ������ �����.
	unsigned int block_size{ 1 };	  // ������ �����, ������� ������������ ������ ������.
	
	std::string hash_algorithm;	// �������� �����������: crc32, md5.

	bool show_help{ false };
	std::string help_text;
};