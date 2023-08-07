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


// // This handler forwards event into an output handler, with filtering the descendent events of specified key.
// template <typename OutputHandler>
// class FilterKeyHandler {
// public:
//     typedef char Ch;

//     FilterKeyHandler(OutputHandler& outputHandler, const Ch* keyString, SizeType keyLength) : 
//         outputHandler_(outputHandler), keyString_(keyString), keyLength_(keyLength), filterValueDepth_(), filteredKeyCount_()
//     {}

//     bool Null()             { return filterValueDepth_ > 0 ? EndValue() : outputHandler_.Null()    && EndValue(); }
//     bool Bool(bool b)       { return filterValueDepth_ > 0 ? EndValue() : outputHandler_.Bool(b)   && EndValue(); }
//     bool Int(int i)         { return filterValueDepth_ > 0 ? EndValue() : outputHandler_.Int(i)    && EndValue(); }
//     bool Uint(unsigned u)   { return filterValueDepth_ > 0 ? EndValue() : outputHandler_.Uint(u)   && EndValue(); }
//     bool Int64(int64_t i)   { return filterValueDepth_ > 0 ? EndValue() : outputHandler_.Int64(i)  && EndValue(); }
//     bool Uint64(uint64_t u) { return filterValueDepth_ > 0 ? EndValue() : outputHandler_.Uint64(u) && EndValue(); }
//     bool Double(double d)   { return filterValueDepth_ > 0 ? EndValue() : outputHandler_.Double(d) && EndValue(); }
//     bool RawNumber(const Ch* str, SizeType len, bool copy) { return filterValueDepth_ > 0 ? EndValue() : outputHandler_.RawNumber(str, len, copy) && EndValue(); }
//     bool String   (const Ch* str, SizeType len, bool copy) { return filterValueDepth_ > 0 ? EndValue() : outputHandler_.String   (str, len, copy) && EndValue(); }
    
//     bool StartObject() { 
//         if (filterValueDepth_ > 0) {
//             filterValueDepth_++;
//             return true;
//         }
//         else {
//             filteredKeyCount_.push(0);
//             return outputHandler_.StartObject();
//         }
//     }
    
//     bool Key(const Ch* str, SizeType len, bool copy) { 
//         if (filterValueDepth_ > 0) 
//             return true;
//         else if (len == keyLength_ && std::memcmp(str, keyString_, len) == 0) {
//             filterValueDepth_ = 1;
//             return true;
//         }
//         else {
//             ++filteredKeyCount_.top();
//             return outputHandler_.Key(str, len, copy);
//         }
//     }

//     bool EndObject(SizeType) {
//         if (filterValueDepth_ > 0) {
//             filterValueDepth_--;
//             return EndValue();
//         }
//         else {
//             // Use our own filtered memberCount
//             SizeType memberCount = filteredKeyCount_.top();
//             filteredKeyCount_.pop();
//             return outputHandler_.EndObject(memberCount) && EndValue();
//         }
//     }

//     bool StartArray() {
//         if (filterValueDepth_ > 0) {
//             filterValueDepth_++;
//             return true;
//         }
//         else
//             return outputHandler_.StartArray();
//     }

//     bool EndArray(SizeType elementCount) {
//         if (filterValueDepth_ > 0) {
//             filterValueDepth_--;
//             return EndValue();
//         }
//         else
//             return outputHandler_.EndArray(elementCount) && EndValue();
//     }

// private:
//     FilterKeyHandler(const FilterKeyHandler&);
//     FilterKeyHandler& operator=(const FilterKeyHandler&);

//     bool EndValue() {
//         if (filterValueDepth_ == 1) // Just at the end of value after filtered key
//             filterValueDepth_ = 0;
//         return true;
//     }
    
//     OutputHandler& outputHandler_;
//     const char* keyString_;
//     const SizeType keyLength_;
//     unsigned filterValueDepth_;
//     std::stack<SizeType> filteredKeyCount_;
// };



template <typename OutputHandler>
class EmptyKeyHandler {
public:
    typedef char Ch;

    EmptyKeyHandler(OutputHandler& outputHandler, const Ch* keyString, SizeType keyLength) : 
        outputHandler_(outputHandler), keyString_(keyString), keyLength_(keyLength)
    {}

    bool Null() { 
        if (keyToBeEmptied_) {
            keyToBeEmptied_ = false;
            return outputHandler_.String(emptyString_, 0, true);
        }
        else {
            return outputHandler_.Null();
        }
    }

