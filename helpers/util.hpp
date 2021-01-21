#include <algorithm>
#include <string>
#include <dirent.h>
#include <vector>


char* get_cmd_option(char** begin, char** end, const std::string & option) {
    char** it = std::find(begin, end, option);
    if (it != end && ++it != end)
        return *it;
    return 0;
}

bool cmd_option_exists(char** begin, char** end, const std::string& option) {
    return std::find(begin, end, option) != end;
}

bool hasEnding (const std::string& fullString, const std::string& ending) {
    if (fullString.length() >= ending.length()) {
        return (0 == fullString.compare (fullString.length() - ending.length(), ending.length(), ending));
    } else {
        return false;
    }
}

std::vector<std::string> get_file_paths_from_directory(const std::string& indir, const std::string& required_suffix = ""){

    std::vector<std::string> img_paths;
    if (auto dir = opendir( indir.c_str() )) {
        while (auto f = readdir(dir)) {
            if (f->d_name[0] == '.')
                continue; // Skip everything that starts with a dot
            std::string fname = indir + "/" + f->d_name;
            
            if (hasEnding(fname,required_suffix)){
                img_paths.push_back(fname);
            }

        }
        closedir(dir);
        std::sort( img_paths.begin(), img_paths.end() );
    }

    if (0 == img_paths.size()){
        throw( "Error: no files found in this folder\n" );
    }

    return img_paths;
}
