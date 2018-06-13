#include "elistener.h"
#include<QHBoxLayout>
#include<QPushButton>
#include<QPlainTextEdit>
#include<QLineEdit>
#include<QVideoWidget>
#include<QMediaPlayer>
#include<QVector>
#include<QStyle>
#include<QSizePolicy>
#include"question.h"
#include<QString>
#include<QFileDialog>
#include<QDir>
#include<QFile>
#include<QTime>
#include<QTimer>

EListener::EListener(QWidget *parent)
    : QWidget(parent){
    //srand by current time
    QTime time = QTime::currentTime();
    qsrand(time.msec()+1000*time.second());

    //initialize widget
    text = new QPlainTextEdit(this);
    input_box = new QLineEdit(this);
    video_widget = new QVideoWidget(this);
    play_button = new QPushButton(this);
    next_button = new QPushButton(this);
    last_button = new QPushButton(this);
    answer_button= new QPushButton(this);
    open_button = new QPushButton(this);
    diff_button = new QPushButton(this);
    order_button = new QPushButton(this);
    player = new QMediaPlayer(this);
    questions = new QVector<Question>;
    timer = new QTimer;
    message = new QLabel("Welcom here!",this);
    correct_mes = new QLabel(this);
    incorrect_mes = new QLabel(this);
    percent_mes = new QLabel(this);
    done_mes = new QLabel(this);

    player->setVolume(100);
    //initialize timer
    timer->setSingleShot(true);
    //set palyer output
    player->setVideoOutput(video_widget);
    //set ui size
    text->setFixedHeight(100);
    //set widget to enabled state
    text->setReadOnly(true);
    input_box->setReadOnly(true);
    play_button->setEnabled(false);
    next_button->setEnabled(false);
    last_button->setEnabled(false);
    answer_button->setEnabled(false);
    diff_button->setEnabled(false);
    order_button->setEnabled(false);
    //set button's icon
    play_button->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    open_button->setIcon(style()->standardIcon(QStyle::SP_DialogOpenButton));
    next_button->setIcon(style()->standardIcon(QStyle::SP_ArrowRight));
    last_button->setIcon(style()->standardIcon(QStyle::SP_ArrowLeft));
    answer_button->setIcon(style()->standardIcon(QStyle::SP_DialogApplyButton));
    diff_button->setIcon(style()->standardIcon(QStyle::SP_ArrowUp));
    order_button->setIcon(style()->standardIcon(QStyle::SP_BrowserReload));
    //set button's shortcut
    open_button->setShortcut(QKeySequence(Qt::Key_F1));
    last_button->setShortcut(QKeySequence(Qt::Key_F2));
    next_button->setShortcut(QKeySequence(Qt::Key_F3));
    play_button->setShortcut(QKeySequence(Qt::Key_F4));
    answer_button->setShortcut(QKeySequence(Qt::Key_F5));
    diff_button->setShortcut(QKeySequence(Qt::Key_F6));
    order_button->setShortcut(QKeySequence(Qt::Key_F7));
    //set layout
    QHBoxLayout *control_layout = new QHBoxLayout;
    control_layout->addWidget(open_button);
    control_layout->addWidget(last_button);
    control_layout->addWidget(next_button);
    control_layout->addWidget(play_button);
    control_layout->addWidget(answer_button);
    control_layout->addWidget(diff_button);
    control_layout->addWidget(order_button);
    QHBoxLayout *label_layout = new QHBoxLayout;
    label_layout->addWidget(message);
    label_layout->addWidget(correct_mes);
    label_layout->addWidget(incorrect_mes);
    label_layout->addWidget(percent_mes);
    label_layout->addWidget(done_mes);

    QVBoxLayout *main_layout = new QVBoxLayout;
    main_layout->addWidget(video_widget);
    main_layout->addLayout(label_layout);
    main_layout->addWidget(text);
    main_layout->addWidget(input_box);
    main_layout->addLayout(control_layout);
    main_layout->setStretchFactor(label_layout, 0);
    main_layout->setStretchFactor(video_widget, 1);
    setLayout(main_layout);

    //initialize connections
    connect(open_button,SIGNAL(clicked()),this,SLOT(openFile()));
    connect(play_button,SIGNAL(clicked()),this,SLOT(play()));
    connect(next_button,SIGNAL(clicked()),this,SLOT(next()));
    connect(last_button,SIGNAL(clicked()),this,SLOT(last()));
    connect(answer_button,SIGNAL(clicked()),this,SLOT(answer()));
    connect(input_box,SIGNAL(returnPressed()),this,SLOT(checkAnswer()));
    connect(diff_button,SIGNAL(clicked()),this,SLOT(changeDiff()));
    connect(order_button,SIGNAL(clicked()),this,SLOT(changeOrder()));
}

EListener::~EListener(){
    delete text;
    delete input_box;
    delete video_widget;
    delete play_button;
    delete next_button;
    delete last_button;
    delete answer_button;
    delete open_button;
    delete diff_button;
    delete player;
    delete questions;
    delete timer;
    delete message;
    delete correct_mes;
    delete incorrect_mes;
    delete percent_mes;
    delete done_mes;
}

