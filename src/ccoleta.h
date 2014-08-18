#ifndef CCOLETA_H
#define CCOLETA_H
#include "cacic_computer.h"

class CColeta
{
private:
    CACIC_Computer oComputer;
public:
    CColeta();
    CACIC_Computer getOComputer() const;
};

#endif // CCOLETA_H
