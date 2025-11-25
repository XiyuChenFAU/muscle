/*

Copyright (C) 2023-2031 Friedrich-Alexander-Universität Erlangen-Nürnberg
27.10.2023
Xiyu Chen

*/

#include <ctime>
#include "qtwindow/mainwindow.h"
#include <QApplication>


int main(int argc, char* argv[]){

    if(argc > 1){
        std::filesystem::path input_path = argv[1];
        if (!std::filesystem::exists(input_path)) {
            std::cerr << "Error: Path does not exist.\n";
            return 1;
        }

        if (std::filesystem::is_regular_file(input_path) && input_path.extension() == ".json") {
            runmodel muscle_model(input_path.string(), 1);
            double t = muscle_model.runprogramm();
            return 0;
        }

        if (std::filesystem::is_directory(input_path)) {
            std::vector<std::filesystem::path> json_files;

            for (const auto& entry : std::filesystem::directory_iterator(input_path)) {
                if (std::filesystem::is_regular_file(entry) && entry.path().extension() == ".json") {
                    json_files.push_back(entry.path());
                }
            }

            if (json_files.empty()) {
                std::cout << "No JSON files found in folder.\n";
                return 0;
            }

            for (const auto& json_file : json_files) {
                runmodel muscle_model(json_file.string(), 1);
                double t = muscle_model.runprogramm();
            }
            return 0;
        }
    } else {
        QApplication a(argc, argv);
        MainWindow mainWin;
        mainWin.show();
        return a.exec();
    }
}

