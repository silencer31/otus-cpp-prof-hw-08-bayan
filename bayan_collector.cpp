#include "bayan_collector.h"


void BayanCollector::scan_for_duplicates()
{
    //std::cout << "Scan started" << std::endl;

    //unsigned int cn = 0;

    // В этот цикл заходим, только если есть файлы для анализа.
    while ( !traversal_ptr->traverse_finished())
    {
        //std::cout << "\nIn traversal round : " << ++cn << std::endl;

        // Берём следующий файл 
        auto next_file_to_compare = traversal_ptr->get_next_file();

        //std::cout << " - Next to compare : " << next_file_to_compare.get_file_path() << std::endl;

        bool is_duplicate = false; // Является дубликатом одного из тех, что уже в основной коллекции.
        bool blocks_match = true;  // Для сравнения хеш блоков.
 
        // Сравниваем с файлами, которые уже есть в основной коллекции.
        for (auto& file_from_collection : main_files_collection) {

            //std::cout << " -- from collection : " << file_from_collection.get_file_path() << std::endl;

            // Сравниваем только файлы, имеющие одинаковый размер.
            if (file_from_collection.get_file_size() != next_file_to_compare.get_file_size()) {
                continue;
            }

            // Итераторы для поблочного сравнения файлов.
            auto to_compare_iter = next_file_to_compare.begin();
            auto ff_collect_iter = file_from_collection.begin();

            blocks_match = true; // Предполагаем, что хеш блоки совпадут.

            // Цикл поблочного сравнения хеш блоков.
            for (; (to_compare_iter != next_file_to_compare.end()) && (ff_collect_iter != file_from_collection.end()); ++to_compare_iter, ++ff_collect_iter)
            {
                if (*to_compare_iter != *ff_collect_iter) {
                    blocks_match = false; // Блоки хешей не совпали.
                    break;
                }
            }

            // Если блоки не совпали, пытаемся взять следующий файл из коллекции.
            if (!blocks_match) {
                continue;
            }

            // Разное кол-во хеш блоков.
            if ((to_compare_iter != next_file_to_compare.end()) || (ff_collect_iter != file_from_collection.end())) {
                continue;
            }
            
            // Все блоки совпали. Запоминаем, что есть дубликат.
            is_duplicate = true;
            // Добавляем как дубликат в коллекцию к файлу, у которого могут быть дубликаты.
            file_from_collection.add_duplicate(next_file_to_compare.get_file_path());

            /*std::cout << "File : " << next_file_to_compare.get_file_path()
                << " added as duplicate of file : " << file_from_collection.get_file_path() << std::endl;
            */
            break;
        }

        // Добавляем в основную коллекцию файлов, у которых могут быть дубликаты.
        if (!is_duplicate) {
            main_files_collection.push_back(next_file_to_compare);

            //std::cout << "Added to main collection : " << next_file_to_compare.get_file_path() << std::endl;
        }
    }

    //std::cout << "\nScan finished" << std::endl;

    // Убираем из главной коллекции файлы, прошедшие отбор по критериям, но не имеющие точных дубликатов.
    main_files_collection.remove_if([](auto item) { return item.no_duplicates(); });
}