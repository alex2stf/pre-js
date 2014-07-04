
// Script based Template using NOt Evaluated Script
#include <node.h>
#include <v8.h>
#include <v8-preparser.h>
#include <v8stdint.h>
#include <stdlib.h>

#include <iostream>



using namespace v8;
using namespace std;



/**********************   PJS parser class *******************************************************/
class pjs_parser{


#define pjsstat_buffer 1
#define pjsstat_code 2
#define pjsstat_line 3
#define pjs_endTag "pjs?>"
#define pjs_startTag "<?pjs"

private:
	std::string codeBlock;
	std::string lineBlock;
	std::string buffer;
	int pstat;
	int num;

public:
	pjs_parser(){
		pstat = pjsstat_line;
		num = 0;
	};
	void parsec(char c, void (onIterate) (std::string value, int stat));
	void parses(std::string input, void (onIterate) (std::string value, int stat));

	void flush(){
		codeBlock.clear();
		lineBlock.clear();
		buffer.clear();
		num = 0;
		pstat = pjsstat_line;
	}
};


void pjs_parser::parses(std::string input, void (onIterate) (std::string value, int stat)){
	num = 0;
	input.append(pjs_endTag);
	for (unsigned int i = 0; i < input.size(); i++){
		parsec(input.at(i), onIterate);
	}


	if(!lineBlock.empty()){
		onIterate(lineBlock, pjsstat_line);
		lineBlock.clear();
	}

	if(!codeBlock.empty()){
		onIterate(codeBlock, pjsstat_line);
		codeBlock.clear();
	}
}

void pjs_parser::parsec(char c, void (onIterate) (std::string value, int stat)){

	buffer+=c;

	if(buffer.size() > 4){
		num++;
		if(buffer == pjs_startTag){
			pstat = pjsstat_code;
			onIterate(lineBlock, pjsstat_line);
			lineBlock.clear();
			buffer.clear();
		}

		else if(buffer == pjs_endTag){
			pstat = pjsstat_line;
			onIterate(codeBlock, pjsstat_code);
			codeBlock.clear();
			buffer.clear();
		}

		else{
			char d = buffer.at(0);
			buffer.erase(buffer.begin());
			if(pstat == pjsstat_code){
				codeBlock+=d;
			}
			else{
				lineBlock+=d;
			}
		}
	}

}

/**********************  END PJS parser class *******************************************************/


/**********************  PROPS:  *******************************************************/
std::string parsedContent;
Persistent<Context> context;
Handle<ObjectTemplate> global_templ;
pjs_parser parser;



/**********************  UTILS:  *******************************************************/
std::string v8StringToStdString(v8::Local<v8::Value> value){
	v8::String::Utf8Value param(value->ToString());
	return std::string(*param);
}







/**********************  v8:  *******************************************************/


Handle<Value> printMessage(const Arguments& args) {
	HandleScope scope;
	std::string all;
	for(int i = 0 ; i < args.Length(); i++){
		v8::String::Utf8Value param(args[i]->ToString());
//		std::cout <<  "print call: " << std::string(*param) << std::endl;
		parsedContent.append(std::string(*param));
		all.append(std::string(*param));
	}

	return scope.Close(String::New(all.c_str()));
}












Handle<Value> CreateContext(const Arguments& args) {
  HandleScope scope;



  global_templ = ObjectTemplate::New();
  global_templ->Set( String::New("print"), FunctionTemplate::New( printMessage ));



  if(args[0]->IsObject()){
	  Handle<Object> obj = Handle<Object>::Cast(args[0]);
	  const Local<Array> props = obj->GetPropertyNames();
	  const uint32_t length = props->Length();

	  for (uint32_t i=0 ; i<length ; ++i)
	  {
	      Local<String> key =Local<String>::Cast(props->Get(i)) ;
	      global_templ->Set(key, Handle<Object>::Cast(obj->Get(key)));
	  }
    }

  context = Context::New(NULL, global_templ);
  context->AllowCodeGenerationFromStrings(true);
  return scope.Close(Boolean::New(true));
}


Handle<Value> DestroyContext(const Arguments& args) {
  HandleScope scope;
  context.Dispose();
  return scope.Close(Boolean::New(true));
}



void parseHandler(std::string line, int status){
	if(!line.empty()){
		if(status == pjsstat_code){
			Handle<String> source = String::New(line.c_str());
			Handle<Script> script = Script::Compile(source);
			script->Run();
		}else{
			parsedContent.append(line);
		}
	}
}

std::string parseContent(std::string input, Handle<Function> function){

	using namespace std;
	parsedContent.clear();
	parser.flush();

	context->Enter();


	parser.parses(input, parseHandler);

	//callback
	Handle<Value> args[1];
	args[0] = v8::String::New(parsedContent.c_str());
	Handle<v8::Object> global = context->Global();
	function->Call(global, 1, args);
	context->Exit();
	return input;
}



std::string appendStringFromFile(std::string filename, Handle<Function> callback){


		FILE* myfile = fopen(filename.c_str(), "r");
		char ch;
		std::string result;

		if (myfile != NULL) {
			context->Enter();
			while( ( ch = fgetc(myfile) ) != EOF ){
			      result+= ch;
			      parser.parsec(ch, parseHandler);
			}

			fclose(myfile);

			parser.parsec(' ', parseHandler);
			parser.parsec(' ', parseHandler);
			parser.parsec(' ', parseHandler);
			parser.parsec(' ', parseHandler);


			Handle<Value> args[1];
			args[0] = v8::String::New(parsedContent.c_str());
			callback->Call(context->Global(), 1, args);
			context->Exit();
			return result;
		}

		return "";
}

Handle<Value> LoadFile(const Arguments& args) {
  HandleScope scope;

  parsedContent.clear();
  parser.flush();


  if(args[0]->IsString() && args[1]->IsFunction()){
	  string fileName = v8StringToStdString(args[0]);
	  Handle<Function> func = v8::Handle<v8::Function>::Cast(args[1]);
	  appendStringFromFile(fileName, func);
//	  cout << fileName << endl;
//	  appendStringFromFile(fileName);
//
//	  parseContent(fileContent, func);
  }

  return scope.Close(Boolean::New(true));
}



Handle<Value> Parse(const Arguments& args) {
  HandleScope scope;

  if(args[0]->IsString() && args[1]->IsFunction()){
  	  string content = v8StringToStdString(args[0]);

  	  Handle<Function> func = v8::Handle<v8::Function>::Cast(args[1]);
  	  parseContent(content, func);
    }

  return scope.Close(Boolean::New(true));
}


Handle<Value> Cin(const Arguments& args) {
  HandleScope scope;

  if(args[0]->IsString()){
	  Local<String> out = Local<String>::Cast(args[0]);
	  cout << v8StringToStdString(out);
  }
  string resp;
  cin >> resp;


  return scope.Close(String::New(resp.c_str()));
}









void init(Handle<Object> exports) {
	exports->Set(String::NewSymbol("cin"), FunctionTemplate::New(Cin)->GetFunction());
	exports->Set(String::NewSymbol("createContext"), FunctionTemplate::New(CreateContext)->GetFunction());
	exports->Set(String::NewSymbol("destroyContext"), FunctionTemplate::New(DestroyContext)->GetFunction());
	exports->Set(String::NewSymbol("loadFile"), FunctionTemplate::New(LoadFile)->GetFunction());
	exports->Set(String::NewSymbol("parse"), FunctionTemplate::New(Parse)->GetFunction());
}

NODE_MODULE(prejs, init)

