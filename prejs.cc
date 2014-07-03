
// Script based Template using NOt Evaluated Script
#include <node.h>
#include <v8.h>
#include <v8-preparser.h>
#include <v8stdint.h>
#include <stdlib.h>

#include <iostream>
#include <istream>
#include <sstream>
#include <ostream>
#include <vector>
#include <sstream>
#include <ostream>
#include <iostream>
#include <string>
#include <time.h>
#include <vector>
#include <algorithm>
#include <cctype>
#include <functional>
#include <fstream>
#include <iterator>
#include <algorithm>
#include <map>
#include <stdio.h>
#include <ctime>
#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string/trim_all.hpp>
#include <set>

using namespace v8;


std::vector<std::string> split(const std::string& s, const std::string& delim) {
    std::vector<std::string> result;
    if (delim.empty()) {
        result.push_back(s);
        return result;
    }
    std::string::const_iterator substart = s.begin(), subend;
    while (true) {
        subend = search(substart, s.end(), delim.begin(), delim.end());
        std::string temp(substart, subend);
        if (!temp.empty()) {
            result.push_back(temp);
        }
        if (subend == s.end()) {
            break;
        }
        substart = subend + delim.size();
    }
    return result;
}//exit split function

std::string v8StringToStdString(v8::Local<v8::Value> value){
	v8::String::Utf8Value param(value->ToString());
	return std::string(*param);
}


std::string appendStringFromFile(std::string filename){
	using namespace std;

		FILE* myfile = fopen(filename.c_str(), "r");
		char ch;
		std::string result;

		if (myfile != NULL) {

			while( ( ch = fgetc(myfile) ) != EOF ){
			      result+= ch;
			}


			fclose(myfile);
			return result;
		}

		return "ERROR";
}








std::string parsedContent;

Handle<Value> printMessage(const Arguments& args) {
	HandleScope scope;
	std::string all;
	for(int i = 0 ; i < args.Length(); i++){
		v8::String::Utf8Value param(args[i]->ToString());

		std::cout << std::string(*param) << std::endl;
		parsedContent.append(std::string(*param));
		all.append(std::string(*param));
	}

	return scope.Close(String::New(all.c_str()));
}





std::string trim_all ( const std::string &str ) {
return boost::algorithm::find_format_all_copy(
    boost::trim_copy(str),
    boost::algorithm::token_finder (boost::is_space(),boost::algorithm::token_compress_on),
    boost::algorithm::const_formatter(" "));
}
Persistent<Context> context;
Handle<ObjectTemplate> global_templ;


Handle<Value> Prepare(const Arguments& args) {
  HandleScope scope;

  global_templ = ObjectTemplate::New();
  global_templ->Set( String::New("print"), FunctionTemplate::New( printMessage ));
  context = Context::New(NULL, global_templ);
  context->AllowCodeGenerationFromStrings(true);
  return scope.Close(Boolean::New(true));
}

std::string parseContent(std::string input, Handle<Function> function){

	using namespace std;
	parsedContent.clear();



	context->Enter();

	input = trim_all(input);

	vector<string> res1 = split(input, "<?stone");

	for(int i = 0 ; i < (int)res1.size(); i++){

		vector<string> res2 = split(res1.at(i), "?>");

		for(int a = 0 ; a < (int)res2.size(); a++){
			string currentBlock = res2.at(a);
			trim_all(currentBlock);
			if(currentBlock.at(currentBlock.size()-1) == '@'){
				currentBlock = currentBlock.substr(0, currentBlock.size()-1);
				Handle<String> source = String::New(currentBlock.c_str());
				Handle<Script> script = Script::Compile(source);
				script->Run();

			}else{
				parsedContent.append(currentBlock);
			}
		}

	}

	Handle<Value> args[1];
	args[0] = v8::String::New(parsedContent.c_str());
	Handle<v8::Object> global = context->Global();
	function->Call(global, 1, args);

	context->Exit();
//	context.Dispose();

	return input;
}

Handle<Value> LoadFile(const Arguments& args) {
  HandleScope scope;

  using namespace std;

  if(args[0]->IsString() && args[1]->IsFunction()){
	  string fileName = v8StringToStdString(args[0]);
	  string fileContent =  appendStringFromFile(fileName);
	  Handle<Function> func = v8::Handle<v8::Function>::Cast(args[1]);
	  parseContent(fileContent, func);

  }

  return scope.Close(String::New("world"));
}



Handle<Value> Parse(const Arguments& args) {
  HandleScope scope;

  using namespace std;

  if(args[0]->IsString()){

  }

  return scope.Close(String::New("world"));
}









void init(Handle<Object> exports) {

	exports->Set(String::NewSymbol("createContext"), FunctionTemplate::New(Prepare)->GetFunction());
	exports->Set(String::NewSymbol("loadFile"), FunctionTemplate::New(LoadFile)->GetFunction());
	exports->Set(String::NewSymbol("parse"), FunctionTemplate::New(Parse)->GetFunction());
}

NODE_MODULE(prejs, init)

