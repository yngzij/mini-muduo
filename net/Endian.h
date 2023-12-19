//
// Created by YngziJ on 2023/12/19.
//

#ifndef UNTITLED8_ENDIAN_H
#define UNTITLED8_ENDIAN_H


#include <cstdint>
#include <libkern/OSByteOrder.h>


namespace sockets {

// the inline assembler code makes type blur,
// so we disable warnings for a while.

    inline uint64_t hostToNetwork64(uint64_t host64) {
        return OSSwapHostToBigInt64(host64);

    }

    inline uint32_t hostToNetwork32(uint32_t host32) {
        //return htobe32(host32);
        return OSSwapHostToBigInt32(host32);
    }

    inline uint16_t hostToNetwork16(uint16_t host16) {
        //return htobe16(host16);
        return OSSwapHostToBigInt16(host16);
    }

    inline uint64_t networkToHost64(uint64_t net64) {
        //return be64toh(net64);
        return OSSwapBigToHostInt64(net64);
    }

    inline uint32_t networkToHost32(uint32_t net32) {
        //return be32toh(net32);
        return OSSwapBigToHostInt32(net32);
    }

    inline uint16_t networkToHost16(uint16_t net16) {
        //return be16toh(net16);
        return OSSwapBigToHostInt16(net16);
    }

}  // namespace sockets





#endif //UNTITLED8_ENDIAN_H
