#include "JsonReader.h"
#include <iostream>


JsonReader::JsonReader()
{
}

JsonReader::~JsonReader()
{
}

bool JsonReader::LoadJsonFile(std::string filename, rapidjson::Document& document)
{
    using namespace rapidjson;
    FILE* fp = 0;
    std::string filePath = FILE_PATH + filename;
    fopen_s(&fp, filePath.c_str(), "rb");

    if (fp == 0)
    {
		return false;
	}

    char readBuffer[65536];
    FileReadStream is(fp, readBuffer, sizeof(readBuffer));
    document.ParseStream(is);

    if (!document.IsObject())
    {
        return false;
    }

  
    fclose(fp);

    return true;
}

void JsonReader::WriteJsonFile(const char* filename, rapidjson::Document &document)
{
    using namespace rapidjson;
	FILE* fp = 0;
	fopen_s(&fp, filename, "wb");

    if (fp == 0)
    {
		return;
	}

	char writeBuffer[65536];
	FileWriteStream os(fp, writeBuffer, sizeof(writeBuffer));
	Writer<FileWriteStream> writer(os);
	document.Accept(writer);

	fclose(fp);
}
