#pragma once

#include "type_aliases.h"

enum class HashAlgoritm {
	CRC32 = 0,
	MD5 = 1
};

struct Parameters
{
	str_vector scan_dirs;	 // ���������� ��� ������������.
	str_vector exclude_dirs; // ���������� ��� ���������� �� ������������.
	str_vector file_masks;	 // ����� ���� ������ ����������� ��� ���������.
	
	// ������� ������������. true - ��� ����������, false - ������ ��������� ���������� ��� ���������.
	bool scan_all_dirs{ false };
	
	unsigned int min_file_size{ 1 };  // ����������� ������ �����.
	unsigned int block_size{ 1 };	  // ������ �����, ������� ������������ ������ ������.
	
	HashAlgoritm hash_algorithm{HashAlgoritm::CRC32};	// �������� �����������: crc32, md5.

	bool show_help{ false };
	std::string help_text;
};