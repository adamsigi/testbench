#include <set>
#include "rapidjson/reader.h"
#include "rapidjson/writer.h"

template <typename OutputHandler>
class ReplacementHandler {
public:
    ReplacementHandler(OutputHandler& iOutputHandler,
                       const std::set<std::string>& iKeysToReplace,
                       const std::string& iNewValue);
    bool Null();
    bool Bool(bool b);
    bool Int(int i);
    bool Uint(unsigned u);
    bool Int64(int64_t i);
    bool Uint64(uint64_t u);
    bool Double(double d);
    bool StartObject();
    bool RawNumber(const char* str, uint len, bool copy);
    bool String(const char* str, uint len, bool copy);
    bool Key(const char* str, uint len, bool copy);
    bool StartArray();
    bool EndArray(uint elementCount);
    bool EndObject(uint memberCount);

private:
    ReplacementHandler(const ReplacementHandler&);
    ReplacementHandler& operator=(const ReplacementHandler&);
    bool Replace();
    
    OutputHandler& _outputHandler;
    std::set<std::string> _keysToReplace;
    std::string _newValue;
    bool _replaceNextValue = false;

};

#include "ReplacementHandler.ipp"