template <typename OutputHandler>
ReplacementHandler<OutputHandler>::ReplacementHandler(OutputHandler& iOutputHandler,
                                                      const std::set<std::string>& iKeysToReplace,
                                                      const std::string& iNewValue)
    : _outputHandler(iOutputHandler), _keysToReplace(iKeysToReplace), _newValue(iNewValue) {}

template <typename OutputHandler>
bool ReplacementHandler<OutputHandler>::Null() {
    return _replaceNextValue ? Replace() : _outputHandler.Null();
}

template <typename OutputHandler>
bool ReplacementHandler<OutputHandler>::Bool(bool b) {
    return _replaceNextValue ? Replace() : _outputHandler.Bool(b);
}

template <typename OutputHandler>
bool ReplacementHandler<OutputHandler>::Int(int i) {
    return _replaceNextValue ? Replace() : _outputHandler.Int(i);
}
template <typename OutputHandler>
bool ReplacementHandler<OutputHandler>::Uint(unsigned u) {
    return _replaceNextValue ? Replace() : _outputHandler.Uint(u);
}

template <typename OutputHandler>
bool ReplacementHandler<OutputHandler>::Int64(int64_t i) {
    return _replaceNextValue ? Replace() : _outputHandler.Int64(i);
}

template <typename OutputHandler>
bool ReplacementHandler<OutputHandler>::Uint64(uint64_t u) {
    return _replaceNextValue ? Replace() : _outputHandler.Uint64(u);
}

template <typename OutputHandler>
bool ReplacementHandler<OutputHandler>::Double(double d) {
    return _replaceNextValue ? Replace() : _outputHandler.Double(d);
}

template <typename OutputHandler>
bool ReplacementHandler<OutputHandler>::StartObject() {
    return _replaceNextValue ? Replace() : _outputHandler.StartObject();
}

template <typename OutputHandler>
bool ReplacementHandler<OutputHandler>::RawNumber(const char* str, uint len, bool copy) {
    return _replaceNextValue ? Replace() : _outputHandler.RawNumber(str, len, copy);
}
    
template <typename OutputHandler>
bool ReplacementHandler<OutputHandler>::String(const char* str, uint len, bool copy) { 
    return _replaceNextValue ? Replace() : _outputHandler.String(str, len, copy);
}

template <typename OutputHandler>
bool ReplacementHandler<OutputHandler>::Key(const char* str, uint len, bool copy) {
    _replaceNextValue = (_keysToReplace.count(std::string(str)) != 0);
    return _outputHandler.Key(str, len, copy);
}

template <typename OutputHandler>
bool ReplacementHandler<OutputHandler>::StartArray() { 
    return _replaceNextValue ? Replace() : _outputHandler.StartArray();
}

template <typename OutputHandler>
bool ReplacementHandler<OutputHandler>::EndArray(uint elementCount) {
    return _outputHandler.EndArray(elementCount);
}

template <typename OutputHandler>
bool ReplacementHandler<OutputHandler>::EndObject(uint memberCount) {
    return _outputHandler.EndObject(memberCount);
}

template <typename OutputHandler>
bool ReplacementHandler<OutputHandler>::Replace() {
    _replaceNextValue = false;
    return _outputHandler.String(_newValue.c_str(), _newValue.length(), true);
}
