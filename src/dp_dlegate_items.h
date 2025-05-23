#ifndef DP_DLEGATE_ITEMS
#define DP_DLEGATE_ITEMS

#include <QItemDelegate>
#include <QStyledItemDelegate>
#include <QComboBox>
#include <QApplication>
#include <QCheckBox>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QMessageBox>

class NotEditableDelegate : public QItemDelegate
{
    Q_OBJECT

public:
    explicit NotEditableDelegate(QObject *parent = 0)
        :QItemDelegate(parent)
    {}

protected:
    bool editorEvent(QEvent *, QAbstractItemModel *, const QStyleOptionViewItem &, const QModelIndex &)
    { return false; }
    QWidget* createEditor(QWidget *, const QStyleOptionViewItem &, const QModelIndex &) const
    { return Q_NULLPTR; }
};

class DoubleDelegateItem : public QItemDelegate
{
    Q_OBJECT

public:
    explicit DoubleDelegateItem(double minVal=0.0, double maxVal=99.0)
    {
        MinVal = minVal; MaxVal = maxVal;
    }
    double MinVal;
    double MaxVal;

protected:
    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &/*index*/) const
    {
        QDoubleSpinBox *editor = new QDoubleSpinBox(parent);
        editor->setMinimum(0.0);
        editor->setMaximum(+1000.0);
        editor->setDecimals(3);
        editor->setGeometry(option.rect);
        editor->setSingleStep(5.625);
        return editor;
    }
    void setEditorData(QWidget * editor, const QModelIndex & index) const
    {
        ((QDoubleSpinBox*)editor)->setValue(index.model()->data(index, Qt::EditRole).toDouble());
        ((QDoubleSpinBox*)editor)->setMinimum(MinVal);//setting maximum nd minimum size for spinbox
        ((QDoubleSpinBox*)editor)->setMaximum(MaxVal);
    }
    void setModelData(QWidget * editor, QAbstractItemModel * model, const QModelIndex & index) const
    {
        if(((QDoubleSpinBox*)editor)->value() < MinVal ||\
                ((QDoubleSpinBox*)editor)->value() > MaxVal)
        {
            QMessageBox::warning(NULL,"Error","Incorrect Value");
            (((QDoubleSpinBox*)editor)->value() < MinVal)?\
                        model->setData(index, MinVal):model->setData(index, MaxVal);
        }
        else
        {            
            int iData = 0;

            iData = ((double)((QDoubleSpinBox*)editor)->value()) * 1000;

            if(iData % 5625 != 0)
            {
                ((QDoubleSpinBox*)editor)->setValue((iData - ( iData % 5625 )) /1000.000);
            }
            model->setData(index,((QDoubleSpinBox*)editor)->value());
        }
    }
};

class SpinBoxDelegateItem : public DoubleDelegateItem
{
    Q_OBJECT

public:
    explicit SpinBoxDelegateItem(double minVal=5.625, double maxVal=354.375):DoubleDelegateItem(minVal,maxVal){}
};

class HexaByteDelegateItem : public QItemDelegate
{
    Q_OBJECT

public:
    explicit HexaByteDelegateItem(QObject *parent = 0):QItemDelegate(parent) {}

protected:
    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &/*index*/) const
    {
        QLineEdit *editor = new QLineEdit(parent);
        editor->setAlignment(Qt::AlignHCenter);
        editor->setInputMask("Hh");
        editor->setGeometry(option.rect);
        return editor;
    }
};

class CheckBoxDelegate : public QStyledItemDelegate {
public:
    explicit CheckBoxDelegate(QObject* parent = NULL) : QStyledItemDelegate(parent) {}

    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const override {
        Q_UNUSED(option);
        if (index.column() == 1) {
            QCheckBox* checkBox = new QCheckBox(parent);
//            checkBox->setStyleSheet("background-color: rgb(25, 25, 25);"); // Add some margin for better appearance
            return checkBox;
        }

        return QStyledItemDelegate::createEditor(parent, option, index);
    }

    void setEditorData(QWidget* editor, const QModelIndex& index) const override {
        if (QCheckBox* checkBox = qobject_cast<QCheckBox*>(editor)) {
            bool checked = index.model()->data(index, Qt::DisplayRole).toBool();
            checkBox->setChecked(!checked);
        } else {
            QStyledItemDelegate::setEditorData(editor, index);
        }
    }

    void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const override {
        if (QCheckBox* checkBox = qobject_cast<QCheckBox*>(editor)) {
            model->setData(index, checkBox->isChecked(), Qt::EditRole);
        } else {
            QStyledItemDelegate::setModelData(editor, model, index);
        }
    }

    void updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const override {
        Q_UNUSED(index);
        editor->setGeometry(option.rect);
    }

    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override {
        if (index.column() == 1) {
            QStyleOptionButton buttonOption;
            buttonOption.rect = option.rect;
            buttonOption.state = QStyle::State_Enabled | (index.data().toBool() ? QStyle::State_On : QStyle::State_Off);

            QApplication::style()->drawControl(QStyle::CE_CheckBox, &buttonOption, painter);
        } else {
            QStyledItemDelegate::paint(painter, option, index);
        }
    }

    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override {
        if (index.column() == 1) {
            return QSize(option.rect.height(), option.rect.height());
        }

        return QStyledItemDelegate::sizeHint(option, index);
    }
};

class ComboBoxDelegateItem : public QItemDelegate
{
    Q_OBJECT

public:
    explicit ComboBoxDelegateItem(QStringList itemList)
    {
        m_itemList = itemList;
    }
    QStringList m_itemList;

protected:
    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override
    {
        QComboBox *editor = new QComboBox(parent);
        editor->addItems(m_itemList);
        return editor;
    }

    void setModelData(QWidget * editor, QAbstractItemModel * model, const QModelIndex & index) const override
    {
        model->setData(index,((QComboBox*)editor)->currentText(),Qt::EditRole);
    }
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override
    {
        ((QComboBox*)editor)->setGeometry(option.rect);
    }
};

#endif // DP_DLEGATE_ITEMS

