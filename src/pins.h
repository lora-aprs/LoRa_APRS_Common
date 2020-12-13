#ifndef PINS_H_
#define PINS_H_

#undef OLED_SDA
#undef OLED_SCL
#undef OLED_RST
#undef LORA_SCK
#undef LORA_MISO
#undef LORA_MOSI
#undef LORA_CS
#undef LORA_RST
#undef LORA_IRQ

#if defined(HELTEC_WIFI_LORA_32_V1) || defined(HELTEC_WIFI_LORA_32_V2) || defined(TTGO_LORA32_V1)
	#define OLED_SDA		4
	#define OLED_SCL		15
#endif

#if defined(TTGO_LORA32_V2) || defined(TTGO_T_Beam_V0_7) || defined(TTGO_T_Beam_V1_0)
	#define OLED_SDA		21
	#define OLED_SCL		22
#endif

#if defined(HELTEC_WIFI_LORA_32_V1) || defined(HELTEC_WIFI_LORA_32_V2) || defined(TTGO_LORA32_V1) || defined(TTGO_LORA32_V2) || defined(TTGO_T_Beam_V0_7) || defined(TTGO_T_Beam_V1_0)
	#define LORA_SCK		5
	#define LORA_MISO		19
	#define LORA_MOSI		27
	#define LORA_CS			18
	#define LORA_RST		14
	#define LORA_IRQ		26
#endif

#ifdef TRACKERD
	#define OLED_SDA		5
	#define OLED_SCL		4
	#define LORA_SCK		18
	#define LORA_MISO		19
	#define LORA_MOSI		23
	#define LORA_CS			16
	#define LORA_RST		14
	#define LORA_IRQ		26
#endif

#ifdef ETH_BOARD
	#define OLED_SDA		33
	#define OLED_SCL		32
	#define LORA_SCK		14
	#define LORA_MISO		2
	#define LORA_MOSI		15
	#define LORA_CS			12
	#define LORA_RST		4
	#define LORA_IRQ		36
	#define ETH_POWER_PIN	-1
	#define ETH_TYPE		ETH_PHY_LAN8720
	#define ETH_ADDR		0
	#define ETH_MDC_PIN		23
	#define ETH_MDIO_PIN	18
	#define NRST			5
#endif

#ifdef ETH_BOARD_V1_0
	#define ETH_CLK			ETH_CLOCK_GPIO17_OUT	// TTGO PoE V1.0
#endif

#ifdef ETH_BOARD_V1_2
	#define ETH_CLK			ETH_CLOCK_GPIO0_OUT		// TTGO PoE V1.2
#endif


#ifdef TTGO_T_Beam_V0_7
	#define GPS_RX			15
	#define GPS_TX			12
#endif

#ifdef TTGO_T_Beam_V1_0
	#define GPS_RX			12
	#define GPS_TX			34
#endif


#undef KEY_BUILTIN
#if defined(TTGO_T_Beam_V0_7)
	#define KEY_BUILTIN		39
#endif

#if defined(TTGO_T_Beam_V1_0)
	#define KEY_BUILTIN		38
#endif

#ifndef KEY_BUILTIN
	#define KEY_BUILTIN		0
#endif

#endif
