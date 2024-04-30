﻿#pragma execution_character_set("utf-8")
#include "utils.h"
#include <QStyle>
#include <QLabel>
#include <QMouseEvent>
#include <QApplication>
#include <QPropertyAnimation>
#include <QGraphicsDropShadowEffect>
#include <QDebug>

#include <QMimeData>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QKeyEvent>
#include <QJsonObject>
#include <QJsonArray>
AnimatedCheckBox::AnimatedCheckBox(bool isChecked, QWidget *parent) : QCheckBox (parent)
{
    indicator = new QLabel(this);
    /* 设置样式 */
    this->setMinimumHeight(22);
    this->setAttribute(Qt::WA_StyledBackground, true);
    this->setProperty("class", "AnimatedCheckBox");
    indicator->setProperty("class", "AnimatedCheckBoxIndicator");

    // this->setStyleSheet(QString(".AnimatedCheckBox[checked=true ]\
    // {\
    //     background: #1e2d36;\
    //     border-radius: %1px;\
    // }\
    // .AnimatedCheckBox[checked=false ]\
    // {\
    //      background: #c5c8ce;\
    //      border-radius: %1px;\
    // }\
    // .AnimatedCheckBoxIndicator\
    // {\
    //     background: white;\
    //     border-radius: %2px;\
    // }").arg(this->height() / 2).arg(indicator->height() / 2));
    this->setContentsMargins(3, 2, 3, 2);
//    QPropertyAnimation *animation = new QPropertyAnimation(indicator, "pos", this);
//    int b = this->contentsMargins().left();
//    int x = this->isChecked() ? this->width() - indicator->width() - b : b;
//    int y = b;
//    animation->stop();
//    animation->setDuration(200);
//    animation->setEndValue(QPoint(x + 22, y));
//    animation->setEasingCurve(QEasingCurve::InOutCubic);
//    animation->start();

//    this->style()->polish(this);


    QPropertyAnimation *animation = new QPropertyAnimation(indicator, "pos", this);
    connect(this, &QCheckBox::toggled, [=] {
        int b = this->contentsMargins().left();
        int x = this->isChecked() ? this->width() - indicator->width() - b : b;
        int y = b;
        animation->stop();
        animation->setDuration(200);
        animation->setEndValue(QPoint(x, y));
        animation->setEasingCurve(QEasingCurve::InOutCubic);
        animation->start();

        this->style()->polish(this);
    });
    if (isChecked) {
        this->setChecked(isChecked);
            //QPropertyAnimation *animation = new QPropertyAnimation(indicator, "pos", this);
            int b = this->contentsMargins().left();
            int x = this->isChecked() ? this->width() - indicator->width() - b : b;
            int y = b;
            animation->stop();
            animation->setDuration(10);
            animation->setEndValue(QPoint(x + 28, y));
            animation->setEasingCurve(QEasingCurve::InOutCubic);
            animation->start();

            this->style()->polish(this);
    }

}

/* 重写 paintEvent 方法，清除 QCheckBox 的默认样式 */
void AnimatedCheckBox::paintEvent(QPaintEvent *) {
}

/* AnimatedCheckBox 的大小改变时调整 indicator 的位置 */
void AnimatedCheckBox::resizeEvent(QResizeEvent *)
{
    /* 设置 AnimatedCheckBox 的最小宽度，避免太窄的时候效果不好 */
    this->setMinimumWidth(height() * 2);

    this->setContentsMargins(3, 2, 3, 2);
    int b = this->contentsMargins().left();
    int x = this->isChecked() ? this->width() - indicator->width() - b : b;
    int y = b;
    int w = height() - b - b;
    int h = w;
    qDebug() << "x = " << x;

    indicator->setGeometry(x,y,w,h);

    this->setStyleSheet(QString(".AnimatedCheckBox[checked=true ]\
    {\
        background: #1e2d36;\
        border-radius: %1px;\
    }\
    .AnimatedCheckBox[checked=false ]\
    {\
         background: #c5c8ce;\
         border-radius: %1px;\
    }\
    .AnimatedCheckBoxIndicator\
    {\
        background: white;\
        border-radius: %2px;\
    }").arg(this->height() / 2).arg(indicator->height() / 2));
    QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect(this);
    effect->setBlurRadius(10);
    effect->setOffset(0, 1);
    indicator->setGraphicsEffect(effect);
}

/* 点击 AnimatedCheckBox 上的任何地方都切换选中状态，QCheckBox 默认只有点击它的 indicator 或者文字时才进行切换 */
void AnimatedCheckBox::mousePressEvent(QMouseEvent *event)
{
    //event->accept();
    setChecked(!isChecked());
}

utils::utils(QWidget *parent) : QWidget(parent)
{

}

