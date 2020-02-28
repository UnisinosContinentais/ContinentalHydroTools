#include "continental/hydrotools/domain/FileCommandOutput.h"

using namespace std;
using namespace continental::hydrotools::domain;

namespace continental
{
namespace hydrotools
{
namespace domain
{

FileCommandOutput::FileCommandOutput()
{
}

FileCommandOutput::FileCommandOutput(QString path, QString group)
{
    m_pathFile = path;
    m_groupName = group;
}

}
}
}
