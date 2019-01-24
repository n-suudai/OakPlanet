#include "Heap.hpp"


namespace Oak {
namespace Memory {


struct AllocHeader
{
    time_t      time;           // 確保時刻
    UInt64      bytes;          // サイズ
    UInt64      bookmark;       // ブックマーク
    UInt64      stackTraceHash; // スタックトレースハッシュ
    UInt64      line;           // 行数
    const Char* file;           // ファイル名
    UInt64*     pBeginTrap;     // 開始トラップ
    UInt64*     pEndTrap;       // 末尾トラップ
};



} // namespace Memory
} // namespace Oak