QString getStyleFile(QString path)
{
    qDebug() << "getStyleFile";
    QFile file(path);
    QString styleSheet;
    /* 判断文件是否存在 */
    if (file.exists() ) {
        /* 以只读的方式打开 */
        file.open(QFile::ReadOnly);
        /* 以字符串的方式保存读出的结果 */
        styleSheet = QLatin1String(file.readAll());
        /* 关闭文件 */
        file.close();
    }
    qDebug() << styleSheet;
    return styleSheet;
}


parsingJsonTextEdit::parsingJsonTextEdit(QWidget *parent)
    : QTextEdit(parent)
{
//    setReadOnly(true);
    // QFont font;
    // font.setPixelSize(28);
    // this->setFont(font);
}
 
void parsingJsonTextEdit::parsingJsonInfo(QString & jsonString)
{
    clear();
    QJsonParseError err;
 
    QJsonDocument jsonDocument = QJsonDocument::fromJson(jsonString.toUtf8(),&err);
    if(jsonDocument.isNull())
    {
        append(err.errorString());
        return;
    }
 
    if(jsonDocument.isObject())
    {
        QJsonObject jsonObject = jsonDocument.object();
        parsingJsonObject(jsonObject,1);
    }
    else if(jsonDocument.isArray())
    {
        QJsonArray array = jsonDocument.array();
        parsingJsonArray(array,1);
    }
}
 
void parsingJsonTextEdit::insertFromMimeData(const QMimeData *source)
{
    if (source->hasText())
    {
        QString str = source->text().trimmed();
        parsingJsonInfo(str);
    }
    else
    {
        QTextEdit::insertFromMimeData(source);
    }
}
 
void parsingJsonTextEdit::parsingJsonObject(QJsonObject &object,const int numberOfLayers)//参数2：层数
{
    int nextLayers = numberOfLayers + 1;
 
    QString spacing;
    for (int i = 0; i < numberOfLayers; ++i)
    {
        spacing.append("    ");
    }
    QString spacingBeforeBrackets;
    int spacingBeforeBracketsCount = numberOfLayers - 1;
    for (int i = 0; i < spacingBeforeBracketsCount; ++i)
    {
        spacingBeforeBrackets.append("    ");
    }
    spacingBeforeBrackets.append("{");
 
    append(spacingBeforeBrackets);
    int index = 1;
    QString endingSymbol;
    int objectLent = object.length();
    for (QJsonObject::const_iterator it = object.constBegin(); it != object.constEnd(); ++it)
    {
        if(index < objectLent)
            endingSymbol = QStringLiteral(",");
        else
            endingSymbol = QStringLiteral("");
 
        const QJsonValue & value = it.value();
        if(value.isString())
        {
            QString str = value.toString();
            QJsonDocument jsonDocument = QJsonDocument::fromJson(str.toUtf8());
            if(jsonDocument.isNull())
            {
                append(QString("<span style='white-space:pre;color:#9C278F;'>%1\"%2\"</span>:<span style='color:#54B54A;'>\"%3\"</span>%4").arg(spacing).arg(it.key()).arg(str).arg(endingSymbol));
            }
            else if(jsonDocument.isObject())
            {
                append(QString("<span style='white-space:pre;color:#9C278F;'>%1\"%2\"</span>:").arg(spacing).arg(it.key()));
                QJsonObject obj = jsonDocument.object();
                parsingJsonObject(obj,nextLayers);
            }
            else if(jsonDocument.isArray())
            {
                append(QString("<span style='white-space:pre;color:#9C278F;'>%1\"%2\"</span>:").arg(spacing).arg(it.key()));
                QJsonArray arr = jsonDocument.array();
                parsingJsonArray(arr,nextLayers);
            }
        }
        else if(value.isDouble())
        {
            append(QString("<span style='white-space:pre;color:#9C278F;'>%1\"%2\"</span>:<span style='color:#128BF1;'>%3</span>%4").arg(spacing).arg(it.key()).arg(value.toDouble()).arg(endingSymbol));
        }
        else if(value.isBool())
        {
            if(value.toBool())
            {
                append(QString("<span style='white-space:pre;color:#9C278F;'>%1\"%2\"</span>:true%3").arg(spacing).arg(it.key()).arg(endingSymbol));
            }
            else
            {
                append(QString("<span style='white-space:pre;color:#9C278F;'>%1\"%2\"</span>:false%3").arg(spacing).arg(it.key()).arg(endingSymbol));
            }
        }
        else if(value.isObject())
        {
            append(QString("<span style='white-space:pre;color:#9C278F;'>%1\"%2\"</span>:").arg(spacing).arg(it.key()));
            QJsonObject obj = value.toObject();
            parsingJsonObject(obj,nextLayers);
        }
        else if(value.isArray())
        {
            append(QString("<span style='white-space:pre;color:#9C278F;'>%1\"%2\"</span>:").arg(spacing).arg(it.key()));
            QJsonArray arr = value.toArray();
            parsingJsonArray(arr,nextLayers);
        }
        else
        {
            append(QString("<span style='white-space:pre;color:#9C278F;'>%1\"%2\"</span>:<span style='color:#128BF1;'>%3</span>%4").arg(spacing).arg(it.key()).arg(value.toInt()).arg(endingSymbol));
        }
        ++index;
    }
    spacingBeforeBrackets.replace("{","}");
    append(spacingBeforeBrackets);
}
 
