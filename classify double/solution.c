#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>


/**
 * Library-level functions.
 * You should use them in the main sections.
 */

uint64_t convertToUint64 (double number) {
    return *((uint64_t *)(&number));
}

bool getBit (const uint64_t number, const uint8_t index) {
    return (number >> index) & 1;
}

bool checkBitRange (const uint64_t number, const uint8_t minIndex, const uint8_t maxIndex,const bool value) {
    for (int i = maxIndex; i >= minIndex; i--) {
        if (getBit(number, i) != value) {
            return false;
        }
    }
    return true;
}

bool checkMinRange (const uint64_t number, const uint8_t minIndex, const uint8_t maxIndex) {
    return checkBitRange(number, minIndex, maxIndex, 0);
}

bool checkMaxRange (const uint64_t number, const uint8_t minIndex, const uint8_t maxIndex) {
    return checkBitRange(number, minIndex, maxIndex, 1);
}

bool checkForSign (const uint64_t number, const bool sign) {
    return getBit(number, 63) == sign;
}

bool checkForPlusSign (const uint64_t number) {
    return checkForSign(number, 0);
}

bool checkForMinusSign (const uint64_t number) {
    return checkForSign(number, 1);
}

bool checkForNormal (const uint64_t number) {
    return !checkMinRange(number, 52, 62) && !checkMaxRange(number, 52, 62);
}

bool checkForDenormal (const uint64_t number) {
    return checkMinRange(number, 52, 62) && !checkMinRange(number, 0, 51);
}


/**
 * Checkers here:
 */

bool checkForPlusZero (uint64_t number) {
    return number == 0x0000000000000000;
}

bool checkForMinusZero (uint64_t number) {
    return number == 0x8000000000000000;
}

bool checkForPlusInf (uint64_t number) {
    return number == 0x7FF0000000000000;
}

bool checkForMinusInf (uint64_t number) {
    return number == 0xFFF0000000000000;
}

bool checkForPlusNormal (uint64_t number) {
    return checkForPlusSign(number) && checkForNormal(number);
}

bool checkForMinusNormal (uint64_t number) {
    return checkForMinusSign(number) && checkForNormal(number);
}

bool checkForPlusDenormal (uint64_t number) {
    return checkForPlusSign(number) && checkForDenormal(number);
}

bool checkForMinusDenormal (uint64_t number) {
    return checkForMinusSign(number) && checkForDenormal(number);
}

bool checkForSignalingNan (uint64_t number) {
    return checkMaxRange(number, 52, 62) && !checkMinRange(number, 0, 51) && getBit(number, 51) == 0;
}

bool checkForQuietNan (uint64_t number) {
    return checkMaxRange(number, 52, 62) && getBit(number, 51) == 1;
}


void classify (double number) {
    if (checkForPlusZero(convertToUint64(number))) {
        printf("Plus zero\n");
    }

    else if (checkForMinusZero(convertToUint64(number))) {
        printf("Minus zero\n");
    }

    else if (checkForPlusInf(convertToUint64(number))) {
        printf("Plus inf\n");
    }

    else if (checkForMinusInf(convertToUint64(number))) {
        printf("Minus inf\n");
    }

    else if (checkForPlusNormal(convertToUint64(number))) {
        printf("Plus normal\n");
    }

    else if (checkForMinusNormal(convertToUint64(number))) {
        printf("Minus normal\n");
    }

    else if (checkForPlusDenormal(convertToUint64(number))) {
        printf("Plus Denormal\n");
    }

    else if (checkForMinusDenormal(convertToUint64(number))) {
        printf("Minus Denormal\n");
    }

    else if (checkForSignalingNan(convertToUint64(number))) {
        printf("Signailing NaN\n");
    }

    else if (checkForQuietNan(convertToUint64(number))) {
        printf("Quiet NaN\n");
    }

    else {
        printf("Error.\n");
    }
}