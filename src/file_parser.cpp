#include "file_parser.hh"


void MediaFileParser::jsUpdate()
{
    
    std::lock_guard<std::mutex> guard(js_file_mutex);
    files["video"].clear();
    files["audio"].clear();
    files["images"].clear();
    for (const auto& file : fs::directory_iterator(path))
    {
        const auto ext = extentions.GetExtType(file.path().extension().string());
        if(ext != std::nullopt)
        {
            switch (ext.value())
            {
            case extType::video:
            {
                files["video"].push_back(file.path().filename().string());
                break;
            }
            case extType::audio:
            {
                files["audio"].push_back(file.path().filename().string());
                break;
            }
            case extType::images:
            {
                files["images"].push_back(file.path().filename().string());
                break;
            }
            }
        }
    }
    
}

MediaFileParser::MediaFileParser()
{   
    files["audio"] = json::array();
    files["video"] = json::array();
    files["images"] = json::array();
}

void MediaFileParser::set_path(std::string path_)
{
    if(fs::exists(path_) && fs::is_directory(path_))
    {
        path = path_;
    }
    else throw std::runtime_error("Error: worsted directory path");
}

json MediaFileParser::GetFilesJson()
{
    std::lock_guard<std::mutex> guard(js_file_mutex);
    return files;
}

void MediaFileParser::RunUpdate(std::stop_token stop_token,double time)
{
    while (!stop_token.stop_requested())
    {
        jsUpdate();
        std::this_thread::sleep_for(std::chrono::duration<double>(time));
    }
}