    bool Bool(bool b) { 
        if (keyToBeEmptied_) {
            keyToBeEmptied_ = false;
            return outputHandler_.String(emptyString_, 0, true);
        }
        else {
            return outputHandler_.Bool(b);
        }
    }

    bool Int(int i) { 
        if (keyToBeEmptied_) {
            keyToBeEmptied_ = false;
            return outputHandler_.String(emptyString_, 0, true);
        }
        else {
            return outputHandler_.Int(i);
        }
    }

    bool Uint(unsigned u) { 
        if (keyToBeEmptied_) {
            keyToBeEmptied_ = false;
            return outputHandler_.String(emptyString_, 0, true);
        }
        else {
            return outputHandler_.Uint(u);
        }
    }

    bool Int64(int64_t i) { 
        if (keyToBeEmptied_) {
            keyToBeEmptied_ = false;
            return outputHandler_.String(emptyString_, 0, true);
        }
        else {
            return outputHandler_.Int64(i);
        }
    }

    bool Uint64(uint64_t u) { 
        if (keyToBeEmptied_) {
            keyToBeEmptied_ = false;
            return outputHandler_.String(emptyString_, 0, true);
        }
        else {
            return outputHandler_.Uint64(u);
        }
    }

    bool Double(double d) { 
        if (keyToBeEmptied_) {
            keyToBeEmptied_ = false;
            return outputHandler_.String(emptyString_, 0, true);
        }
        else {
            return outputHandler_.Double(d);
        }
    }

    bool StartObject() { 
        if (keyToBeEmptied_) {
            keyToBeEmptied_ = false;
            return outputHandler_.String(emptyString_, 0, true);
        }
        else {
            return outputHandler_.StartObject();
        }
    }


    bool RawNumber(const Ch* str, SizeType len, bool copy) { 
        if (keyToBeEmptied_) {
            keyToBeEmptied_ = false;
            return outputHandler_.String(emptyString_, 0, true);
        }
        else {
            return outputHandler_.RawNumber(str, len, copy);
        }
    }
    

    bool String(const Ch* str, SizeType len, bool copy) { 
        if (keyToBeEmptied_) {
            keyToBeEmptied_ = false;
            return outputHandler_.String(emptyString_, 0, true);
        }
        else {
            return outputHandler_.String(str, len, copy);
        }
    }
    
    bool Key(const Ch* str, SizeType len, bool copy) {
        keyToBeEmptied_ = (len == keyLength_ && std::memcmp(str, keyString_, len) == 0);
        return outputHandler_.Key(str, len, copy);
    }

    bool EndObject(SizeType memberCount) {
        return outputHandler_.EndObject(memberCount);
    }

    bool StartArray() { 
        if (keyToBeEmptied_) {
            keyToBeEmptied_ = false;
            return outputHandler_.String(emptyString_, 0, true);
        }
        else {
            return outputHandler_.StartArray();
        }
    }
    
    bool EndArray(SizeType elementCount) {
        return outputHandler_.EndArray(elementCount);
    }


private:
    EmptyKeyHandler(const EmptyKeyHandler&);
    EmptyKeyHandler& operator=(const EmptyKeyHandler&);
    
    OutputHandler& outputHandler_;
    const char* keyString_;
    const SizeType keyLength_;
    bool keyToBeEmptied_ = false;
    const char* emptyString_ = "";

};


int main(int argc, char* argv[]) {
    // Get json c-string from file.
    std::string json_str = ReadFileToString("json_file.json");
    int json_len = json_str.length();
    char json_char_arr[json_len+1];
    std::strcpy(json_char_arr, json_str.c_str());
    printf("Original json:\n-----\n%s\n\n", json_char_arr);


    // Prepare JSON reader and input stream.
    Reader reader;
    StringStream is(json_char_arr);

    // Prepare JSON writer and output stream.
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);

    // Prepare Filter
    const char* key_to_filter = "first_name";
    EmptyKeyHandler<Writer<StringBuffer> > filter(writer, key_to_filter, static_cast<SizeType>(strlen(key_to_filter)));

    // JSON reader parse from the input stream, filter handler filters the events, and forward to writer.
    // i.e. the events flow is: reader -> filter -> writer
    if (!reader.Parse(is, filter)) {
        fprintf(stderr, "\nError(%u): %s\n", static_cast<unsigned>(reader.GetErrorOffset()), GetParseError_En(reader.GetParseErrorCode()));
        return 1;
    }

    std::cout << buffer.GetString() << std::endl;

    return 0;
}
