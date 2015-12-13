#ifndef QUESTION_H
#define QUESTION_H

#include<QString>

class Question{
public:
    Question(qint64 start_time, qint64 end_time, const QString &words);
    Question(){};
    qint64 start_time;
    qint64 end_time;
    QString words;
};

#endif //QUESTION_H
