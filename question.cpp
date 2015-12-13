#include"question.h"

Question::Question(qint64 start_time, qint64 end_time, const QString &words){
    this->start_time = start_time;
    this->end_time = end_time;
    this->words = words;
}
