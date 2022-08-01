#include "s21_decimal.h"
#include <stdio.h>
#include <math.h>
#include <string.h>

void print_decimal(s21_decimal *dst) {
    printf("bits[0]: ");
    print_int_to_binary(dst->bits[0]);
    printf("bits[1]: ");
    print_int_to_binary(dst->bits[1]);
    printf("bits[2]: ");
    print_int_to_binary(dst->bits[2]);
    printf("bits[3]: ");
    print_int_to_binary(dst->bits[3]);
}

void print_big_decimal(big_decimal *dst) {
    printf("bits[0]: ");
    print_int_to_binary(dst->bits[0]);
    printf("bits[1]: ");
    print_int_to_binary(dst->bits[1]);
    printf("bits[2]: ");
    print_int_to_binary(dst->bits[2]);
    printf("bits[3]: ");
    print_int_to_binary(dst->bits[3]);
    printf("bits[4]: ");
    print_int_to_binary(dst->bits[4]);
    printf("bits[5]: ");
    print_int_to_binary(dst->bits[5]);
    printf("bits[6]: ");
    print_int_to_binary(dst->bits[6]);
}

void print_int_to_binary(unsigned int k) {
    unsigned int size = sizeof(k);
    unsigned int i;
    for (i = size * 8; i > 0; --i) {
        if ((i % 4 == 0) && (i != size * 8)) {
            printf(" ");
        }
        printf("%d", (k & (1 << (i - 1))) ? 1 : 0);
    }
    printf("\n");
}


// arifmetics

// сумма двух децималов
int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
    int err = 0;
    int resScale = 0;
    s21_decimal tempFirstDec, tempSecondDec, tempResDecimal;
    decimal_init(&tempResDecimal);
    copyDec(&tempFirstDec, value_1);
    copyDec(&tempSecondDec, value_2);
    clear_bit(127, &tempFirstDec);
    clear_bit(127, &tempSecondDec);
    int sig1, sig2;
    sig1 = get_sign(value_1);
    sig2 = get_sign(value_2);
    if (!err) {
        setEqualScale(&tempFirstDec, &tempSecondDec);
        if (sig1 == sig2) {
            tempResDecimal = add_decimal(tempFirstDec, tempSecondDec, &err);
            if (sig1&sig2) set_sign(1, &tempResDecimal);
        } else if (sig1 > sig2) {
            if (s21_is_less(&tempSecondDec, &tempFirstDec)) {
                tempResDecimal = sub_decimal(tempFirstDec, tempSecondDec, &err);
                set_sign(1, &tempResDecimal);
            } else {
                tempResDecimal = sub_decimal(tempSecondDec, tempFirstDec, &err);
            }
        } else if (sig2 > sig1) {
            if (s21_is_less(&tempFirstDec, &tempSecondDec)) {
                tempResDecimal = sub_decimal(tempSecondDec, tempFirstDec, &err);
                set_sign(1, &tempResDecimal);
            } else {
                tempResDecimal = sub_decimal(tempFirstDec, tempSecondDec, &err);
            }
        }
        if (err == 1 && get_sign(tempResDecimal)) err = 2;
        resScale = getDecimalScale(tempFirstDec);
        setDecimalScale(&tempResDecimal, resScale);
        copyDec(result, tempResDecimal);
    }
    return err;
    }

// Вычитание децималов
int s21_sub(s21_decimal dec_1, s21_decimal dec_2, s21_decimal *result) {
    int err = 0;
    int resScale = 0;
    s21_decimal value_1, value_2, tempDecimal;
    decimal_init(&tempDecimal);
    copyDec(&value_1, dec_1);
    copyDec(&value_2, dec_2);
    clear_bit(127, &value_1);
    clear_bit(127, &value_2);
    int sig1, sig2;
    sig1 = get_sign(dec_1);
    sig2 = get_sign(dec_2);
    if (!err) {
        setEqualScale(&value_1, &value_2);
        if ((s21_is_equal(&value_1, &value_2))) {
            if (sig1 == sig2) {
                decimal_init(&tempDecimal);
            } else {
                tempDecimal = add_decimal(value_1, value_2, &err);
                if (sig1) set_sign(1, &tempDecimal);
            }
        } else if ((s21_is_less(&value_1, &value_2))) {
            if (sig1 == sig2) {
                tempDecimal = sub_decimal(value_2, value_1, &err);
                if (!(sig1&sig2)) set_sign(1, &tempDecimal);
            } else if (sig1 > sig2) {
                tempDecimal = add_decimal(value_1, value_2, &err);
                set_sign(1, &tempDecimal);
            } else if (sig2 > sig1) {
                tempDecimal = add_decimal(value_1, value_2, &err);
            }
        } else {
            if (sig1 == sig2) {
                tempDecimal = sub_decimal(value_1, value_2, &err);
                if (sig1&sig2) set_sign(1, &tempDecimal);
            } else if (sig1 > sig2) {
                tempDecimal = add_decimal(value_1, value_2, &err);
                set_sign(1, &tempDecimal);
            } else if (sig2 > sig1) {
                tempDecimal = add_decimal(value_1, value_2, &err);
            }
        }
        if (err == 1 && get_sign(tempDecimal)) err = 2;
        resScale = getDecimalScale(value_1);
        setDecimalScale(&tempDecimal, resScale);
        copyDec(result, tempDecimal);
    }
    return err;
}


