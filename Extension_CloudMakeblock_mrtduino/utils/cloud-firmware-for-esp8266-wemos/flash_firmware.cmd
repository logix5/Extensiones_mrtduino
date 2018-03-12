@echo off
echo.
echo =================================
echo   ESP8266/Wemos Firmware Flaser 
echo.
echo     http://cloud.makeblock.es
echo =================================

echo.
echo.
echo Listado de puertos COM
echo ---------------------------------
call comports.bat
echo.


set /p comport= Indica el numero del puerto COM asignado al FTDI (ejemplo 3, 4, ..): 
echo ------------------------------------------------------------------------
echo.
echo Puerto COM%comport% seleccionado, grabando ...
echo.


esptool.exe  -cd nodemcu -cb 115200 -cp COM%comport% -ca 0x00000 -cf wemos_esp8266.bin
echo.
echo.
echo Listo !
echo.
pause
