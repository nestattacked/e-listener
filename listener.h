#ifndef ELISTENER_H
#define ELISTENER_H

#include<QWidget>
#include<QPushButton>
#include<QVideoWidget>
#include<QLineEdit>
#include<QPlainTextEdit>
#include<QMediaPlayer>
#include<QVector>
#include<QTimer>
#include"question.h"

class EListener : public QWidget
{
    Q_OBJECT

public:
    EListener(QWidget *parent = 0);
    ~EListener();
public slots:
    void openFile();
    void play();
    void next();
    void answer();
    void checkAnswer();
private:
    qint64 strToInt(const QString &str);
    QPlainTextEdit *text;
    QLineEdit *input_box;
    QVideoWidget *video_widget;
    QPushButton *play_button;
    QPushButton *next_button;
    QPushButton *answer_button;
    QPushButton *open_button;
    QMediaPlayer *player;
    QVector<Question> *questions;
    QTimer *timer;
    int present_question_index;
};

#endif // ELISTENER_H
