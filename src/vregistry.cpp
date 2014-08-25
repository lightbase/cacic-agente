/*
Wrapper for the Windows Registry
Version: 1.0
Modified on: 8-20-2009
Created with: QT 4.5 and QT Creator 1.2
Tested on: Windows XP SP3
Bryan Cairns
*/

#include "vregistry.h"
#include "windows.h"
#include "VQTConvert.h"
#include <QString>
#include <QByteArray>
#include <QStringList>
#include <qdebug.h>

namespace voidrealms
{
    namespace win32
    {

        //////////////////////////////////////////////////////////////////////////////////////
        // Windows API Methods
        //////////////////////////////////////////////////////////////////////////////////////

        //Returns a boolean indicating if the Registry is open
        bool VRegistry::isOpen()
        {
            return this->mOpen;
        }

        //Opens a Registry Key
        bool VRegistry::OpenKey(HKEY RootKey,LPCTSTR SubKey)
        {
            LONG lResult;

            lResult = RegOpenKeyEx(RootKey,SubKey,0,KEY_ALL_ACCESS | KEY_WOW64_64KEY,&mKey);

            switch(lResult)
            {
                case ERROR_SUCCESS: //ok
                    this->mOpen = true;
                    break;
                default: //an error code from windows
                    this->mOpen = false;
                    break;
            }

            return this->mOpen;
        }

        //Creates a registry key and opens it
        bool VRegistry::CreateKey(HKEY RootKey,LPCTSTR SubKey)
        {
            LONG lResult;
            DWORD dwDisposition;

            //Create and open the key
            lResult = RegCreateKeyEx(RootKey,SubKey,0,NULL,0,0,NULL,&mKey,&dwDisposition);

            switch(lResult)
            {
                case ERROR_SUCCESS: //ok
                    this->mOpen = true;
                    break;
                default: //an error code from windows
                    this->mOpen = false;
                    break;
            }

            return this->mOpen;
        }

        //Closes the Registry Key
        bool VRegistry::CloseKey()
        {
           //If it is not open just return
            if(this->mOpen != true) return true;

            LONG lResult;
            lResult = RegCloseKey(this->mKey);

            switch(lResult)
            {
                case ERROR_SUCCESS: //ok
                    this->mOpen = false;
                    break;
                default: //an error code from windows
                    this->mOpen = true;
                    break;
            }

            return this->mOpen;
        }

        //Deletes a registry key and all values
        bool VRegistry::DeleteKey(HKEY RootKey,LPCTSTR SubKey)
        {
            LONG lResult;
            bool mReturn;
            //Delete a key an all the values
            lResult = RegDeleteKey(RootKey,SubKey);

            switch(lResult)
            {
                case ERROR_SUCCESS: //ok
                    mReturn = true;
                    break;
                default: //an error code from windows
                    mReturn = false;
                    break;
            }

            return mReturn;
        }

        //Determines if the key exists
        bool VRegistry::KeyExists(HKEY RootKey,LPCTSTR SubKey)
        {
            LONG lResult;
            HKEY mTempKey;
            lResult = RegOpenKeyEx(RootKey,SubKey,0,KEY_ALL_ACCESS,&mTempKey);

            switch(lResult)
            {
                case ERROR_SUCCESS: //ok
                    RegCloseKey(mTempKey);
                    return true;
                    break;
                default: //an error code from windows
                    return false;
                    break;
            }
        }

        //Deletes a registry value and all values
        bool VRegistry::DeleteValue(LPCTSTR ValueName)
        {
            LONG lResult;
            bool mReturn;
            //Delete a key an all the values
            lResult = RegDeleteValue(this->mKey,ValueName);

            switch(lResult)
            {
                case ERROR_SUCCESS: //ok
                    mReturn = true;
                    break;
                default: //an error code from windows
                    mReturn = false;
                    break;
            }

            return mReturn;
        }

        //Determines if a value exists
        bool VRegistry::ValueExists(LPCTSTR ValueName)
        {
            LONG lResult;
            DWORD dwSize = 0;
            DWORD ValueType = REG_BINARY;
            BYTE* pBuffer = new BYTE[dwSize];

            //get the needed buffer size, recreate the buffer to match the size
            lResult = RegQueryValueEx(this->mKey,ValueName,0,&ValueType,pBuffer, &dwSize);
            delete[] pBuffer;

            switch(lResult)
            {
                case ERROR_SUCCESS: //ok
                   return true;
                    break;
                case ERROR_MORE_DATA:   //buffer was too small
                    return true;
                    break;
                case ERROR_FILE_NOT_FOUND: //value was not found
                    return false;
                    break;
                default: //an error code from windows
                    return false;
                    break;
            }
        }

