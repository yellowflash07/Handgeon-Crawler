#pragma once

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include <rapidjson/filereadstream.h>
#include <rapidjson/filewritestream.h>
#include <string>

class JsonReader
{
public:
	JsonReader();
	~JsonReader();

	bool LoadJsonFile(std::string filename, rapidjson::Document &document);
	void WriteJsonFile(const char* filename, rapidjson::Document &document);

private:
	const std::string FILE_PATH = "../Assets/Json/";

};

