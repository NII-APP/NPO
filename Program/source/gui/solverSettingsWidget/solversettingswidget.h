#ifndef SOLVERSETTINGSWIDGET_H
#define SOLVERSETTINGSWIDGET_H

/*
Для выполнения расчета на собственные частоты (СЧ) заготовки необходимо определить 3 группы исходных данных и настроек:
2) Параметры расчета  (требуется меню GUI):
2.1) Количество СЧ, которые необходимо определить (int)
2.2) Параметр сходимости при расчете СЧ (float). Обычно выбирается равным 1.e-5.
Является критерием окончания итерационного процесса поиска СЧ. Если максимальное относительное изменение квадрата каждой из определяемых СЧ за предыдущую итерацию меньше заданного параметра сходимости – процесс расчета прекращается. Таким образом, параметр сходимости определяет численную точность поиска собственных частот.
2.3) Настройки количества выполнения расчетов в течение маршрута обработки детали (требует согласования)
3) Общие настройки (требуется меню GUI):
3.1) Число ядер процессора (int)
3.2) Флаг использования видеокарты (int = 0/1)
3.3) Объем оперативной памяти, который разрешено использовать во время расчета, [Мб] (float)
3.4) Количество временных директорий для хранения вспомогательных данных (int). Во вспомогательных директориях хранятся матрицы жесткости суперэлементов, и в процессе расчета происходит их многократное считывание. Для ускорения расчета целесообразно создавать по 1 временной папке на 1 физическом жестком диске компьютера, поскольку в этом случае считывание информации может осуществляться параллельно.
3.5) Пути к временным папкам (**char) в соответствии с их количеством  из п. 3.4

        static const QString exeInnerFileName;
        static const QDir exeInnerFileLocation;
        QString exeFileName;
        QDir exeFileLocation;
        QList<QDir> outDir;
        bool sanitize;
        bool reextractExe;
        QDir solverWorkingDir;
        int modesCount;

        //
        int ncores;
        int npt_matr;
        int npt_vect;
        float RAMpermitted;
        float RAMmatblock;
        //
        int SE0elnum;
        int SEtotalnodes;
        int SEboundarynodes;
        //
        double eps;
        int nblock;

        bool printModelAnsFormat;
        bool printModelElEnergy;
        bool printModelEigForms;
        bool printModelElElast;
*/

#include <QWidget>
#include "kernel/igofesolver.h"

class SolverSettingsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SolverSettingsWidget(QWidget *parent = 0);

private:
    IgoFESolver::SolverOptions value;
};

#endif // SOLVERSETTINGSWIDGET_H
