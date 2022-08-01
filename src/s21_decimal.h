#ifndef SRC_S21_DECIMAL_H_
#define SRC_S21_DECIMAL_H_

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <limits.h>

#define PLUS 0
#define MINUS 1
#define PLUS_PLUS 0
#define MINUS_PLUS 1
#define PLUS_MINUS 2
#define MINUS_MINUS 3
#define SIGN 0x80000000
#define EXP 0x00ff0000
#define MAX_DECIMAL 79228162514264337593543950335.0

typedef struct {
  int bits[4];
} s21_decimal;

typedef struct {
  int bits[7];
} big_decimal;

int get_bit(int position, s21_decimal dec);
void clear_bit(int position, s21_decimal* dec);
void clearSign(s21_decimal* dst);
int getDecimalScale(s21_decimal dec);  // blorie
void setDecimalScale(s21_decimal *dec, int scale);
int decimal_verification(s21_decimal dec);
void print_decimal_to_binary(unsigned int k);
void print_int_to_binary(unsigned int k);
void print_decimal(s21_decimal *dst);  // печатает децимал полностью


void copyDec(s21_decimal *result, s21_decimal dec);

// arifmetics
// +
int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
// int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
// int s21_mod(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);


int setEqualScale(s21_decimal *dec_1, s21_decimal *dec_2);
int mulDecimal10(s21_decimal dec, s21_decimal *result);
int shiftDecLeft(s21_decimal *dec, int pos);
int getDecimalLength(s21_decimal dec);


// operator equals

int s21_is_less(s21_decimal* dst1, s21_decimal* dst2);
int s21_is_less_or_equal(s21_decimal* dst1, s21_decimal* dst2);
int s21_is_greater(s21_decimal* dst1, s21_decimal* dst2);
int s21_is_greater_or_equal(s21_decimal* dst1, s21_decimal* dst2);
int s21_is_equal(s21_decimal* dst1, s21_decimal* dst2);
int s21_is_not_equal(s21_decimal* dst1, s21_decimal* dst2);
int decimal_compare(s21_decimal src_one, s21_decimal src_two);

// convertation function
int s21_from_int_to_decimal(int src, s21_decimal *dst);
// int s21_from_float_to_decimal(float src, s21_decimal *dst);
int s21_from_decimal_to_int(s21_decimal src, int *dst);
int s21_from_decimal_to_float(s21_decimal src, float *dst);

// over function

//-

// int s21_floor(s21_decimal value, s21_decimal *result);
int s21_round(s21_decimal value, s21_decimal *result);
int s21_truncate(s21_decimal value, s21_decimal *result);
int s21_negate(s21_decimal value, s21_decimal *result);
// help function

int get_bit(int position, s21_decimal dec);
void set_bit(int position, s21_decimal* dec);
void clear_bit(int position, s21_decimal* dec);

int get_sign(s21_decimal dst);
void set_sign(int sign, s21_decimal *dst);
void clearSign(s21_decimal* dst);

int shiftDecLeft(s21_decimal *dec, int posit);
int setEqualScale(s21_decimal *dec_1, s21_decimal *dec_2);

s21_decimal add_decimal(s21_decimal value_1, s21_decimal value_2, int *temp);
s21_decimal sub_decimal(s21_decimal value_1, s21_decimal value_2, int *status);
int mulDecimal10(s21_decimal dec, s21_decimal *result);
double s21_pow(double num, int exp);
s21_decimal invert_for_sub_decimal(s21_decimal dec);

//-
// int s21_getScale(s21_decimal dec);
// void s21_setScale(s21_decimal *dec, int scale);


int decimal_verification(s21_decimal dec);
void print_int_to_binary(unsigned int k);

void big_decimal_init(big_decimal *src);
int get_bit_big(const int n, big_decimal src);
void zero_bit_big(const int n, big_decimal *src);
void set_bit_big(const int n, big_decimal *src);
int compare_mantis(s21_decimal *src_one, s21_decimal *src_two);
int simple_comp_dec(s21_decimal dec1, s21_decimal dec2);
int simple_comp_u(unsigned a, unsigned b);
int normalise(s21_decimal *src_one, s21_decimal *src_two, s21_decimal *remainder_out);
void decimal_init(s21_decimal *src);
int decimal_is_zero(s21_decimal *dec);
s21_decimal simple_div_by_ten(s21_decimal dec, s21_decimal *resid);
s21_decimal simple_div(s21_decimal num, s21_decimal den, s21_decimal *ptr_resid);
int div_ten_mod(s21_decimal dec, s21_decimal *result_div);
s21_decimal sub_dec_bits(s21_decimal dec1, s21_decimal dec2);
int substract_decimal_bits(s21_decimal *dec1, const s21_decimal *dec2);
s21_decimal s21_move_left(s21_decimal src, int shift);
s21_decimal s21_move_right(s21_decimal src, int shift);
s21_decimal s21_multi_10(s21_decimal src);
int max_dec_bit(s21_decimal dec);
int max_bit(unsigned a);
s21_decimal sum_dec_bits(s21_decimal dec1, s21_decimal dec2);
int add_decimal_bits(s21_decimal *dec1, const s21_decimal *dec2);
void great_shift(big_decimal *src);
int add_big(big_decimal one, big_decimal two, big_decimal *res);
void dec_to_big(s21_decimal *src, big_decimal *dst);
void big_to_dec(big_decimal *src, s21_decimal *dst);
void shiftBigDecLeftOne(big_decimal *src);
int big_to_dec_cut(big_decimal src, s21_decimal *dst);
int simple_mul_10_big(big_decimal value, big_decimal *result);
int getBigDecimalLength(big_decimal dec);
int big_decimal_is_empty(big_decimal src);
int simple_big_decimal_compare(big_decimal value_1, big_decimal value_2);
int simple_sub_big(big_decimal value_1, big_decimal value_2, big_decimal *result);
void print_big_decimal(big_decimal *dst);
void shiftBigDecRightOne(big_decimal *src);
int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
big_decimal simple_div_big(big_decimal value_1, big_decimal value_2, big_decimal *result);

int firstNotZeroBit(s21_decimal src);

#endif  // SRC_S21_DECIMAL_H_
