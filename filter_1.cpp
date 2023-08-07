#include <iostream>
#include <fstream>
#include <string>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"


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


void ReplaceValue(rapidjson::Value& json_value,
                  rapidjson::MemoryPoolAllocator<>& allocator, 
                  const char* key_to_replace,
                  const char* new_value) {

    if (json_value.IsObject()) {
        for (auto it = json_value.MemberBegin(); it != json_value.MemberEnd(); it++) {
            const char* current_key = it->name.GetString();
            if (std::strcmp(current_key, key_to_replace) == 0) {
                it->value.SetString(new_value, allocator);
            }
            else {
                ReplaceValue(it->value, allocator, key_to_replace, new_value);
            }
        }
    }
    else if (json_value.IsArray()) {
        for (auto& value : json_value.GetArray()) {
            ReplaceValue(value, allocator, key_to_replace, new_value);
        }
    }
}

void ReplaceValue(rapidjson::Document& json_document, const char* key_to_replace, const char* new_value) {
    ReplaceValue(json_document, json_document.GetAllocator(), key_to_replace, new_value);
}


int main() {
    // Get json c-string from file.
    std::string json_str = ReadFileToString("json_file.json");
    int json_len = json_str.length();
    char json_char_arr[json_len+1];
    std::strcpy(json_char_arr, json_str.c_str());
    printf("Original json:\n-----\n%s\n\n", json_char_arr);
    
    // Parse info DOM
    rapidjson::Document document;
    document.Parse(json_char_arr);
    if (document.HasParseError() && !document.IsObject()) {
        printf("Document is not a valid json\n");
        return 1;
    }

    // Process the DOM
    ReplaceValue(document, "first_name", "ass");


    // Stringify the DOM
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    document.Accept(writer);
    
    const char* json_char_arr_fin = buffer.GetString();
    printf("Final json:\n-----\n%s\n\n", json_char_arr_fin);
    return 0;
}
