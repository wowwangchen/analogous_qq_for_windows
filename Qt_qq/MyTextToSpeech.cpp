#include "MyTextToSpeech.h"

MyTextToSpeech::MyTextToSpeech()
{

}

void MyTextToSpeech::initSpeech(double rate, double pitch, double volume)
{
    //,QString voice_s
    setRate(rate);
    setPitch(pitch);
    setVolume(volume);
    //setVoice(QVoice::Male);

//    QList<QVoice> voices = QVoice::availableVoices();

//    for (const QVoice &voice : voices) {
//        if (voice.gender() == QVoice::Male) {
//            QTextToSpeech speech;
//            speech.setVoice(voice);
//            break;
//        }
    //    }
}

