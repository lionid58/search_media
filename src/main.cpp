#include <httplib.h>
#include "file_parser.hh"

MediaFileParser parser;

void print_usage(const char* program_name) {
    std::cerr << "Usage: " << program_name << " <path_to_media_files> [update_interval_ms]" << std::endl;
    std::cerr << "  path_to_media_files:   Path to the directory with media files" << std::endl;
    std::cerr << "  update_interval_ms:    Update interval in seconds (default: 1)" << std::endl;
}

int main(int argc, char* argv[])
{   
    if (argc < 2) {
        print_usage(argv[0]);
        return 1;
    }

    try
    {
        const double default_interval = 1;
        double time_milliseconds = default_interval;

        std::string media_path = argv[1];
        parser.set_path(media_path);
        // Задание интервала
        if (argc >= 3) {
            try {
                time_milliseconds = std::stod(argv[2]);
                if (time_milliseconds <= 0) {
                    std::cerr << "Warning: Invalid interval, using default " << default_interval << "ms" << std::endl;
                    time_milliseconds = default_interval;
                }
            } catch (const std::invalid_argument&) {
                std::cerr << "Warning: Invalid interval format, using default " << default_interval << "ms" << std::endl;
                time_milliseconds = default_interval;
            }
        }

        //Настройка сервера
        httplib::Server serv;
        serv.Get("/media_files", [](const httplib::Request&, httplib::Response& res){
            res.set_header("Access-Control-Allow-Origin", "*");
            try {
                json js_file = parser.GetFilesJson();
                res.set_content(js_file.dump(2), "application/json; charset=utf-8");
            } catch (const std::exception& e) {
                res.status = 500;
                res.set_content(R"({"error": ")" + std::string(e.what()) + R"("})", "application/json");
            }
        });

        // Создание потока обновления
        std::jthread update_thread([&parser, time_milliseconds](std::stop_token st) {
            try {
                parser.RunUpdate(st, time_milliseconds);
            } catch (const std::exception& e) {
                std::cerr << "Update thread error: " << e.what() << std::endl;
            }
        });


        std::cout << "Сервер запущен.\n";
        std::cout << "Откройте в браузере: http://localhost:8080/media_files\n";
        std::cout << "Server starting on http://localhost:8080" << std::endl;
        std::cout << "Media path: " << media_path << std::endl;
        std::cout << "Update interval: " << time_milliseconds << "ms" << std::endl;
        
        // Запуск сервера
        if (!serv.listen("localhost", 8080)) {
            std::cerr << "Failed to start server" << std::endl;
            return 1;
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return 1;
    }
}