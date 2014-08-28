/*
Wrapper for the Windows Registry
Version: 1.0
Modified on: 8-20-2009
Created with: QT 4.5 and QT Creator 1.2
Tested on: Windows XP SP3
Bryan Cairns
*/
#include <QObject>
#if defined(Q_OS_WIN)
#ifndef VREGISTRY_H
#define VREGISTRY_H

#include "windows.h"
#include <QString>
#include <QByteArray>
#include <QStringList>

namespace voidrealms
{
    namespace win32
    {

        class VRegistry
        {
        private:
            HKEY mKey;  //The current opened key
            bool mOpen; //Boolean indicates if the registry is open

            //////////////////////////////////////////////////////////////////////////////////////
            // Windows API Methods
            //////////////////////////////////////////////////////////////////////////////////////

            //Opens a Registry Key
            bool OpenKey(HKEY RootKey,LPCTSTR SubKey);

            //Creates a registry key and opens it
            bool CreateKey(HKEY RootKey,LPCTSTR SubKey);

            //Deletes a registry key and all values
            bool DeleteKey(HKEY RootKey,LPCTSTR SubKey);

            //Determines if the key exists
            bool KeyExists(HKEY RootKey,LPCTSTR SubKey);

            //Deletes a registry value and all values
            bool DeleteValue(LPCTSTR ValueName);

            //Determines if a value exists
            bool ValueExists(LPCTSTR ValueName);

            //Gets a Pointer to a Byte array filled with a reg type value from an open key (be sure to delete[] mVar)
            BYTE* QueryValue(LPCTSTR ValueName, DWORD ValueType);

            //Sets a value in the regsitry
            bool SetValue(LPCTSTR ValueName, DWORD ValueType, BYTE* Value, DWORD dwSize);

        public:

            //////////////////////////////////////////////////////////////////////////////////////
            // QT friendly Methods
            //////////////////////////////////////////////////////////////////////////////////////

            //Returns a boolean indicating if the Registry is open
            bool isOpen();

            //Closes the Registry Key
            bool CloseKey();

            //Opens a Registry Key
            bool OpenKey(HKEY RootKey,QString SubKey);

            //Creates a registry key and opens it
            bool CreateKey(HKEY RootKey,QString SubKey);

            //Deletes a registry key and all values
            bool DeleteKey(HKEY RootKey,QString SubKey);

            //Determines if the key exists
            bool KeyExists(HKEY RootKey,QString SubKey);

            //Deletes a registry value and all values
            bool DeleteValue(QString ValueName);

            //Determines if a value exists
            bool ValueExists(QString ValueName);

            //Gets a REG_SZ value as a QString
            QString get_REG_SZ(QString ValueName);

            //Gets a REG_EXPAND_SZ value as a QString
            QString get_REG_EXPAND_SZ(QString ValueName);

            //Gets a REG_DWORD value as a UINT
            uint get_REG_DWORD(QString ValueName);

            //Gets a REG_BINARY value as a QByteArray
            QByteArray get_REG_BINARY(QString ValueName);

            //Gets a REG_MUTLI_SZ value as a QStringList
            QStringList get_MULTI_SZ(QString ValueName);

            //Sets a REG_SZ value with a QString
            bool set_REG_SZ(QString ValueName, QString Value);

            //Sets a REG_EXPAND_SZ value with a QString
            bool set_REG_EXPAND_SZ(QString ValueName, QString Value);

            //Sets a REG_DWORD value with a UINT
            bool set_REG_DWORD(QString ValueName, UINT Value);

            //Sets a REG_BINARY value with a QByteArray
            bool set_REG_BINARY(QString ValueName, QByteArray Value);

            //Sets a REG_MULTI_SZ value with a QStringList
            bool set_REG_MULTI_SZ(QString ValueName, QStringList Value);

            //Gets a string list filled with names of the sub keys from the current open key
            QStringList enum_Keys();

            //Gets a string list filled with names of the values from the current open key
            QStringList enum_Values();

        };

    } //end voidrealms::win32 namespace
} //end voidrealms namespace
#endif // VREGISTRY_H
#endif // defined(Q_OS_WIN)
