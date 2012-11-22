

// Includes
#include "datasource.h"


DataSource::DataSource(QObject *parent)
    : QObject(parent)
    , m_isUpdateAllowed(true)
{

}


void DataSource::setUpdateAllowed(bool allow)
{
    if (allow != m_isUpdateAllowed) {
        m_isUpdateAllowed = allow;
        emit updateAllowedChanged();
        onUpdateAllowedChanged();
    }
}
