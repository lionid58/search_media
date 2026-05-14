#include <nlohmann/json.hpp>
#include <string>
#include <vector>
#include <set>
#include <optional>
#include <mutex>
#include <thread>
#include <stop_token>

using json = nlohmann::json;
namespace fs = std::filesystem;

enum extType {
    video,
    audio,
    images
};

struct ExtTabel
{
    inline static const std::set<std::string> file_ext_video = {".mp4", ".mov", ".mkv", ".avi", ".webm", ".wmv"};
    inline static const std::set<std::string> file_ext_audio = {".mp3", ".wav", ".aac", ".ogg"};
    inline static const std::set<std::string> file_ext_images = {".jpg", ".png", ".svg", ".tiff"};

    static std::optional<extType> GetExtType(std::string ext)
    {
        if(file_ext_video.contains(ext))
            return extType::video;
        else if(file_ext_audio.contains(ext))
            return extType::audio;
        else if(file_ext_images.contains(ext))
            return extType::images;
        else
            return std::nullopt;
    }
};


class MediaFileParser
{
private:
    json files;
    std::string path;
    std::mutex js_file_mutex;
    ExtTabel extentions;

    void jsUpdate();

public:
    MediaFileParser();

    void set_path(std::string path_);

    json GetFilesJson();

    void RunUpdate(std::stop_token stop_token,double time);
};