#include "bayan_collector.h"


void BayanCollector::scan_for_duplicates()
{
    while ( !traversal_ptr->traverse_finished())
    {
        // ���� ��������� ���� 
        auto file_to_analyze = traversal_ptr->get_next_file();
        
        bool is_duplicate = false; // ������ �� ��������.
        bool blocks_match = true;  // ��� ��������� ��� ������.
 
        // ���������� �����, � ������� ����� ���� ���������.
        for (auto& file_from_collection : files_with_duplicates) {
            
            // ���������� ������ �����, ������� ���������� ������.
            if (file_from_collection.get_file_size() != file_to_analyze.get_file_size()) {
                continue;
            }

            // ��������� ��� ���������� ��������� ������.
            auto to_analyze_iter = file_to_analyze.begin();
            auto to_compare_iter = file_from_collection.begin();

            blocks_match = true; // ������������, ��� ��� ����� ��������.

            // ���� ���������� ��������� ��� ������.
            for (; (to_analyze_iter != file_to_analyze.end()) && (to_compare_iter != file_from_collection.end()); ++to_analyze_iter, ++to_compare_iter)
            {
                if (*to_analyze_iter != *to_compare_iter) {
                    blocks_match = false; // ����� ����� �� �������.
                    break;
                }
            }

            // ���� ����� �� �������, ��������� � ���������� �����.
            if (!blocks_match) {
                continue;
            }

            // ������ ���-�� ��� ������.
            if ((to_analyze_iter != file_to_analyze.end()) || (to_compare_iter != file_from_collection.end())) {
                continue;
            }
            
            // ��� ����� �������. ����������, ��� ���� ��������.
            is_duplicate = true;
            // ��������� ��� �������� � ��������� � �����, � �������� ����� ���� ���������.
            file_from_collection.add_duplicate(file_to_analyze.get_path());
        }

        // ��������� � ��������� ������, � ������� ����� ���� ���������.
        if (!is_duplicate) {
            files_with_duplicates.push_back(file_to_analyze);
        }
    }
}