// JSON filterkey example with SAX-style API.

// This example parses JSON text from stdin with validation.
// During parsing, specified key will be filtered using a SAX handler.
// It re-output the JSON content to stdout without whitespace.
#include <iostream>
#include <fstream>
#include <string>
#include "rapidjson/document.h"
#include "rapidjson/reader.h"
#include "rapidjson/writer.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/filewritestream.h"
#include "rapidjson/error/en.h"
#include <stack>
#include <set>

#include "ReplacementHandler.hpp"

using namespace rapidjson;


std::string ReadFileToString(const std::string& filename) {
    std::ifstream input_file(filename);
    if (!input_file.is_open()) {
        std::cout << "Failed to open the file." << std::endl;
        return "";
    }
    std::string content, line;
    while (std::getline(input_file, line)) {
        content += line;
    }
    input_file.close();
    return content;
}



Reader reader;
rapidjson::StringBuffer buffer;
rapidjson::Writer<rapidjson::StringBuffer> writer;
std::string first_name = "first_name";
std::string last_name = "last_name";

std::set<std::string> labels_to_filter { first_name, last_name };
ReplacementHandler<Writer<StringBuffer> > filter(writer, labels_to_filter, "ass");
std::string clear(std::string& json_str, bool print) {
    buffer.Clear();
    writer.Reset(buffer);
    StringStream is(json_str.c_str());    

    // Prepare Filter
    


    // JSON reader parse from the input stream, filter handler filters the events, and forward to writer.
    // i.e. the events flow is: reader -> filter -> writer
    if (!reader.Parse(is, filter)) {

        throw std::invalid_argument("JSON parsing failed at position " + std::to_string(reader.GetErrorOffset()) + ": "
                                    + std::string(GetParseError_En(reader.GetParseErrorCode())));
        std::cout << "ass\n";
    }
    std::string filtered_str(buffer.GetString());
    if (print) {
        std::cout << '\'' << filtered_str << '\'' << '\n' << filtered_str.length() << "\n-----------\n";
    }
    // std::cout << json_char_arr << '\n';

    return filtered_str;
}




int main(int argc, char* argv[]) {
    // Get json c-string from file.
    std::string json_str = ReadFileToString("json_file_big.json");
    std::cout << "Original length: " << json_str.length() << '\n';

    for (int i = 0; i < 20000; i++) {
        std::string hi = clear(json_str, i == 9999);
    }

    return 0;


}