        //Gets a Pointer to a Byte array filled with a REG_BINARY value from an open key (be sure to delete[] mVar)
        BYTE* VRegistry::QueryValue(LPCTSTR ValueName, DWORD ValueType)
        {
            LONG lResult;
            DWORD dwSize = 0;
            BYTE* pBuffer = new BYTE[dwSize];

            //get the needed buffer size, recreate the buffer to match the size
            lResult = RegQueryValueEx(this->mKey,ValueName,0,&ValueType,pBuffer, &dwSize);
            delete[] pBuffer;
            pBuffer = new BYTE[dwSize];

            //fill the buffer
            lResult = RegQueryValueEx(this->mKey,ValueName,0,&ValueType,pBuffer, &dwSize);

            switch(lResult)
            {
                case ERROR_SUCCESS: //ok
                   return pBuffer;
                    break;
                case ERROR_MORE_DATA:   //buffer was too small
                    throw "ERROR_MORE_DATA = Buffer is too small to read the value";
                    break;
                case ERROR_FILE_NOT_FOUND: //value was not found
                    throw "ERROR_FILE_NOT_FOUND = The value was not found";
                    break;
                default: //an error code from windows
                    throw "ERROR = " + lResult;
                    break;
            }
        }

        //Sets a value in the regsitry
        bool VRegistry::SetValue(LPCTSTR ValueName, DWORD ValueType, LPBYTE Value,DWORD dwSize)
        {

            LONG lResult;

            //Attempt to save the value
            lResult = RegSetValueEx(this->mKey,ValueName,0,ValueType,Value,dwSize);

            switch(lResult)
            {
                case ERROR_SUCCESS: //ok
                   return true;
                    break;
                default: //an error code from windows
                    return false;
                    break;
            }
        }

        //////////////////////////////////////////////////////////////////////////////////////
        // QT Friendly Methods
        //////////////////////////////////////////////////////////////////////////////////////

        //Opens a Registry Key
        bool VRegistry::OpenKey(HKEY RootKey,QString SubKey)
        {
            LPCTSTR mSubKey = VQTConvert::QString_To_LPCTSTR(SubKey);
            return OpenKey(RootKey,mSubKey);
        }

        //Creates a registry key and opens it
        bool VRegistry::CreateKey(HKEY RootKey,QString SubKey)
        {
            LPCTSTR mSubKey = VQTConvert::QString_To_LPCTSTR(SubKey);
            return CreateKey(RootKey,mSubKey);
        }

        //Deletes a registry key and all values
        bool VRegistry::DeleteKey(HKEY RootKey,QString SubKey)
        {
            LPCTSTR mSubKey = VQTConvert::QString_To_LPCTSTR(SubKey);
            return DeleteKey(RootKey,mSubKey);
        }

        //Determines if the key exists
        bool VRegistry::KeyExists(HKEY RootKey,QString SubKey)
        {
            LPCTSTR mSubKey = VQTConvert::QString_To_LPCTSTR(SubKey);
            return KeyExists(RootKey,mSubKey);
        }

        //Deletes a registry value and all values
        bool VRegistry::DeleteValue(QString ValueName)
        {
            LPCTSTR mValueName = VQTConvert::QString_To_LPCTSTR(ValueName);
            return DeleteValue(mValueName);
        }

        //Determines if a value exists
        bool VRegistry::ValueExists(QString ValueName)
        {
            LPCTSTR mValueName = VQTConvert::QString_To_LPCTSTR(ValueName);
            return ValueExists(mValueName);
        }

        //Gets REG_SZ value as a QString
        QString VRegistry::get_REG_SZ(QString ValueName)
        {
            if (ValueExists(ValueName)) {
                LPCTSTR mValueName = VQTConvert::QString_To_LPCTSTR(ValueName);
                DWORD mType = REG_SZ;
                BYTE* mBuffer = QueryValue(mValueName, mType);
                QString mRet = VQTConvert::WCHAR_to_QString((wchar_t*)mBuffer);
                delete[] mBuffer;
                return mRet;
            } else {
                return "";
            }
        }

        //Gets REG_EXPAND_SZ value as a QString
        QString VRegistry::get_REG_EXPAND_SZ(QString ValueName)
        {
            if (ValueExists(ValueName)){
                LPCTSTR mValueName = VQTConvert::QString_To_LPCTSTR(ValueName);
                DWORD mType = REG_EXPAND_SZ;
                BYTE* mBuffer = QueryValue(mValueName, mType);
                QString mRet =  VQTConvert::WCHAR_to_QString((wchar_t*)mBuffer);
                delete[] mBuffer;
                return mRet;
            } else
                return "";
        }

        //Gets REG_DWORD value as a UINT
        uint VRegistry::get_REG_DWORD(QString ValueName)
        {
            LPCTSTR mValueName = VQTConvert::QString_To_LPCTSTR(ValueName);
            DWORD mType = REG_DWORD;
            BYTE* mBuffer = QueryValue(mValueName, mType);
            DWORD mVal = *(DWORD*)&mBuffer[0];
            delete[] mBuffer;
            return (uint)mVal;
        }

        //Gets a REG_BINARY value as a QByteArray
        QByteArray VRegistry::get_REG_BINARY(QString ValueName)
        {
            LPCTSTR mValueName = VQTConvert::QString_To_LPCTSTR(ValueName);
            DWORD mType = REG_BINARY;
            BYTE* mBuffer = QueryValue(mValueName, mType);
            QByteArray mArray((const char*)mBuffer);
            delete[] mBuffer;
            return mArray;
        }

