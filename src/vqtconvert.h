#if defined(Q_OS_WIN)

#ifndef VQTCONVERT_H
#define VQTCONVERT_H

/*
Converts between QT types and Windows Types
Version: 1.0
Modified on: 8-20-2009
Created with: QT 4.5 and QT Creator 1.2
Tested on: Windows XP SP3
Bryan Cairns - August 2009
*/

#include <QString>
#include "windows.h"

namespace voidrealms
{
    namespace win32
    {

        class VQTConvert
        {
        public:
            //Convert a QString To LPCTSTR
            LPCTSTR static QString_To_LPCTSTR(QString mQTData);

            //Convert a QString To LPCSTR
            LPCSTR static QString_To_LPCSTR(QString mQTData);

            //Convert a QString To LPTSTR
            LPTSTR static QString_To_LPTSTR(QString mQTData);

            //Convert a LPCTSTR To QString
            QString static LPCTSTR_To_QString(LPCTSTR mWinData);

            //Convert a LPBYTE To QString
            QString static LPBYTE_To_QString(LPBYTE mWinData);

            //Convert a Char[] To QString
            QString static Char_To_QString(char mWinData[]);

            //Convert a WCHAR* to a QString
            QString static WCHAR_to_QString(WCHAR* mBuffer);

            //Convert a TCHAR To QString
            QString static TChar_To_QString(TCHAR mWinData[]);
        };

    } //end voidrealms::win32 namespace
} //end voidrealms namespace
#endif // VQTCONVERT_H
#endif
