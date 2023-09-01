// #include <set>
// #include <string>
// #include "rapidjson/reader.h"
// #include "rapidjson/writer.h"

// class JsonConcealer {
// public:
//     /**
//      * Create a JsonConcealer object that can replace the values of the provided keys with a new
//      * string value.
//      * 
//      * @param iKeysToConceal The set of keys whose values will be replaced.
//      * @param iNewValue The string value to replace the original values with (default: empty string)
//      * @return A new JsonConcealer object.
//      */
//     JsonConcealer(const std::set<std::string>& iKeysToConceal, const std::string& iNewValue = "");

//     /**
//      * Parse a JSON string and replace the values of all keys to be concealed.
//      * 
//      * @param iJSON The input JSON string to be processed.
//      * @return The modified JSON string with specified key values replaced. 
//      * 
//      * @throws std::invalid_argument if the provided string is not valid JSON.
//      */
//     std::string conceal(const std::string& iJSON);

// private:
//     JsonConcealer(const JsonConcealer&);
//     JsonConcealer& operator=(const JsonConcealer&);

//     rapidjson::Reader reader;
//     rapidjson::StringBuffer buffer;
//     rapidjson::Writer<rapidjson::StringBuffer> writer;
// };