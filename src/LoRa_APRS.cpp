#include <LoRa_APRS.h>

LoRa_APRS::LoRa_APRS()
	: _LastReceivedMsg(0), _RxFrequency(LORA_RX_FREQUENCY), _TxFrequency(LORA_TX_FREQUENCY)
{
	SPI.begin(LORA_SCK, LORA_MISO, LORA_MOSI, LORA_CS);
	setPins(LORA_CS, LORA_RST, LORA_IRQ);

	setSpreadingFactor(LORA_SPREADING_FACTOR);
	setSignalBandwidth(LORA_SIGNAL_BANDWIDTH);
	setCodingRate4(LORA_CODING_RATE4);
	enableCrc();
}

LoRa_APRS::LoRa_APRS(int sck, int miso, int mosi, int cs, int rst, int irq)
	: _LastReceivedMsg(0), _RxFrequency(LORA_RX_FREQUENCY), _TxFrequency(LORA_TX_FREQUENCY)
{
	SPI.begin(sck, miso, mosi, cs);
	setPins(cs, rst, irq);

	setSpreadingFactor(LORA_SPREADING_FACTOR);
	setSignalBandwidth(LORA_SIGNAL_BANDWIDTH);
	setCodingRate4(LORA_CODING_RATE4);
	enableCrc();
}

bool LoRa_APRS::hasMessage()
{
	if(!parsePacket())
	{
		return false;
	}
	// read header:
	char dummy[4];
	readBytes(dummy, 3);
	if(dummy[0] != '<')
	{
		// is no APRS message, ignore message
		while(available())
		{
			read();
		}
		return false;
	}
	// read APRS data:
	String str;
	while(available())
	{
		str += (char)read();
	}
	_LastReceivedMsg = std::shared_ptr<APRSMessage>(new APRSMessage());
	_LastReceivedMsg->decode(str);
	return true;
}

std::shared_ptr<APRSMessage> LoRa_APRS::getMessage()
{
	return _LastReceivedMsg;
}

// cppcheck-suppress unusedFunction
void LoRa_APRS::sendMessage(const std::shared_ptr<APRSMessage> msg)
{
	setFrequency(_TxFrequency);
	String data = msg->encode();
	beginPacket();
	// Header:
	write('<');
	write(0xFF);
	write(0x01);
	// APRS Data:
	write((const uint8_t *)data.c_str(), data.length());
	endPacket();
	setFrequency(_RxFrequency);
}

void LoRa_APRS::setRxFrequency(long frequency)
{
	_RxFrequency = frequency;
	setFrequency(_RxFrequency);
}

long LoRa_APRS::getRxFrequency() const
{
	return _RxFrequency;
}

void LoRa_APRS::setTxFrequency(long frequency)
{
	_TxFrequency = frequency;
}

long LoRa_APRS::getTxFrequency() const
{
	return _TxFrequency;
}
