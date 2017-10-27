
#include "sapi.h"
#include "api.h"
#include "main.h"
CONSOLE_PRINT_ENABLE

/* configuraciÃ³n de mi aplicaciÃ³n */
#define _SYS_CFG_DATALOG_FILENAME 	"datalog.txt"
#define _SYS_CFG_SAMPLINGTIME		(1000)

/************* VARIABLES GLOBALES**************/
uint8_t bandera_temperatura = OFF;
uint8_t bandera_humedad = OFF;
uint8_t bandera_viento = OFF;

/************* FUNCION BORRAR PANTALLA **************/

void BorrarPantalla(void)
{
	uartWriteByte( UART_USB, 27 ); // ESC command
	uartWriteString( UART_USB, "[2J" ); // Clear screen command
	uartWriteByte( UART_USB, 27 ); // ESC command
	uartWriteString( UART_USB, "[H" ); // Cursor to home command
	delay(1); // Pequeño delay para que permita mostrar nuevos mensajes
}
/************* FUNCION MENU**************/

void Menu(void)
{
	consolePrintlnString( ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\r\n");
	consolePrintlnString( "   Bienvenido a la Estacion Meterologica \r\n " );
	consolePrintlnString( ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\r\n");
	consolePrintlnString(  "¿Que sensor desea Inicializar?-Por defecto todos se encuentran apagados- \r\n" );
	consolePrintlnString( " 't' = Temperatura \r \n" );
	consolePrintlnString( " 'h' = Humedad \r \n" );
	consolePrintlnString( " 'v' = Viento  \r\n" );
	consolePrintlnString( " 's' = Salir  \r\n" );
	consolePrintlnString( " 'e' = Estado  \r\n" );
}
/************* FUNCION HABILITACION **************/

void habilitacion_sensores_estadotemperatura(void)
{
	if(bandera_temperatura == OFF)
	{
		consolePrintlnString( "Sensor de Temperatura inicializado " );
		delay(1000);
		bandera_temperatura=ON;
		BorrarPantalla();
		Menu();

	}
	else
	{
		consolePrintlnString( "Sensor de Temperatura apagado " );
		delay(1000);
		bandera_temperatura=OFF;
		BorrarPantalla();
		Menu();
	}
}


void Habilitacion_Sensores(void)
{
	uint8_t habilitacion;

	Menu();

	while(habilitacion!='s')
	{

		if(uartReadByte (UART_USB, &habilitacion))
		{
			switch(habilitacion)
			{
			case 't': habilitacion_sensores_estadotemperatura(); break;

			case 'h':
				if(bandera_humedad == OFF)
				{
					consolePrintlnString( "Sensor de Humedad inicializado " );
					delay(1000);
					bandera_humedad=ON;
					BorrarPantalla();
					Menu();
				}
				else
				{
					consolePrintlnString( "Sensor de Humedad apagado " );
					bandera_humedad=OFF;
					delay(1000);
					BorrarPantalla();
					Menu();
				}
				break;

			case 'v':
				if(bandera_viento == OFF)
				{
					consolePrintlnString( "Sensor de Viento inicializado  " );
					bandera_viento=ON;
					delay(1000);
					BorrarPantalla();
					Menu();
				}
				else
				{
					consolePrintlnString( "Sensor de Viento apagado  " );
					bandera_viento=OFF;
					delay(1000);
					BorrarPantalla();
					Menu();
				}
				break;

			case 'e':
				BorrarPantalla();
				consolePrintlnString( "Estado de los Sensores: " );
				consolePrintlnString( ">>>>>>>>>>>>>>>>>>>>>>>>> \n\n" );
				consolePrintlnString( "Sensor de temperatura: " );
				consolePrintInt(bandera_temperatura);
				consolePrintlnString( "\n" );
				consolePrintlnString( "Sensor de humedad: " );
				consolePrintInt(bandera_humedad);
				consolePrintlnString( "\n" );
				consolePrintlnString( "Sensor de viento: " );
				consolePrintInt(bandera_viento);
				consolePrintlnString( "\n" );
				delay(4000);
				Menu();
				break;

			case 's':
				consolePrintlnString( "Salir  " );
				delay(1000);
				BorrarPantalla();
				break;
			default:
				consolePrintlnString( "Caracter Ingresado Invalido ");
				delay(1000);
				BorrarPantalla();
				Menu();

			}

		}
	}
}
/************* FUNCION SENSORES**************/

void Sensores(void)
{
	uint16_t sensorTempValue, sensorHumValue, sensorWindValue = 0;
	char bufferDataLog[128];

	apiReadSensor(&sensorTempValue, &sensorHumValue, &sensorWindValue);

	if(bandera_temperatura==OFF)
		sensorTempValue=0;
	if(bandera_humedad==OFF)
		sensorHumValue=0;
	if(bandera_viento==OFF)
		sensorWindValue=0;

	apiProcessInformation(sensorTempValue,
			sensorHumValue,
			sensorWindValue,
			bufferDataLog);

	apiWriteSD(_SYS_CFG_DATALOG_FILENAME, bufferDataLog);
}

/************* APLICACION **************/
int main( void ){

	boardConfig();
	uartConfig(UART_USB, 115200);
	adcConfig(ADC_ENABLE);
	spiConfig(SPI0);
	tickConfig(10, diskTickHook);

	Habilitacion_Sensores();


	while(TRUE) {

		delay(_SYS_CFG_SAMPLINGTIME);
		Sensores();

	}

	return 0;
}



