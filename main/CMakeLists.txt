set(COMPONENT_SRCDIRS . src src/user src/bsp src/core)
set(COMPONENT_ADD_INCLUDEDIRS inc)
set(COMPONENT_EMBED_TXTFILES ../spiffs/cert/cert0.pem)
set(COMPONENT_EMBED_FILES
    ../spiffs/audio/snd0.mp3
    ../spiffs/audio/snd1.mp3
    ../spiffs/audio/snd2.mp3
    ../spiffs/audio/snd3.mp3
    ../spiffs/audio/snd4.mp3
    ../spiffs/audio/snd5.mp3
    ../spiffs/audio/snd6.mp3
    ../spiffs/audio/snd7.mp3
    ../spiffs/images/ani0_160x80.gif
    ../spiffs/images/ani1_160x80.gif
    ../spiffs/images/ani2_160x80.gif
    ../spiffs/images/ani3_160x80.gif
    ../spiffs/images/ani4_160x80.gif
    ../spiffs/images/ani5_160x80.gif
    ../spiffs/images/ani6_160x80.gif
    ../spiffs/images/ani7_160x80.gif
    ../spiffs/images/ani8_160x80.gif
    ../spiffs/images/ani0_240x135.gif
    ../spiffs/images/ani1_240x135.gif
    ../spiffs/images/ani2_240x135.gif
    ../spiffs/images/ani3_240x135.gif
    ../spiffs/images/ani4_240x135.gif
    ../spiffs/images/ani5_240x135.gif
    ../spiffs/images/ani6_240x135.gif
    ../spiffs/images/ani7_240x135.gif
    ../spiffs/images/ani8_240x135.gif
)

register_component()
