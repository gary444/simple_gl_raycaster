#pragma once

#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <sstream>

//  ===========
//  Shadinclude
//  ===========
/*
LICENCE
MIT License

Copyright (c) [2018] [Tahar Meijs]

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

INTRODUCTION
The sole purpose of this class is to load a file and extract the text that is in it.
In theory, this class could be used for a variety of text-processing purposes, but
it was initially designed to be used to load shader source code.

USING THIS CLASS
Since the entire class is a static class, you only have to add this single line of
code to your project:

--------------------------------------------------------------------------------------
std::string shaderSource = Shadinclude::load("./path/to/shader.extension");
--------------------------------------------------------------------------------------

This will (recursively) extract the source code from the first shader file.
Now, you might be wondering, what is the point of using your code for something
so trivial as to loading a file and calling the "std::getline()" function on it?

Well, besides loading the shader source code from a single file, the loader also
supports custom keywords that allow you to include external files inside your
shader source code!

PARAMETERS OF THE LOAD FUNCTION
- std::string   path                path to the "main" shader file
- std::string   includeIdentifier       keyword to look for when scanning for files

MISCELLANEOUS
- Author    :   Tahar Meijs
- Date      :   10th - 12th of April 2018
- Language  :   C++ (can easily be converted into other languages)
*/

class Shadinclude
{
public:
    // Return the source code of the complete shader
    static std::string load(std::string path, std::string includeIndentifier = "#include")
    {
        includeIndentifier += ' ';
        static bool isRecursiveCall = false;

        std::string fullSourceCode = "";
        std::ifstream file(path);

        if (!file.is_open())
        {
            std::cerr << "ERROR: could not open the shader at: " << path << "\n" << std::endl;
            return fullSourceCode;
        }

        std::string lineBuffer;
        while (std::getline(file, lineBuffer))
        {
            // Look for the new shader include identifier
            if (lineBuffer.find(includeIndentifier) != lineBuffer.npos)
            {
                // Remove the include identifier, this will cause the path to remain
                lineBuffer.erase(0, includeIndentifier.size());

                // The include path is relative to the current shader file path
                std::string pathOfThisFile;
                getFilePath(path, pathOfThisFile);
                lineBuffer.insert(0, pathOfThisFile);

                // By using recursion, the new include file can be extracted
                // and inserted at this location in the shader source code
                isRecursiveCall = true;
                fullSourceCode += load(lineBuffer);

                // Do not add this line to the shader source code, as the include
                // path would generate a compilation issue in the final source code
                continue;
            }

            fullSourceCode += lineBuffer + '\n';
        }

        // Only add the null terminator at the end of the complete file,
        // essentially skipping recursive function calls this way
        if (!isRecursiveCall)
            fullSourceCode += '\0';

        file.close();

        return fullSourceCode;
    }

    static std::string resolve_includes(std::string shader_string, std::string includeIndentifier = "#include") {

        // // prepare map for resolving includes 
        std::map<std::string, std::string> include_file_map;
        // include_file_map.insert({"include/svo_types.incl", 
        //     #include <grt/Shaders/include/svo_types.incl>
        // });
        // include_file_map.insert({"include/utils.incl", 
        //     #include <grt/Shaders/include/utils.incl>
        // });
        // include_file_map.insert({"include/marching_cubes.incl", 
        //     #include <grt/Shaders/include/marching_cubes.incl>
        // });
        // include_file_map.insert({"include/distinct_colors.incl", 
        //     #include <grt/Shaders/include/distinct_colors.incl>
        // });

        // include_file_map.insert({"include/cut_list_definition_time_tree.incl", 
        //     #include <grt/Shaders/include/cut_list_definition_time_tree.incl>
        // });
        // include_file_map.insert({"include/cut_list_definition_tsdfc.incl", 
        //     #include <grt/Shaders/include/cut_list_definition_tsdfc.incl>
        // });
        // include_file_map.insert({"include/apron_extraction_implementation.incl", 
        //     #include <grt/Shaders/include/apron_extraction_implementation.incl>
        // });
        // include_file_map.insert({"include/triangle_extraction_implementation.incl", 
        //     #include <grt/Shaders/include/triangle_extraction_implementation.incl>
        // });

        // include_file_map.insert({"include/camera_evaluation_implementation.incl", 
        //     #include <grt/Shaders/include/camera_evaluation_implementation.incl>
        // });

        //include_file_map.insert({"include/projective_texturing.incl", 
            //#include <grt/Shaders/include/projective_texturing.incl>
            //});

        includeIndentifier += ' ';


        std::string fullSourceCode = "";
        // std::ifstream file(path);
        std::stringstream stream;
        stream << shader_string;


        std::string lineBuffer;
        while (std::getline(stream, lineBuffer))
        {
            // std::cout << lineBuffer << std::endl;

            // Look for the new shader include identifier
            if (lineBuffer.find(includeIndentifier) != lineBuffer.npos)
            {
                // Remove the include identifier, this will cause the path to remain
                lineBuffer.erase(0, includeIndentifier.size());

                // check that identifier is present in map
                if (include_file_map.find(lineBuffer) == include_file_map.end()) {
                    throw std::logic_error("Error: required include shader " + lineBuffer + " was not found in grt::gl::Shadinclude. It needs to be added explicitly to the map of known include shaders!");
                }
                // if found, add string to source code 
                fullSourceCode += include_file_map[lineBuffer];

                continue;
            }

            fullSourceCode += lineBuffer + '\n';
        }

        // Only add the null terminator at the end of the complete file,
        fullSourceCode += '\0';

        return fullSourceCode;
    
    }

private:
    static void getFilePath(const std::string & fullPath, std::string & pathWithoutFileName)
    {
        // Remove the file name and store the path to this folder
        size_t found = fullPath.find_last_of("/\\");
        pathWithoutFileName = fullPath.substr(0, found + 1);
    }





};
