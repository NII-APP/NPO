#ifndef MODELSPICKER_H
#define MODELSPICKER_H

#include <QWidget>
class FEM;
class QComboBox;

class ModelsPicker : public QWidget
{
    Q_OBJECT

public:
    ModelsPicker(QWidget* parent = 0);
    ~ModelsPicker();

    const FEM* a();
    const FEM* b();

    void setPick(const std::pair<const FEM*, const FEM*>&);

signals:
    void modelsPicked(std::pair<const FEM*, const FEM*>);

private:
    QComboBox* const first;
    QComboBox* const second;
};

#endif // MODELSPICKER_H
