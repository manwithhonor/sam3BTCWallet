# sam3BTCWallet
Микропрограммное обеспечение предназначено для решения класса задач, связанных с генерацией криптовалютных ключей по протоколам BIP 32, BIP 43, BIP 44; подписью криптовалютных транзакций; представлением информации о используемых ключевых парах.
На данный момент реализована аппаратная поддержка платфоормы sam3x. В будущем будет добавлена поддержка платформы sam3u.

## Установка и подготовка к работе
Для начала  работы необходимо установить arduino-cli. Инструкция по установке на Mac/Linux/Windows: https://arduino.github.io/arduino-cli/installation/

Далее необходимо установить библиотеки [Arduino-Due-RTC-Library](https://github.com/MarkusLange/Arduino-Due-RTC-Library) и [DueFlashStorage](https://github.com/sebnil/DueFlashStorage), а также модуль для платы [arduinoDue](https://www.arduino.cc/en/Guide/ArduinoDue). Это можно сделать командами:

```
arduino-cli core install arduino:sam
arduino-cli lib install DueFlashStorage
arduino-cli lib install RTCDue
```


## Компиляция и прошивка
Подключите плату к кмпьютеру. Из корневого каталога проекта перейдите в sam3BTCWallet-master/ platform/ sam3x. Далее используйте команды:  
* Компиляция: `make`
* Прошивка микроконтроллера: `make upload`

## Использование программы  
На данный момент для демонстрации proof of concept программа поддерживает небольшой перечень тестовых функций. На вход контроллеру через serial port подается команда, цифра от 0 до 3, после чего также по этому соединению возращается ответ в соотвествии с таблицей 

Команда | Описание
--- | :--- 
0 | Генерация **seed** и его запись в постоянную память
1 | Вывод **master private key**, **master publiс key**, 20 адресов для совершения платежей и 10 для получения сдачи
2 | вывод **master private key**, **child private key**, и вывод подписи сообщения, подписанного на child private key
3 | добавление одной записи в журнал в постоянную память, и вывод журнала
