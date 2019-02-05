
#pragma once

#include "Oak/Platform/AtomicDataTypes.hpp"


namespace Oak {


/*
"r"     テキスト    読み取り                  エラー
"w"     テキスト    書き込み                  新規作成
"a"     テキスト    追加書き込み              新規作成

"rb"    バイナリ    読み取り                  エラー
"wb"    バイナリ    書き込み                  新規作成
"ab"    バイナリ    追加書き込み               新規作成

"r+"    テキスト    更新(読み取り及び書き込み)   エラー
"w+"    テキスト    更新(読み取り及び書き込み)   新規作成
"a+"    テキスト    更新(読み取り及び書き込み)   新規作成

"rb+"   バイナリ    更新(読み取り及び書き込み)   エラー
"wb+"   バイナリ    更新(読み取り及び書き込み)   新規作成
"ab+"   バイナリ    更新(追加書き込み)          新規作成
*/
enum FILE_OPEN_MODE : UInt32
{
    FILE_OPEN_MODE_NONE     = 0
                                                //  FileType    Mode            If not exists
    , FILE_OPEN_MODE_TEXT_READ                  //  TEXT        READ            ERROR
    , FILE_OPEN_MODE_TEXT_WRITE                 //  TEXT        WRITE           CREATE
    , FILE_OPEN_MODE_TEXT_ADD                   //  TEXT        WRITE_ADD       CREATE

    , FILE_OPEN_MODE_BINARY_READ                //  BINARY      READ            ERROR
    , FILE_OPEN_MODE_BINARY_WRITE               //  BINARY      WRITE           CREATE
    , FILE_OPEN_MODE_BINARY_ADD                 //  BINARY      WRITE_ADD       CREATE

    , FILE_OPEN_MODE_TEXT_READ_WRITE            //  TEXT        READ_WRITE      ERROR
    , FILE_OPEN_MODE_TEXT_READ_WRITE_CREATE     //  TEXT        READ_WRITE      CREATE
    , FILE_OPEN_MODE_TEXT_READ_WRITE_ADD        //  TEXT        READ_WRITE_ADD  CREATE

    , FILE_OPEN_MODE_BINARY_READ_WRITE          //  BINARY      READ_WRITE      ERROR
    , FILE_OPEN_MODE_BINARY_READ_WRITE_CREATE   //  BINARY      READ_WRITE      CREATE
    , FILE_OPEN_MODE_BINARY_READ_WRITE_ADD      //  BINARY      READ_WRITE_ADD  CREATE

    , FILE_OPEN_MODE_NUM
};


namespace Detail {


class IFileSystem
{
protected:
    IFileSystem() = default;

    virtual ~IFileSystem() = default;

public:
    virtual Bool Open(const Char* filename, FILE_OPEN_MODE fileOpenMode) = 0;

    virtual SizeT Read(Void* pBlock, SizeT size) = 0;

    virtual SizeT Write(Void* pBlock, SizeT size) = 0;

    virtual Void Close() = 0;
};


} // namespace Detail
} // namespace Oak

