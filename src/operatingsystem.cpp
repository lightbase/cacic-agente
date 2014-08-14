#include "operatingsystem.h"

/*pegarOS
 * @return: int;
 *      retorna um id referente a versão do SO.
 * 48 = Windows XP
 * 128 = Windows Vista
 * 144 = Windows 7
 * 160 = Windows 8
 * 176 = Windows 8.1
 * 200 = Linux
 */
int OperatingSystem::pegarOS(){
#if defined (Q_OS_WIN)
    return QSysInfo::WindowsVersion;
#elif defined (Q_OS_LINUX)
    return LINUX_ARCH;
#else
    return 0;
#endif
}

int OperatingSystem::getIdSo() const
{
    return idSo;
}

void OperatingSystem::setIdSo(int value)
{
    idSo = value;
}

OperatingSystem::OperatingSystem()
{
}