// Приведение к общему Scale умножением на 10
int setEqualScale(s21_decimal *dec_1, s21_decimal *dec_2) {
    int err = 0;
    int resScale = 0;
    s21_decimal tempMul = {{0, 0, 0, 0}};
    int diffScale = getDecimalScale(*dec_1) - getDecimalScale(*dec_2);
    if (diffScale < 0) {
        diffScale *= -1;
        copyDec(&tempMul, *dec_1);
        for (int i = 1;  i <= diffScale; i++) {
            err = mulDecimal10(tempMul, dec_1);
            copyDec(&tempMul, *dec_1);
        }
        resScale = getDecimalScale(*dec_2);
        setDecimalScale(dec_1, resScale);
    } else if (diffScale > 0) {
        copyDec(&tempMul, *dec_2);
        for (int i = 1;  i <= diffScale; i++) {
            err = mulDecimal10(tempMul, dec_2);
            copyDec(&tempMul, *dec_2);
        }
        resScale = getDecimalScale(*dec_1);
        if (resScale > 0) setDecimalScale(dec_2, resScale);
    }
    return err;
}

// Алгоритм умножение децимал на 10
int mulDecimal10(s21_decimal dec, s21_decimal *result) {
    int errInf = 0;
    int tempSign = get_sign(dec);
    s21_decimal tempTen = {{10, 0, 0, 0}};
    s21_decimal decShift, decTemp;
    decimal_init(&decTemp);
    for (int i = 0; i <= 95; i++) {
        if (get_bit(i, tempTen) == 1) {
            copyDec(&decShift, dec);
            errInf = shiftDecLeft(&decShift, i);
            decTemp = add_decimal(decShift, decTemp, &errInf);
        }
    }
    copyDec(result, decTemp);
    if (tempSign) set_sign(tempSign, result);
    return errInf;
}

// возведение в степень
double s21_pow(double num, int exp) {
  double resultPow = 1;
  for (int i = 0; i < exp; i++) {
    resultPow *= num;
  }
  return resultPow;
}

// /(деление)
int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
    int exit_code = 0;
    decimal_init(result);
    if (decimal_is_zero(&value_2)) {
        exit_code = 3;
    } else {
        int length_1 = getDecimalLength(value_1);
        int length_2 = getDecimalLength(value_2);
        int scale_1 = getDecimalScale(value_1);
        int scale_2 = getDecimalScale(value_2);
        int result_scale = 0;
        setDecimalScale(&value_1, 0);
        setDecimalScale(&value_2, 0);
        // printf("length 1: %d\n", length_1);
        // printf("length 2: %d\n", length_2);

        big_decimal value_1_big, value_2_big, result_big;
        big_decimal_init(&value_1_big);
        big_decimal_init(&value_2_big);
        big_decimal_init(&result_big);

        dec_to_big(&value_1, &value_1_big);
        dec_to_big(&value_2, &value_2_big);

        for ( ; scale_2 > 0; scale_2--) {
            if (scale_1 > 0) {
                scale_1--;
            } else {
                simple_mul_10_big(value_1_big, &value_1_big);
            }
        }
        value_1_big = simple_div_big(value_1_big, value_2_big, &result_big);
        big_decimal temp_res;
        while (!big_decimal_is_empty(value_1_big) && result_scale <28) {
            big_decimal_init(&temp_res);
            simple_mul_10_big(value_1_big, &value_1_big);
            value_1_big = simple_div_big(value_1_big, value_2_big, &temp_res);
            result_scale++;
            simple_mul_10_big(result_big, &result_big);
            add_big(result_big, temp_res, &result_big);
        }
        setBigDecimalScale(&result_big, result_scale);
        exit_code = big_to_dec_cut(result_big, result);
    }
    return exit_code;
}

// Целочисленное деление big_decimal. Возвращает остаток от деления
big_decimal simple_div_big(big_decimal value_1, big_decimal value_2, big_decimal *result) {
    big_decimal_init(result);
    int zeros = 1;
    int length_1 = getBigDecimalLength(value_1);
    int length_2 = getBigDecimalLength(value_2);
    int start_length_2 = length_2;
    if (length_1 > length_2) {
        while (length_1 != length_2) {
            shiftBigDecLeftOne(&value_2);
            length_2++;
        }
    }
    while ((!big_decimal_is_empty(value_1) && getBigDecimalLength(value_2) >= start_length_2) || zeros > 0) {
        if (simple_big_decimal_compare(value_1, value_2) != -1) {
            simple_sub_big(value_1, value_2, &value_1);

            shiftBigDecLeftOne(result);
            set_bit_big(0, result);

        } else if (simple_big_decimal_compare(value_1, value_2) == -1 || zeros > 0) {
            shiftBigDecLeftOne(result);
            zeros--;
        }
        if (getBigDecimalLength(value_2) >= start_length_2) {
            shiftBigDecRightOne(&value_2);
        }
        if (!big_decimal_is_empty(value_1)) {
            zeros = getBigDecimalLength(value_2) - start_length_2;
        }
    }
    return  value_1;
}

