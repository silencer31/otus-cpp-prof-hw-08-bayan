#include "bayan_collector.h"


void BayanCollector::scan_for_duplicates()
{
    while (!traversal_ptr->dir_traversed())
    {
        auto file = traversal_ptr->get_next_file();
        auto is_duplicate = false;

        for (auto& compared_file : file_duplicates) {
            
            if (compared_file.get_file_size() != file.get_file_size()) {
                continue;
            }

            bool blocks_match = true;

            auto blocks_iterator = file.begin();
            auto compared_iter = compared_file.begin();

            for (; (blocks_iterator != file.end()) && (compared_iter != compared_file.end()); ++blocks_iterator, ++compared_iter)
            {
                if (*blocks_iterator != *compared_iter) {
                    blocks_match = false;
                    break;
                }
            }

            if ((blocks_iterator == file.end()) && (compared_iter == compared_file.end()) && blocks_match) {
                is_duplicate = true;
                compared_file.add_duplicate(file.get_path());
            }
        }

        if (!is_duplicate) {
            file_duplicates.push_back(file);
        }
    }
}