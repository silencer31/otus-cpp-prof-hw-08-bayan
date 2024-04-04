#include "bayan_collector.h"


void BayanCollector::scan_for_duplicates()
{
    while ( !traversal_ptr->traverse_finished())
    {
        // Берём следующий файл 
        auto file_to_analyze = traversal_ptr->get_next_file();
        
        bool is_duplicate = false; // Найден ли дубликат.
        bool blocks_match = true;  // Для сравнения хеш блоков.
 
        // Перебираем файлы, у которых могут быть дубликаты.
        for (auto& file_from_collection : files_with_duplicates) {
            
            // Сравниваем только файлы, имеющие одинаковый размер.
            if (file_from_collection.get_file_size() != file_to_analyze.get_file_size()) {
                continue;
            }

            // Итераторы для поблочного сравнения файлов.
            auto to_analyze_iter = file_to_analyze.begin();
            auto to_compare_iter = file_from_collection.begin();

            blocks_match = true; // Предполагаем, что хеш блоки совпадут.

            // Цикл поблочного сравнения хеш блоков.
            for (; (to_analyze_iter != file_to_analyze.end()) && (to_compare_iter != file_from_collection.end()); ++to_analyze_iter, ++to_compare_iter)
            {
                if (*to_analyze_iter != *to_compare_iter) {
                    blocks_match = false; // Блоки хешей не совпали.
                    break;
                }
            }

            // Если блоки не совпали, переходим к следующему файлу.
            if (!blocks_match) {
                continue;
            }

            // Разное кол-во хеш блоков.
            if ((to_analyze_iter != file_to_analyze.end()) || (to_compare_iter != file_from_collection.end())) {
                continue;
            }
            
            // Все блоки совпали. Запоминаем, что есть дубликат.
            is_duplicate = true;
            // Добавляем как дубликат в коллекцию к файлу, у которого могут быть дубликаты.
            file_from_collection.add_duplicate(file_to_analyze.get_path());
        }

        // Добавляем в коллекцию файлов, у которых могут быть дубликаты.
        if (!is_duplicate) {
            files_with_duplicates.push_back(file_to_analyze);
        }
    }
}