/*

Copyright (C) 2023-2031 Friedrich-Alexander-Universität Erlangen-Nürnberg
27.10.2023
Xiyu Chen

*/

#include "postprocessingpage.h"
#include "../setmodelwindow.h"
#include <QPainter>
#include <QPainterPath>
#include <algorithm>
#include <numeric>

class MuscleLengthChart : public QWidget
{
public:
    explicit MuscleLengthChart(QWidget *parent = nullptr) : QWidget(parent) {}

    void setData(const std::vector<std::vector<double>> &lengthSeriesValue,
                 const std::vector<std::vector<double>> &diffSeriesValue,
                 const std::vector<QString> &namesValue)
    {
        lengthSeries = lengthSeriesValue;
        diffSeries = diffSeriesValue;
        names = namesValue;
        update();
    }

protected:
    void paintEvent(QPaintEvent *) override
    {
        QPainter painter(this);
        painter.fillRect(rect(), QColor("#CCCCCC"));
        painter.setRenderHint(QPainter::Antialiasing, true);

        QRect leftPlotRect(70, 55, (width() - 330) / 2, height() - 125);
        QRect rightPlotRect(leftPlotRect.right() + 85, 55, (width() - 330) / 2, height() - 125);
        drawPlot(painter, leftPlotRect, lengthSeries, "total length", "muscle length");
        drawPlot(painter, rightPlotRect, diffSeries, "length diff", "length diff");

        const QVector<QColor> colors = chartColors();
        const int legendX = rightPlotRect.right() + 25;
        for (int m = 0; m < static_cast<int>(names.size()); ++m) {
            const int legendY = 65 + (m % 22) * 22;
            const int columnX = legendX + (m / 22) * 120;
            painter.setPen(QPen(colors[m % colors.size()], 2));
            painter.drawLine(columnX, legendY - 5, columnX + 22, legendY - 5);
            painter.setPen(Qt::black);
            painter.drawText(columnX + 28, legendY, names[m]);
        }
    }

private:
    QVector<QColor> chartColors() const
    {
        return {
            QColor("#d62728"), QColor("#1f77b4"), QColor("#2ca02c"),
            QColor("#ff7f0e"), QColor("#9467bd"), QColor("#17becf"),
            QColor("#8c564b"), QColor("#e377c2")
        };
    }

    void drawPlot(QPainter &painter, const QRect &plotRect, const std::vector<std::vector<double>> &series, const QString &title, const QString &yLabel)
    {
        painter.setPen(QPen(Qt::black, 2));
        painter.drawLine(plotRect.bottomLeft(), plotRect.bottomRight());
        painter.drawLine(plotRect.bottomLeft(), plotRect.topLeft());
        painter.drawText(plotRect.left(), plotRect.top() - 20, title);
        painter.drawText(plotRect.left(), plotRect.bottom() + 35, "stepnum");
        painter.save();
        painter.translate(plotRect.left() - 45, plotRect.center().y() + 45);
        painter.rotate(-90);
        painter.drawText(0, 0, yLabel);
        painter.restore();

        bool hasData = false;
        for (const auto &line : series) {
            if (!line.empty()) {
                hasData = true;
                break;
            }
        }
        if (!hasData) {
            painter.drawText(plotRect, Qt::AlignCenter, "No postprocessing length data yet");
            return;
        }

        double minY = 0.0;
        double maxY = 0.0;
        bool initialized = false;
        int maxStep = 1;
        for (const auto &line : series) {
            if (line.empty()) {
                continue;
            }
            maxStep = std::max(maxStep, static_cast<int>(line.size()) - 1);
            for (double value : line) {
                if (!initialized) {
                    minY = value;
                    maxY = value;
                    initialized = true;
                }
                minY = std::min(minY, value);
                maxY = std::max(maxY, value);
            }
        }
        if (maxY - minY < 1.0e-12) {
            maxY += 1.0;
            minY -= 1.0;
        }

        painter.setPen(QPen(QColor("#777777"), 1));
        for (int i = 0; i <= 5; ++i) {
            const int y = plotRect.bottom() - i * plotRect.height() / 5;
            painter.drawLine(plotRect.left(), y, plotRect.right(), y);
            const double labelValue = minY + (maxY - minY) * i / 5.0;
            painter.drawText(plotRect.left() - 62, y + 5, QString::number(labelValue, 'g', 4));
        }

        const QVector<QColor> colors = chartColors();

        for (int m = 0; m < static_cast<int>(series.size()); ++m) {
            const auto &line = series[m];
            if (line.size() < 2) {
                continue;
            }
            QPainterPath path;
            for (int i = 0; i < static_cast<int>(line.size()); ++i) {
                const double xRatio = maxStep > 0 ? static_cast<double>(i) / maxStep : 0.0;
                const double yRatio = (line[i] - minY) / (maxY - minY);
                QPointF point(plotRect.left() + xRatio * plotRect.width(),
                              plotRect.bottom() - yRatio * plotRect.height());
                if (i == 0) {
                    path.moveTo(point);
                } else {
                    path.lineTo(point);
                }
            }
            QColor color = colors[m % colors.size()];
            painter.setPen(QPen(color, 2));
            painter.drawPath(path);
        }
    }

    std::vector<std::vector<double>> lengthSeries;
    std::vector<std::vector<double>> diffSeries;
    std::vector<QString> names;
};

postprocessingpage::postprocessingpage(setmodelwindow *setmodelwin, QWidget *parent):
    QWidget(parent),
    setmodelwin(setmodelwin)
{
    rectangle = new QFrame(this);
    rectangle->setGeometry(0, 100, setmodelwin->width(), setmodelwin->width()+1000);
    rectangle->setStyleSheet("background-color: #CCCCCC;");

    titleLabel = new QLabel("Muscle length", this);
    titleLabel->setGeometry(20, 115, 300, 30);
    titleLabel->setStyleSheet("QLabel { color : black; background-color : #CCCCCC; }");

    lengthChart = new MuscleLengthChart(this);
    lengthChart->setGeometry(20, 155, 1250, 620);
    updatevalue();
}

postprocessingpage::~postprocessingpage(){
    delete titleLabel;
    delete lengthChart;
    delete rectangle;
}

void postprocessingpage::updatevalue()
{
    std::vector<std::vector<double>> lengthSeries;
    std::vector<std::vector<double>> diffSeries;
    std::vector<QString> names;
    auto lengthAll = setmodelwin->getRunmodel()->getModel()->getPostprocessing()->getlengthall();
    int muscleCount = setmodelwin->getRunmodel()->getModel()->getparm()->getn_muscles();
    for (int i = 0; i < static_cast<int>(lengthAll.size()) && i < muscleCount; ++i) {
        std::vector<double> totalLength;
        for (const auto &lengthEachStep : lengthAll[i]) {
            totalLength.push_back(std::accumulate(lengthEachStep.begin(), lengthEachStep.end(), 0.0));
        }
        if (totalLength.size() > 1) {
            std::vector<double> totalLengthWithoutInitial(totalLength.begin() + 1, totalLength.end());
            std::vector<double> totalLengthDiff;
            for (int step = 2; step < static_cast<int>(totalLength.size()); ++step) {
                totalLengthDiff.push_back(totalLength[step] - totalLength[step - 1]);
            }
            lengthSeries.push_back(totalLengthWithoutInitial);
            diffSeries.push_back(totalLengthDiff);
            names.push_back(QString::fromStdString(setmodelwin->getRunmodel()->getModel()->getparm()->getmuscleindex(i)->getname()));
        }
    }
    lengthChart->setData(lengthSeries, diffSeries, names);
}
