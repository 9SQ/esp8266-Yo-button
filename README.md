#esp8266-Yo-button

押すと[Yo](www.justyo.co)を送る**物理**ボタン

## Description

プッシュスイッチを押すと、設定したusername宛にYoが送信される装置です。

ESP-WROOM-02で動作確認済み。

Maybe it will work on other ESP* board.

### 構成例

![sample](http://4.bp.blogspot.com/-PztNQoC86X8/Vn_eQN7xKvI/AAAAAAAATZA/JLmkJJSw5MM/s400/yo_button1.jpg)

詳細は[ブログの記事](http://eleclog.quitsq.com/2015/12/yo-button.html)を参照してください。

## How to use

以下の通り接続

* GPIO 5 - LED
* GPIO 12 - Push Switch

以下の4か所を適宜変更

```C
const char* ssid = "**********"; // Wi-FiアクセスポイントのSSID
const char* password = "**********"; // パスワード
const char* api_token = "**********"; // YoのAPIキー
const char* username = "**********"; // Yoを送りたい相手のusername
```

## Requirements

* [Arduino core for ESP8266 WiFi chip](https://github.com/esp8266/Arduino)
* [Arduino JSON library](https://github.com/bblanchon/ArduinoJson)