/*
сравнение big_decimal без учёта scale и знака
1 - первый больше второго
-1 - второй больше первого
0 - равны
*/
int simple_big_decimal_compare(big_decimal value_1, big_decimal value_2) {
    int exit_code = 0;
    for (int i = 192; i >= 0; i--) {
        if (get_bit_big(i, value_1) > get_bit_big(i, value_2)) {
            exit_code = 1;
            i = -1;
        } else if (get_bit_big(i, value_1) < get_bit_big(i, value_2)) {
            exit_code = -1;
            i = -1;
        }
    }
    return exit_code;
}

// записывет в result разность value_1 и value_2. первое должно быть больше второго
int simple_sub_big(big_decimal value_1, big_decimal value_2, big_decimal *result) {
    big_decimal_init(result);
    int v_ume = 0;
    for (int i = 0; i < 192; i++) {
        if ((get_bit_big(i, value_1) + get_bit_big(i, value_2) + v_ume) % 2 == 1) {
            set_bit_big(i, result);
        }
        if (!get_bit_big(i, value_1) && get_bit_big(i, value_2)) {
                v_ume = 1;
        } else {
            v_ume = 0;
        }
    }
}

// возвращает длину значащей части big_decimal
int getBigDecimalLength(big_decimal dec) {
    int bit = 191;
    while (!get_bit_big(bit, dec) && bit != 0) bit--;
    return bit + 1;
}

int simple_mul_10_big(big_decimal value, big_decimal *result) {
    big_decimal_init(result);
    for (int i = 0; i < 4; i++) {
        if (i % 2)
            add_big(*result, value, result);
        shiftBigDecLeftOne(&value);
    }
    return 0;
}

// *
int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
    int err_flag = 0;
    decimal_init(result);
    if (decimal_is_zero(&value_1) || decimal_is_zero(&value_2)) {
        err_flag = 0;
    } else {
        int length_1 = getDecimalLength(value_1);
        int length_2 = getDecimalLength(value_2);
        int scale_1 = getDecimalScale(value_1);
        int scale_2 = getDecimalScale(value_2);
        setDecimalScale(&value_1, 0);
        setDecimalScale(&value_2, 0);
        // printf("length 1: %d\n", length_1);
        // printf("length 2: %d\n", length_2);

        big_decimal value_1_big, value_2_big, result_big;
        big_decimal_init(&value_1_big);
        big_decimal_init(&value_2_big);
        big_decimal_init(&result_big);

        dec_to_big(&value_1, &value_1_big);
        dec_to_big(&value_2, &value_2_big);

        for (int i = 0; i < length_2; i++) {
            if (get_bit_big(i, value_2_big))
                add_big(result_big, value_1_big, &result_big);
            shiftBigDecLeftOne(&value_1_big);
        }

        if (result_big.bits[3] == 0 && result_big.bits[4] == 0 && result_big.bits[5] == 0) {
            big_to_dec(&result_big, result);
        } else if (scale_1 + scale_2 > 0) {
            err_flag = big_to_dec_cut(result_big, result);
        } else {
            err_flag == 1;
        }
        if (err_flag == 0)
            setDecimalScale(result, scale_1+scale_2);
    }
    return err_flag;
}

// обрезает дробную часть до вмещающегося в s21_decimal размера
int big_to_dec_cut(big_decimal src, s21_decimal *dst) {
    int exit_code = 0;
    big_decimal ten = {{10, 0, 0, 0, 0, 0, 0}};
    big_decimal five = {{5, 0, 0, 0, 0, 0, 0}};
    big_decimal one = {{1, 0, 0, 0, 0, 0, 0}};
    big_decimal ost;
    while ((src.bits[3] != 0 || src.bits[4] != 0 || src.bits[5] != 0) && getBigDecimalScale(src) > 0) {
        ost = simple_div_big(src, ten, &src);
        setBigDecimalScale(&src, getBigDecimalScale(src) - 1);
            // это математическое округление, но нужно ли тут банковское?
        if (simple_big_decimal_compare(ost, five) != -1) {
            add_big(src, one, &src);
        }
    }
    if (src.bits[3] == 0 && src.bits[4] == 0 && src.bits[5] == 0) {
        big_to_dec(&src, dst);
    } else {
        decimal_init(dst);
        exit_code = 1;
    }
    return exit_code;
}

// возвращает длину значащей части децимал
int getDecimalLength(s21_decimal dec) {
    int bit = 95;
    while (!get_bit(bit, dec) && bit != 0) bit--;
    return bit + 1;
}

// копирует децимал
void copyDec(s21_decimal *result, s21_decimal dec) {
    decimal_init(result);
    for (int i = 127; i >= 0; i--) {
        if (get_bit(i, dec) == 1) set_bit(i, result);
    }
}