void parsingJsonTextEdit::parsingJsonArray(QJsonArray &array,const int numberOfLayers)
{
    int nextLayers = numberOfLayers + 1;
 
    QString spacing;
    for (int i = 0; i < numberOfLayers; ++i)
    {
        spacing.append("    ");
    }
    QString spacingBeforeBrackets;
    int spacingBeforeBracketsCount = numberOfLayers - 1;
    for (int i = 0; i < spacingBeforeBracketsCount; ++i)
    {
        spacingBeforeBrackets.append("    ");
    }
    spacingBeforeBrackets.append("[");
 
    append(spacingBeforeBrackets);
 
    int index = 1;
    QString endingSymbol;
    int arrayLent = array.size();
 
    for(int i = 0;i < array.size();++i)
    {
        if(index < arrayLent)
            endingSymbol = QStringLiteral(",");
        else
            endingSymbol = QStringLiteral("");
 
        QJsonValueRef ref = array[i];
        QJsonValue::Type jsonValueType = ref.type();
        if(jsonValueType == QJsonValue::String)
        {
            QString str = ref.toString();
            QJsonDocument jsonDocument = QJsonDocument::fromJson(str.toUtf8());
            if(jsonDocument.isNull())
            {
                append(QString("<span style='white-space:pre;color:#54B54A;'>%1\"%2\"</span>%3").arg(spacing).arg(str).arg(endingSymbol));
            }
            else if(jsonDocument.isObject())
            {
                QJsonObject obj = jsonDocument.object();
                parsingJsonObject(obj,nextLayers);
            }
            else if(jsonDocument.isArray())
            {
                QJsonArray arr = jsonDocument.array();
                parsingJsonArray(arr,nextLayers);
            }
        }
        else if(jsonValueType == QJsonValue::Bool)
        {
            if(ref.toBool())
            {
                append(QString("%1true%2").arg(spacing).arg(endingSymbol));
            }
            else
            {
                append(QString("%1false%2").arg(spacing).arg(endingSymbol));
            }
        }
        else if(jsonValueType == QJsonValue::Double)
        {
            append(QString("<span style='white-space:pre;color:#128BF1;'>%1%2</span>%3").arg(spacing).arg(ref.toDouble()).arg(endingSymbol));
        }
        else if(jsonValueType == QJsonValue::Array)
        {
            QJsonArray arr = ref.toArray();
            parsingJsonArray(arr,nextLayers);
        }
        else if(jsonValueType == QJsonValue::Object)
        {
            QJsonObject obj = ref.toObject();
            parsingJsonObject(obj,nextLayers);
        }
        else
        {
            append(QString("<span style='white-space:pre;color:#128BF1;'>%1%2</span>%3").arg(spacing).arg(ref.toInt()).arg(endingSymbol));
        }
    }
    spacingBeforeBrackets.replace("[","]");
    append(spacingBeforeBrackets);
}
 
void parsingJsonTextEdit::keyPressEvent(QKeyEvent * e)
{
    int keyNumber = e->key();
    if(keyNumber != Qt::Key_Delete && keyNumber != Qt::Key_Backspace)
    {
        return;
    }
    QTextEdit::keyPressEvent(e);
}

void getGraphicsEffectUtils(QWidget * widget, int x, int y, int radius)
{
    //根据widget自身背景颜色获取color
    QPalette palette = widget->palette();
    QColor backgroundColor = palette.color(QPalette::Background);
    QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect();
    effect->setOffset(x, y);            //设置向哪个方向产生阴影效果(dx,dy)，(0,0)代表向四周发散
    effect->setColor(backgroundColor);            //设置阴影颜色
    effect->setBlurRadius(radius);      //设定阴影的模糊半径，数值越大越模糊
    widget->setGraphicsEffect(effect);
}

void getGraphicsEffectUtils(QWidget * widget, int x, int y, int radius, QColor &color)
{
    QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect();
    effect->setOffset(x, y);            //设置向哪个方向产生阴影效果(dx,dy)，(0,0)代表向四周发散
    effect->setColor(color);            //设置阴影颜色
    effect->setBlurRadius(radius);      //设定阴影的模糊半径，数值越大越模糊
    widget->setGraphicsEffect(effect);
}
