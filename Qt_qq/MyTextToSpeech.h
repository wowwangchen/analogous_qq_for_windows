#ifndef MYTEXTTOSPEECH_H
#define MYTEXTTOSPEECH_H

#include<QTextToSpeech>
#include<QVoice>
class MyTextToSpeech : public QTextToSpeech
{
public:
    MyTextToSpeech();
    //range               -1.0~1.0          -1.0~1.0            0.0~1.0
    void initSpeech(double rate=0.0,double pitch=0.0,double volume=0.5);
    //,QString voice_s="MALE"
};

#endif // MYTEXTTOSPEECH_H
