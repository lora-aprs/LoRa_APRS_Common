#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_

#include <list>

#include <Arduino.h>
#include <ArduinoJson.h>

class Configuration;

class ConfigurationManagement
{
public:
	explicit ConfigurationManagement(String FilePath);
	virtual ~ConfigurationManagement();

	Configuration * readConfiguration();
	void writeConfiguration(Configuration * conf);

private:
	virtual Configuration * readProjectConfiguration(DynamicJsonDocument & data) = 0;
	virtual void writeProjectConfiguration(Configuration * conf, DynamicJsonDocument & data) = 0;

	const String mFilePath;
};

#endif
