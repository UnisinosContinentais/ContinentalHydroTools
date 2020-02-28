#include "continental/hydrotools/domain/FileCommandInput.h"

using namespace std;
using namespace continental::hydrotools::domain;

namespace continental
{
namespace hydrotools
{
namespace domain
{

FileCommandInput::FileCommandInput()
{
}

FileCommandInput::FileCommandInput(QString path, QString group)
{
    m_pathFile = path;
    m_groupName = group;
}

}
}
}