// алгоритм суммы двух децималов
s21_decimal add_decimal(s21_decimal value_1, s21_decimal value_2, int *status) {
    s21_decimal result;
    decimal_init(&result);
    int temp = 0;
    int firstBit, secondBit;
    for (int i = 0; i <= 95; i++) {
        firstBit = get_bit(i, value_1);
        secondBit = get_bit(i, value_2);
        if (firstBit && secondBit) {
            if (temp == 1) set_bit(i, &result);
            temp = 1;
        } else if (firstBit ^ secondBit) {
            if (!temp) set_bit(i, &result);
        } else if (!firstBit && !secondBit && temp) {
            set_bit(i, &result);
            temp = 0;
        }
    }
    *status = temp;
    return result;
}

s21_decimal sub_decimal(s21_decimal value_1, s21_decimal value_2, int *
status) {
    s21_decimal result, invertValue2;
    decimal_init(&result);
    int temp = 0;
    // int firstBit, secondBit;
    invertValue2 = invert_for_sub_decimal(value_2);
    result = add_decimal(value_1, invertValue2, &temp);
    // *status = temp;
    return result;
}

s21_decimal invert_for_sub_decimal(s21_decimal dec) {
    int tmp;
    s21_decimal result = dec;
    s21_decimal one = {{1, 0, 0, 0}};
    for (int i = 0; i <= 95; i++) {
        if (!get_bit(i, dec)) {
            set_bit(i, &result);
        } else {
            clear_bit(i, &result);
        }
    }
    result = add_decimal(result, one, &tmp);
    return result;
}

// устанавливает знак -
void set_sign(int sign, s21_decimal *dst) {
    if (sign > 0) {
        dst->bits[3] ^= 1u << 31;
    }
}

// возвращает знак
int get_sign(s21_decimal dst) {
    int sign = get_bit(127, dst);
    return sign;
}

int s21_from_int_to_decimal(int src, s21_decimal *dst) {
    int err = 0;
    if (src > INT_MIN && src < INT_MAX) {
        if (src >= 0) {
            dst->bits[3] = 0;
            int temp = *((unsigned int*)&src);
            dst->bits[0] = temp;
        } else {
            dst->bits[3] = 1u << 31;
            src = -src;
            dst->bits[0] = *((unsigned int*)&src);
        }
        dst->bits[1] = dst->bits[2] = 0;
    } else {
        err = 1;
    }
    return err;
}

int s21_from_decimal_to_int(s21_decimal src, int *dst) {
    int temp = 0;
    int err = 0;
    s21_decimal tmpDec;
    decimal_init(&tmpDec);
    s21_truncate(src, &tmpDec);
    int bits = firstNotZeroBit(tmpDec);
    if (bits < 31) {
        int bit = 0;
        while (bit < 96) {
            temp += (get_bit(bit, tmpDec) * s21_pow(2, bit));
            bit++;
        }
        if (get_bit(127, src)) temp *= -1;
        *dst = temp;
    } else {
        err = 1;
    }
    return err;
}

int firstNotZeroBit(s21_decimal src) {
    int nBits = 0;
    int over = 0;
    s21_decimal tmp;
    copyDec(&tmp, src);
    for (int i = 95; (i >= 0) && (over <= 0); i--) {
        if (get_bit(i, tmp)) over = 1;
        nBits = i;
    }
    return nBits;
}



int getDecimalScale(s21_decimal dec) {
    int scale = dec.bits[3] & EXP;
    scale >>=16;
    return scale;
}

int getBigDecimalScale(big_decimal dec) {
    int scale = dec.bits[6] & EXP;
    scale >>=16;
    return scale;
}

void setDecimalScale(s21_decimal *dec, int scale) {
    dec->bits[3] = (dec->bits[3] & SIGN) | (scale << 16);
}

void setBigDecimalScale(big_decimal *dec, int scale) {
    dec->bits[6] = (dec->bits[6] & SIGN) | (scale << 16);
}

// Возвращает состояние бита на позиции
int get_bit(int position, s21_decimal dec) {
    unsigned int mask = 1u << (position % 32);
    return ((dec.bits[position / 32] & mask) != 0);
}

void clearSign(s21_decimal* dst) {
    if (get_bit(127, *dst)) {
        dst->bits[3] ^= 1u << 31;
    }
}

// Установить бит на позицию
void set_bit(int position, s21_decimal* dec) {
    unsigned int mask = 1u << (position % 32);
    dec->bits[position / 32] |= mask;
}

// Зануляет существующий бит
void clear_bit(int position, s21_decimal* dec) {
    unsigned int mask = 1u << (position % 32);
    dec->bits[position / 32] &= ~mask;
}


// comparison functions

