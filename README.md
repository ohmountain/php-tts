# TTS
> 基于科大讯飞文字转语音的PHP扩展

## 平台支持
1. 目前只支持linux64

## 安装
```shell
git clone https://github.com/ohmountain/php-tts.git
cd php-tts/
sudo sh ./auto_conf.sh

sudo make
sudo make install
```

## 常量
1. TTS\_RN\_AUTO_VALUE    0  /* 自动，不确定时按照值发音*/
2. TTS\_RN\_VALUE         1  /* 按照值发音 */
3. TTS\_RN\_DIGIT         2  /* 按照串发音 */
4. TTS\_RN\_AUTO\_DIGIT   3  /* 自动，不确定时按照串发音 */

## 使用
```php
$tts = new \TTS($appid);

/* 设置要转换的文字 */
$tts->setTxt('啊，今天天气真好啊，我们出去玩吧!');

/* 设置要要保存的位置，注意：如果保存文件的目录不存在，转换将会失败(扩展不会主动帮你创建目录) */
$tts->setDest('go-play.wav');

/* 设置语音(可选)，默认为‘xiaoyan’，可用语音可以参考科大讯飞语音种类库 */
$tts->setVoice('xiaolong');

/* 设置语速(可选),默认语速为50 */
$tts->setSpeed(60);

/* 设置音高，默认音高为50 */
$tts->setPitch(60);

/* 设置音量，默认音量万50 */
$tts->setVolume(60);

/* 设置语音合成方式，可选值为上面给的常量，默认值为TTS\_RN\_DIGIT */
$tts->setRdn(TTS::TTS_RN_VALUE);

/* 开始转换,注意：这是非常耗时和网络带宽的，请异步操作 */
$tts->run();
```

## 坑
1. 这个扩展是基于科大讯飞“文字转语音”的Linux SDK开发的，包含了一个与应用(appid)对应的动态连接库(libmsc.so)，如果使用不同的appid，请在编译前下载appid对应的SKD并把`libmsc.so`替换掉`tts_libs/x64/libmsc.so.bk`。
2. 因为动态链接库对应appid的唯一性，所以理论上一台主机只能部署一个tts应用。