        //Gets a REG_MUTLI_SZ value as a QStringList
        QStringList VRegistry::get_MULTI_SZ(QString ValueName)
        {
            QStringList mList;
            LPCTSTR mValueName = VQTConvert::QString_To_LPCTSTR(ValueName);
            DWORD mType = REG_MULTI_SZ;
            BYTE* mBuffer = QueryValue(mValueName, mType);
            WCHAR* mItems =(WCHAR*)mBuffer;
            while(*mItems != '\0')
            {
                //mItems is the current string
                QString mRet =  VQTConvert::WCHAR_to_QString(mItems);
                mList.append(mRet);
                mItems = mItems + lstrlen(mItems); // pStr now points to null at end of string
                mItems++; // pStr now points to the next string, or the second null to terminate
            }

            delete[] mBuffer;
            return mList;
        }

        //Sets a REG_SZ value with a QString
        bool VRegistry::set_REG_SZ(QString ValueName, QString Value)
        {
            LPCTSTR mValueName = VQTConvert::QString_To_LPCTSTR(ValueName);
            DWORD mType = REG_SZ;
            DWORD mSize = sizeof(Value.utf16()) * Value.length();
            return SetValue(mValueName,mType,(LPBYTE)Value.utf16(),mSize);
        }

        //Sets a REG_EXPAND_SZ value with a QString
        bool VRegistry::set_REG_EXPAND_SZ(QString ValueName, QString Value)
        {
            LPCTSTR mValueName = VQTConvert::QString_To_LPCTSTR(ValueName);
            DWORD mType = REG_EXPAND_SZ;
            DWORD mSize = sizeof(Value.utf16()) * Value.length();
            return SetValue(mValueName,mType,(LPBYTE)Value.utf16(),mSize);
        }

        //Sets a REG_DWORD value with a UINT
        bool VRegistry::set_REG_DWORD(QString ValueName, UINT Value)
        {
            LPCTSTR mValueName = VQTConvert::QString_To_LPCTSTR(ValueName);
            DWORD mType = REG_DWORD;
            DWORD mSize = 4;
            return SetValue(mValueName,mType,(LPBYTE)&Value,mSize);
        }

        //Sets a REG_BINARY value with a QByteArray
        bool VRegistry::set_REG_BINARY(QString ValueName, QByteArray Value)
        {
            LPCTSTR mValueName = VQTConvert::QString_To_LPCTSTR(ValueName);
            DWORD mType = REG_BINARY;
            BYTE* mData = (BYTE*)Value.data();
            DWORD mSize = Value.length();
            return SetValue(mValueName,mType,mData,mSize);
        }

        //Sets a REG_MULTI_SZ value with a QStringList
        bool VRegistry::set_REG_MULTI_SZ(QString ValueName, QStringList Value)
        {
            LPCTSTR mValueName = VQTConvert::QString_To_LPCTSTR(ValueName);
            DWORD mType = REG_MULTI_SZ;
            QString mValue;
            //DWORD mSize = 0;
            for (int i = 0; i < Value.size(); ++i)
            {
                mValue += Value.at(i);
                mValue += '\0';
            }
            mValue += '\0';

            DWORD mSize = (sizeof(mValue.utf16()) * mValue.length()) / 2;

            return SetValue(mValueName,mType,(LPBYTE)mValue.utf16(),mSize);
        }

        //Gets a string list filled with names of the sub keys from the current open key
        QStringList VRegistry::enum_Keys()
        {
            LONG lResult;
            LONG lNumber = 0;
            QStringList mList;
            bool mProcess = true;

            //Enum through the items
            do
            {
                DWORD dwSize = 255;
                WCHAR* pBuffer = new WCHAR[dwSize];

                lResult = RegEnumKeyEx(this->mKey,lNumber, pBuffer, &dwSize,NULL,NULL,NULL,NULL);
                if(lResult == ERROR_SUCCESS)
                {
                    QString mRet =  VQTConvert::WCHAR_to_QString(pBuffer);
                    mList.append(mRet);
                }
                else
                {
                    mProcess = false;
                }
                delete[] pBuffer;

                lNumber++;
            }while(mProcess);

            return mList;
        }

        //Gets a string list filled with names of the values from the current open key
        QStringList VRegistry::enum_Values()
        {
            LONG lResult;
            LONG lNumber = 0;
            QStringList mList;
            bool mProcess = true;

            //Enum through the items
            do
            {
                DWORD dwSize = 16383;
                WCHAR* pBuffer = new WCHAR[dwSize];
                lResult = RegEnumValue(this->mKey,lNumber, pBuffer, &dwSize,NULL,NULL,NULL,NULL);
                if(lResult == ERROR_SUCCESS)
                {
                    QString mRet =  VQTConvert::WCHAR_to_QString(pBuffer);
                    mList.append(mRet);
                }
                else
                {
                    mProcess = false;
                }
                delete[] pBuffer;

                lNumber++;
            }while(mProcess);

            return mList;
        }

    } //end voidrealms::win32 namespace
} //end voidrealms namespace