int decimal_compare(s21_decimal src_one, s21_decimal src_two) {
  int result = 0;
  int scale_one, scale_two;
  scale_one = getDecimalScale(src_one);
  scale_two = getDecimalScale(src_two);
  printf("scale до нормализации\n");
  printf("scale_one: %d\n", scale_one);
  printf("scale_two: %d\n", scale_two);
  if (scale_one == scale_two) {
    result = compare_mantis(&src_one, &src_two);
  } else {
    setEqualScale(&src_one, &src_two);
    printf("scale после нормализации\n");
    printf("scale_one: %d\n", getDecimalScale(src_one));
    printf("scale_two: %d\n", getDecimalScale(src_two));
    printf("децималы после нормализации \n");
    print_decimal(&src_one);
    printf("\n");
    print_decimal(&src_two);
    result = compare_mantis(&src_one, &src_two);
    if (result != 0) {
    //   result = scale_one > scale_two ? 1 : -1;
      result *= (get_sign(src_one) ? -1 : 1);
    }
  }
  return result;
}

int compare_mantis(s21_decimal *src_one, s21_decimal *src_two) {
    int res = 0;
    int sig_one = get_sign(*src_one), sig_two = get_sign(*src_two);
    if (sig_one > sig_two) {
        res = -1;
    } else if (sig_one < sig_two) {
        res = 1;
    } else {
        res = simple_comp_dec(*src_one, *src_two);
    }
    return res;
}

int simple_comp_dec(s21_decimal dec1, s21_decimal dec2) {
    int res = simple_comp_u(dec1.bits[2], dec2.bits[2]);
    if (res == 0) res = simple_comp_u(dec1.bits[1], dec2.bits[1]);
    if (res == 0) res = simple_comp_u(dec1.bits[0], dec2.bits[0]);
    return res;
}

int simple_comp_u(unsigned int a, unsigned int b) {
    int res;
    if (a > b) {
        res = 1;
    } else if (a == b) {
        res = 0;
    } else {
        res = -1;
    }
    return res;
}


s21_decimal simple_div_by_ten(s21_decimal dec, s21_decimal *resid) {
    s21_decimal ten;
    s21_from_int_to_decimal(10, &ten);
    return simple_div(dec, ten, resid);
}

s21_decimal simple_div(s21_decimal num, s21_decimal den, s21_decimal *ptr_resid) {
    s21_decimal one, res;
    s21_decimal buf = den;
    s21_from_int_to_decimal(1, &one);
    decimal_init(&res);

    int num_pwr = getDecimalScale(num);

    int sign = get_sign(num) == get_sign(den)? 0 : 1;

    if (simple_comp_dec(num, den) == -1) {
        s21_from_int_to_decimal(0, &res);
    } else {
        while (simple_comp_dec(num, den) != -1) {
            int max_bit = max_dec_bit(num) - max_dec_bit(den);
            den = s21_move_left(den, max_bit);
            one = s21_move_left(one, max_bit);

            if (simple_comp_dec(num, den) == -1) {
                den = s21_move_right(den, 1);
                one = s21_move_right(one, 1);
            }
            num = sub_dec_bits(num, den);
            res = sum_dec_bits(res, one);
            s21_from_int_to_decimal(1, &one);
            den = buf;
        }
    }
    if (ptr_resid) {
        *ptr_resid = num;
    }
    setDecimalScale(&res, num_pwr);
    set_sign(sign, &res);
    return res;
}

// Остаток и результат от деления на 10
int div_ten_mod(s21_decimal dec, s21_decimal *result_div) {
    int err, mod = 0;
    s21_decimal decTemp, quotOfDiv, one;
    decimal_init(&decTemp);
    decimal_init(&quotOfDiv);
    decimal_init(&one);
    s21_from_int_to_decimal(1, &one);
    for (int i = 95; i >= 0; i--) {
        shiftDecLeft(&decTemp, 1);
        if (get_bit(i, dec)) {
            set_bit(0, &decTemp);
        }
        shiftDecLeft(&quotOfDiv, 1);
        if (decTemp.bits[0] >= 10) {
            decTemp.bits[0] -= 10;
            quotOfDiv = add_decimal(quotOfDiv, one, &err);
        }
    }
    mod = decTemp.bits[0];
    copyDec(result_div, quotOfDiv);
    if (get_sign(dec)) set_sign(1, &quotOfDiv);
    return mod;
}



int max_dec_bit(s21_decimal dec) {
    int position = -1;
    if (dec.bits[2]) {
        position = 32 + 32 + max_bit(dec.bits[2]);
    } else if (dec.bits[1]) {
        position = 32 + max_bit(dec.bits[1]);
    } else {
        position = max_bit(dec.bits[0]);
    }
    return position;
}

int max_bit(unsigned a) {
    /*  searches last non-zero bit of int
        returns its position [0-31] */
    unsigned int one = 1;
    int bit_numb = 0;
    while (a >= one) {
        one = one << 1;
        bit_numb++;
        if (bit_numb > 32) {
            bit_numb = 32;
            break;
        }
    }
    bit_numb--;
    return bit_numb;
}

/*  sum all bits of decimals, controling overflow of 0th and 1st, but not of 2nd
        sing and scale is ignored */
s21_decimal sum_dec_bits(s21_decimal dec1, s21_decimal dec2) {
    s21_decimal res = dec1;
    add_decimal_bits(&res, &dec2);
    return res;
}

