#include "bayan_collector.h"


void BayanCollector::scan_for_duplicates()
{
    //std::cout << "Scan started" << std::endl;

    //unsigned int cn = 0;

    // � ���� ���� �������, ������ ���� ���� ����� ��� �������.
    while ( !traversal_ptr->traverse_finished())
    {
        //std::cout << "\nIn traversal round : " << ++cn << std::endl;

        // ���� ��������� ���� 
        auto next_file_to_compare = traversal_ptr->get_next_file();

        //std::cout << " - Next to compare : " << next_file_to_compare.get_file_path() << std::endl;

        bool is_duplicate = false; // �������� ���������� ������ �� ���, ��� ��� � �������� ���������.
        bool blocks_match = true;  // ��� ��������� ��� ������.
 
        // ���������� � �������, ������� ��� ���� � �������� ���������.
        for (auto& file_from_collection : main_files_collection) {

            //std::cout << " -- from collection : " << file_from_collection.get_file_path() << std::endl;

            // ���������� ������ �����, ������� ���������� ������.
            if (file_from_collection.get_file_size() != next_file_to_compare.get_file_size()) {
                continue;
            }

            // ��������� ��� ���������� ��������� ������.
            auto to_compare_iter = next_file_to_compare.begin();
            auto ff_collect_iter = file_from_collection.begin();

            blocks_match = true; // ������������, ��� ��� ����� ��������.

            // ���� ���������� ��������� ��� ������.
            for (; (to_compare_iter != next_file_to_compare.end()) && (ff_collect_iter != file_from_collection.end()); ++to_compare_iter, ++ff_collect_iter)
            {
                if (*to_compare_iter != *ff_collect_iter) {
                    blocks_match = false; // ����� ����� �� �������.
                    break;
                }
            }

            // ���� ����� �� �������, �������� ����� ��������� ���� �� ���������.
            if (!blocks_match) {
                continue;
            }

            // ������ ���-�� ��� ������.
            if ((to_compare_iter != next_file_to_compare.end()) || (ff_collect_iter != file_from_collection.end())) {
                continue;
            }
            
            // ��� ����� �������. ����������, ��� ���� ��������.
            is_duplicate = true;
            // ��������� ��� �������� � ��������� � �����, � �������� ����� ���� ���������.
            file_from_collection.add_duplicate(next_file_to_compare.get_file_path());

            /*std::cout << "File : " << next_file_to_compare.get_file_path()
                << " added as duplicate of file : " << file_from_collection.get_file_path() << std::endl;
            */
            break;
        }

        // ��������� � �������� ��������� ������, � ������� ����� ���� ���������.
        if (!is_duplicate) {
            main_files_collection.push_back(next_file_to_compare);

            //std::cout << "Added to main collection : " << next_file_to_compare.get_file_path() << std::endl;
        }
    }

    //std::cout << "\nScan finished" << std::endl;

    // ������� �� ������� ��������� �����, ��������� ����� �� ���������, �� �� ������� ������ ����������.
    main_files_collection.remove_if([](auto item) { return item.no_duplicates(); });
}