void EListener::openFile(){
    QString file_name = QFileDialog::getOpenFileName(this, tr("Open Movie"),QDir::homePath());

    if (!file_name.isEmpty()) {
        hits=0;
        correct=0;
        incorrect=0;
        difficulty=0;
        order=1;
        present_question_index = -1;
        front_delay=0;
        back_delay=0;
        message->setText(QString("hits:")+QString::number(hits));
        correct_mes->setText(QString("correct:")+QString::number(correct));
        incorrect_mes->setText(QString("incorrect:")+QString::number(incorrect));
        percent_mes->setText(QString("percent:0"));
        done_mes->setText(QString("done:0/0"));
        questions->clear();
        player->setMedia(QUrl::fromLocalFile(file_name));
        play_button->setEnabled(true);
        next_button->setEnabled(true);
        last_button->setEnabled(true);
        answer_button->setEnabled(true);
        diff_button->setEnabled(true);
        order_button->setEnabled(true);
        input_box->setReadOnly(false);
        file_name.replace(QRegExp(".[^.]+$"), ".srt");
        QFile words_file(file_name);
        if(words_file.open(QIODevice::Text|QIODevice::ReadOnly)){
            while(!words_file.atEnd()){
                Question q;
                words_file.readLine();
                QStringList times = QString(words_file.readLine()).split(QRegExp(" --> "));
                QString start = times[0];
                QString end = times[1];end.chop(1);
                //set question's time
                q.start_time = strToInt(start);
                q.end_time = strToInt(end);
                //set question's words
                QStringList words;
                QString line = words_file.readLine();
                while(line != "\n" && !line.isEmpty()){
                    line.chop(1);
                    words.append(line);
                    line = words_file.readLine();
                }
                q.words = words.join(' ');
                questions->append(q);
            }
            words_file.close();
            getQuestion(true);
        }
    }
}

void EListener::play(){
    timer->stop();
    qint64 begin = questions->at(present_question_index).start_time;
    begin = begin-front_delay<0?0:begin-front_delay;
    player->setPosition(begin);
    qint64 duration = questions->at(present_question_index).end_time - questions->at(present_question_index).start_time + front_delay + back_delay;
    if (duration < 0) {
        duration = 0;
    }
    connect(timer,SIGNAL(timeout()),player,SLOT(pause()));
    timer->start(duration);
    player->play();
}

void EListener::next(){
    getQuestion(true);
    play();
}

void EListener::last(){
    getQuestion(false);
    play();
}

void EListener::answer(){
    answered = 1;
    text->setPlainText(questions->at(present_question_index).words);
}

void EListener::checkAnswer(){
    QString answer_word = input_box->text();
    input_box->clear();
    //check command
    if(answer_word[0]=='-'){
        //delay command
        if(answer_word[1]=='d'){
            QStringList commands = answer_word.split(" ");
            front_delay = commands[1].toInt();
            back_delay = commands[2].toInt();
        }
        return;
    }
    if(answered)
        return;
    //fill blank if it's easy mod
    if(difficulty==0){
        QString hint = text->toPlainText();
        hint.replace("___",answer_word);
        answer_word = hint;
    }
    //check answer is right or wrong
    if(answer_word == questions->at(present_question_index).words){
        hits++;
        correct++;
        answer();
    }
    else{
        hits=0;
        incorrect++;
        play();
    }
    //set message
    message->setText(QString("hits:")+QString::number(hits));
    correct_mes->setText(QString("correct:")+QString::number(correct));
    incorrect_mes->setText(QString("incorrect:")+QString::number(incorrect));
    percent_mes->setText(QString("percent:")+QString::number(((double)correct)/(incorrect+correct)));
}

void EListener::getQuestion(bool forward){
    answered = 0;
    if(order==0){
        present_question_index = qrand()%(questions->size());
    }
    else {
        present_question_index = (forward ? present_question_index + 1 : present_question_index - 1 + questions->size())%(questions->size());
    }
    if(difficulty==0){
        QString hint = questions->at(present_question_index).words;
        makeBlank(hint);
        text->setPlainText(hint);
    }
    else{
        text->clear();
    }
    done_mes->setText(QString("done:")+QString::number(present_question_index+1)+QString("/")+QString::number(questions->length()));
}

void EListener::changeDiff(){
    difficulty = (!difficulty);
    next();
}

void EListener::changeOrder(){
    order = (!order);
    next();
}

void EListener::makeBlank(QString &hint){
    //get a letter as begin
    int begin = qrand()%(hint.size());
    while(!hint[begin].isLetter()){
        begin = qrand()%(hint.size());
    }
    //present length
    int length = 1;
    //go forward
    while(begin+length<hint.size() && hint[begin+length].isLetter()){
        length++;
    }
    //go back
    while(begin!=0 && hint[begin-1].isLetter()){
        begin--;
        length++;
    }
    //replace word to blank
    hint.replace(begin,length,"___");
}

qint64 EListener::strToInt(const QString &str){
    qint64 res = 0;
    res+=(3600000*(str.mid(0,2).toInt()));
    res+=(60000*(str.mid(3,2).toInt()));
    res+=(1000*(str.mid(6,2).toInt()));
    res+=(str.mid(9,3).toInt());
    return res;
}