// adds second argument to first, powers ignored
// returns overflow
// DESTRUCTIVE for dec1
int add_decimal_bits(s21_decimal *dec1, const s21_decimal *dec2) {
    int carry = 0;
    for (int i = 0; i < 128; i++) {
            int temp = ((dec1->bits[i / 32] >> i % 32) & 1) + ((dec2->bits[i / 32] >> i % 32) & 1) + carry;
            carry = temp / 2;
            (temp % 2) ? set_bit(i, dec1) : clear_bit(i, dec1);
    }
    return carry;
}


/* substract all bits of decimals, dec1 > dec2 is nessesary
       sing and scale is ignored */
s21_decimal sub_dec_bits(s21_decimal dec1, s21_decimal dec2) {
    s21_decimal res = dec1;
    substract_decimal_bits(&res, &dec2);
    return res;
}

int substract_decimal_bits(s21_decimal *dec1, const s21_decimal *dec2) {
    unsigned int carry0 = simple_comp_u(dec1->bits[0], dec2->bits[0]) < 0;
    dec1->bits[0] = (unsigned int)dec1->bits[0] - (unsigned int)dec2->bits[0];
    int carry1 = simple_comp_u(dec1->bits[1], dec2->bits[1] + carry0) < 0;
    dec1->bits[1] = (unsigned int)dec1->bits[1] - (unsigned int)dec2->bits[1] - carry0;
    int carry2 = simple_comp_u(dec1->bits[2], dec2->bits[2] + carry1) < 0;
    dec1->bits[2] = (unsigned int)dec1->bits[2] - (unsigned int)dec2->bits[2] - carry1;
    return (int)carry2;
}

s21_decimal s21_move_left(s21_decimal src, int shift) {
    int inf_flag = 0;
    s21_decimal dec;
    decimal_init(&dec);
    dec.bits[3] = src.bits[3];
    for (int pos = 95; pos >= shift; pos--) {
        int bit = get_bit(pos - shift, src);
        bit ? set_bit(pos, &dec) : clear_bit(pos, &dec);
    }
    return dec;
}

s21_decimal s21_move_right(s21_decimal src, int shift) {
    s21_decimal res;
    decimal_init(&res);
    for (int i = 0; i < 95; get_bit(shift + i, src) ? set_bit(i, &res) : clear_bit(i, &res), i++) {
    }
    return res;
}

s21_decimal s21_multi_10(s21_decimal src) {
    s21_decimal dec = {{0, 0, 0, 0}};
        dec.bits[3] = src.bits[3];
        int inf_flag = 0;
        for (int i = 1; i <= 2 && !inf_flag; i++) {
            src = s21_move_left(src, i);
            s21_add(dec, src, &dec);
        }
    return dec;
}

int s21_is_equal(s21_decimal* dst1, s21_decimal* dst2) {
    int result = 0;
    if (decimal_compare(*dst1, *dst2) == 0)
        result = 1;
    return result;
}

int s21_is_not_equal(s21_decimal* dst1, s21_decimal* dst2) {
    int result = 0;
    if (decimal_compare(*dst1, *dst2) != 0)
        result = 1;
    return result;
}

int s21_is_less(s21_decimal* dst1, s21_decimal* dst2) {
    int result = 0;
    if (decimal_compare(*dst1, *dst2) == -1)
        result = 1;
    return result;
}

int s21_is_less_or_equal(s21_decimal* dst1, s21_decimal* dst2) {
    int result = 0;
    if (decimal_compare(*dst1, *dst2) != 1)
        result = 1;
    return result;
}

int s21_is_greater(s21_decimal* dst1, s21_decimal* dst2) {
    int result = 0;
    if (decimal_compare(*dst1, *dst2) == 1)
        result = 1;
    return result;
}

int s21_is_greater_or_equal(s21_decimal* dst1, s21_decimal* dst2) {
    int result = 0;
    if (decimal_compare(*dst1, *dst2) != -1)
        result = 1;
    return result;
}

int float_to_int(float src) {
  char buffer[33], buffer_2[33];
  int res = 0, lenght = 0;
  snprintf(buffer, sizeof(buffer), "%.6e", src);
  for (int i = 0; i < 33; i++) {
    if (buffer[i] == 'e' || buffer[i] == 'E') {
      buffer[i] = '\0';
      break;
    }
  }
  int j = 0;
  lenght = strlen(buffer);
  for (int i = 0; i < lenght; i++) {
    if (buffer[i] != '.') {
      buffer_2[j] = buffer[i];
      j++;
    }
  }
  lenght = strlen(buffer_2);
  for (int i = 0; i < lenght; i++) {
    res += (buffer_2[i] + '0' - 96);
    if (i < lenght - 1) res *= 10;
  }
  return res;
}

void big_decimal_init(big_decimal *src) {
  src->bits[0] = 0;
  src->bits[1] = 0;
  src->bits[2] = 0;
  src->bits[3] = 0;
  src->bits[4] = 0;
  src->bits[5] = 0;
  src->bits[6] = 0;
}

