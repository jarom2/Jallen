ListKeyVal<std::string, ListKeyVal<std::string, int> > outer;
ListKeyVal<std::string, int> inner;
std::shared_ptr<KeyVal<std::string, int> > pKeyVal(new ListKeyVal<std::string, int>); 
