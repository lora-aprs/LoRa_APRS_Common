#include "BoardFinder.h"
#include "logger.h"
#include "power_management.h"

BoardConfig::BoardConfig(
	String name, BoardType type,
	uint8_t oledsda, uint8_t oledscl, uint8_t oledaddr, uint8_t oledreset,
	uint8_t lorasck, uint8_t loramiso, uint8_t loramosi, uint8_t loracs, uint8_t lorareset, uint8_t lorairq,
	bool needcheckpowerchip, bool powercheckstatus)
{
	Name = name;
	Type = type;

	OledSda = oledsda;
	OledScl = oledscl;
	OledAddr = oledaddr;
	OledReset = oledreset;

	LoraSck = lorasck;
	LoraMiso = loramiso;
	LoraMosi = loramosi;
	LoraCS = loracs;
	LoraReset = lorareset;
	LoraIRQ = lorairq;

	needCheckPowerChip = needcheckpowerchip;
	powerCheckStatus = powercheckstatus;
}

BoardFinder::BoardFinder()
{
	_boardConfigs.push_back(std::shared_ptr<BoardConfig>(new BoardConfig("TTGO_LORA32_V1",         eTTGO_LORA32_V1,          4, 15, 0x3C,  0,  5, 19, 27, 18, 14, 26)));
	_boardConfigs.push_back(std::shared_ptr<BoardConfig>(new BoardConfig("TTGO_LORA32_V2",         eTTGO_LORA32_V2,         21, 22, 0x3C,  0,  5, 19, 27, 18, 14, 26, true)));
	_boardConfigs.push_back(std::shared_ptr<BoardConfig>(new BoardConfig("TTGO_T_Beam_V0_7",       eTTGO_T_Beam_V0_7,       21, 22, 0x3C,  0,  5, 19, 27, 18, 14, 26, true)));
	_boardConfigs.push_back(std::shared_ptr<BoardConfig>(new BoardConfig("TTGO_T_Beam_V1_0",       eTTGO_T_Beam_V1_0,       21, 22, 0x3C,  0,  5, 19, 27, 18, 14, 26, true, true)));
	_boardConfigs.push_back(std::shared_ptr<BoardConfig>(new BoardConfig("ETH_BOARD",              eETH_BOARD,              33, 32, 0x3C,  0, 14,  2, 15, 12,  4, 36)));
	_boardConfigs.push_back(std::shared_ptr<BoardConfig>(new BoardConfig("TRACKERD",               eTRACKERD,                5,  4, 0x3C,  0, 18, 19, 23, 16, 14, 26)));
	_boardConfigs.push_back(std::shared_ptr<BoardConfig>(new BoardConfig("HELTEC_WIFI_LORA_32_V1", eHELTEC_WIFI_LORA_32_V1,  4, 15, 0x3C, 16,  5, 19, 27, 18, 14, 26)));
	_boardConfigs.push_back(std::shared_ptr<BoardConfig>(new BoardConfig("HELTEC_WIFI_LORA_32_V2", eHELTEC_WIFI_LORA_32_V2,  4, 15, 0x3C, 16,  5, 19, 27, 18, 14, 26)));

}

std::shared_ptr<BoardConfig> BoardFinder::searchBoardConfig()
{
	logPrintlnI("looking for a board config.");
	logPrintlnI("searching for OLED...");

	for(std::shared_ptr<BoardConfig> boardconf : _boardConfigs)
	{
		if(boardconf->needCheckPowerChip && checkPowerConfig(boardconf) == boardconf->powerCheckStatus)
		{
			PowerManagement powerManagement;
			TwoWire wire(0);
			wire.begin(boardconf->OledSda, boardconf->OledScl);
			powerManagement.begin(wire);
			powerManagement.activateOLED();
		}
		else if(boardconf->needCheckPowerChip)
		{
			continue;
		}
		if(checkOledConfig(boardconf))
		{
			logPrintI("found a board config: ");
			logPrintlnI(boardconf->Name);
			return boardconf;
		}
	}

	logPrintlnW("could not find OLED, will search for the modem now...");

	for(std::shared_ptr<BoardConfig> boardconf : _boardConfigs)
	{
		if(boardconf->needCheckPowerChip && checkPowerConfig(boardconf) == boardconf->powerCheckStatus)
		{
			PowerManagement powerManagement;
			TwoWire wire(0);
			wire.begin(boardconf->OledSda, boardconf->OledScl);
			powerManagement.begin(wire);
			powerManagement.activateLoRa();
		}
		if(checkModemConfig(boardconf))
		{
			logPrintI("found a board config: ");
			logPrintlnI(boardconf->Name);
			return boardconf;
		}
	}

	logPrintlnW("could not find a board config!");

	return 0;
}

std::shared_ptr<BoardConfig> BoardFinder::getBoardConfig(String name)
{
	for(std::shared_ptr<BoardConfig> boardconf : _boardConfigs)
	{
		if(boardconf->Name == name)
		{
			return boardconf;
		}
	}
	return 0;
}

bool BoardFinder::checkOledConfig(std::shared_ptr<BoardConfig> boardConfig)
{
	if(boardConfig->OledReset > 0)
	{
		pinMode(boardConfig->OledReset, OUTPUT);
		digitalWrite(boardConfig->OledReset, HIGH);
		delay(1);
		digitalWrite(boardConfig->OledReset, LOW);
		delay(10);
		digitalWrite(boardConfig->OledReset, HIGH);
	}
	TwoWire wire(0);
	if(!wire.begin(boardConfig->OledSda, boardConfig->OledScl))
	{
		logPrintlnW("issue with wire");
		return false;
	}
	wire.beginTransmission(boardConfig->OledAddr);
	if(!wire.endTransmission())
	{
		return true;
	}
	return false;
}

bool BoardFinder::checkModemConfig(std::shared_ptr<BoardConfig> boardConfig)
{
	pinMode(boardConfig->LoraReset, OUTPUT);
	digitalWrite(boardConfig->LoraReset, LOW);
	delay(10);
	digitalWrite(boardConfig->LoraReset, HIGH);
	delay(10);

	pinMode(boardConfig->LoraCS, OUTPUT);
	digitalWrite(boardConfig->LoraCS, HIGH);

	SPIClass spi;
	spi.begin(boardConfig->LoraSck, boardConfig->LoraMiso, boardConfig->LoraMosi, boardConfig->LoraCS);

	digitalWrite(boardConfig->LoraCS, LOW);

	spi.beginTransaction(SPISettings(8E6, MSBFIRST, SPI_MODE0));
	spi.transfer(0x42);
	uint8_t response = spi.transfer(0x00);
	spi.endTransaction();

	digitalWrite(boardConfig->LoraCS, HIGH);

	if(response == 0x12)
	{
		return true;
	}
	return false;
}

bool BoardFinder::checkPowerConfig(std::shared_ptr<BoardConfig> boardConfig)
{
	TwoWire wire(0);
	if(!wire.begin(boardConfig->OledSda, boardConfig->OledScl))
	{
		logPrintlnW("issue with wire");
		return false;
	}
	wire.beginTransmission(0x34);
	wire.write(0x03);
	wire.endTransmission();
	
	wire.requestFrom(0x34, 1);
	int response = wire.read();
	wire.endTransmission();

	logPrintlnD(String(response));
	if(response == 0x03)
	{
		logPrintlnD("power chip found!");
		return true;
	}
	logPrintlnD("power chip NOT found");
	return false;
}