void decimal_init(s21_decimal *src) {
  src->bits[0] = 0;
  src->bits[1] = 0;
  src->bits[2] = 0;
  src->bits[3] = 0;
}

void dec_to_big(s21_decimal *src, big_decimal *dst) {
  dst->bits[0] = src->bits[0];
  dst->bits[1] = src->bits[1];
  dst->bits[2] = src->bits[2];
  dst->bits[6] = src->bits[3];
}

void big_to_dec(big_decimal *src, s21_decimal *dst) {
  dst->bits[0] = src->bits[0];
  dst->bits[1] = src->bits[1];
  dst->bits[2] = src->bits[2];
  dst->bits[3] = src->bits[6];
}

int big_decimal_is_empty(big_decimal src) {
  int flag = 1;
  for (int i = 5; i >= 0; i--) {
    if (src.bits[i]) flag = 0;
  }
  return flag;
}

int decimal_is_zero(s21_decimal *dec) {
    return dec->bits[0] == 0 && dec->bits[1] == 0 && dec->bits[2] == 0;
}

// Сдвиг влево на кол-во битов
int shiftDecLeft(s21_decimal *dec, int pos) {
    int tempBitsOne = 0;
    int tempBitsTwo = 0;
    int tempBitsThree = 0;
    for (int i = 0; i < pos; i++) {
        if (get_bit(31, *dec)) tempBitsOne = 1;
        if (get_bit(63, *dec)) tempBitsTwo = 1;
        if (get_bit(95, *dec)) tempBitsThree = 1;
        for (int g = 0; g <= 3; g++) dec->bits[g] = dec->bits[g] << 1;
        if (tempBitsOne == 1) {
            set_bit(32, dec);
            tempBitsOne = 0;
        }
        if (tempBitsTwo == 1) {
            set_bit(64, dec);
            tempBitsOne = 0;
        }
    }
    return tempBitsThree;
}


void shift_one_bit_big(big_decimal *src) {
  src->bits[5] <<= 1;
  src->bits[4] <<= 1;
  src->bits[3] <<= 1;
  src->bits[2] <<= 1;
  src->bits[1] <<= 1;
  src->bits[0] <<= 1;
//   for (int i = 0; i <= 5; i ++ ) {
//     src -> bits[i] <<= 1;
//   }
}

void shiftBigDecLeftOne(big_decimal *src) {
  int bts0 = 0, bts1 = 0, bts2 = 0, bts3 = 0, bts4 = 0;
  if (get_bit_big(31, *src)) bts0 = 1;
  if (get_bit_big(63, *src)) bts1 = 1;
  if (get_bit_big(95, *src)) bts2 = 1;
  if (get_bit_big(127, *src)) bts3 = 1;
  if (get_bit_big(159, *src)) bts4 = 1;

  shift_one_bit_big(src);

  if (bts0) set_bit_big(32, src);
  if (bts1) set_bit_big(64, src);
  if (bts2) set_bit_big(96, src);
  if (bts3) set_bit_big(128, src);
  if (bts4) set_bit_big(160, src);
}

void shiftBigDecRightOne(big_decimal *src) {
  int bts0 = 0, bts1 = 0, bts2 = 0, bts3 = 0, bts4 = 0;
  if (get_bit_big(32, *src)) bts0 = 1;
  if (get_bit_big(64, *src)) bts1 = 1;
  if (get_bit_big(96, *src)) bts2 = 1;
  if (get_bit_big(128, *src)) bts3 = 1;
  if (get_bit_big(160, *src)) bts4 = 1;

  src->bits[5] >>= 1;
  src->bits[4] >>= 1;
  src->bits[3] >>= 1;
  src->bits[2] >>= 1;
  src->bits[1] >>= 1;
  src->bits[0] >>= 1;

  if (bts0) set_bit_big(31, src);
  if (bts1) set_bit_big(63, src);
  if (bts2) set_bit_big(95, src);
  if (bts3) set_bit_big(127, src);
  if (bts4) set_bit_big(159, src);
}

int get_bit_big(const int n, big_decimal src) {
  return (src.bits[n / 32] & (1 << (n % 32))) != 0;
}

void zero_bit_big(const int n, big_decimal *src) {
  src->bits[n / 32] &= ~(1 << (n % 32));
}

void set_bit_big(const int n, big_decimal *src) {
  src->bits[n / 32] |= (1 << (n % 32));
}

int add_big(big_decimal one, big_decimal two, big_decimal *res) {
  int v_ume = 0, err = 0;
  big_decimal_init(res);

  for (int i = 0; i < 192; i++) {
    if (get_bit_big(i, one) && get_bit_big(i, two)) {
      if (v_ume == 0) {
        zero_bit_big(i, res);
      } else {
        set_bit_big(i, res);
      }
      v_ume = 1;
    } else if (get_bit_big(i, one) || get_bit_big(i, two)) {
      if (v_ume == 1) {
        zero_bit_big(i, res);
      } else {
        set_bit_big(i, res);
      }
    } else {
      if (v_ume == 1) {
        set_bit_big(i, res);
        v_ume = 0;
      } else {
        zero_bit_big(i, res);
      }
    }
  }
  return err;
}


