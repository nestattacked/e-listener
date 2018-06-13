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
#include<QLabel>

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
    void last();
    void answer();
    void checkAnswer();
    void changeDiff();
    void changeOrder();
private:
    void makeBlank(QString &hint);
    void getQuestion(bool forward);
    qint64 strToInt(const QString &str);
    QPlainTextEdit *text;
    QLineEdit *input_box;
    QVideoWidget *video_widget;
    QPushButton *play_button;
    QPushButton *next_button;
    QPushButton *last_button;
    QPushButton *answer_button;
    QPushButton *open_button;
    QPushButton *diff_button;
    QPushButton *order_button;
    QMediaPlayer *player;
    QVector<Question> *questions;
    QTimer *timer;
    QLabel *message,*correct_mes,*incorrect_mes,*percent_mes,*done_mes;
    int present_question_index;
    int hits,correct,incorrect,difficulty,order;
    int front_delay,back_delay;
    int answered;
};

#endif // ELISTENER_H
