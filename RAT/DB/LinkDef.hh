#ifdef __CINT__

#pragma namespace RAT+;
#pragma namespace RAT::DB+;

#pragma link C++ class RAT::DB+;
#pragma link C++ class RAT::DBLink+;
#pragma link C++ class RAT::DBLinkPtr+;
#pragma link C++ class RAT::DBTable+;
#pragma link C++ class RAT::DBTextLoader+;
#pragma link C++ class RAT::DBJsonLoader+;
#pragma link C++ class json::Value+; 

// #pragma link C++ class RAT::ObjInt+;
// #pragma link C++ class RAT::ObjDbl+;
// #pragma link C++ class RAT::Log+;
// #pragma link C++ class RAT::HTTPDownloader+;
// #pragma link C++ class RAT::DBFieldCallback+;

#endif // __CINT__

#ifdef __MAKECINT__

#pragma link C++ class vector<RAT::DBTable*>;

#endif