// int s21_from_decimal_to_float(s21_decimal src, float *dst) {
//     int result = 1;
//     int scale = getDecimalScale(src);
//     double temp = 0.0;
//     if (scale < 29 ) {
//         int bit = 0;
//         while (bit < 96) {
//             temp += (get_bit(bit, src) * (pow(2, bit)));
//             bit++;
//         }
//         if (scale > 0) {
//             temp /=(pow(10.0,scale));
//         }
//         *dst = (float)temp;
//         if (get_bit(127, src) == 1) {
//             *dst *= -1;
//         }
//         printf("%f\n", *dst);  // не забыть убрать
//         result = 0;
//     }
//     return result;
// }

// void set_sign(int sign, s21_decimal *dst) {
//     if (sign == 1) {
//         dst->bits[3] = 1u << 31;
//     }
//     if (sign == 0) {
//         dst->bits[3] = 0u << 31;
//     }
// }


// s21_decimal s21_negate(s21_decimal* dst) {
//     int sign;
//     sign = get_bit(127, *dst);
//     if (sign == 1) {
//         dst->bits[3] = 0u << 31;
//     }
//     if (sign == 0) {
//         dst->bits[3] = 1u << 31;
//     }
//     return *dst;
// }

// s21_decimal clearSign(s21_decimal* dst) {
//     int sign;
//     s21_decimal result = *dst;
//     sign = get_bit(127, *dst);
//     if (sign == 1) {
//         result.bits[3] &= 2147483647;
//     }
//     return result;
// }


// value type
// s21_decimal s21_negate(s21_decimal* dec) {
//     s21_decimal result = *dec;
//         if (result.value_type == 0) {
//             result.bits[3] ^= SIGN;
//         } else if (result.value_type == 1) {
//             result.value_type = 2;
//         } else if (result.value_type == 2) {
//             result.value_type = 1;
//         }
//     return result;
// }


//*********************************************************************
// value type

// int decimal_verification(s21_decimal dec) {
//     int result = 1;
//     s21_decimal temp = dec;
//     int scale = (temp.bits[3] & EXP) >> 16;
//     int type = temp.value_type;
//     if ((type != 0) || (scale > 28)) {
//         result = 0;
//     }
//     return result;
// }

// value type in verification


// Округление до ближайшего целого числа
int s21_round(s21_decimal value, s21_decimal *result) {
    int err = 0;
    int tmp = 0;
    int mod = 0;
    s21_decimal decTemp, one;
    decimal_init(&decTemp);
    copyDec(&decTemp, value);
    err += s21_from_int_to_decimal(1, &one);
    if (get_sign(value)) set_sign(1, &one);
    int valueScale = getDecimalScale(value);
    while (valueScale >= 1) {
        mod = div_ten_mod(decTemp, &decTemp);
        if (tmp) {
            ++mod;
            tmp = 0;
        }
        setDecimalScale(&decTemp, --valueScale);
        if (mod > 5) {
            tmp = 1;
        } else if (mod == 5 && get_bit(0, decTemp)) {
            tmp = 1;
        }
    }
    err += s21_truncate(decTemp, result);
    if (get_sign(value)) set_sign(1, result);
    if (mod > 5) {
        err += s21_add(*result, one, result);
    } else if (mod == 5 && get_bit(0, *result)) {
        err += s21_add(*result, one, result);
    }
    if (err > 0) err = 1;
    return err;
}

int s21_truncate(s21_decimal value, s21_decimal *result) {
    int output = 1;
    int scale = getDecimalScale(value);
    copyDec(result, value);
    if (scale <= 28) {
        for (int i = 0; i < scale; i++) {
            *result = simple_div_by_ten(*result, result);
        }
        setDecimalScale(result, 0);
        output = 0;
    }
    return output;
}

int s21_negate(s21_decimal value, s21_decimal *result) {
    *result = value;
    result->bits[3] ^= SIGN;
    return 0;
}

int s21_from_decimal_to_float(s21_decimal src, float *dst) {
    int result = 1;
    int scale = getDecimalScale(src);
    double temp = 0.0;
    if (scale <= 28) {
        int bit = 0;
        while (bit < 96) {
            temp += (get_bit(bit, src) * s21_pow(2, bit));
            bit++;
        }
        if (scale > 0) {
            temp /=(s21_pow(10.0, scale));
        }
        *dst = (float)temp;
        if (get_bit(127, src) == 1) {
            *dst *= -1;
        }
        result = 0;
    }
    return result;
}

int s21_floor(s21_decimal value, s21_decimal *result) {
    int output = 1;
    if (getDecimalScale(value) <= 28) {
        s21_truncate(value, result);
        s21_decimal dec_one = {{1, 0, 0, 0}};
        if (s21_is_equal(&value, result) == 0 && get_sign(value) == 1) {
            s21_sub(*result, dec_one, result);
            output = 0;
        }
    }
    return output;
}

